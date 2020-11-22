/* Copyright 2020 Bastian de Byl */
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <main.h>
#include <src/common.h>
#include <src/ili9341.h>
#include <src/ltdc.h>
#include <src/sdram.h>

/*  _ _ _ ___ _____ _  _
 * (_) (_) _ \__ / | |/ |
 * | | | \_, /|_ \_  _| |
 * |_|_|_|/_/|___/ |_||_|
 *                        */
static ili_init_t ili9341_init = {
    .spi_rcc = RCC_SPI1,
    .spi_bus = SPI1,
    .csx = ILI9341_CSX,
    .rdx = ILI9341_RDX,
    .wrx = ILI9341_WRX,
};

static pin_def_t ili_pin_defs[] = {
    {RCC_GPIOC, GPIOC, ILI9341_CSX},
    {RCC_GPIOD, GPIOD, ILI9341_RDX | ILI9341_WRX},
};
uint8_t ili_pin_defs_size = 2;

static pin_def_t ili_spi_pin_defs[] = {
    {RCC_GPIOF, GPIOF, ILI9341_SCK | ILI9341_MISO | ILI9341_MOSI},
};
uint8_t ili_spi_pin_defs_size = 1;

/*         _
 *  ___ __| |_ _ __ _ _ __
 * (_-</ _` | '_/ _` | '  \
 * /__/\__,_|_| \__,_|_|_|_|
 *                           */
static pin_def_t sdram_pin_defs[] = {
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
uint8_t sdram_pin_defs_size = 6;

/*  _ _      _
 * | | |_ __| |__
 * | |  _/ _` / _|
 * |_|\__\__,_\__|
 *                 */
static pin_def_t ltdc_pin_defs[] = {
    {.rcc = RCC_GPIOA,
     .gpio = GPIOA,
     .pins = LCD_R4 | LCD_R5 | LCD_G2 | LCD_B5},
    {.rcc = RCC_GPIOB,
     .gpio = GPIOB,
     .pins = LCD_R3 | LCD_R6 | LCD_G4 | LCD_G5 | LCD_B7},
    {.rcc = RCC_GPIOC, .gpio = GPIOC, .pins = LCD_R2 | LCD_G6 | LCD_HSYNC},
    {.rcc = RCC_GPIOD,
     .gpio = GPIOD,
     .pins = LCD_G7 | LCD_B2 | LCD_IM0 | LCD_IM1 | LCD_IM2 | LCD_IM3},
    {.rcc = RCC_GPIOG,
     .gpio = GPIOG,
     .pins = LCD_R7 | LCD_G3 | LCD_B3 | LCD_B4 | LCD_CLK},
};
uint8_t ltdc_pin_defs_size = 5;

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    init_sdram(sdram_pin_defs, sdram_pin_defs_size);
    init_ltdc(ltdc_pin_defs, ltdc_pin_defs_size);

    init_ili9341(ili_pin_defs, ili_pin_defs_size, ili_spi_pin_defs,
                 ili_spi_pin_defs_size, &ili9341_init);

    while (1) {
        /* do nothing */
    }

    return 0;
}
