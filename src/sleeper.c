/* Copyright 2020 Bastian de Byl*/
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <sleeper.h>

volatile uint32_t SYS_MILLIS;

void sys_tick_handler(void) { SYS_MILLIS++; }

void sleep_enable(void) {
    systick_set_reload(rcc_ahb_frequency / 1000 / 8);
    /* Note that STK_CSR_CLKSOURCE_AHB_DIV8 is defined for the STM32F4 and may
     * not be in other STM32Fx series */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    systick_clear();

    systick_counter_enable();
    systick_interrupt_enable();
}

void sleep_ms(uint32_t ms) {
    uint32_t wait_for = ms + SYS_MILLIS;
    while (wait_for > SYS_MILLIS)
        ;
}
