/* Copyright 2020 Bastian de Byl */
#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_
#include <common.h>

/* GPIOC */
#define ILI9341_CSX GPIO2 /* Chip Select */

/* GPIOD */
#define ILI9341_RDX GPIO12 /* Read (low for command; high for data) */
#define ILI9341_WRX GPIO13 /* Write Pin (low for command; high for data) */

/* GPIOF */
#define ILI9341_SCK GPIO7  /* SPI Clock */
#define ILI9341_MISO GPIO8 /* SPI MISO */
#define ILI9341_MOSI GPIO9

#endif /* SRC_ILI9341_H_ */
