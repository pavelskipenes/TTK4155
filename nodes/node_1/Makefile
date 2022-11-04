# Options
SOURCE_DIR := src/
BUILD_DIR := build/
MODULES_DIR := $(SOURCE_DIR)modules/
INCLUDE_DIR := $(MODULES_DIR)include
SYS_INCLUDE_DIR := /usr/avr/include/ /lib/avr/include/

PROGRAM_WITH_JTAG := yes
NAME_EXECUTABLE := output

SOURCE_FILES := $(foreach sdir,$(SOURCE_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,$(MODULES_DIR),$(wildcard $(sdir)*.c))

OBJECT_FILES := $(subst $(MODULES_DIR),$(BUILD_DIR),$(SOURCE_FILES:.c=.o))
OBJECT_FILES := $(subst $(SOURCE_DIR),$(BUILD_DIR),$(OBJECT_FILES))

INCLUDE_DIR := $(addprefix -I,$(INCLUDE_DIR))
SYS_INCLUDE_DIR := $(addprefix -isystem ,$(SYS_INCLUDE_DIR))

PROGRAMMER := atmelice_isp # avrdude -c --help for valid options
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
DEFINES := -D __AVR_ATmega162__ -D F_CPU=4912500UL -D __ASSERT_USE_STDERR -D DEBUG
LD_OPTIONS := --gc-sections#,-u,vfprintf#,--print-gc-sections,--defsym=__heap_start=0x801800,--defsym=__heap_end=0x801FFF
C_OPTIONS := -O0 -std=c11 -g3 -Wpadded -Wall -Wextra -Wshadow -Wdouble-promotion -Wformat=2 -Wundef -fno-common -Wconversion -fstack-usage -Wstack-usage=255 -ffunction-sections -fdata-sections# -lprintf_flt
C_FLAGS := $(INCLUDE_DIR) $(SYS_INCLUDE_DIR) -mmcu=$(TARGET_CPU) $(DEFINES) $(C_OPTIONS) -Wl,$(LD_OPTIONS)

.DEFAULT_GOAL := $(BUILD_DIR)$(NAME_EXECUTABLE).hex

QUIET := @

$(BUILD_DIR):
	$(QUIET) mkdir $(BUILD_DIR)

$(BUILD_DIR)%.o: $(MODULES_DIR)%.c | $(BUILD_DIR)
#	$(QUIET) echo compiling $(notdir $<)
	$(QUIET) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.c | $(BUILD_DIR)
#	$(QUIET) echo compiling $(notdir $<)
	$(QUIET) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)$(NAME_EXECUTABLE).elf: $(OBJECT_FILES)
#	$(QUIET) echo compiling program
	$(QUIET) $(CC) $(C_FLAGS) $^ -o $@

$(BUILD_DIR)$(NAME_EXECUTABLE).hex: $(BUILD_DIR)$(NAME_EXECUTABLE).elf | $(BUILD_DIR)
#	$(QUIET) echo converting binary
	$(QUIET) avr-objcopy -j .text -j .data -j .rodata  -j .bss -O ihex $< $@
# -j flag removes everything except that specified section. Maybe try without -j flag?

.PHONY: flash
flash: $(BUILD_DIR)$(NAME_EXECUTABLE).hex
	$(QUIET) avrdude -q -q -q -q -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)$(NAME_EXECUTABLE).hex:i -U lfuse:w:0xbd:m
# only flash is targeted with -U flag. lfuse is targeted to expose CPU clock on PB0

.PHONY: clean
clean:
	$(QUIET) rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	$(QUIET) avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e

.PHONY: check
check:
	cppcheck --enable=all $(SOURCE_FILES) $(INCLUDE_DIR) --language=c --platform=avr8 -q $(DEFINES)
