###############################################################################
#	makefile
#	 by Sunghan Park
#
#	A makefile script for generation of Markhor kernel images.
###############################################################################

# C compiler & Tools
CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-gcc
#LD = arm-none-eabi-ld
OBJDUMP = arm-none-eabi-objdump
OBJCOPY = arm-none-eabi-objcopy
NM = arm-none-eabi-nm

# The directory in which source files are stored.
SRCDIR = srcs

# The intermediate directory for compiled object files.
BUILDDIR = build

# Compiler flags
CFLAGS = -O3 -mfpu=vfp -mfloat-abi=softfp -march=armv6zk -mtune=arm1176jzf-s -c
#CFLAGS = -O0 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c
LDFLAGS = -Wno-undef -nostartfiles -T kernel.ld
#LDFLAGS = -Wl,-verbose
ASFLAGS = -mfpu=vfp -I $(SRCDIR)

# The name of the output file to generate.
TARGET = kernel.img

# The name of the assembler listing file to generate.
DISASM = kernel.disasm
NAMELIST = kernel.nm

# The names of all object files that must be generated. Deduced from the
# C source code files in source.
C_OBJS := $(patsubst $(SRCDIR)/%.c, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.c))
ASM_OBJS := $(patsubst $(SRCDIR)/%.s, $(BUILDDIR)/%.o, $(wildcard $(SRCDIR)/*.s))
OBJECTS := $(ASM_OBJS) $(C_OBJS)

# Rule to make everything.
all: $(TARGET) $(DISASM)

# Rule to remake everything. Does not include clean.
rebuild: all

# Rule to make the disassem file & the symbol list file.
$(DISASM) : $(BUILDDIR)/kernel.elf
	$(OBJDUMP) -D $(BUILDDIR)/kernel.elf > $(DISASM)
	$(NM) $(BUILDDIR)/kernel.elf > ${NAMELIST}

# Rule to make the kernel.img file.
$(TARGET) : $(BUILDDIR)/kernel.elf
	$(OBJCOPY) $(BUILDDIR)/kernel.elf -O binary $(TARGET)
	
# Rule to make the kernel.elf file.
$(BUILDDIR)/kernel.elf : $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(BUILDDIR)/kernel.elf

# Rule to make the object files for C sources.
$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

# Rule to make the object files for Assembly sources.
$(BUILDDIR)/%.o: $(SRCDIR)/%.s $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@

# Rule to make build directory
$(BUILDDIR):
	mkdir $@

# Rule to clean files.
clean :
	-rm -rf $(BUILDDIR)
	-rm -f $(TARGET)
	-rm -f $(DISASM)
	-rm -f $(NAMELIST)
