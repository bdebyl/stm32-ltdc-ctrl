/* Copyright 2020 Bastian de Byl */
#include <common.h>
#include <ili9341.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <ltdc.h>
#include <sleeper.h>

static void _wait_for_spi(uint32_t spi) {
    while (!(SPI_SR(spi) & SPI_SR_TXE))
        ;
    while (SPI_SR(spi) & SPI_SR_BSY)
        ;
}

void wrx_cmd_ili9341(ili_init_t* ili_init, uint8_t ili_reg) {
    /* pull write (WRX) low for command */
    gpio_clear(ili_init->wrx_gpio, ili_init->wrx);

    /* pull chip-select (CSX) low (enabled) */
    gpio_clear(ili_init->csx_gpio, ili_init->csx);

    spi_send(ili_init->spi_bus, ili_reg);
    _wait_for_spi(ili_init->spi_bus);

    /* pull chip-select (CSX) high (disabled) */
    gpio_set(ili_init->csx_gpio, ili_init->csx);
}

void wrx_data_ili9341(ili_init_t* ili_init, uint8_t ili_data) {
    /* pull write (WRX) high for data */
    gpio_set(ili_init->wrx_gpio, ili_init->wrx);

    /* pull chip-select (CSX) low (enabled) */
    gpio_clear(ili_init->csx_gpio, ili_init->csx);

    spi_send(ili_init->spi_bus, ili_data);
    _wait_for_spi(ili_init->spi_bus);

    /* pull chip-select (CSX) high (disabled) */
    gpio_set(ili_init->csx_gpio, ili_init->csx);
}

static void conf_ili9341(ili_init_t* ili_init) {
    /* ILI9341 initialization sequence */

    /* software reset */
    wrx_cmd_ili9341(ili_init, ILI9341_SOFTWARE_RESET);

    /* display out of sleep mode */
    wrx_cmd_ili9341(ili_init, ILI9341_SLEEP_OUT);

    /* set memory access control */
    wrx_cmd_ili9341(ili_init, ILI9341_MEMORY_ACCESS_CONTROL);
    wrx_data_ili9341(ili_init, 0x00);

    /* set frame rate control */
    wrx_cmd_ili9341(ili_init, ILI9341_FRAME_RATE_CONTROL_1);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x1B);

    /* set display function control */
    wrx_cmd_ili9341(ili_init, ILI9341_DISPLAY_FUNCTION_CONTROL);
    wrx_data_ili9341(ili_init, 0x0A);
    wrx_data_ili9341(ili_init, 0xA7);
    wrx_data_ili9341(ili_init, 0x27);
    wrx_data_ili9341(ili_init, 0x04);

    /* select pixel data format*/
    wrx_cmd_ili9341(ili_init, ILI9341_PIXEL_FORMAT_SET);
    wrx_data_ili9341(ili_init, 0x66);

    /* configure RGB interface */
    wrx_cmd_ili9341(ili_init, ILI9341_RGB_INTERFACE_CONTROL);
    wrx_data_ili9341(ili_init, 0xC2);

    /* configure blanking porch */
    /* wrx_cmd_ili9341(ili_init, ILI9341_BLANKING_PORCH_CONTROL);
     * wrx_data_ili9341(ili_init, VFP);
     * wrx_data_ili9341(ili_init, VBP);
     * wrx_data_ili9341(ili_init, HFP);
     * wrx_data_ili9341(ili_init, HBP); */

    /* enable display */
    wrx_cmd_ili9341(ili_init, ILI9341_DISPLAY_ON);

    /* delay */
    sleep_ms(120);

    /* select RGB interface */
    wrx_cmd_ili9341(ili_init, ILI9341_INTERFACE_CONTROL);
    wrx_data_ili9341(ili_init, 0x01);
    wrx_data_ili9341(ili_init, 0x00);
    wrx_data_ili9341(ili_init, 0x06);
}

void init_ili9341(pin_def_t* ili_pin_defs, uint8_t size_defs,
                  pin_def_t* ili_spi_pin_defs, uint8_t size_spi_defs,
                  ili_init_t* ili_init) {
    /* initialize the base GPIO */
    init_pin_defs_c(ili_pin_defs, size_defs);
    gpio_set(ili_init->csx_gpio, ili_init->csx);
    init_pin_defs_af(ili_spi_pin_defs, size_spi_defs, GPIO_AF5);

    /* enable the SPI clock defined in the ili_init_t struct */
    rcc_periph_clock_enable(ili_init->spi_rcc);
    /* Reset SPI, SPI_CR1 register cleared, SPI is disabled */
    spi_reset(ili_init->spi_bus);

    spi_init_master(ili_init->spi_bus, SPI_CR1_BAUDRATE_FPCLK_DIV_64,
                    SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT,
                    SPI_CR1_MSBFIRST);
    spi_enable_software_slave_management(ili_init->spi_bus);
    spi_set_nss_high(ili_init->spi_bus);

    spi_enable(ili_init->spi_bus);

    sleep_enable();
    conf_ili9341(ili_init);
}
