# Options
SOURCE_DIR := src/
BUILD_DIR := build/
MODULES_DIR := $(SOURCE_DIR)modules/
INCLUDE_DIR := $(MODULES_DIR)include/ /usr/avr/include/ /lib/avr/include/

PROGRAM_WITH_JTAG := yes
NAME_EXECUTABLE := output

SOURCE_FILES := $(foreach sdir,$(SOURCE_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,$(MODULES_DIR),$(wildcard $(sdir)*.c))

OBJECT_FILES := $(subst $(MODULES_DIR),$(BUILD_DIR),$(SOURCE_FILES:.c=.o))
OBJECT_FILES := $(subst $(SOURCE_DIR),$(BUILD_DIR),$(OBJECT_FILES))

INCLUDE_DIR := $(addprefix -I,$(INCLUDE_DIR))

PROGRAMMER := atmelice_isp # avrdude -c --help for valid options
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
C_FLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb -Wall -Wextra $(INCLUDE_DIR) -D __AVR_ATmega162__ -D F_CPU=4912500UL


.DEFAULT_GOAL := $(BUILD_DIR)$(NAME_EXECUTABLE).hex

QUIET := @

$(BUILD_DIR):
	$(QUIET) mkdir $(BUILD_DIR)

$(BUILD_DIR)%.o: $(MODULES_DIR)%.c | $(BUILD_DIR)
	$(QUIET) echo compiling $(notdir $<)
	$(QUIET) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.c | $(BUILD_DIR)
	$(QUIET) echo compiling $(notdir $<)
	$(QUIET) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)$(NAME_EXECUTABLE).elf: $(OBJECT_FILES)
	$(QUIET) echo compiling program
	$(QUIET) $(CC) $(C_FLAGS) $^ -o $@

$(BUILD_DIR)$(NAME_EXECUTABLE).hex: $(BUILD_DIR)$(NAME_EXECUTABLE).elf | $(BUILD_DIR)
	$(QUIET) echo converting binary
	$(QUIET) avr-objcopy -j .text -j .data -O ihex $< $@

.PHONY: flash
flash: $(BUILD_DIR)$(NAME_EXECUTABLE).hex
	$(QUIET) avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)$(NAME_EXECUTABLE).hex:i -U lfuse:w:0xbd:m

.PHONY: clean
clean:
	$(QUIET) rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	$(QUIET) avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e

