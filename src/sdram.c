/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <sdram.h>

static pin_def_t sdram_pins[] = {
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

static struct sdram_timing timing = {
    .trcd = SDRAM_TIMING_RCD,
    .trp = SDRAM_TIMING_RP,
    .twr = SDRAM_TIMING_WR,
    .trc = SDRAM_TIMING_RC,
    .tras = SDRAM_TIMING_RAS,
    .txsr = SDRAM_TIMING_XSR,
    .tmrd = SDRAM_TIMING_MRD,
};

void init_sdram() {
    /* control, timing registers */
    uint32_t cr_tmp, tr_tmp;

    init_pin_defs(sdram_pins, GPIO_AF12);

    /* Enable the SDRAM Controller */
    rcc_periph_clock_enable(RCC_FSMC);

    /* Note the STM32F429-DISCO board has the ram attached to bank 2 */
    /* Timing parameters computed for a 168Mhz clock */
    /* These parameters are specific to the SDRAM chip on the board */

    cr_tmp = FMC_SDCR_RPIPE_1CLK;
    cr_tmp |= FMC_SDCR_SDCLK_2HCLK;
    cr_tmp |= FMC_SDCR_CAS_3CYC;
    cr_tmp |= FMC_SDCR_NB4;
    cr_tmp |= FMC_SDCR_MWID_16b;
    cr_tmp |= FMC_SDCR_NR_12;
    cr_tmp |= FMC_SDCR_NC_8;

    /* We're programming BANK 2, but per the manual some of the parameters
     * only work in CR1 and TR1 so we pull those off and put them in the
     * right place.
     */
    FMC_SDCR1 |= (cr_tmp & FMC_SDCR_DNC_MASK);
    FMC_SDCR2 = cr_tmp;

    tr_tmp = sdram_timing(&timing);
    FMC_SDTR1 |= (tr_tmp & FMC_SDTR_DNC_MASK);
    FMC_SDTR2 = tr_tmp;

    /* Now start up the Controller per the manual
     *	- Clock config enable
     *	- PALL state
     *	- set auto refresh
     *	- Load the Mode Register
     */
    sdram_command(SDRAM_BANK2, SDRAM_CLK_CONF, 1, 0);

    uint32_t i;
    for (i = 0; i < 0xFFFF; ++i) {
        /* sleep at least 100uS */
    }

    sdram_command(SDRAM_BANK2, SDRAM_PALL, 1, 0);
    sdram_command(SDRAM_BANK2, SDRAM_AUTO_REFRESH, 4, 0);
    tr_tmp = SDRAM_MODE_BURST_LENGTH_2 | SDRAM_MODE_BURST_TYPE_SEQUENTIAL |
             SDRAM_MODE_CAS_LATENCY_3 | SDRAM_MODE_OPERATING_MODE_STANDARD |
             SDRAM_MODE_WRITEBURST_MODE_SINGLE;

    sdram_command(SDRAM_BANK2, SDRAM_LOAD_MODE, 1, tr_tmp);

    /*
     * set the refresh counter to insure we kick off an
     * auto refresh often enough to prevent data loss.
     */
    FMC_SDRTR = 683;
    /* and Poof! a 8 megabytes of ram shows up in the address space */
}
