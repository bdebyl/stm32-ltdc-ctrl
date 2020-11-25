/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/ltdc.h>
#include <libopencm3/stm32/rcc.h>
#include <ltdc.h>
#include <sdram.h>

void lcd_tft_isr(void) {
    LTDC_ICR |= LTDC_ICR_CRRIF;

    LTDC_SRCR |= LTDC_SRCR_VBR;
}

static void init_ltdc_rcc(void) {
    /* Timing:
     *  Input Clock = (8MHz HSE) / PLLM (/8)
     *  PLLSAI = N * Input Clock
     *  PLLLCDCLK = (PLLSAI / R) / PLLSAIP
     * note thate PLLSAIP is set in postscalers below
     * for 6.5MHz: sain=130, sair=3, pllsaip=div4 */

    uint32_t sain = 192; /* division factor for VCO */
    uint32_t saiq =
        (RCC_PLLSAICFGR >> RCC_PLLSAICFGR_PLLSAIQ_SHIFT) &
        RCC_PLLSAICFGR_PLLSAIQ_MASK; /* division factor for SAI1 clock */
    uint32_t sair = 4; /* division factor for LCD clock (valid: 2-7) */
    rcc_pllsai_postscalers(10, 20);
    rcc_pllsai_config(sain, 0, saiq, sair);
    rcc_pllsai_postscalers(0, RCC_PLLSAICFGR_PLLSAIP_DIV8);
    rcc_osc_on(RCC_PLLSAI);
    rcc_wait_for_osc_ready(RCC_PLLSAI);

    while ((RCC_CR & RCC_CR_PLLSAIRDY) == 0) {
        continue;
    }
    /* while (!rcc_is_osc_ready(rcc_pllsai)) {
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

    /* LTDC_SSCR */
    uint16_t HSYNC = 10;
    uint16_t VSYNC = 2;
    /* LTDC_BPCR */
    uint16_t HBP = 20;
    uint16_t VBP = 2;
    /* LTDC_AWCR */
    uint16_t WIDTH = 240;
    uint16_t HEIGHT = 320;
    /* LTDC_TWCR */
    uint16_t HFP = 10;
    uint16_t VFP = 4;
    ltdc_set_tft_sync_timings(HSYNC, VSYNC, HBP, VBP, WIDTH, HEIGHT, HFP, VFP);

    ltdc_set_background_color(0x00, 0x00, 0x00); /* LTDC_BCCR */

    /* LTDC Interrup Enable Register to LTDC Register Reload Interrup Enable
     * (LTDC_RRIE) */
    LTDC_IER = LTDC_IER_RRIE;
    /* NVIC to enable LCD_TFT Interrupt */
    nvic_enable_irq(NVIC_LCD_TFT_IRQ);
    nvic_set_priority(NVIC_LCD_TFT_IRQ, 0xE); /* Lowest priority */

    /* LAYER 1 */
    {
        ltdc_setup_windowing(LTDC_LAYER_1, HBP, VBP, WIDTH,
                             HEIGHT); /* LTDC_LxWHPCR */

        ltdc_set_pixel_format(LTDC_LAYER_1,
                              LTDC_LxPFCR_RGB565); /* LTDC_LxPFCR */

        ltdc_set_fbuffer_address(
            LTDC_LAYER_1, (uint32_t)SDRAM_BASE_ADDRESS); /* LTDC_LxCFBAR */

        uint16_t pitch = WIDTH * sizeof(uint32_t);
        uint16_t len = WIDTH * sizeof(uint32_t) + 3;
        ltdc_set_fb_line_length(LTDC_LAYER_1, len, pitch); /* LTDC_LxCFBLR */

        ltdc_set_fb_line_count(LTDC_LAYER_1, HEIGHT); /* LTDC_LxCFBLNR */

        ltdc_set_default_colors(LTDC_LAYER_1, 0xFF, 0x00, 0x00,
                                0x00); /* LTDC_LxDCCR */

        ltdc_set_constant_alpha(LTDC_LAYER_1, 0x000000FF); /* LTDC_LxCACR */

        uint8_t bf1 = LTDC_LxBFCR_BF1_CONST_ALPHA;
        uint8_t bf2 = LTDC_LxBFCR_BF2_CONST_ALPHA;
        ltdc_set_blending_factors(LTDC_LAYER_1, bf1, bf2);

        /* Dithering?
         * ltdc_set_color_key(uint32_t layer, uint8_t r, uint8_t g, uint8_t b);
         * [> LTDC_LxCKCR <] */
    }

    ltdc_layer_ctrl_enable(LTDC_LAYER_1,
                           LTDC_LxCR_LAYER_ENABLE); /* LTDC_LxCR */

    /* Reload the shadow registers to active registers */
    ltdc_reload(LTDC_SRCR_RELOAD_VBR); /* LTDC_SRCR */

    ltdc_ctrl_enable(LTDC_GCR_LTDCEN); /* LTDC_GCR */
}
