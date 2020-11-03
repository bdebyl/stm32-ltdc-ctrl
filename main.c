/* Copyright 2020 Bastian de Byl */
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <main.h>
#include <src/common.h>
#include <src/ltdc.h>
// #include <src/sdram.h>

static pin_def_t ltdc_pin_defs[] = {
    {.rcc = RCC_GPIOA,
     .gpio = GPIOA,
     .pins = LCD_R4 | LCD_R5 | LCD_G2 | LCD_B5},
    {.rcc = RCC_GPIOB,
     .gpio = GPIOB,
     .pins = LCD_R3 | LCD_R6 | LCD_G4 | LCD_G5 | LCD_B7},
    {.rcc = RCC_GPIOC, .gpio = GPIOC, .pins = LCD_R2 | LCD_G6},
    {.rcc = RCC_GPIOD,
     .gpio = GPIOD,
     .pins = LCD_G7 | LCD_B2 | LCD_IM0 | LCD_IM1 | LCD_IM2 | LCD_IM3},
    {.rcc = RCC_GPIOG,
     .gpio = GPIOG,
     .pins = LCD_R7 | LCD_G3 | LCD_B3 | LCD_B4},
};

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* init_sdram(); */
    init_ltdc(ltdc_pin_defs);

    while (1) {
        /* do nothing */
    }

    return 0;
}
