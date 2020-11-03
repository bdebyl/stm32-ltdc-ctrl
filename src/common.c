/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void init_pin_defs_c(pin_def_t* pin_def) {
    uint32_t i;
    for (i = 0; i < sizeof(*pin_def) / sizeof(pin_def[0]); ++i) {
        rcc_periph_clock_enable(pin_def[i].rcc);
        gpio_mode_setup(pin_def[i].gpio, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                        pin_def[i].pins);
    }
}

void init_pin_defs_af(pin_def_t* pin_def, uint32_t gpio_af) {
    uint32_t i;
    for (i = 0; i < sizeof(*pin_def) / sizeof(pin_def[0]); ++i) {
        rcc_periph_clock_enable(pin_def[i].rcc);
        gpio_mode_setup(pin_def[i].gpio, GPIO_MODE_AF, GPIO_PUPD_NONE,
                        pin_def[i].pins);

        gpio_set_output_options(pin_def[i].gpio, GPIO_OTYPE_PP,
                                GPIO_OSPEED_50MHZ, pin_def[i].pins);

        gpio_set_af(pin_def[i].gpio, gpio_af, pin_def[i].pins);
    }
}
