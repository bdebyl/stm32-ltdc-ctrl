/* Copyright 2020 Bastian de Byl */
#ifndef SRC_SDRAM_H_
#define SRC_SDRAM_H_
#include <common.h>
#include <libopencm3/stm32/gpio.h>

#define SDRAM_BASE_ADDRESS ((uint8_t *)(0xd0000000))

#define SDRAM_TIMING_RCD 2 /* RCD Delay */
#define SDRAM_TIMING_RP 2  /* RP Delay */
#define SDRAM_TIMING_WR 2  /* Write Recovery Time */
#define SDRAM_TIMING_RC 7  /* Row Cycle Delay */
#define SDRAM_TIMING_RAS 4 /* Self Refresh Time */
#define SDRAM_TIMING_XSR 7 /* Exit Self Refresh Time */
#define SDRAM_TIMING_MRD 2 /* Load to Active Delay */

/* GPIOB */
#define SDRAM_SDCKE1 GPIO5
#define SDRAM_SDNE1 GPIO6

/* GPIOC */
#define SDRAM_SDNEWE GPIO0

/* GPIOD */
#define SDRAM_D2 GPIO0
#define SDRAM_D3 GPIO1
#define SDRAM_D13 GPIO8
#define SDRAM_D14 GPIO9
#define SDRAM_D15 GPIO10
#define SDRAM_D0 GPIO14
#define SDRAM_D1 GPIO15

/* GPIOE */
#define SDRAM_NBL0 GPIO0
#define SDRAM_NBL1 GPIO1
#define SDRAM_D4 GPIO7
#define SDRAM_D5 GPIO8
#define SDRAM_D6 GPIO9
#define SDRAM_D7 GPIO10
#define SDRAM_D8 GPIO11
#define SDRAM_D9 GPIO12
#define SDRAM_D10 GPIO13
#define SDRAM_D11 GPIO14
#define SDRAM_D12 GPIO15

/* GPIOF */
#define SDRAM_A0 GPIO0
#define SDRAM_A1 GPIO1
#define SDRAM_A2 GPIO2
#define SDRAM_A3 GPIO3
#define SDRAM_A4 GPIO4
#define SDRAM_A5 GPIO5
#define SDRAM_SDNRAS GPIO11
#define SDRAM_A6 GPIO12
#define SDRAM_A7 GPIO13
#define SDRAM_A8 GPIO14
#define SDRAM_A9 GPIO15

/* GPIOG */
#define SDRAM_A10 GPIO0
#define SDRAM_A11 GPIO1
#define SDRAM_A14 GPIO4 /* FMC_BA1 */
#define SDRAM_INT2 GPIO5
#define SDRAM_SDCLK GPIO8
#define SDRAM_SDNCAS GPIO15

void init_sdram(void);

#endif /* SRC_SDRAM_H_ */
