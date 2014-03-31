# This variable holds optimization options
OPTIMIZATION_CFLAGS ?= -g -ggdb -O2

# This variable holds compilation flags for user level executables
CFLAGS ?= -msoft-float -mno-mmx -mno-sse -Wall -fno-builtin \
          -Werror -fno-strict-aliasing -fno-common -pedantic \
          -std=gnu99

INCLUDE_DIRS = -Isrc/include/

# The following lines holds compiler options
KERNEL_FLAGS32    = -flto -msoft-float -mno-mmx -mno-sse -Wall -fno-builtin \
                    -Werror -fno-strict-aliasing $(OPTIMIZATION_CFLAGS) \
                    -fno-common

KERNEL_FLAGS64    = $(KERNEL_FLAGS32) -mcmodel=kernel -mno-red-zone

KERNEL_CFLAGS32   = $(KERNEL_FLAGS32) -std=gnu99 -m32 -march=i386 
KERNEL_CFLAGS64   = $(KERNEL_FLAGS64) -std=gnu99 -pedantic

# Rules for cd-image generation and booting
bochs/boot.iso : objects/kernel/32bit/kernel.stripped bochs/grub.cfg
	-rm -r bochs/iso
	-mkdir -p bochs/iso/boot/grub
	cp bochs/grub.cfg bochs/iso/boot/grub/
	cp objects/kernel/32bit/kernel.stripped bochs/iso/kernel
	grub-mkrescue -o bochs/boot.iso bochs/iso

boot: bochs/boot.iso
	(cd bochs/; nice -20 bochs -q -f bochsrc)

boot-gdb: bochs/boot.iso
	(cd bochs/; nice -20 bochs-gdb -q -f bochsrc.gdb)

# Rules for the 32-bit kernel
objects/kernel/32bit/kernel: objects/kernel/32bit/multiboot2.o objects/kernel/32bit/relocate.o objects/kernel/32bit/setup_long_mode.o src/kernel/32bit/kernel32_link.ld objects/kernel/64bit/kernel.o | objects/kernel/32bit
	x86_64-unknown-elf-gcc $(KERNEL_CFLAGS32) -nostdlib -Wl,--no-warn-mismatch -Wl,-zmax-page-size=4096 -Tsrc/kernel/32bit/kernel32_link.ld -o objects/kernel/32bit/kernel objects/kernel/32bit/multiboot2.o objects/kernel/32bit/relocate.o objects/kernel/32bit/setup_long_mode.o objects/kernel/64bit/kernel.o

objects/kernel/32bit/kernel.stripped: objects/kernel/32bit/kernel | objects/kernel/32bit
	x86_64-unknown-elf-strip -o objects/kernel/32bit/kernel.stripped objects/kernel/32bit/kernel

objects/kernel/32bit/multiboot2.o: src/kernel/32bit/multiboot2.s | objects/kernel/32bit
	x86_64-unknown-elf-as --gstabs --32 -o objects/kernel/32bit/multiboot2.o src/kernel/32bit/multiboot2.s

objects/kernel/32bit/setup_long_mode.o: src/kernel/32bit/setup_long_mode.c | objects/kernel/32bit
	x86_64-unknown-elf-gcc $(KERNEL_CFLAGS32) -c -o objects/kernel/32bit/setup_long_mode.o src/kernel/32bit/setup_long_mode.c

objects/kernel/32bit/relocate.o: src/kernel/32bit/relocate.s | objects/kernel/32bit
	x86_64-unknown-elf-as --gstabs --64 -o objects/kernel/32bit/relocate.o src/kernel/32bit/relocate.s

# Rules for the 64-bit kernel

# This variable holds object files which hold user level executables
EXECUTABLES = \
 objects/program_0/executable.o \
 objects/program_1/executable.o \
 objects/program_2/executable.o 

# This variable holds object files which are to be linked into the main
# 64-bit kernel image.
KERNEL_OBJECTS = \
 objects/kernel/64bit/system_initialization.o \
 objects/kernel/64bit/ELF_parser.o \
 objects/kernel/64bit/process_queue.o \
 objects/kernel/64bit/scheduler.o \
 objects/kernel/64bit/entry_routines.o \
 objects/kernel/64bit/video.o \
 $(EXECUTABLES)

KERNEL_SOURCES = \
 src/kernel/64bit/system_initialization.c \
 src/kernel/64bit/ELF_parser.c \
 src/kernel/64bit/process_queue.c \
 src/kernel/64bit/scheduler.c \
 src/kernel/64bit/entry_routines.c \
 src/kernel/64bit/video.c

objects/kernel/64bit/kernel.o: objects/kernel/64bit/kernel.stripped | objects/kernel/64bit
	x86_64-unknown-elf-objcopy -I binary -O elf32-i386 -B i386 --set-section-flags .data=alloc,contents,load,readonly,data objects/kernel/64bit/kernel.stripped objects/kernel/64bit/kernel.o

objects/kernel/64bit/kernel.stripped: objects/kernel/64bit/kernel | objects/kernel/64bit
	x86_64-unknown-elf-strip --strip-all -o objects/kernel/64bit/kernel.stripped objects/kernel/64bit/kernel

objects/kernel/64bit/kernel: objects/kernel/64bit/start.o objects/kernel/64bit/startap.o objects/kernel/64bit/entry_handlers.o $(KERNEL_OBJECTS) src/kernel/64bit/kernel64_link.ld | objects/kernel/64bit
	x86_64-unknown-elf-gcc $(KERNEL_CFLAGS64) -nostdlib -Wl,--no-warn-mismatch -Wl,-zmax-page-size=4096 -Tsrc/kernel/64bit/kernel64_link.ld -o objects/kernel/64bit/kernel objects/kernel/64bit/start.o objects/kernel/64bit/startap.o objects/kernel/64bit/entry_handlers.o $(KERNEL_OBJECTS)

objects/kernel/64bit/start.o: src/kernel/64bit/start.s | objects/kernel/64bit
	x86_64-unknown-elf-as --64 -o objects/kernel/64bit/start.o src/kernel/64bit/start.s

objects/kernel/64bit/startap.o: src/kernel/64bit/startap.s | objects/kernel/64bit
	x86_64-unknown-elf-as --64 -o objects/kernel/64bit/startap.o src/kernel/64bit/startap.s

objects/kernel/64bit/entry_handlers.o: src/kernel/64bit/entry_handlers.s | objects/kernel/64bit
	x86_64-unknown-elf-as --64 -o objects/kernel/64bit/entry_handlers.o src/kernel/64bit/entry_handlers.s

objects/kernel/64bit/%.d: src/kernel/64bit/%.c | objects/kernel/64bit
	@set -e; rm -f $@; \
        x86_64-unknown-elf-gcc $(KERNEL_CFLAGS64) $(INCLUDE_DIRS) -M $< > $@.$$$$; \
        sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
        rm -f $@.$$$$

objects/kernel/64bit/%.o: src/kernel/64bit/%.c objects/kernel/64bit/%.d | objects/kernel/64bit
	x86_64-unknown-elf-gcc $(KERNEL_CFLAGS64) $(INCLUDE_DIRS) -c -o $@ $<

-include $(KERNEL_SOURCES:src/kernel/64bit/%.c=objects/kernel/64bit/%.d)

# Rules for the user code
objects/program_startup_code/startup.o: src/program_startup_code/startup.s | objects/program_startup_code
	x86_64-unknown-elf-as --64 -o objects/program_startup_code/startup.o src/program_startup_code/startup.s

objects/program_0/main.o: src/program_0/main.c src/include/scwrapper.h | objects/program_0
	x86_64-unknown-elf-gcc -fPIE -m64 $(CFLAGS) $(INCLUDE_DIRS) $(OPTIMIZATIONFLAGS) -c -o objects/program_0/main.o src/program_0/main.c

objects/program_0/executable: objects/program_startup_code/startup.o objects/program_0/main.o src/program_startup_code/program_link.ld | objects/program_0
	x86_64-unknown-elf-ld  -z max-page-size=4096 -static -Tsrc/program_startup_code/program_link.ld -o objects/program_0/executable objects/program_startup_code/startup.o objects/program_0/main.o

objects/program_0/executable.stripped: objects/program_0/executable | objects/program_0
	x86_64-unknown-elf-strip -o objects/program_0/executable.stripped objects/program_0/executable

objects/program_0/executable.o: objects/program_0/executable.stripped | objects/program_0
	x86_64-unknown-elf-objcopy  -I binary -O elf64-x86-64 -B i386:x86-64 --set-section-flags .data=alloc,contents,load,readonly,data objects/program_0/executable.stripped objects/program_0/executable.o

objects/program_1/main.o: src/program_1/main.c src/include/scwrapper.h | objects/program_1
	x86_64-unknown-elf-gcc -fPIE -m64 $(CFLAGS) $(INCLUDE_DIRS) $(OPTIMIZATIONFLAGS) -c -o objects/program_1/main.o src/program_1/main.c

objects/program_1/executable: objects/program_startup_code/startup.o objects/program_1/main.o src/program_startup_code/program_link.ld | objects/program_1
	x86_64-unknown-elf-ld  -z max-page-size=4096 -static -Tsrc/program_startup_code/program_link.ld -o objects/program_1/executable objects/program_startup_code/startup.o objects/program_1/main.o

objects/program_1/executable.stripped: objects/program_1/executable | objects/program_1
	x86_64-unknown-elf-strip -o objects/program_1/executable.stripped objects/program_1/executable

objects/program_1/executable.o: objects/program_1/executable.stripped | objects/program_1
	x86_64-unknown-elf-objcopy  -I binary -O elf64-x86-64 -B i386:x86-64 --set-section-flags .data=alloc,contents,load,readonly,data objects/program_1/executable.stripped objects/program_1/executable.o

objects/program_2/main.o: src/program_2/main.c src/include/scwrapper.h | objects/program_2
	x86_64-unknown-elf-gcc -fPIE -m64 $(CFLAGS) $(INCLUDE_DIRS) $(OPTIMIZATIONFLAGS) -c -o objects/program_2/main.o src/program_2/main.c

objects/program_2/executable: objects/program_startup_code/startup.o objects/program_2/main.o src/program_startup_code/program_link.ld | objects/program_2
	x86_64-unknown-elf-ld  -z max-page-size=4096 -static -Tsrc/program_startup_code/program_link.ld -o objects/program_2/executable objects/program_startup_code/startup.o objects/program_2/main.o

objects/program_2/executable.stripped: objects/program_2/executable | objects/program_2
	x86_64-unknown-elf-strip -o objects/program_2/executable.stripped objects/program_2/executable

objects/program_2/executable.o: objects/program_2/executable.stripped | objects/program_2
	x86_64-unknown-elf-objcopy  -I binary -O elf64-x86-64 -B i386:x86-64 --set-section-flags .data=alloc,contents,load,readonly,data objects/program_2/executable.stripped objects/program_2/executable.o


# Misc rules
clean:
	-rm -rf objects bochs/iso bochs/boot.iso

objects:
	-mkdir -p objects

objects/kernel/32bit:
	-mkdir -p objects/kernel/32bit

objects/kernel/64bit:
	-mkdir -p objects/kernel/64bit

objects/program_startup_code:
	-mkdir -p objects/program_startup_code

objects/program_0:
	-mkdir -p objects/program_0

objects/program_1:
	-mkdir -p objects/program_1

objects/program_2:
	-mkdir -p objects/program_2

compile: objects/kernel/32bit/kernel

debugger: objects/kernel/64bit/kernel objects/kernel/64bit/kernel
	gdb-bochs -x gdb-commands objects/kernel/64bit/kernel	

all: boot
