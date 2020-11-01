// Copyright 2020 Bastian de Byl
#ifndef SRC_LTDC_H_
#define SRC_LTDC_H_

#include <common.h>
#include <libopencm3/stm32/gpio.h>

// RED
#define LCD_R2 GPIO10
#define LCD_R3 GPIO0
#define LCD_R4 GPIO11
#define LCD_R5 GPIO12
#define LCD_R6 GPIO1
#define LCD_R7 GPIO6

// GREEN
#define LCD_G2 GPIO6
#define LCD_G3 GPIO10
#define LCD_G4 GPIO10
#define LCD_G5 GPIO11
#define LCD_G6 GPIO7
#define LCD_G7 GPIO3

// BLUE
#define LCD_B2 GPIO6
#define LCD_B3 GPIO11
#define LCD_B4 GPIO12
#define LCD_B5 GPIO3
#define LCD_B6 GPIO8
#define LCD_B7 GPIO9

// IM
#define LCD_IM0 GPIO2
#define LCD_IM1 GPIO4
#define LCD_IM2 GPIO5
#define LCD_IM3 GPIO7

void init_ltdc_rcc(void);
void init_ltdc_gpio(void);
void init_ltdc(void);

#endif  // SRC_LTDC_H_
