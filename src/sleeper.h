/* Copyright 2020 Bastian de Byl */
#ifndef SRC_SLEEPER_H_
#define SRC_SLEEPER_H_

#include <stdint.h>

void sleep_enable(void);
void sleep_ms(uint32_t ms);
void set_sleep_cb(void (*)(void));

#endif /* SRC_SLEEPER_H_ */
