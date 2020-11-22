/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <ili9341.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>

void wrx_cmd_ili9341(ili_init_t* ili_init, uint8_t ili_reg) {
    /* pull write (WRX) low for command */
    gpio_clear(ili_init->wrx_gpio, ili_init->wrx);

    /* pull chip-select (CSX) low (enabled) */
    gpio_clear(ili_init->csx_gpio, ili_init->csx);

    spi_send(ili_init->spi_bus, ili_reg);

    /* pull chip-select (CSX) high (disabled) */
    gpio_set(ili_init->csx_gpio, ili_init->csx);
}

void wrx_data_ili9341(ili_init_t* ili_init, uint8_t ili_data) {
    /* pull write (WRX) high for data */
    gpio_set(ili_init->wrx_gpio, ili_init->wrx);

    /* pull chip-select (CSX) low (enabled) */
    gpio_clear(ili_init->csx_gpio, ili_init->csx);

    spi_send(ili_init->spi_bus, ili_data);

    /* pull chip-select (CSX) high (disabled) */
    gpio_set(ili_init->csx_gpio, ili_init->csx);
}

static void conf_ili9341(ili_init_t* ili_init) {
    wrx_cmd_ili9341(ili_init, 0xCA);
    wrx_data_ili9341(ili_init, 0xC3);
    wrx_data_ili9341(ili_init, 0x08);
    wrx_data_ili9341(ili_init, 0x50);
    wrx_cmd_ili9341(ili_init, ILI_POWERB);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0xC1);
    wrx_data_ili9341(ili_init, 0x30);
    wrx_cmd_ili9341(ili_init, ILI_POWER_SEQ);
    wrx_data_ili9341(ili_init, 0x64);
    wrx_data_ili9341(ili_init, 0x03);
    wrx_data_ili9341(ili_init, 0x12);
    wrx_data_ili9341(ili_init, 0x81);
    wrx_cmd_ili9341(ili_init, ILI_DTCA);
    wrx_data_ili9341(ili_init, 0x85);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x78);
    wrx_cmd_ili9341(ili_init, ILI_POWERA);
    wrx_data_ili9341(ili_init, 0x39);
    wrx_data_ili9341(ili_init, 0x2C);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x34);
    wrx_data_ili9341(ili_init, 0x02);
    wrx_cmd_ili9341(ili_init, ILI_PRC);
    wrx_data_ili9341(ili_init, 0x20);
    wrx_cmd_ili9341(ili_init, ILI_DTCB);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_cmd_ili9341(ili_init, ILI_FRMCTR1);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x1B);
    wrx_cmd_ili9341(ili_init, ILI_DFC);
    wrx_data_ili9341(ili_init, 0x0A);
    wrx_data_ili9341(ili_init, 0xA2);
    wrx_cmd_ili9341(ili_init, ILI_POWER1);
    wrx_data_ili9341(ili_init, 0x10);
    wrx_cmd_ili9341(ili_init, ILI_POWER2);
    wrx_data_ili9341(ili_init, 0x10);
    wrx_cmd_ili9341(ili_init, ILI_VCOM1);
    wrx_data_ili9341(ili_init, 0x45);
    wrx_data_ili9341(ili_init, 0x15);
    wrx_cmd_ili9341(ili_init, ILI_VCOM2);
    wrx_data_ili9341(ili_init, 0x90);
    wrx_cmd_ili9341(ili_init, ILI_MAC);
    wrx_data_ili9341(ili_init, 0xC8);
    wrx_cmd_ili9341(ili_init, ILI_3GAMMA_EN);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_cmd_ili9341(ili_init, ILI_RGB_INTERFACE);
    wrx_data_ili9341(ili_init, 0xC2);
    wrx_cmd_ili9341(ili_init, ILI_DFC);
    wrx_data_ili9341(ili_init, 0x0A);
    wrx_data_ili9341(ili_init, 0xA7);
    wrx_data_ili9341(ili_init, 0x27);
    wrx_data_ili9341(ili_init, 0x04);

    /* Colomn address set */
    wrx_cmd_ili9341(ili_init, ILI_COLUMN_ADDR);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0xEF);
    /* Page address set */
    wrx_cmd_ili9341(ili_init, ILI_PAGE_ADDR);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x01);
    wrx_data_ili9341(ili_init, 0x3F);
    wrx_cmd_ili9341(ili_init, ILI_INTERFACE);
    wrx_data_ili9341(ili_init, 0x01);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x06);

    wrx_cmd_ili9341(ili_init, ILI_GRAM);
    /* TODO DELAY */
    /* ILI_Delay(200); */

    wrx_cmd_ili9341(ili_init, ILI_GAMMA);
    wrx_data_ili9341(ili_init, 0x01);

    wrx_cmd_ili9341(ili_init, ILI_PGAMMA);
    wrx_data_ili9341(ili_init, 0x0F);
    wrx_data_ili9341(ili_init, 0x29);
    wrx_data_ili9341(ili_init, 0x24);
    wrx_data_ili9341(ili_init, 0x0C);
    wrx_data_ili9341(ili_init, 0x0E);
    wrx_data_ili9341(ili_init, 0x09);
    wrx_data_ili9341(ili_init, 0x4E);
    wrx_data_ili9341(ili_init, 0x78);
    wrx_data_ili9341(ili_init, 0x3C);
    wrx_data_ili9341(ili_init, 0x09);
    wrx_data_ili9341(ili_init, 0x13);
    wrx_data_ili9341(ili_init, 0x05);
    wrx_data_ili9341(ili_init, 0x17);
    wrx_data_ili9341(ili_init, 0x11);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_cmd_ili9341(ili_init, ILI_NGAMMA);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x16);
    wrx_data_ili9341(ili_init, 0x1B);
    wrx_data_ili9341(ili_init, 0x04);
    wrx_data_ili9341(ili_init, 0x11);
    wrx_data_ili9341(ili_init, 0x07);
    wrx_data_ili9341(ili_init, 0x31);
    wrx_data_ili9341(ili_init, 0x33);
    wrx_data_ili9341(ili_init, 0x42);
    wrx_data_ili9341(ili_init, 0x05);
    wrx_data_ili9341(ili_init, 0x0C);
    wrx_data_ili9341(ili_init, 0x0A);
    wrx_data_ili9341(ili_init, 0x28);
    wrx_data_ili9341(ili_init, 0x2F);
    wrx_data_ili9341(ili_init, 0x0F);

    wrx_cmd_ili9341(ili_init, ILI_SLEEP_OUT);
    /* TODO DELAY */
    /* ILI_Delay(200); */
    wrx_cmd_ili9341(ili_init, ILI_DISPLAY_ON);
    /* GRAM start writing */
    wrx_cmd_ili9341(ili_init, ILI_GRAM);
}

void init_ili9341(pin_def_t* ili_pin_defs, uint8_t size_defs,
                  pin_def_t* ili_spi_pin_defs, uint8_t size_spi_defs,
                  ili_init_t* ili_init) {
    /* initialize the base GPIO */
    init_pin_defs_c(ili_pin_defs, size_defs);
    init_pin_defs_af(ili_spi_pin_defs, size_spi_defs, GPIO_AF5);

    /* enable the SPI clock defined in the ili_init_t struct */
    rcc_periph_clock_enable(ili_init->spi_rcc);
    spi_init_master(ili_init->spi_bus, SPI_CR1_BAUDRATE_FPCLK_DIV_256,
                    SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);
    spi_enable(ili_init->spi_bus);

    conf_ili9341(ili_init);
}
