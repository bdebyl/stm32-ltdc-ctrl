/* Copyright 2020 Bastian de Byl */
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/ltdc.h>
#include <libopencm3/stm32/rcc.h>
#include <ltdc.h>
#include <sdram.h>

static pin_def_t ltdc_pins[] = {
    {.rcc = RCC_GPIOA,
     .gpio = GPIOA,
     .pins = LCD_R4 | LCD_R5 | LCD_G2 | LCD_B5},
    {.rcc = RCC_GPIOB,
     .gpio = GPIOB,
     .pins = LCD_R3 | LCD_R6 | LCD_G4 | LCD_G5 | LCD_B7},
    {.rcc = RCC_GPIOC, .gpio = GPIOC, .pins = LCD_R2 | LCD_G6},
    {.rcc = RCC_GPIOD,
     .gpio = GPIOD,
     .pins = LCD_G7 | LCD_B2 | LCD_IM0 | LCD_IM1 | LCD_IM2 | LCD_IM3},
    {.rcc = RCC_GPIOG,
     .gpio = GPIOG,
     .pins = LCD_R7 | LCD_G3 | LCD_B3 | LCD_B4},
};

void init_ltdc_rcc() {
    /* Timing: */
    /*  Input Clock = (8MHz HSE) / PLLM (/8) */
    /*  PLLSAI = N * Input Clock */
    /*  PLLLCDCLK = (PLLSAI / R) / PLLSAIP */
    /* for 6.5MHz: sain=130, sair=3, div4 */
    uint32_t sain = 100; /* division factor for VCO */
    uint32_t saiq =
        (RCC_PLLSAICFGR >> RCC_PLLSAICFGR_PLLSAIQ_SHIFT) &
        RCC_PLLSAICFGR_PLLSAIQ_MASK; /* division factor for SAI1 clock */
    uint32_t sair = 5; /* division factor for LCD clock (valid: 2-7) */
    rcc_pllsai_postscalers(10, 20);
    rcc_pllsai_config(sain, 0, saiq, sair);
    rcc_pllsai_postscalers(0, RCC_PLLSAICFGR_PLLSAIP_DIV2);
    rcc_osc_on(RCC_PLLSAI);
    rcc_wait_for_osc_ready(RCC_PLLSAI);

    /* while (!rcc_is_osc_ready(RCC_PLLSAI)) { */
    /*    (RCC_CR & RCC_CR_PLLSAIRDY) == 0 */
    /*    continue; */
    /* } */

    rcc_periph_clock_enable(RCC_LTDC);
}

void init_ltdc_gpio() {
    /* Loop through the list of defined ltdc_pins and set them up via GPIO and
     */
    /* enable the RCC peripheral clocks of each */
    init_pin_defs(ltdc_pins, GPIO_AF14);
}

void init_ltdc() {
    init_ltdc_rcc();
    init_ltdc_gpio();

    ltdc_ctrl_enable(LTDC_GCR_DEPOL | LTDC_GCR_HSPOL | LTDC_GCR_PCPOL);

    /* Lol I don't know what these timings do yet */
    /* ltdc_set_tft_sync_timings(320, 240, 10, 10, 323, 269, 10, 10); */
    ltdc_set_tft_sync_timings(320, 240, 68, 18, 18 + 320, 240 + 68, 20, 4);
    ltdc_set_background_color(0x00, 0x00, 0x00);

    ltdc_setup_windowing(LTDC_LAYER_1, 10, 10, 320, 240);
    ltdc_set_pixel_format(LTDC_LAYER_1, LTDC_LxPFCR_RGB888);
    ltdc_set_fbuffer_address(LTDC_LAYER_1, (uint32_t)SDRAM_BASE_ADDRESS);
    ltdc_set_fb_line_length(LTDC_LAYER_1, 240 * 3, 240 * 3 + 3);
    ltdc_set_fb_line_count(LTDC_LAYER_1, 320);
    ltdc_set_default_colors(LTDC_LAYER_1, 0xFF, 0x00, 0x00, 0x00);

    ltdc_set_constant_alpha(LTDC_LAYER_1, 0xFF);

    ltdc_layer_ctrl_enable(LTDC_LAYER_1, LTDC_LxCR_LAYER_ENABLE);
    ltdc_reload(LTDC_SRCR_RELOAD_IMR);

    ltdc_ctrl_enable(LTDC_GCR_LTDCEN);
}
