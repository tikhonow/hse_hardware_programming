# Установите имя вашего приложения и платы
APPLICATION = task_vars
BOARD =  bluepill-stm32f103c8
USEMODULE += periph_gpio
USEMODULE += xtimer
USEMODULE += periph_gpio # для работы с GPIO
USEMODULE += periph_gpio_irq # поддержка прерываний от GPIO
USEMODULE += atomic_utils # атомарный доступ к памяти
USEMODULE += stdio_uart
USEMODULE += printf_float


PORT_DARWIN = /dev/tty.usbserial-0001

# Это путь к базовой директории RIOT OS
RIOTBASE ?= $(CURDIR)/../..

# Включаем в сборку модули
USEMODULE += shell

DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

include $(RIOTBASE)/Makefile.include
