#include <stdio.h>
#include "xtimer.h"
#include "periph/gpio.h"

#define LED_PIN GPIO_PIN(PORT_C, 13)
#define BUTTON_PIN GPIO_PIN(PORT_A, 12)

static const int static_const_global_var = 0;

void gpio_callback(void *arg) {
    static int static_local_var = 0;
    int local_var = 0;
    const int const_local_var = 0;
    static const int static_const_local_var = 0;

    (void)arg;

    printf("Interrupt triggered:\n");
    printf("Address of static const global variable: %p\n", (void *)&static_const_global_var);
    printf("Address of static const local variable: %p\n", (void *)&static_const_local_var);
    printf("Address of static local variable: %p\n", (void *)&static_local_var);
    printf("Address of local variable: %p\n", (void *)&local_var);
    printf("Address of const local variable: %p\n", (void *)&const_local_var);
}

int main(void) {
    if (gpio_init(LED_PIN, GPIO_OUT)) {
        printf("Error initializing LED pin.\n");
        return -1;
    }

    if (gpio_init_int(BUTTON_PIN, GPIO_IN_PU, GPIO_FALLING, gpio_callback, NULL) < 0) {
        printf("Error initializing button pin.\n");
        return -1;
    }

    printf("Memory addresses in RIOT OS:\n");
    printf("Address of static const global variable: %p\n", (void *)&static_const_global_var);


    while (1) {
        gpio_toggle(LED_PIN);
        xtimer_sleep(60); // Задержка 1 минута
    }

    return 0;
}
