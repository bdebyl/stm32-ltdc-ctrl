/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void init_pin_defs_c(pin_def_t* pin_defs, uint8_t pin_defs_size) {
    uint8_t i;
    for (i = 0; i < pin_defs_size; ++i) {
        rcc_periph_clock_enable(pin_defs[i].rcc);
        gpio_mode_setup(pin_defs[i].gpio, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                        pin_defs[i].pins);
    }
}

void init_pin_defs_af(pin_def_t pin_defs[], uint8_t pin_defs_size,
                      uint32_t pin_defs_af) {
    uint8_t i;
    for (i = 0; i < pin_defs_size; ++i) {
        rcc_periph_clock_enable(pin_defs[i].rcc);

        gpio_mode_setup(pin_defs[i].gpio, GPIO_MODE_AF, GPIO_PUPD_NONE,
                        pin_defs[i].pins);

        gpio_set_output_options(pin_defs[i].gpio, GPIO_OTYPE_PP,
                                GPIO_OSPEED_50MHZ, pin_defs[i].pins);

        gpio_set_af(pin_defs[i].gpio, pin_defs_af, pin_defs[i].pins);
    }
}
