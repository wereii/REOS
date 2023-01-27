CC=clang

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SRCDIR = $(dir $(MKFILE_PATH))

CFLAGS=-target x86_64-unknown-windows -std=c11 -Wall -Wextra	\
		-ffreestanding -fno-builtin -fshort-wchar -mno-red-zone 				\
		-I$(SRCDIR)/c-efi/src -I$(SRCDIR)/include

LDFLAGS=-target x86_64-unknown-windows -nostdlib --warn-common \
	'-Wl,-entry:efi_main' '-Wl,-subsystem:efi_application' -fuse-ld=lld-link

.PHONY: all run clean

all: BOOTX64.efi

%.efi: %.o
	$(CC) $(LDFLAGS) -o $@ $<$(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

run: BOOTX64.efi
	uefi-run $(SRCDIR)/BOOTX64.efi -- -net none -serial stdio -norestart

clean:
	$(RM) *.o *.efi