/* Copyright 2020 Bastian de Byl */
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/dma2d.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <lvgl/lvgl.h>
#include <main.h>
#include <src/common.h>
#include <src/ili9341.h>
#include <src/ltdc.h>
#include <src/lv_screen.h>
#include <src/sdram.h>
#include <src/sleeper.h>

#define USE_DMA_MEM2MEM 1

static int32_t x1_flush;
static int32_t y1_flush;
static int32_t x2_flush;
static int32_t y2_fill;
static int32_t y_fill_act;
static const lv_color_t* buf_to_flush;

static lv_disp_drv_t disp_drv;
static lv_disp_draw_buf_t disp_buf;

static volatile uint16_t* fb = (volatile uint16_t*)(SDRAM_BASE_ADDRESS);

/*  _ _ _ ___ _____ _  _
 * (_) (_) _ \__ / | |/ |
 * | | | \_, /|_ \_  _| |
 * |_|_|_|/_/|___/ |_||_|
 *                        */
static ili_init_t ili9341_init = {
    .spi_rcc = RCC_SPI5,
    .spi_bus = SPI5,
    .csx_gpio = GPIOC,
    .csx = ILI9341_CSX,
    .rdx_gpio = GPIOD,
    .rdx = ILI9341_RDX,
    .wrx_gpio = GPIOD,
    .wrx = ILI9341_WRX,
};

static pin_def_t ili_pin_defs[] = {
    {RCC_GPIOC, GPIOC, ILI9341_CSX},
    {RCC_GPIOD, GPIOD, ILI9341_RDX | ILI9341_WRX},
};
uint8_t ili_pin_defs_size = 2;

static pin_def_t ili_spi_pin_defs[] = {
    {RCC_GPIOF, GPIOF, ILI9341_SCK | ILI9341_MISO | ILI9341_MOSI},
};
uint8_t ili_spi_pin_defs_size = 1;

/*         _
 *  ___ __| |_ _ __ _ _ __
 * (_-</ _` | '_/ _` | '  \
 * /__/\__,_|_| \__,_|_|_|_|
 *                           */
static pin_def_t sdram_pin_defs[] = {
    {.rcc = RCC_GPIOB, .gpio = GPIOB, .pins = SDRAM_SDCKE1 | SDRAM_SDNE1},
    {.rcc = RCC_GPIOC, .gpio = GPIOC, .pins = SDRAM_SDNEWE},
    {.rcc = RCC_GPIOD,
     .gpio = GPIOD,
     .pins = SDRAM_D2 | SDRAM_D3 | SDRAM_D13 | SDRAM_D14 | SDRAM_D15 |
             SDRAM_D0 | SDRAM_D1},
    {.rcc = RCC_GPIOE,
     .gpio = GPIOE,
     .pins = SDRAM_NBL0 | SDRAM_NBL1 | SDRAM_D4 | SDRAM_D5 | SDRAM_D6 |
             SDRAM_D7 | SDRAM_D8 | SDRAM_D9 | SDRAM_D10 | SDRAM_D11 |
             SDRAM_D12},
    {.rcc = RCC_GPIOF,
     .gpio = GPIOF,
     .pins = SDRAM_A0 | SDRAM_A1 | SDRAM_A2 | SDRAM_A3 | SDRAM_A4 | SDRAM_A5 |
             SDRAM_SDNRAS | SDRAM_A6 | SDRAM_A7 | SDRAM_A8 | SDRAM_A9},
    {.rcc = RCC_GPIOG,
     .gpio = GPIOG,
     .pins = SDRAM_A10 | SDRAM_A11 | SDRAM_A14 | SDRAM_INT2 | SDRAM_SDCLK |
             SDRAM_SDNCAS},
};
uint8_t sdram_pin_defs_size = 6;

/*  _ _      _
 * | | |_ __| |__
 * | |  _/ _` / _|
 * |_|\__\__,_\__|
 *                 */
static pin_def_t ltdc_pin_defs[] = {
    {.rcc = RCC_GPIOA,
     .gpio = GPIOA,
     .pins = LCD_R4 | LCD_R5 | LCD_G2 | LCD_B5 | LCD_VSYNC},
    {.rcc = RCC_GPIOB,
     .gpio = GPIOB,
     .pins = LCD_R3 | LCD_R6 | LCD_G4 | LCD_G5 | LCD_B7},
    {.rcc = RCC_GPIOC, .gpio = GPIOC, .pins = LCD_R2 | LCD_G6 | LCD_HSYNC},
    {.rcc = RCC_GPIOD, .gpio = GPIOD, .pins = LCD_G7 | LCD_B2},
    {.rcc = RCC_GPIOG,
     .gpio = GPIOG,
     .pins = LCD_R7 | LCD_G3 | LCD_B3 | LCD_B4 | LCD_CLK},
    {.rcc = RCC_GPIOF, .gpio = GPIOF, .pins = LCD_DE}};
uint8_t ltdc_pin_defs_size = 6;

#ifdef USE_DMA_MEM2MEM
void dma2_stream0_isr(void) {
    if (dma_get_interrupt_flag(DMA2, DMA_STREAM0, DMA_TCIF)) {
        gpio_set(GPIOG, GPIO13);
        dma_disable_stream(DMA2, DMA_STREAM0);
        dma_clear_interrupt_flags(DMA2, DMA_STREAM0, DMA_TCIF);
        y_fill_act++;

        if (y_fill_act > y2_fill) {
            lv_disp_flush_ready(&disp_drv);
        } else {
            buf_to_flush += x2_flush - x1_flush + 1;

            dma_set_peripheral_address(DMA2, DMA_STREAM0,
                                       (uint32_t)buf_to_flush);
            dma_set_memory_address(
                DMA2, DMA_STREAM0,
                (uint32_t)&fb[y_fill_act * WIDTH + x1_flush]);
            dma_set_number_of_data(DMA2, DMA_STREAM0,
                                   (uint32_t)(x2_flush - x1_flush + 1));
            dma_enable_stream(DMA2, DMA_STREAM0);
        }
    }
}

static void init_dma_mem2mem(void) {
    /* RCC Enable DMA */
    rcc_periph_clock_enable(RCC_DMA2);
    /* rcc_periph_clock_enable(RCC_DMA2D); */

    /* DMA Stream Reset */
    dma_stream_reset(DMA2, DMA_STREAM0);

    /* DMA Interrupts */
    dma_enable_transfer_complete_interrupt(DMA2, DMA_STREAM0);
    dma_enable_transfer_error_interrupt(DMA2, DMA_STREAM0);
    dma_enable_direct_mode_error_interrupt(DMA2, DMA_STREAM0);
    dma_enable_fifo_error_interrupt(DMA2, DMA_STREAM0);
    nvic_enable_irq(NVIC_DMA2_STREAM0_IRQ);

    /* DMA Mem-to-Mem Setup */
    dma_enable_direct_mode(DMA2, DMA_STREAM0);
    dma_enable_memory_increment_mode(DMA2, DMA_STREAM0);
    dma_enable_peripheral_increment_mode(DMA2, DMA_STREAM0);
    dma_set_memory_size(DMA2, DMA_STREAM0, DMA_SxCR_MSIZE_16BIT);
    dma_set_peripheral_size(DMA2, DMA_STREAM0, DMA_SxCR_PSIZE_16BIT);
    dma_set_priority(DMA2, DMA_STREAM0, DMA_SxCR_PL_HIGH);
    dma_set_transfer_mode(DMA2, DMA_STREAM0, DMA_SxCR_DIR_MEM_TO_MEM);

    /* DMA FIFO and Burst Mode */
    dma_enable_fifo_mode(DMA2, DMA_STREAM0);
    dma_set_fifo_threshold(DMA2, DMA_STREAM0, DMA_SxFCR_FTH_1_4_FULL);
    dma_set_memory_burst(DMA2, DMA_STREAM0, DMA_SxCR_MBURST_SINGLE);
    dma_set_peripheral_burst(DMA2, DMA_STREAM0, DMA_SxCR_PBURST_SINGLE);

    /* DMA Set initial memory addresses */
    dma_set_initial_target(DMA2, DMA_STREAM0, 0);
    dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)&buf_to_flush);
    dma_set_memory_address(DMA2, DMA_STREAM0, (uint32_t)SDRAM_BASE_ADDRESS);

    dma_channel_select(DMA2, DMA_STREAM0, DMA_SxCR_CHSEL_0);
}
#else
static void put_px(int32_t x, int32_t y, lv_color_t color_p) {
    uint16_t* px = (uint16_t*)(SDRAM_BASE_ADDRESS) + (y * WIDTH + x);
    uint16_t color = color_p.full;
    *px = color;
}
#endif

static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area,
                          lv_color_t* color_p) {
    if (area->x2 < 0) return;
    if (area->y2 < 0) return;
    if (area->x1 > WIDTH - 1) return;
    if (area->y1 > HEIGHT - 1) return;

#ifdef USE_DMA_MEM2MEM
    int32_t act_x1 = area->x1 < 0 ? 0 : area->x1;
    int32_t act_y1 = area->y1 < 0 ? 0 : area->y1;
    int32_t act_x2 = area->x2 > WIDTH - 1 ? WIDTH - 1 : area->x2;
    int32_t act_y2 = area->y2 > HEIGHT - 1 ? HEIGHT - 1 : area->y2;

    x1_flush = act_x1;
    y1_flush = act_y1;
    x2_flush = act_x2;
    y2_fill = act_y2;
    y_fill_act = act_y1;
    buf_to_flush = color_p;

    dma_set_peripheral_address(DMA2, DMA_STREAM0, (uint32_t)buf_to_flush);
    dma_set_memory_address(DMA2, DMA_STREAM0,
                           (uint32_t)&fb[y_fill_act * WIDTH + x1_flush]);
    dma_set_number_of_data(DMA2, DMA_STREAM0,
                           (uint32_t)(x2_flush - x1_flush + 1));
    dma_enable_stream(DMA2, DMA_STREAM0);

    UNUSED(disp);
#else
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++) {
        for (x = area->x1; x <= area->x2; x++) {
            put_px(x, y, *color_p);
            color_p++;
        }
    }
    lv_disp_flush_ready(disp);
#endif
}

static volatile uint32_t t_saved = 0;
static void monitor_cb(lv_disp_drv_t* d, uint32_t t, uint32_t p) {
    t_saved = t;

    UNUSED(d);
    UNUSED(p);
}

static void lv_tick_cb(void) { lv_tick_inc(1); }
int main(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
    rcc_periph_clock_enable(RCC_GPIOG);

    // debug LED
    gpio_mode_setup(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);

    init_sdram(sdram_pin_defs, sdram_pin_defs_size);
    init_ltdc(ltdc_pin_defs, ltdc_pin_defs_size);

    init_ili9341(ili_pin_defs, ili_pin_defs_size, ili_spi_pin_defs,
                 ili_spi_pin_defs_size, &ili9341_init);

#ifdef USE_DMA_MEM2MEM
    init_dma_mem2mem();
#endif
    lv_init();

    static lv_color_t _buf_a[WIDTH * 60];
    static lv_color_t _buf_b[WIDTH * 60];
    lv_disp_draw_buf_init(&disp_buf, _buf_a, _buf_b, WIDTH * 40);
    lv_disp_drv_init(&disp_drv);

    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.monitor_cb = monitor_cb;
    disp_drv.hor_res = WIDTH;
    disp_drv.ver_res = HEIGHT;
    lv_disp_drv_register(&disp_drv);

    set_sleep_cb(lv_tick_cb);
    lv_ex_btn_1();
    /* sdram_draw_color(); */

    /* int i; */
    while (1) {
        /* do nothing */
        /* TESTING SPI:
         * gpio_clear(ili9341_init.csx_gpio, ili9341_init.csx);
         * spi_send(ili9341_init.spi_bus, 0x55);
         * while (!(SPI_SR(ili9341_init.spi_bus) & SPI_SR_TXE))
         *     ;
         * while (SPI_SR(ili9341_init.spi_bus) & SPI_SR_BSY)
         *     ;
         * gpio_set(ili9341_init.csx_gpio, ili9341_init.csx);
         * for (i = 0; i < 0xff; ++i)
         *     ; */
        sleep_ms(3);
        lv_task_handler();
    }

    return 0;
}
