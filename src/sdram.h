/* Copyright 2020 Bastian de Byl */
#ifndef SRC_SDRAM_H_
#define SRC_SDRAM_H_

#include <common.h>
#include <stdint.h>

#define SDRAM_BASE_ADDRESS ((uint32_t)(0xd0000000))

#define SDRAM_TIMING_RCD 2 /* RCD Delay */
#define SDRAM_TIMING_RP 2  /* RP Delay */
#define SDRAM_TIMING_WR 2  /* Write Recovery Time */
#define SDRAM_TIMING_RC 7  /* Row Cycle Delay */
#define SDRAM_TIMING_RAS 4 /* Self Refresh Time */
#define SDRAM_TIMING_XSR 7 /* Exit Self Refresh Time */
#define SDRAM_TIMING_MRD 2 /* Load to Active Delay */

void init_sdram(pin_def_t* pin_defs, uint8_t pin_defs_size);

#endif /* SRC_SDRAM_H_ */
