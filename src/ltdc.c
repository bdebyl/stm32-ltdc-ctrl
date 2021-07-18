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

static void init_sdram_ltdc_color(void) {
    int i;
    for (i = 0; i < WIDTH * HEIGHT * BPP; i++) {
        ((volatile uint8_t*)SDRAM_BASE_ADDRESS)[i] = 0x00;
    }
}

void sdram_draw_color(void) {
    int i;
    for (i = (WIDTH * HEIGHT * BPP) / 2; i < WIDTH * HEIGHT * BPP; i++) {
        ((volatile uint8_t*)SDRAM_BASE_ADDRESS)[i] = 0xFF;
    }
}

static void init_ltdc_rcc(void) {
    uint32_t sain = 192; /* division factor for VCO */
    uint32_t saiq =
        (RCC_PLLSAICFGR >> RCC_PLLSAICFGR_PLLSAIQ_SHIFT) &
        RCC_PLLSAICFGR_PLLSAIQ_MASK; /* division factor for SAI1 clock */
    uint32_t sair = 4; /* division factor for LCD clock (valid: 2-7) */
    rcc_pllsai_config(sain, 0, saiq, sair);
    rcc_pllsai_postscalers(0, RCC_DCKCFGR_PLLSAIDIVR_DIVR_8);

    rcc_osc_on(RCC_PLLSAI);
    rcc_wait_for_osc_ready(RCC_PLLSAI);

    while ((RCC_CR & RCC_CR_PLLSAIRDY) == 0) {
        continue;
    }

    rcc_periph_clock_enable(RCC_LTDC);
}

void init_ltdc(pin_def_t* pin_defs, uint8_t pin_defs_size) {
    init_pin_defs_af(pin_defs, pin_defs_size, GPIO_AF14);

    init_ltdc_rcc();

    init_sdram_ltdc_color();
    ltdc_ctrl_enable(LTDC_GCR_HSPOL_ACTIVE_LOW | LTDC_GCR_VSPOL_ACTIVE_LOW |
                     LTDC_GCR_PCPOL_ACTIVE_HIGH | LTDC_GCR_DEPOL_ACTIVE_LOW);

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
        ltdc_setup_windowing(LTDC_LAYER_1, HBP, VBP, HSYNC, VSYNC, WIDTH,
                             HEIGHT);

        ltdc_set_pixel_format(LTDC_LAYER_1,
                              LTDC_LxPFCR_RGB565); /* LTDC_LxPFCR */

        /* LTDC_LxCFBAR */
        /* ltdc_set_fbuffer_address(LTDC_LAYER_1, (uint32_t)img); */
        ltdc_set_fbuffer_address(LTDC_LAYER_1, (uint32_t)SDRAM_BASE_ADDRESS);

        /* LTDC_L1CFBLR = (WIDTH * BPP) << LTDC_LxCFBLR_CFBP_SHIFT |
         *                (WIDTH * BPP + 3) << LTDC_LxCFBLR_CFBLL_SHIFT; */
        uint16_t len = WIDTH * BPP + 3;
        uint16_t pitch = WIDTH * BPP;
        /* LTDC_LxCFBLR */
        ltdc_set_fb_line_length(LTDC_LAYER_1, len, pitch);

        /* LTDC_LxCFBLNR */
        ltdc_set_fb_line_count(LTDC_LAYER_1, HEIGHT);

        /* LTDC_LxDCCR */
        ltdc_set_default_colors(LTDC_LAYER_1, 0xFF, 0x00, 0xFF, 0x00);

        /* LTDC_LxCACR */
        ltdc_set_constant_alpha(LTDC_LAYER_1, 0xFF);

        /* LTDC_LxBFCR */
        ltdc_set_blending_factors(LTDC_LAYER_1, LTDC_LxBFCR_BF1_CONST_ALPHA,
                                  LTDC_LxBFCR_BF2_CONST_ALPHA);

        /* Dithering?
         * ltdc_set_color_key(uint32_t layer, uint8_t r, uint8_t g, uint8_t b);
         * [> LTDC_LxCKCR <] */
    }

    ltdc_layer_ctrl_enable(LTDC_LAYER_1,
                           LTDC_LxCR_LAYER_ENABLE); /* LTDC_LxCR */

    /* Reload the immediate shadow registers to active registers */
    ltdc_reload(LTDC_SRCR_RELOAD_IMR); /* LTDC_SRCR */

    ltdc_ctrl_enable(LTDC_GCR_LTDCEN); /* LTDC_GCR */
}
