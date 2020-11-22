/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/ltdc.h>
#include <libopencm3/stm32/rcc.h>
#include <ltdc.h>
#include <sdram.h>

static void init_ltdc_rcc(void) {
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

    /* while (!rcc_is_osc_ready(RCC_PLLSAI)) {
     *     if ((RCC_CR & RCC_CR_PLLSAIRDY) == 0) break;
     * } */

    rcc_periph_clock_enable(RCC_LTDC);
}

void init_ltdc(pin_def_t* pin_defs, uint8_t pin_defs_size) {
    init_pin_defs_af(pin_defs, pin_defs_size, GPIO_AF14);

    /*     rcc_periph_clock_enable(RCC_GPIOC);
     *     gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6);
     *     gpio_set_af(GPIOC, GPIO_AF14, GPIO6);
     *     gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ,
     * GPIO6);
     *  */
    init_ltdc_rcc();

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
