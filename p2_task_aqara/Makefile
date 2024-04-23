APPLICATION = morse_code

BOARD =  bluepill-stm32f103c8

RIOTBASE ?= $(CURDIR)/../..

USEMODULE += dht
USEMODULE += bme280_i2c
USEMODULE += fmt
USEMODULE += opt3001


USEMODULE += xtimer
USEMODULE += ztimer
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += atomic_utils
USEMODULE += ztimer_msec
USEMODULE += atomic_utils


PORT_DARWIN = /dev/tty.usbserial-0001

.PHONY: ask-input

ask-input:
	@echo "Put a mess: "
	@read userInput; \
	echo "CFLAGS += -DYOUR_INPUT_STRING=\\\"$$userInput\\\"" > $(CURDIR)/Makefile.userinput

DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

-include $(CURDIR)/Makefile.userinput

include $(RIOTBASE)/Makefile.include
