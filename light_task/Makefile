APPLICATION = Light_with_shim

BOARD =  bluepill-stm32f103c8

RIOTBASE ?= $(CURDIR)/../..

USEMODULE += dht
USEMODULE += bme280_i2c
USEMODULE += fmt
USEMODULE += opt3001
USEMODULE += lsm6ds3
USEMODULE += periph_adc



USEMODULE += xtimer
USEMODULE += ztimer
USEMODULE += periph_gpio
USEMODULE += periph_gpio_irq
USEMODULE += atomic_utils
USEMODULE += ztimer_msec
USEMODULE += atomic_utils


PORT_DARWIN = /dev/tty.usbserial-0001

DEVELHELP ?= 1

QUIET ?= 1

include $(RIOTBASE)/Makefile.include

