APP := nexus-hub
LUMIA_SDK ?= ../../sdk/toolchain
CC ?= gcc
LD ?= ld

SRCS = src/main.c src/core/state.c src/core/event_loop.c \
       src/ui/theme.c src/ui/window.c src/ui/layout.c src/ui/widgets.c \
       src/modules/system_monitor.c src/modules/network_diag.c src/modules/audio_player.c \
       src/platform/storage.c src/platform/notifications.c \
       src/utils/string_utils.c

OBJS = $(patsubst src/%.c, build/%.o, $(SRCS)) build/lumia_sys.o build/crt0.o

.PHONY: all clean dirs

all: dirs build/$(APP).lxe

dirs:
	mkdir -p build/core build/ui build/modules build/platform build/utils

build/%.o: src/%.c
	$(CC) -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -fno-exceptions -O2 -Wall -Wextra -I$(LUMIA_SDK) -c $< -o $@

build/lumia_sys.o: $(LUMIA_SDK)/lumia_sys.c
	$(CC) -m32 -ffreestanding -nostdlib -fno-builtin -fno-stack-protector -fno-exceptions -O2 -Wall -Wextra -I$(LUMIA_SDK) -c $< -o $@

build/crt0.o: $(LUMIA_SDK)/crt0.s
	nasm -f elf32 $< -o $@

build/$(APP).lxe: $(OBJS)
	$(LD) -m elf_i386 -T $(LUMIA_SDK)/linker.ld --emit-relocs -nostdlib $(OBJS) -o build/$(APP).elf
	$(MAKE) -C $(LUMIA_SDK) elf2lxe
	$(LUMIA_SDK)/elf2lxe build/$(APP).elf build/$(APP).lxe --ring3

clean:
	rm -rf build
