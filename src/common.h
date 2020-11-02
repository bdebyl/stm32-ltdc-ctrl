/* Copyright 2020 Bastian de Byl */
#ifndef SRC_COMMON_H_
#define SRC_COMMON_H_

#include <stdint.h>

typedef struct _pin_def_t {
  uint32_t rcc;
  uint32_t gpio;
  uint32_t pins;
} pin_def_t;

void init_pin_defs(pin_def_t*, uint32_t);

#endif /* SRC_COMMON_H_ */
