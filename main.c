/* Copyright 2020 Bastian de Byl */
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <lvgl/lvgl.h>
#include <main.h>
#include <src/common.h>
#include <src/ili9341.h>
#include <src/ltdc.h>
#include <src/sdram.h>

static lv_disp_drv_t disp_drv;

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

static void set_pixel(uint32_t x, uint32_t y, lv_color_t color_p) {
    // Set pixel at x, y to color_p from SDRAM_BASE_ADDRESS
    /* color_p->full */
}

static void my_disp_flush(lv_disp_drv_t* disp, const lv_area_t* area,
                          lv_color_t* color_p) {
    int32_t x, y;
    for (y = area->y1; y <= area->y2; y++) {
        for (x = area->x1; x <= area->x2; x++) {
            set_pixel(x, y, *color_p);
            color_p++;
        }
    }

    lv_disp_flush_ready(disp);
}

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    init_sdram(sdram_pin_defs, sdram_pin_defs_size);
    init_ltdc(ltdc_pin_defs, ltdc_pin_defs_size);

    init_ili9341(ili_pin_defs, ili_pin_defs_size, ili_spi_pin_defs,
                 ili_spi_pin_defs_size, &ili9341_init);

    lv_init();

    static lv_disp_buf_t disp_buf;
    static lv_color_t _buf[LV_HOR_RES_MAX * 10];

    lv_disp_buf_init(&disp_buf, _buf, NULL, LV_HOR_RES_MAX * 10);
    lv_disp_drv_init(&disp_drv);

    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &_buf;
    lv_disp_drv_register(&disp_drv);

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
    }

    return 0;
}
