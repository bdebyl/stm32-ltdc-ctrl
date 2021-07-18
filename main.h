/* Copyright 2020 Bastian de Byl */
#ifndef MAIN_H_
#define MAIN_H_

#ifndef NULL
#define NULL (void *)(0)
#endif

#ifndef STM32F4
#define STM32F4
#endif

#ifndef UNUSER
#define UNUSED(x) (void)(x)
#endif

#define USE_DMA_MEM2MEM 1

/* (GPIO)A */
#define LCD_B5       (GPIO3)
#define LCD_VSYNC    (GPIO4)
#define LCD_G2       (GPIO6)
#define LCD_R4       (GPIO11)
#define LCD_R5       (GPIO12)

/* (GPIO)B */
#define LCD_R3       (GPIO0)
#define LCD_R6       (GPIO1)
#define SDRAM_SDCKE1 (GPIO5)
#define SDRAM_SDNE1  (GPIO6)
#define LCD_B6       (GPIO8)
#define LCD_B7       (GPIO9)
#define LCD_G4       (GPIO10)
#define LCD_G5       (GPIO11)

/* (GPIO)C */
#define SDRAM_SDNEWE (GPIO0)
#define ILI9341_CSX  (GPIO2) /* Chip Select */
#define LCD_HSYNC    (GPIO6)
#define LCD_G6       (GPIO7)
#define LCD_R2       (GPIO10)

/* (GPIO)D */
#define SDRAM_D2     (GPIO0)
#define SDRAM_D3     (GPIO1)
#define LCD_G7       (GPIO3)
#define LCD_B2       (GPIO6)
#define SDRAM_D13    (GPIO8)
#define SDRAM_D14    (GPIO9)
#define SDRAM_D15    (GPIO10)
#define ILI9341_RDX  (GPIO12) /* Read (low for command; high for data) */
#define ILI9341_WRX  (GPIO13) /* Write Pin (low for command; high for data) */
#define SDRAM_D0     (GPIO14)
#define SDRAM_D1     (GPIO15)

/* (GPIO)E */
#define SDRAM_NBL0   (GPIO0)
#define SDRAM_NBL1   (GPIO1)
#define SDRAM_D4     (GPIO7)
#define SDRAM_D5     (GPIO8)
#define SDRAM_D6     (GPIO9)
#define SDRAM_D7     (GPIO10)
#define SDRAM_D8     (GPIO11)
#define SDRAM_D9     (GPIO12)
#define SDRAM_D10    (GPIO13)
#define SDRAM_D11    (GPIO14)
#define SDRAM_D12    (GPIO15)

/* (GPIO)F */
#define SDRAM_A0     (GPIO0)
#define SDRAM_A1     (GPIO1)
#define SDRAM_A2     (GPIO2)
#define SDRAM_A3     (GPIO3)
#define SDRAM_A4     (GPIO4)
#define SDRAM_A5     (GPIO5)
#define ILI9341_SCK  (GPIO7)
#define ILI9341_MISO (GPIO8)
#define ILI9341_MOSI (GPIO9)
#define LCD_DE       (GPIO10)
#define SDRAM_SDNRAS (GPIO11)
#define SDRAM_A6     (GPIO12)
#define SDRAM_A7     (GPIO13)
#define SDRAM_A8     (GPIO14)
#define SDRAM_A9     (GPIO15)

/* (GPIO)G */
#define SDRAM_A10    (GPIO0)
#define SDRAM_A11    (GPIO1)
#define SDRAM_A14    (GPIO4) /* FMC_BA1 */
#define SDRAM_INT2   (GPIO5)
#define LCD_R7       (GPIO6)
#define LCD_CLK      (GPIO7)
#define SDRAM_SDCLK  (GPIO8)
#define LCD_G3       (GPIO10)
#define LCD_B3       (GPIO11)
#define LCD_B4       (GPIO12)
#define SDRAM_SDNCAS (GPIO15)

/* UNUSED */
// #define LCD_IM0 (GPIO2)
// #define LCD_IM1 (GPIO4)
// #define LCD_IM2 (GPIO5)
// #define LCD_IM3 (GPIO7)
#endif /* MAIN_H_ */
