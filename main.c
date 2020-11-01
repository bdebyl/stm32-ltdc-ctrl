#include "main.h"

#include <libopencm3/stm32/rcc.h>

#include "src/ltdc.h"
#include "src/sdram.h"

int main(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    init_sdram();
    init_ltdc();

    while (1)
        ;

    return 0;
}
