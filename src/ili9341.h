/* Copyright 2020 Bastian de Byl */
#ifndef SRC_ILI9341_H_
#define SRC_ILI9341_H_

#include <common.h>
#include <stdint.h>

/* Level 1 Commands */
#define ILI_SWRESET 0x01         /* Software Reset */
#define ILI_READ_DISPLAY_ID 0x04 /* Read display identification information */
#define ILI_RDDST 0x09           /* Read Display Status */
#define ILI_RDDPM 0x0A           /* Read Display Power Mode */
#define ILI_RDDMADCTL 0x0B       /* Read Display MADCTL */
#define ILI_RDDCOLMOD 0x0C       /* Read Display Pixel Format */
#define ILI_RDDIM 0x0D           /* Read Display Image Format */
#define ILI_RDDSM 0x0E           /* Read Display Signal Mode */
#define ILI_RDDSDR 0x0F          /* Read Display Self-Diagnostic Result */
#define ILI_SPLIN 0x10           /* Enter Sleep Mode */
#define ILI_SLEEP_OUT 0x11       /* Sleep out register */
#define ILI_PTLON 0x12           /* Partial Mode ON */
#define ILI_NORMAL_MODE_ON 0x13  /* Normal Display Mode ON */
#define ILI_DINVOFF 0x20         /* Display Inversion OFF */
#define ILI_DINVON 0x21          /* Display Inversion ON */
#define ILI_GAMMA 0x26           /* Gamma register */
#define ILI_DISPLAY_OFF 0x28     /* Display off register */
#define ILI_DISPLAY_ON 0x29      /* Display on register */
#define ILI_COLUMN_ADDR 0x2A     /* Colomn address register */
#define ILI_PAGE_ADDR 0x2B       /* Page address register */
#define ILI_GRAM 0x2C            /* GRAM register */
#define ILI_RGBSET 0x2D          /* Color SET */
#define ILI_RAMRD 0x2E           /* Memory Read */
#define ILI_PLTAR 0x30           /* Partial Area */
#define ILI_VSCRDEF 0x33         /* Vertical Scrolling Definition */
#define ILI_TEOFF 0x34           /* Tearing Effect Line OFF */
#define ILI_TEON 0x35            /* Tearing Effect Line ON */
#define ILI_MAC 0x36             /* Memory Access Control register*/
#define ILI_VSCRSADD 0x37        /* Vertical Scrolling Start Address */
#define ILI_IDMOFF 0x38          /* Idle Mode OFF */
#define ILI_IDMON 0x39           /* Idle Mode ON */
#define ILI_PIXEL_FORMAT 0x3A    /* Pixel Format register */
#define ILI_WRITE_MEM_CONTINUE 0x3C /* Write Memory Continue */
#define ILI_READ_MEM_CONTINUE 0x3E  /* Read Memory Continue */
#define ILI_SET_TEAR_SCANLINE 0x44  /* Set Tear Scanline */
#define ILI_GET_SCANLINE 0x45       /* Get Scanline */
#define ILI_WDB 0x51                /* Write Brightness Display register */
#define ILI_RDDISBV 0x52            /* Read Display Brightness */
#define ILI_WCD 0x53                /* Write Control Display register*/
#define ILI_RDCTRLD 0x54            /* Read CTRL Display */
#define ILI_WRCABC 0x55     /* Write Content Adaptive Brightness Control */
#define ILI_RDCABC 0x56     /* Read Content Adaptive Brightness Control */
#define ILI_WRITE_CABC 0x5E /* Write CABC Minimum Brightness */
#define ILI_READ_CABC 0x5F  /* Read CABC Minimum Brightness */
#define ILI_READ_ID1 0xDA   /* Read ID1 */
#define ILI_READ_ID2 0xDB   /* Read ID2 */
#define ILI_READ_ID3 0xDC   /* Read ID3 */

/* Level 2 Commands */
#define ILI_RGB_INTERFACE 0xB0 /* RGB Interface Signal Control */
#define ILI_FRMCTR1 0xB1       /* Frame Rate Control (In Normal Mode) */
#define ILI_FRMCTR2 0xB2       /* Frame Rate Control (In Idle Mode) */
#define ILI_FRMCTR3 0xB3       /* Frame Rate Control (In Partial Mode) */
#define ILI_INVTR 0xB4         /* Display Inversion Control */
#define ILI_BPC 0xB5           /* Blanking Porch Control register */
#define ILI_DFC 0xB6           /* Display Function Control register */
#define ILI_ETMOD 0xB7         /* Entry Mode Set */
#define ILI_BACKLIGHT1 0xB8    /* Backlight Control 1 */
#define ILI_BACKLIGHT2 0xB9    /* Backlight Control 2 */
#define ILI_BACKLIGHT3 0xBA    /* Backlight Control 3 */
#define ILI_BACKLIGHT4 0xBB    /* Backlight Control 4 */
#define ILI_BACKLIGHT5 0xBC    /* Backlight Control 5 */
#define ILI_BACKLIGHT7 0xBE    /* Backlight Control 7 */
#define ILI_BACKLIGHT8 0xBF    /* Backlight Control 8 */
#define ILI_POWER1 0xC0        /* Power Control 1 register */
#define ILI_POWER2 0xC1        /* Power Control 2 register */
#define ILI_VCOM1 0xC5         /* VCOM Control 1 register */
#define ILI_VCOM2 0xC7         /* VCOM Control 2 register */
#define ILI_NVMWR 0xD0         /* NV Memory Write */
#define ILI_NVMPKEY 0xD1       /* NV Memory Protection Key */
#define ILI_RDNVM 0xD2         /* NV Memory Status Read */
#define ILI_READ_ID4 0xD3      /* Read ID4 */
#define ILI_PGAMMA 0xE0        /* Positive Gamma Correction register */
#define ILI_NGAMMA 0xE1        /* Negative Gamma Correction register */
#define ILI_DGAMCTRL1 0xE2     /* Digital Gamma Control 1 */
#define ILI_DGAMCTRL2 0xE3     /* Digital Gamma Control 2 */
#define ILI_INTERFACE 0xF6     /* Interface control register */

/* Extend register commands */
#define ILI_POWERA 0xCB    /* Power control A register */
#define ILI_POWERB 0xCF    /* Power control B register */
#define ILI_DTCA 0xE8      /* Driver timing control A */
#define ILI_DTCB 0xEA      /* Driver timing control B */
#define ILI_POWER_SEQ 0xED /* Power on sequence register */
#define ILI_3GAMMA_EN 0xF2 /* 3 Gamma enable register */
#define ILI_PRC 0xF7       /* Pump ratio control register */

typedef struct _ili_init_t {
  uint32_t spi_rcc;
  uint32_t spi_bus;
  uint32_t wrx_gpio;
  uint32_t wrx;
  uint32_t csx_gpio;
  uint32_t csx;
  uint32_t rdx_gpio;
  uint32_t rdx;
} ili_init_t;

void init_ili9341(pin_def_t* ili_pin_defs, uint8_t size_defs,
                  pin_def_t* ili_spi_pin_defs, uint8_t size_spi_defs,
                  ili_init_t* ili_init);

void wrx_cmd_ili9341(ili_init_t* ili_init, uint8_t ili_reg);
void wrx_data_ili9341(ili_init_t* ili_init, uint8_t ili_data);

#endif /* SRC_ILI9341_H_ */
