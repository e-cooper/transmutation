PRODUCT_NAME       = project
SOURCES            = main.c myLib.c text.c gb_images/gameScreen.c gb_images/instructionScreen.c gb_images/pauseScreen.c gb_images/startScreen.c gb_images/winScreen.c gb_images/sprites.c

DKPATH             = H:/devkitARM/bin
CCPATH             = H:/cygwin64/bin
VBASIM             = H:/vba/VisualBoyAdvance.exe
FIND               = find
COPY               = cp -r

# --- File Names
ELF_NAME           = $(PRODUCT_NAME).elf
ROM_NAME           = $(PRODUCT_NAME).gba
BIN_NAME           = $(PRODUCT_NAME)

#MODEL              = -mthumb-interwork -mthumb #This does not make interrupts work
MODEL              = -mthumb-interwork -marm -mlong-calls #This makes interrupts work
SPECS              = -specs=gba.specs

# --- Archiver
AS                 = $(DKPATH)/arm-eabi-as
ASFLAGS            = -mthumb-interwork

# --- Compiler
CC                 = $(DKPATH)/arm-eabi-gcc
CFLAGS             = $(MODEL) -O2 -Wall -pedantic -Wextra -Werror -std=c99 -D_ROM=$(ROM_NAME) -D_VBA=$(VBASIM) -save-temps
CC_WRAP            = $(CCPATH)/gcc
CFLAGS_WRAP        = -O3 -Wall -pedantic -D_ROM='"$(ROM_NAME)"' -D_VBA='"$(VBASIM)"'

# --- Linker
LD                 = $(DKPATH)/arm-eabi-gcc
LDFLAGS            = $(SPECS) $(MODEL) -lm

# --- Object/Executable Packager
OBJCOPY            = $(DKPATH)/arm-eabi-objcopy
OBJCOPYFLAGS       = -O binary

# --- ROM Fixer
GBAFIX             = $(DKPATH)/gbafix

# --- Delete
RM                 = rm -f

OBJECTS = $(filter-out gba_wrapper%,$(SOURCES:.c=.o))

# --- Main Build Target
all : build $(BIN_NAME)

run : build
	$(VBASIM) $(ROM_NAME)

build : UNZIP $(ROM_NAME)

# --- GBA Wrapper Stuff
#$(BIN_NAME) : gba_wrapper.c
#	$(CC_WRAP) $(CFLAGS_WRAP) -o $@ $^

# --- Build .elf file into .gba ROM file
$(ROM_NAME) : $(ELF_NAME)
	$(OBJCOPY) $(OBJCOPYFLAGS) $(ELF_NAME) $(ROM_NAME)
	$(GBAFIX) $(ROM_NAME)

# --- Build .o files into .elf file
$(ELF_NAME) : $(OBJECTS)
	$(LD) $(OBJECTS) $(LDFLAGS) -o $@

# --- Build .c files into .o files
$(OBJECTS) : %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@


# ============ Common
UNZIP :
	-@$(FIND) . -iname "*.zip" -exec unzip -n {} \;-exec echo "This project must be rebuilt" \; -exec rm {} \;

clean:
	$(RM) $(ROM_NAME)
	$(RM) $(ELF_NAME)
	$(RM) $(BIN_NAME)
	$(RM) *.o
# ============ Common