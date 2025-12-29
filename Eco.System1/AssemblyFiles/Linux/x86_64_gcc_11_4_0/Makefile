ifeq ($(OS), Windows_NT)
	#WINDOWS USE THESE DEFINITIONS
	RM = -del /q /f
	SLASH = \\
	SWALLOW_OUTPUT = >nul 2>nul
	TOOLCHAIN = $(ECO_TOOLCHAIN)/gcc-win64-11.1.0/bin/
	TOOL = x86_64-w64-mingw32-
else
	#LINUX USE THESE DEFINITIONS
	RM = -rm -f
	SLASH = /
	SWALLOW_OUTPUT =
	TOOLCHAIN = 
	TOOL = 
endif 

ifeq ($(TARGET), 1)
OUT_TARGET = lib00000000000000000000000053595333.a
ifeq ($(DEBUG), 1)
CONFIG_TARGET = StaticDebug
CCFLAGS_TARGET = -Wall -g -DECO_LIB
LDFLAGS_TARGET =
ARCH_TARGET = x86_64
else
CONFIG_TARGET = StaticRelease
CCFLAGS_TARGET = -O3 -DECO_LIB
LDFLAGS_TARGET =
ARCH_TARGET = x86_64
endif
else
OUT_TARGET = lib00000000000000000000000053595333.a
ifeq ($(DEBUG), 1)
CONFIG_TARGET = DynamicDebug
CCFLAGS_TARGET = -Wall  -g -DECO_DLL
LDFLAGS_TARGET =
ARCH_TARGET = x86_64
else
CONFIG_TARGET = DynamicRelease
CCFLAGS_TARGET = -O3 -DECO_DLL
LDFLAGS_TARGET =
ARCH_TARGET = x86_64
endif
endif

# include directories
INC = \
-I ..$(SLASH)..$(SLASH)..$(SLASH)HeaderFiles \
-I ..$(SLASH)..$(SLASH)..$(SLASH)SharedFiles \
-I $(ECO_FRAMEWORK)$(SLASH)Eco.Core1$(SLASH)SharedFiles \
-I $(ECO_FRAMEWORK)$(SLASH)Eco.InterfaceBus1$(SLASH)SharedFiles \
-I $(ECO_FRAMEWORK)$(SLASH)Eco.FileSystemManagement1$(SLASH)SharedFiles \
-I $(ECO_FRAMEWORK)$(SLASH)Eco.MemoryManager1$(SLASH)SharedFiles

# source files.
SRC = \
..$(SLASH)..$(SLASH)..$(SLASH)SourceFiles$(SLASH)CEcoCommandArguments1.c \
..$(SLASH)..$(SLASH)..$(SLASH)SourceFiles$(SLASH)CEcoSystem1.c \
..$(SLASH)..$(SLASH)..$(SLASH)SourceFiles$(SLASH)CEcoSystem1EntryPoint.c

OBJ = $(SRC:.c=.o)

OUT = $(OUT_TARGET)
OUT_DIR = ..$(SLASH)..$(SLASH)..$(SLASH)BuildFiles$(SLASH)Linux$(SLASH)$(ARCH_TARGET)$(SLASH)$(CONFIG_TARGET)

 
# C++ compiler flags (-g -O2 -Wall)
CCFLAGS = $(CCFLAGS_TARGET) -DECO_MEM_EXT -DECO_FILE_EXT -DECO_NET_EXT -DECO_LINUX -DLINUX -DUGUID_UTILITY -DECO_X86_64
 
# compiler
GNU = $(TOOLCHAIN)
CC = $(TOOL)gcc
AR = $(TOOL)gcc-ar

ifeq ($(TARGET), 1)
CMD_TARGET=$(GNU)$(AR) rcs
else
CMD_TARGET=$(GNU)$(AR) rcs
endif

# library paths
LIBS = -lc -lm -lgcc
 
# compile flags
LDFLAGS = $(LDFLAGS_TARGET)
 
.SUFFIXES: .c

all: $(OUT)

.c.o:
	$(GNU)$(CC) $(INC) $(CCFLAGS) -c $< -o $@
 
$(OUT): $(OBJ)
	mkdir -p $(OUT_DIR)
	$(CMD_TARGET) $(OUT_DIR)$(SLASH)$(OUT) $(OBJ) $(LDFLAGS)
	rm $(OBJ)

clean:
	$(RM) $(OBJ) $(OUT_DIR)$(SLASH)$(OUT)
