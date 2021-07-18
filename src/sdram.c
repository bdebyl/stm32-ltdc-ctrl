/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <sdram.h>

static struct sdram_timing timing = {
    .trcd = SDRAM_TIMING_RCD,
    .trp = SDRAM_TIMING_RP,
    .twr = SDRAM_TIMING_WR,
    .trc = SDRAM_TIMING_RC,
    .tras = SDRAM_TIMING_RAS,
    .txsr = SDRAM_TIMING_XSR,
    .tmrd = SDRAM_TIMING_MRD,
};

void init_sdram(pin_def_t* pin_defs, uint8_t pin_defs_size) {
    /* control, timing registers */
    uint32_t cr_tmp, tr_tmp;

    init_pin_defs_af(pin_defs, pin_defs_size, GPIO_AF12);

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
     *
     * Formula: SDRAM Refresh Timer (SDRTR) = SDRAM Refresh Period / Number of
     * Rows ex: TODO(bastian) explanation of this is missing!
     */
    FMC_SDRTR = 683;
    /* and Poof! a 8 megabytes of ram shows up in the address space */
}
