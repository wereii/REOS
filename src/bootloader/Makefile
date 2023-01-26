CC=gcc
ARCH := x86_64
OBJCOPY := objcopy

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SRCDIR = $(dir $(MKFILE_PATH))

GNUEFI_PATH=$(SRCDIR)/gnu-efi
GNUEFI_BUILD_DIR=$(GNUEFI_PATH)/$(ARCH)

CFLAGS=-std=c11 -Wno-error=pragmas -mno-red-zone -mno-avx -fPIC -g -O2 -Wall -Wextra -Werror	\
		-fshort-wchar -fno-strict-aliasing -ffreestanding -fno-stack-protector -fno-stack-check -fno-merge-all-constants -maccumulate-outgoing-args	\
		-DEFI_FUNCTION_WRAPPER -DCONFIG_x86_64 -DGNU_EFI_USE_MS_ABI -D__KERNEL__	\
		-I $(GNUEFI_PATH)/inc -I $(GNUEFI_PATH)/inc/protocol -I $(GNUEFI_PATH)/inc/$(ARCH)

LDFLAGS=-nostdlib -znocombreloc --warn-common --no-undefined --fatal-warnings --build-id=sha1 -shared -Bsymbolic \
	-L $(GNUEFI_BUILD_DIR)/gnuefi -L $(GNUEFI_BUILD_DIR)/lib \
	-T $(GNUEFI_PATH)/gnuefi/elf_$(ARCH)_efi.lds $(GNUEFI_BUILD_DIR)/gnuefi/crt0-efi-$(ARCH).o
LDLIBS=-lgnuefi -lefi
LOADLIBES=$(LDLIBS)

FORMAT= --target=efi-app-$(ARCH)

.PHONY: all gnuefilib run clean


all: gnuefilib main.efi

gnuefilib: $(GNUEFI_BUILD_DIR)

$(GNUEFI_BUILD_DIR):
	$(MAKE) -C $(SRCDIR)/gnu-efi/ lib gnuefi

run: main.efi
	uefi-run $(SRCDIR)/main.efi -- -net none -serial stdio -cpu qemu64

clean:
	$(RM) main.o main.so main.efi
	$(MAKE) -C $(GNUEFI_PATH) clean
	rm -rf $(GNUEFI_BUILD_DIR)


include $(GNUEFI_PATH)/Make.rules