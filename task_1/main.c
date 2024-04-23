#include "periph/gpio.h"
#include "xtimer.h"

#define DELAY_DEBOUNCE 300000
#define PRESS_LONG 3000000
#define BLINK_SHORT 100000
#define BLINK_LONG 500000
#define BLINK_ALTERNATE 2000000  // Новая частота мигания

#define CALC_PRESS_DURATION(start, end) \
    (((end) < (start)) ? (UINT32_MAX - (start) + (end)) : ((end) - (start)))

static gpio_t pin_led = GPIO_PIN(PORT_C, 13);
static gpio_t pin_button = GPIO_PIN(PORT_A, 12);
static xtimer_t timer_debounce;
static xtimer_t timer_led;
static uint32_t time_press_start;
static uint32_t duration_press;
static bool state_led = false;
static bool fire_led = false;
static bool pressed_button = false;
static bool alternate_blink = false; // Флаг для переключения частоты мигания

// Сигнатуры функций
void ledToggle(void *arg);
void callbackDebounce(void *arg);
void handlerButton(void *arg);

int main(void) {
    gpio_init(pin_led, GPIO_OUT);
    gpio_init_int(pin_button, GPIO_IN_PU, GPIO_BOTH, handlerButton, NULL);

    timer_debounce.callback = callbackDebounce;
    timer_led.callback = ledToggle;

    while (1) {}
    return 0;
}

// Реализации функций
void ledToggle(void *arg) {
    (void)arg;
    if (state_led) {
        gpio_toggle(pin_led);
        if (alternate_blink) {
            xtimer_set(&timer_led, BLINK_ALTERNATE);
        } else {
            xtimer_set(&timer_led, duration_press >= PRESS_LONG ? BLINK_LONG : BLINK_SHORT);
        }
    }
}

void callbackDebounce(void *arg) {
    (void)arg;
    gpio_irq_enable(pin_button);
}

void handlerButton(void *arg) {
    (void)arg;
    if (!pressed_button) {
        time_press_start = xtimer_now_usec();
        pressed_button = true;
    } else {
        uint32_t time_press_end = xtimer_now_usec();
        duration_press = CALC_PRESS_DURATION(time_press_start, time_press_end);

        if (duration_press >= PRESS_LONG) {
            alternate_blink = !alternate_blink; // Переключаем режим мигания
            if (!fire_led) {
                fire_led = true;
                state_led = true;
                ledToggle(NULL);
            }
        } else {
            fire_led = !fire_led;
            state_led = fire_led;
            if (fire_led) {
                ledToggle(NULL);
            } else {
                xtimer_remove(&timer_led);
                gpio_set(pin_led);
            }
        }

        pressed_button = false;
        gpio_irq_disable(pin_button);
        xtimer_set(&timer_debounce, DELAY_DEBOUNCE);
    }
}
