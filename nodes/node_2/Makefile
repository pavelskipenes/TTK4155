SOURCE_DIR := src/
BUILD_DIR := build/
MODULES_DIR := $(SOURCE_DIR)modules/
INCLUDE_DIR := $(MODULES_DIR)include vendor/sam/sam3x/include vendor/sam/sam3x/source vendor/sam/interrupt vendor/sam/cmsis vendor/sam
SYS_INCLUDE_DIR := /usr/avr/include/ /lib/avr/include/

SOURCE_FILES := $(foreach sdir,$(SOURCE_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,$(MODULES_DIR),$(wildcard $(sdir)*.c))
SOURCE_FILES += $(foreach sdir,vendor/sam/sam3x/source/,$(wildcard $(sdir)*.c))
OBJECT_FILES := $(subst $(MODULES_DIR),$(BUILD_DIR),$(SOURCE_FILES:.c=.o))
OBJECT_FILES := $(subst $(SOURCE_DIR),$(BUILD_DIR),$(OBJECT_FILES))
INCLUDE_DIR := $(addprefix -I,$(INCLUDE_DIR))

VPATH:= $(dir $(SOURCE_FILES))
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AS:=arm-none-eabi-as

LDSCRIPT = vendor/sam/flash.ld
TARGET_CPU = __SAM3X8E__
LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections
CFLAGS:= -mcpu=cortex-m3 -mthumb -ggdb -std=c11 -Wall
CFLAGS+= $(INCLUDE_DIR)
CFLAGS+= -D $(TARGET_CPU)

QUITE := @

ELF=$(BUILD_DIR)main.elf
.DEFAULT_GOAL := $(ELF)
$(BUILD_DIR):
	$(QUITE) mkdir $(BUILD_DIR)

$(BUILD_DIR)%.o : %.c | $(BUILD_DIR)
	$(QUITE) $(CC) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)%.o : %.s | $(BUILD_DIR)
	$(QUITE) $(AS) $< -o $@

$(ELF) : $(OBJECT_FILES) | $(BUILD_DIR)
	$(QUITE) $(LD) $(LDFLAGS) -o $@ $(OBJECT_FILES) $(LDLIBS)

.PHONY: clean
clean:
	$(QUITE) rm -rf $(BUILD_DIR)

.PHONY: flash
flash: $(ELF)
	$(QUITE) openocd -d0 -f vendor/sam/openocd.cfg -c "exit"

