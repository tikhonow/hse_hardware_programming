//openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "init; reset halt; stm32f1x mass_erase 0; exit"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "timex.h"
#include "mutex.h"

#define DOT_TIME 250000UL
#define DASH_TIME 750000UL
#define LETTER_PAUSE 3000000UL
#define BLUE_LED_PIN GPIO_PIN(PORT_A, 10)
#define WHITE_LED_PIN GPIO_PIN(PORT_C, 13)
#define RED_LED_PIN GPIO_PIN(PORT_A, 12)
#define BUTTON_DOT_PIN GPIO_PIN(PORT_B, 4)
#define BUTTON_DASH_PIN GPIO_PIN(PORT_B, 6)

static mutex_t input_mutex;
static char input_sequence[71];
static size_t input_index = 0;

const char *morse_dict[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", // K-T
        "..-", "...-", ".--", "-..-", "-.--", "--..", // U-Z
        "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----." // 0-9
};

void blink_led(gpio_t pin, uint32_t duration) {
    gpio_set(pin);
    xtimer_usleep(duration);
    gpio_clear(pin);
}

void blink_morse(const char *sequence) {
    while (*sequence) {
        if (*sequence == '.') {
            blink_led(BLUE_LED_PIN, DOT_TIME);
        } else if (*sequence == '-') {
            blink_led(BLUE_LED_PIN, DASH_TIME);
        }
        xtimer_usleep(DOT_TIME);
        sequence++;
    }
}

void input_callback(char input_char) {
    mutex_lock(&input_mutex);
    if (input_index < sizeof(input_sequence) - 1) {
        input_sequence[input_index++] = input_char;
        gpio_toggle(WHITE_LED_PIN);
    }
    mutex_unlock(&input_mutex);
}

void dot_button_handler(void *arg) {
    (void)arg;
    input_callback('.');
}

void dash_button_handler(void *arg) {
    (void)arg;
    input_callback('-');
}

int main(void) {
    mutex_init(&input_mutex);
    gpio_init(BLUE_LED_PIN, GPIO_OUT);
    gpio_init(WHITE_LED_PIN, GPIO_OUT);
    gpio_init(RED_LED_PIN, GPIO_OUT);
    gpio_init_int(BUTTON_DOT_PIN, GPIO_IN_PU, GPIO_FALLING, dot_button_handler, NULL);
    gpio_init_int(BUTTON_DASH_PIN, GPIO_IN_PU, GPIO_FALLING, dash_button_handler, NULL);

    for (size_t i = 0; i < strlen(YOUR_INPUT_STRING); i++) {
        char c = toupper(YOUR_INPUT_STRING[i]);
        if (c >= 'A' && c <= 'Z') {
            blink_morse(morse_dict[c - 'A']);
        } else if (c >= '0' && c <= '9') {
            blink_morse(morse_dict[26 + c - '0']);
        }
        xtimer_usleep(LETTER_PAUSE);
    }

    return 0;
}
