
all:  bootloader kernel userland image

bootloader:
	cd 0_Bootloader; make all

toolchain:
	cd 2_Toolchain; make all

kernel:
	cd 3_Kernel; make all

userland:
	cd 4_Userland; make all

image: kernel bootloader userland
	cd 1_Image; make all

run: image
	./run.sh

run-gdb:
	./run.sh gdb

test:
	cd 7_Test; make all

clean:
	cd 0_Bootloader; make clean
	cd 1_Image; make clean
	cd 2_Toolchain; make clean
	cd 3_Kernel; make clean
	cd 4_Userland; make clean

clean-test:
	cd 7_Test; make clean

clean-all: clean clean-test

.PHONY: bootloader image collections kernel userland all clean
