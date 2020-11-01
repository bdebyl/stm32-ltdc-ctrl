// Copyright 2020 Bastian de Byl
#include <libopencm3/stm32/fsmc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <sdram.h>
#include <stdint.h>

static pin_definitions sdram_pins[6] = {
    {RCC_GPIOB, GPIOB, GPIO5 | GPIO6},
    {RCC_GPIOC, GPIOC, GPIO0},
    {RCC_GPIOD, GPIOD,
     GPIO0 | GPIO1 | GPIO8 | GPIO9 | GPIO10 | GPIO14 | GPIO15},
    {RCC_GPIOE, GPIOE,
     GPIO0 | GPIO1 | GPIO7 | GPIO8 | GPIO9 | GPIO10 | GPIO11 | GPIO12 | GPIO13 |
         GPIO14 | GPIO15},
    {RCC_GPIOF, GPIOF,
     GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | GPIO11 | GPIO12 | GPIO13 |
         GPIO14 | GPIO15},
    {RCC_GPIOG, GPIOG, GPIO0 | GPIO1 | GPIO4 | GPIO5 | GPIO8 | GPIO15}};

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
    int i;
    /* control, timing registers */
    uint32_t cr_tmp, tr_tmp;

    for (i = 0; i < 6; i++) {
        rcc_periph_clock_enable(sdram_pins[i].rcc);
        gpio_mode_setup(sdram_pins[i].gpio, GPIO_MODE_AF, GPIO_PUPD_NONE,
                        sdram_pins[i].pins);

        gpio_set_output_options(sdram_pins[i].gpio, GPIO_OTYPE_PP,
                                GPIO_OSPEED_50MHZ, sdram_pins[i].pins);

        gpio_set_af(sdram_pins[i].gpio, GPIO_AF12, sdram_pins[i].pins);
    }

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
    for (i = 0; i < 0xFFFF; ++i) {
        // sleep at least 100uS
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
