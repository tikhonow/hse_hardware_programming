APPLICATION = morse_code

BOARD =  bluepill-stm32f103c8

RIOTBASE ?= $(CURDIR)/../..

USEMODULE += xtimer
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq

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
