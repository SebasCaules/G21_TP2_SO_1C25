
all:  bootloader kernel userland image

bootloader:
	cd 0_Bootloader; make all

kernel:
	cd 3_Kernel; make all

userland:
	cd 4_Userland; make all

image: kernel bootloader userland
	cd 1_Image; make all

clean:
	cd 0_Bootloader; make clean
	cd 1_Image; make clean
	cd 3_Kernel; make clean
	cd 4_Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
