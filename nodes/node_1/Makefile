NAME_EXECUTABLE := program

SOURCE_DIR := src/
MODULES_DIR := $(SOURCE_DIR)modules/
INCLUDE_DIR := $(MODULES_DIR)include/

SOURCE_FILES := $(foreach sdir,$(SOURCE_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,$(MODULES_DIR),$(wildcard $(sdir)*.c))
HEADER_FILES := $(foreach sdir,$(INCLUDE_DIR),$(wildcard $(sdir)*.h))

PROGRAM_WITH_JTAG := yes

# Feel free to ignore anything below this line
PROGRAMMER := atmelice_isp
ifeq ($(PROGRAM_WITH_JTAG), yes)
	PROGRAMMER := atmelice
endif

BUILD_DIR := build/
TARGET_CPU := atmega162
TARGET_DEVICE := m162

CC := avr-gcc
C_FLAGS := -O -std=c11 -mmcu=$(TARGET_CPU) -ggdb -Wall -Wextra -I $(INCLUDE_DIR)
OBJECT_FILES := $(subst $(MODULES_DIR),$(BUILD_DIR),$(SOURCE_FILES:.c=.o))
OBJECT_FILES := $(subst $(SOURCE_DIR),$(BUILD_DIR),$(OBJECT_FILES))

.DEFAULT_GOAL := build

QUITE := @

$(BUILD_DIR):
	$(QUITE) mkdir $(BUILD_DIR)

build: $(OBJECT_FILES) | $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)$(NAME_EXECUTABLE)

$(BUILD_DIR)%.o : $(MODULES_DIR)%.c | $(BUILD_DIR)
	$(QUITE) echo compiling $(notdir $<)
	$(QUITE) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)%.o : $(SOURCE_DIR)%.c | $(BUILD_DIR)
	$(QUITE) echo compiling $(notdir $<)
	$(QUITE) $(CC) $< $(C_FLAGS) -c -o $@

$(BUILD_DIR)main.hex: $(OBJECT_FILES) | $(BUILD_DIR)
	$(QUITE) echo converting binary
	$(QUITE) $(CC) $(C_FLAGS) $(OBJECT_FILES) -o $(BUILD_DIR)$(NAME_EXECUTABLE)
	$(QUITE) avr-objcopy -j .text -j .data -O ihex $(BUILD_DIR)$(NAME_EXECUTABLE) $(BUILD_DIR)main.hex

.PHONY: flash
flash: $(BUILD_DIR)main.hex
	$(QUITE) echo flashing
	$(QUITE) avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)main.hex:i

.PHONY: clean
clean:
	$(QUITE) rm -rf $(BUILD_DIR)

.PHONY: erase
erase:
	$(QUITE) avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -e
	
.PHONY: debug
debug:
	if pgrep avarice; then pkill avarice; fi
	avrdude -p $(TARGET_DEVICE) -c $(PROGRAMMER) -U flash:w:$(BUILD_DIR)main.hex:i
	x-terminal-emulator -e avarice --edbg --ignore-intr :4242
	sleep 2
	avr-gdb -tui -iex "target remote localhost:4242" $(BUILD_DIR)a.out
	killall -s 9 avarice	