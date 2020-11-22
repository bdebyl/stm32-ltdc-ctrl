/* Copyright 2020 Bastian de Byl */
#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_

#include <common.h>
#include <stdint.h>

typedef struct _ili_init_t {
  uint32_t spi_rcc;
  uint32_t spi_bus;
  uint32_t wrx;
  uint32_t csx;
  uint32_t rdx;
} ili_init_t;

void init_ili9341(pin_def_t* ili_pin_defs, uint8_t size_defs,
                  pin_def_t* ili_spi_pin_defs, uint8_t size_spi_defs,
                  ili_init_t* ili_init);

void wrx_cmd_ili9341(ili_init_t ili_init, uint8_t ili_reg);
void wrx_data_ili9341(ili_init_t ili_init, uint8_t ili_data);

#endif /* SRC_ILI9341_H_ */
