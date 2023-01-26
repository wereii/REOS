QEMU:=qemu-system-x86_64

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
SRCDIR = $(dir $(MKFILE_PATH))

BOOTLOADER_EFI=$(SRCDIR)/src/bootloader/BOOTX64.efi

IMAGE_TARGET=$(SRCDIR)/reos.img

.PHONY: image run-image clear

image: $(IMAGE_TARGET)

$(IMAGE_TARGET): | $(BOOTLOADER_EFI)
	dd if=/dev/zero of=$@ bs=1k count=1440
	mformat -i $@ -f 1440 ::
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ $(BOOTLOADER_EFI) ::/EFI/BOOT

$(BOOTLOADER_EFI):
	$(MAKE) -C src/bootloader BOOTX64.efi

run-image: $(IMAGE_TARGET)
	$(QEMU) -bios OVMF.fd -cpu qemu64 -net none -serial stdio -drive "file=$<,index=0,media=disk,format=raw"

clean:
	$(RM) reos.img
	$(MAKE) -C src/bootloader clean