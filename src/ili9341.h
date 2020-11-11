/* Copyright 2020 Bastian de Byl */
#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_

#include <common.h>
#include <stdint.h>

void init_ili9341(pin_def_t *, pin_def_t *);
void wrx_cmd_ili9341(uint32_t ili_port, uint32_t ili_wrx, uint32_t ili_csx,
                     uint32_t spi, uint8_t cmd);
void wrx_data_ili9341(uint32_t, uint32_t, uint32_t);

#endif /* SRC_ILI9341_H_ */
