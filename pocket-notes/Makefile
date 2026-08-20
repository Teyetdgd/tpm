APP := pocket-notes
LUMIA_SDK ?= ../../sdk/toolchain
include $(LUMIA_SDK)/lumia.mk
CC ?= gcc
LD ?= ld

.PHONY: all clean
all: build/$(APP).lxe build/$(APP).lxe.sha256

build/$(APP).lxe: src/main.c $(LUMIA_LXE_DEPS)
	mkdir -p build
	$(CC) $(LUMIA_CFLAGS) -c src/main.c -o build/main.o
	$(CC) $(LUMIA_CFLAGS) -c $(LUMIA_SDK)/lumia_sys.c -o build/lumia_sys.o
	nasm -f elf32 $(LUMIA_SDK)/crt0.s -o build/crt0.o
	$(LD) -m elf_i386 -T $(LUMIA_SDK)/linker.ld --emit-relocs -nostdlib build/crt0.o build/main.o build/lumia_sys.o -o build/$(APP).elf
	$(MAKE) -C $(LUMIA_SDK) elf2lxe
	$(LUMIA_SDK)/elf2lxe build/$(APP).elf build/$(APP).lxe --ring3

build/$(APP).lxe.sha256: build/$(APP).lxe
	sha256sum $< | awk '{print $$1}' > $@

clean:
	rm -rf build
