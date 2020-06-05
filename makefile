#
#             LUFA Library
#     Copyright (C) Dean Camera, 2014.
#
#  dean [at] fourwalledcubicle [dot] com
#           www.lufa-lib.org
#
# --------------------------------------
#         LUFA Project Makefile.
# --------------------------------------

# Run "make help" for target help.

# Set the MCU accordingly to your device (e.g. at90usb1286 for a Teensy 2.0++, or atmega16u2 for an Arduino UNO R3)
MCU          = at90usb1286
ARCH         = AVR8
F_CPU        = 16000000
F_USB        = $(F_CPU)
OPTIMIZATION = s
TEENSY	     = /Applications/Arduinolatest.app/Contents/Java/hardware/teensy/avr/cores/teensy
TARGET       = acnh-helper
SRC          = hardware.c Descriptors.c macro.c command.c stack.c  logger.c regset.c usbservice.c $(LUFA_SRC_USB)
LUFA_PATH    = ./lufa/LUFA

# DEF_MACRO is the 'default macro'
# which is run if you do not make the 'dip-switch' hardware modification or
# is run when the dip-switch settings are set as 'off, off, off' on teensy pins C2, C1, C0)
# _DEFAULTMACRO_WISH - used for wishing on those starry nights when Celeste is around. (face camera and start teensy helper)
# _DEFAULTMACRO_BUILD - Build 40 items
# _DEFAULTMACRO_FRUITBUY400  - Stock up on Fruit to sell for a *BIG* profit on a friend's island.
# _DEFAULTMACRO_FRUITBUY300
# _DEFAULTMACRO_FRUITBUY200
# _DEFAULTMACRO_FRUITBUY100
# _DEFAULTMACRO_FRUITBUY10
# _DEFAULTMACRO_DEMO - Useless demo - move cursor around pocket and swipe net/axe a few times.

DEF_MACRO		 = _DEFAULTMACRO_FRUITBUY400
CC_FLAGS     = -DUSE_LUFA_CONFIG_HEADER -D$(DEF_MACRO) -I$(TEENSY)/ -IConfig/
LD_FLAGS     =

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk

# Target for LED/buzzer to alert when print is done
with-alert: all
with-alert: CC_FLAGS += -DALERT_WHEN_DONE
