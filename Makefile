CC = clang
OBJCOPY = llvm-objcopy

AVD_VER := 3
AVD_TIER := 1

PAD = 0x12000

# avd-version = 3 in device tree
ifeq ($(AVD_VER),2)
PAD = 0xc000
endif

ifeq ($(AVD_VER),3)
PAD = 0x10000
endif

ifeq ($(AVD_VER),5)
ifeq ($(AVD_TIER),0)
PAD = 0x10000
endif
endif

LD_SCRIPT = avd-cm3.ld
CFLAGS = -Wall -O2 -nostdlib \
	 -mthumb -mcpu=cortex-m3 --target=arm-none-eabi \
	 -DAVD_VER=$(AVD_VER) -DAVD_TIER=$(AVD_TIER)

NAME = avd-fw-v$(AVD_VER)-t$(AVD_TIER)

OBJECTS := util.o main.o \
	   tunable/tun-v$(AVD_VER)-t$(AVD_TIER).o

BUILD_OBJS := $(patsubst %,build/%,$(OBJECTS))

.PHONY: all clean
all: build/$(NAME).bin
clean:
	rm -rf build/*

build/$(NAME).elf: $(BUILD_OBJS)
	$(CC) $(CFLAGS) -T $(LD_SCRIPT) -o $@ $^

build/%.o: src/%.c
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c -o $@ $<

build/$(NAME).bin: build/$(NAME).elf
	$(OBJCOPY) -O binary --strip-debug --pad-to=$(PAD) $< $@
