/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <ili9341.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

/* void wrx_cmd_ili9341(uint32_t, uint32_t, uint32_t, uint8_t){}; */
/* void wrx_data_ili9341(uint32_t, uint32_t, uint32_t){}; */

void init_ili9341(pin_def_t* ili_pin_defs, pin_def_t* ili_spi_pin_defs) {
    init_pin_defs_c(ili_pin_defs);
    init_pin_defs_af(ili_spi_pin_defs, GPIO_AF5);
}
