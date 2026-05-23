# Configurations

# Platform detection
PLATFORM = EcoOS

ifeq ($(OS), Windows_NT)
	#WINDOWS USE THESE DEFINITIONS
	RM = -rm -f
	SLASH = \\
	SWALLOW_OUTPUT = >nul 2>nul
else
	#LINUX USE THESE DEFINITIONS
	RM = -rm -f
	SLASH = /
	SWALLOW_OUTPUT =
	TOOLCHAIN = 
	TOOL = 
endif 

# Target configuration
ifeq ($(TARGET), 1)
else
endif

# Debug symbols configuration
ifeq ($(DEBUG), 1)
	CONFIG_TARGET = $(BASE_TARGET)Debug
	CCFLAGS_TARGET += /Zi /Fd$(subst /,$(SLASH),$(OUT_DIR)/$(OUT_TARGET:.lib=.pdb))
	LDFLAGS_TARGET += /DEBUG
	ifeq ($(TARGET), 1)
		LDFLAGS_TARGET += /PDB:$(subst /,$(SLASH),$(OUT_DIR)/$(OUT_TARGET:.dll=.pdb))
	endif
else
	CONFIG_TARGET = $(BASE_TARGET)Release
	CCFLAGS_TARGET += /Z7
	LDFLAGS_TARGET += /DEBUG:NONE
endif

# Architecture selection
ifeq ($(ARCH), x86_16)
	CCFLAGS_TARGET +=
	LDFLAGS_TARGET +=
	ARCH_TARGET = x86_16
else ifeq ($(ARCH), x86)
	ARCH_TARGET = x86
else ifeq ($(ARCH), x86_64)
	ARCH_TARGET = amd64
else ifeq ($(ARCH), arm)
    CCFLAGS_TARGET +=   
    LDFLAGS_TARGET += 
    ARCH_TARGET = arm
else ifeq ($(ARCH), aarch32)
	CCFLAGS_TARGET +=   
	LDFLAGS_TARGET += 
	ARCH_TARGET = armv7
else ifeq ($(ARCH), aarch64)
	CCFLAGS_TARGET +=   
	LDFLAGS_TARGET += 
	ARCH_TARGET = armv8
else ifeq ($(ARCH), rv32)
	CCFLAGS_TARGET +=   
	LDFLAGS_TARGET += 
	ARCH_TARGET = rv32
else ifeq ($(ARCH), rv64)
	CCFLAGS_TARGET +=   
	LDFLAGS_TARGET += 
	ARCH_TARGET = rv64gcv
else ifeq ($(ARCH), mips)
    CCFLAGS_TARGET +=   
    LDFLAGS_TARGET += 
    ARCH_TARGET = mips
else ifeq ($(ARCH), mips64)
    CCFLAGS_TARGET +=   
    LDFLAGS_TARGET += 
    ARCH_TARGET = mips64
endif

# Device selection
ifdef DEVICE
    DEVICE_TARGET = $(DEVICE)$(SLASH)
endif

# Include directories
INC = \
/I ..$(SLASH)..$(SLASH)..$(SLASH)HeaderFiles \
/I ..$(SLASH)..$(SLASH)..$(SLASH)SharedFiles \
/I "$(ECO_FRAMEWORK)$(SLASH)Eco.Core1$(SLASH)SharedFiles" \
/I "$(ECO_FRAMEWORK)$(SLASH)Eco.System1$(SLASH)SharedFiles" \
/I "$(ECO_FRAMEWORK)$(SLASH)Eco.InterfaceBus1$(SLASH)SharedFiles" \
/I "$(ECO_FRAMEWORK)$(SLASH)Eco.MemoryManager1$(SLASH)SharedFiles" \
/I "$(ECO_FRAMEWORK)$(SLASH)Eco.FileSystemManagement1$(SLASH)SharedFiles" 

# Source files.
SRC = \
..$(SLASH)..$(SLASH)..$(SLASH)SourceFiles$(SLASH)CEcoTest1. \
..$(SLASH)..$(SLASH)..$(SLASH)SourceFiles$(SLASH)CEcoTest1Factory.

# Libraries

# Object files
OBJ = $(SRC:.=.o)

OUT = $(OUT_TARGET)
OUT_DIR = ..$(SLASH)..$(SLASH)..$(SLASH)BuildFiles$(SLASH)$(PLATFORM)$(SLASH)$(ARCH_TARGET)$(SLASH)$(DEVICE_TARGET)$(CONFIG_TARGET)

# C/C++ compiler flags
CCFLAGS = $(CCFLAGS_TARGET)
 
 # Compile flags
LDFLAGS = $(LDFLAGS_TARGET)

# Tools

 
.SUFFIXES: .

all:  $(OUT)

..o:
	mkdir -p "$(subst /,\,$(OUT_DIR))"
 
$(OUT): $(OBJ)
	$(CMD_TARGET)
	$(RM) $(OBJ)

clean:
