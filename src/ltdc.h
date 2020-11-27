/* Copyright 2020 Bastian de Byl */
#ifndef SRC_LTDC_H_
#define SRC_LTDC_H_

#include <common.h>

/* LTDC_SSCR */
#define HSYNC 10
#define VSYNC 2
/* LTDC_BPCR */
#define HBP 20
#define VBP 2
/* LTDC_AWCR */
#define WIDTH 240
#define HEIGHT 320
/* LTDC_TWCR */
#define HFP 10
#define VFP 4

#define BPP 3
void init_ltdc(pin_def_t* pin_defs, uint8_t pin_defs_size);

#endif /* SRC_LTDC_H_ */
