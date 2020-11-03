OBJS = main.o $(patsubst %.c,%.o,$(wildcard src/*.c))

OPENCM3_DIR := ./libopencm3

# Our output name
BINARY = main

LDSCRIPT = stm32f4.ld #stm32f0.ld

# Using the stm32f4 series chip
LIBNAME     = opencm3_stm32f4
DEFS        += -DSTM32F4 -I./src/ -I./

# Target-specific flags
FP_FLAGS    ?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS   = -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

OOCD         = $(shell command -v openocd)
OOCD_IF      = interface/stlink-v2-1.cfg
OOCD_TG      = target/stm32f4x.cfg
STFLASH      = $(shell command -v st-flash)

# Compiler configuration
PREFIX      ?= arm-none-eabi
CC          := $(PREFIX)-gcc
LD          := $(PREFIX)-gcc
AR          := $(PREFIX)-ar
AS          := $(PREFIX)-as
SIZE        := $(PREFIX)-size
OBJCOPY     := $(PREFIX)-objcopy
OBJDUMP     := $(PREFIX)-objdump
GDB         := $(PREFIX)-gdb
OPT         := -Os
DEBUG       := -ggdb3
CSTD        ?= -std=c99

# C flags
TGT_CFLAGS  += $(OPT) $(CSTD) $(DEBUG)
TGT_CFLAGS  += $(ARCH_FLAGS)
TGT_CFLAGS  += -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS  += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS  += -fno-common -ffunction-sections -fdata-sections

# C preprocessor common flags
TGT_CFLAGS    += -MD
TGT_CFLAGS    += -Wall -Wundef
TGT_CFLAGS    += $(DEFS)

# Linker flags
TGT_LDFLAGS     += --static -nostartfiles
TGT_LDFLAGS     += -T$(LDSCRIPT)
TGT_LDFLAGS     += $(ARCH_FLAGS) $(DEBUG)
TGT_LDFLAGS     += -Wl,-Map=$(*).map -Wl,--cref
TGT_LDFLAGS     += -Wl,--gc-sections

# Used libraries
DEFS        += -I$(OPENCM3_DIR)/include
LDFLAGS     += -L$(OPENCM3_DIR)/lib
LDLIBS      += -l$(LIBNAME)
LDLIBS      += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

all: size

$(OPENCM3_DIR)/lib/lib$(LIBNAME).a:
ifeq (,$(wildcard $@))
	$(warning $(LIBNAME).a not found, attempting to rebuild in $(OPENCM3_DIR))
	$(MAKE) -C $(OPENCM3_DIR)
endif

size: $(BINARY).size
elf: $(BINARY).elf
bin: $(BINARY).bin
map: $(BINARY).map

GENERATED_BINARIES=$(BINARY).elf $(BINARY).bin $(BINARY).map

%.bin: %.elf
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.elf %.map: $(OBJS) $(LDSCRIPT)
	$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(*).elf

%.o: %.c $(OPENCM3_DIR)/lib/lib$(LIBNAME).a
	$(CC) $(TGT_CFLAGS) $(CFLAGS)  -o $(*).o -c $(*).c

%.size: %.elf
	@$(SIZE) -d $(*).elf | tee $(*).size

flash: $(BINARY).elf
	@printf "  FLASH  $<\n"
	$(OOCD) -f $(OOCD_IF) -f $(OOCD_TG) -c "program $(BINARY).elf verify reset exit"

# flash-stlink: $(BINARY).elf
# 	@printf "  FLASH  $<\n"
# 	$(STFLASH) write $(BINARY).bin 0x8000000

.PHONY: reset
reset:
	$(STFLASH)

.PHONY: clean
clean:
	$(RM) $(GENERATED_BINARIES) generated.* $(OBJS) $(OBJS:%.o=%.d)
