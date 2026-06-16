CC = clang
OBJCOPY = llvm-objcopy

VERSION := 3

# avd-version = 3 in device tree
ifeq ($(VERSION),2)
PAD = 0xc000
else
PAD = 0x10000
endif

LD_SCRIPT = avd-cm3.ld
CFLAGS = -Wall -O2 -nostdlib \
	 -mthumb -mcpu=cortex-m3 --target=arm-none-eabi \
	 -T $(LD_SCRIPT) \
	 -DVERSION=$(VERSION)


NAME = avd-fw-v$(VERSION)

.PHONY: all clean
all: build/$(NAME).bin
clean:
	rm -rf build/*

build/$(NAME).elf: main.c
	mkdir -p build
	$(CC) $(CFLAGS) -o $@ $<

build/$(NAME).bin: build/$(NAME).elf
	$(OBJCOPY) -O binary --strip-debug --pad-to=$(PAD) $< $@
