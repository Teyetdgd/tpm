APP := nexus-hub
LUMIA_SDK ?= ../../sdk/toolchain
CC ?= gcc
LD ?= ld

.PHONY: all clean
all: build/$(APP).lxe

build/$(APP).lxe: src/main.c
	mkdir -p build
	$(CC) -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -fno-exceptions -O2 -Wall -Wextra -I$(LUMIA_SDK) -c src/main.c -o build/main.o
	$(CC) -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -fno-exceptions -O2 -Wall -Wextra -I$(LUMIA_SDK) -c $(LUMIA_SDK)/lumia_sys.c -o build/lumia_sys.o
	nasm -f elf32 $(LUMIA_SDK)/crt0.s -o build/crt0.o
	$(LD) -m elf_i386 -T $(LUMIA_SDK)/linker.ld --emit-relocs -nostdlib build/crt0.o build/main.o build/lumia_sys.o -o build/$(APP).elf
	$(MAKE) -C $(LUMIA_SDK) elf2lxe
	$(LUMIA_SDK)/elf2lxe build/$(APP).elf build/$(APP).lxe --ring3

clean:
	rm -rf build
