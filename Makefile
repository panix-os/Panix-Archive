
# sudo apt-get install g++ binutils
# sudo apt-get install VirtualBox grub-pc:i386 xorriso

CXX_SRC = $(wildcard src/*.cpp src/*/*.cpp)
HEADERS = $(wildcard include/*.h include/*.hpp)

AS = i386-elf-as
GCC = i386-elf-gcc
GDB = i386-elf-gdb
LD = i386-elf-ld
NASM = i386-elf-nasm

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	AS = as
	GCC = gcc
	GDB = gdb
	LD = ld
	NASM = nasm
endif

GCC_FLAGS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
AS_FLAGS = --32
LD_FLAGS = -melf_i386

LINKER = src/boot/linker.ld

OBJ = src/boot/loader.o $(CXX_SRC:.cpp=.o)

%.o: %.cpp $(HEADERS)
	$(GCC) $(GCC_FLAGS) -c -o $@ $<

%.o: %.s
	$(AS) $(AS_FLAGS) -o $@ $<

dist/panix.bin: $(LINKER) $(OBJ)
	@ mkdir -p dist
	$(LD) $(LD_FLAGS) -T $< -o $@ $(OBJ)

dist/panix.iso: dist/panix.bin
	@ echo Making iso directory...
	@ mkdir -p iso
	@ mkdir -p iso/boot
	@ mkdir -p iso/boot/grub
	@ cp $< iso/boot/

	@ echo Creating grub.cfg...
	@ echo 'set timeout=0'                      > iso/boot/grub/grub.cfg
	@ echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	@ echo ''                                  >> iso/boot/grub/grub.cfg
	@ echo 'menuentry "Panix" {'               >> iso/boot/grub/grub.cfg
	@ echo '  multiboot /boot/panix.bin'       >> iso/boot/grub/grub.cfg
	@ echo '  boot'                            >> iso/boot/grub/grub.cfg
	@ echo '}'                                 >> iso/boot/grub/grub.cfg

	@ echo Creating panix.iso...
	@ grub-mkrescue -o dist/panix.iso iso

	@ echo Cleaning up iso directory
	@ rm -rf iso

run: dist/panix.iso
	qemu-system-i386 $< 

install: dist/panix.bin
	sudo cp $< /boot/panix.bin

clean:
	@ echo Cleaning src directory...
	@ rm -rf src/*.o src/*/*.o

	@ echo Cleaning bin files...
	@ rm -rf dist/*.bin

	@ echo "\nDone cleaning!"