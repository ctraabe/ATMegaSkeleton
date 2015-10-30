TARGET := $(notdir $(shell pwd))

BOARD  := ArduinoMega
# BOARD  := ArduinoMicro
# BOARD  := ArduinoNano328p
# BOARD  := ArduinoNano168

# PROGRAMMER := avrisp2
PROGRAMMER := stk500 -P /dev/ttyUSB0
# PROGRAMMER := atmelice_isp

DEPFLAGS  = -MM -MT '$(addprefix $(BUILD_PATH)/, $(<:.c=.o)) $@' $< -MF $@
CFLAGS    = -c -g $(LDFLAGS)
CCFLAGS   = -std=gnu99 -Wstrict-prototypes
CPPFLAGS  = -std=c++11 -fno-exceptions
LSTFLAGS  = -Wa,-adhlns=$(addprefix $(BUILD_PATH)/,$(addsuffix .lst, $<))
LDFLAGS   = -Ofast -pedantic -Werror -Wall -Wextra \
            -Wundef -fshort-enums -ffreestanding -Wl,--relax
ALLFLAGS  = -mmcu=$(MCU) -DF_CPU="$(F_CPU)" -I$(PINS)

CC     := avr-gcc
CPP    := avr-g++
CP     := avr-objcopy
DUDE   := avrdude

F_CPU  := 16000000L
ifeq ($(BOARD), ArduinoMega)
  MCU  := atmega2560
  BAUD := 57600
  PINS := ArduinoMega
else ifeq ($(BOARD), ArduinoMicro)
  MCU  := atmega32u4
  BAUD := 57600
  PINS := ArduinoMicro
else ifeq ($(BOARD), ArduinoNano328p)
  MCU  := atmega328p
  BAUD := 57600
  PINS := ArduinoNano
else ifeq ($(BOARD), ArduinoNano168)
  MCU  := atmega168
  BAUD := 19200
  PINS := ArduinoNano
else
  $(error Unsupported board: $(BOARD))
endif

# If the environment variable DEV_BUILD_PATH is set, then the build files will
# be placed there in a named sub-folder, otherwise a build directory will be
# created in the current directory
ifneq ($(DEV_BUILD_PATH),)
  BUILD_PATH := $(DEV_BUILD_PATH)/build/$(TARGET)
else
  BUILD_PATH := build
endif

SOURCES   = $(wildcard *.c)
SOURCES  += $(wildcard *.S)
SOURCES  += $(wildcard *.cpp)
DEPENDS   = $(addsuffix .d, $(addprefix $(BUILD_PATH)/, $(SOURCES)))
OBJECTS   = $(addsuffix .o, $(addprefix $(BUILD_PATH)/, $(SOURCES)))
ASSEMBL   = $(addsuffix .lst, $(addprefix $(BUILD_PATH)/, $(SOURCES)))

ELF    := $(BUILD_PATH)/$(TARGET).elf
HEX    := $(BUILD_PATH)/$(TARGET).hex

# Rules to make dependency "makefiles"
$(BUILD_PATH)/%.c.d: %.c
	mkdir -p $(BUILD_PATH)
	$(CC) $(DEPFLAGS) $(ALLFLAGS)

$(BUILD_PATH)/%.S.d: %.S
	mkdir -p $(BUILD_PATH)
	$(CC) $(DEPFLAGS) $(ALLFLAGS)

$(BUILD_PATH)/%.cpp.d: %.cpp
	mkdir -p $(BUILD_PATH)
	$(CPP) $(DEPFLAGS) $(ALLFLAGS)

# Rules to make the compiled objects
$(BUILD_PATH)/%.c.o: %.c $(BUILD_PATH)/%.c.d
	$(CC) $(CFLAGS) $(CCFLAGS) $(LSTFLAGS) $(ALLFLAGS) -o $@ $<

$(BUILD_PATH)/%.S.o: %.S $(BUILD_PATH)/%.S.d
	$(CC) $(CFLAGS) $(CCFLAGS) $(LSTFLAGS) $(ALLFLAGS) -o $@ $<

$(BUILD_PATH)/%.cpp.o: %.cpp $(BUILD_PATH)/%.cpp.d
	$(CPP) $(CFLAGS) $(CPPFLAGS) $(LSTFLAGS) $(ALLFLAGS) -o $@ $<

# Declare targets that are not files
.PHONY: program clean


# Note that without an argument, make simply tries to build the first target
# (not rule), which in this case is this target to build the .hex
$(HEX): $(ELF)
	$(CP) -O ihex $(ELF) $(HEX)

# Target to build the .elf file
# NOTE: -lm includes the math library (libm.a)
$(ELF): $(OBJECTS)
	$(CC) $(LDFLAGS) $(CCFLAGS) $(ALLFLAGS) -o $(ELF) $(OBJECTS) -lm

# Include the dependency "makefiles"
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

# Target to program the board
program: $(HEX)
	$(DUDE) -c $(PROGRAMMER) -p $(MCU) -U flash:w:$(HEX):i
# ifeq ($(MCU),atmega32u4)
# 	$(DUDE) -C/usr/share/arduino/hardware/tools/avrdude.conf -p$(MCU) \
# 	-cavr109 -P/dev/ttyACM0 -b$(BAUD) -D -Uflash:w:$(HEX):i
# else
# 	$(DUDE) -C/usr/share/arduino/hardware/tools/avrdude.conf -p$(MCU) \
# 	-carduino -P/dev/ttyUSB0 -b$(BAUD) -D -Uflash:w:$(HEX):i
# endif

# Target to clean up the directory (leaving only source)
clean:
	rm -f $(HEX) $(ELF) $(OBJECTS) $(DEPENDS) $(ASSEMBL)
	rmdir $(BUILD_PATH)
