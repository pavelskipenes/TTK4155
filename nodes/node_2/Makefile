LDSCRIPT = sam/flash.ld
BOOTUP = sam/sam3x/source/exceptions.c sam/sam3x/source/startup_sam3x.c sam/sam3x/source/system_sam3x.c
MCUTYPE = __SAM3X8E__
# List all source files to be compiled; separate with space
SOURCE_FILES=$(BOOTUP) main.c uart_and_printf/uart.c uart_and_printf/printf_stdarg.c can_lib/can_controller.c can_lib/can_interrupt.c

# Feel free to ignore anything below this line
VPATH:= $(dir $(SOURCE_FILES))
BUILD_DIR := build
OBJS := $(patsubst %.c, $(BUILD_DIR)/%$(ARCH)$(DEBUG).o, $(notdir $(SOURCE_FILES)))
#Tools
CC:=arm-none-eabi-gcc
LD:=arm-none-eabi-gcc
AR:=arm-none-eabi-ar
AS:=arm-none-eabi-as

ELF=$(BUILD_DIR)/main.elf

LDFLAGS:= -T$(LDSCRIPT) -mthumb -mcpu=cortex-m3 -Wl,--gc-sections
CFLAGS:= -mcpu=cortex-m3 -mthumb -g -std=c11 -MMD
CFLAGS+= -I sam -I sam/sam3x/include -I sam/sam3x/source -I sam/cmsis -I . -Iuart_and_printf -I can_lib
CFLAGS+= -D $(MCUTYPE)

QUITE := @

.DEFAULT_GOAL := $(ELF)
# compile and generate dependancy info
$(BUILD_DIR):
	$(QUITE) mkdir $(BUILD_DIR)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(QUITE) $(CC) -c $(CFLAGS) $< -o $@
	$(QUITE) #$(CC) -MM $(CFLAGS) $< > $@.d

$(BUILD_DIR)/%.o : %.s | $(BUILD_DIR)
	$(QUITE) $(AS) $< -o $@

$(ELF) : $(OBJS) | $(BUILD_DIR)
	$(QUITE) $(LD) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS)

.PHONY: clean
clean:
	$(QUITE) rm -rf $(BUILD_DIR)

#.PHONY: debug
#debug: $(ELF)
#	if pgr openocd; then killall -s 9 openocd; fi
#	x-terminal-emulator -e openocd -f sam/openocd.cfg -c "program $(ELF) verify"
#	sleep 5
#	arm-eabi-gdb -tui -iex "target extended-remote localhost:3333" $(ELF)
#	killall -s 9 openocd	

.PHONY: flash
flash: $(ELF)
	$(QUITE) openocd -f sam/openocd.cfg -c "program ${BUILD_DIR}/main.elf verify reset exit"

# pull in dependencies
-include	$(OBJS:.o=.d)
