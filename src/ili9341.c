/* Copyright 2020 Bastian de Byl */
#include <ili9341.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

static pin_def_t ili9341_pins[3] = {
    {RCC_GPIOC, GPIOC, ILI9341_CSX},
    {RCC_GPIOD, GPIOD, ILI9341_RDX | ILI9341_WRX},
    {RCC_GPIOF, GPIOF, ILI9341_SCK | ILI9341_MISO | ILI9341_MOSI},
};

void init_ili9341_gpio() {
    uint32_t i;
    for (i = 0; i < 3; ++i) {
        rcc_periph_clock_enable(ili9341_pins[i].rcc);
    }
}
