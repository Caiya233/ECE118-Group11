include nbproject/Makefile-local-default.mk

MP_PROCESSOR_OPTION=32MX320F128H
OBJECTDIR=build/default/production
DISTDIR=dist/default/production
TARGET_ELF=$(DISTDIR)/final_test.X.production.elf
TARGET_HEX=$(DISTDIR)/final_test.X.production.hex

SOURCES= \
    main.c \
    TestShell.c \
    DebugPrint.c \
    DriveTest.c \
    LauncherTest.c \
    ServoTest.c \
    SensorTest.c \
    BatteryTest.c \
    ECE118/src/AD.c \
    ECE118/src/BOARD.c \
    ECE118/src/IO_Ports.c \
    ECE118/src/pwm.c \
    ECE118/src/RC_Servo.c \
    ECE118/src/serial.c \
    ECE118/src/timers.c

OBJECTS=$(patsubst %.c,$(OBJECTDIR)/%.o,$(SOURCES))

CFLAGS=-g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -O0 -fno-common \
    -I"." -I"ECE118/include" -MP -MMD -DXPRJ_default=default \
    -mdfp="$(DFP_DIR)"

LDFLAGS=-mprocessor=$(MP_PROCESSOR_OPTION) \
    -Wl,--defsym=__MPLAB_BUILD=1,--script="ECE118/bootloader320.ld",--defsym=_min_heap_size=10,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="$(DISTDIR)/final_test.X.production.map",--memorysummary,$(DISTDIR)/memoryfile.xml \
    -mdfp="$(DFP_DIR)"

build: $(TARGET_HEX)

all: build

$(TARGET_HEX): $(TARGET_ELF)
	$(MP_CC_DIR)/xc32-bin2hex $(TARGET_ELF)

$(TARGET_ELF): $(OBJECTS) ECE118/bootloader320.ld
	@mkdir -p $(DISTDIR)
	$(MP_CC) $(LDFLAGS) -o $(TARGET_ELF) $(OBJECTS)

$(OBJECTDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(MP_CC) $(CFLAGS) -MF "$@.d" -o $@ $<

clean:
	rm -rf build dist

.PHONY: build all clean
