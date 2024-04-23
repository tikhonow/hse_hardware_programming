#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "xtimer.h"
#include "periph/gpio.h"
#include "timex.h"

#define DOT_TIME 250000UL
#define DASH_TIME 750000UL
#define LETTER_PAUSE 3000000UL
#define WORD_PAUSE 10000000UL
#define INPUT_TIMEOUT 5000000UL
#define END_INPUT_TIMEOUT 30000000UL
#define BLUE_LED_PIN GPIO_PIN(PORT_C, 13)
#define RED_LED_PIN GPIO_PIN(PORT_A, 5) // Adjust pin according to your board
#define BUTTON_PIN GPIO_PIN(PORT_B, 0) // Adjust pin according to your board

// Morse code dictionary for A-Z
const char *morse_dict[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--.."
};

// Function prototypes
void blink_led(gpio_t pin, uint32_t duration);
void blink_morse(const char *code);
void blink_string(const char *str);
char decode_morse(const char *code);
void handle_input(void);

int main(void) {
    gpio_init(BLUE_LED_PIN, GPIO_OUT);
    gpio_init(RED_LED_PIN, GPIO_OUT);
    // Initialize button without interrupt for simplicity, polling mode
    gpio_init(BUTTON_PIN, GPIO_IN_PU);
#ifdef YOUR_INPUT_STRING
    const char *DEMO_STRING = YOUR_INPUT_STRING;
#else
    const char *DEMO_STRING = "SOS";
#endif
    // Blink DEMO_STRING three times
    for (int i = 0; i < 3; ++i) {
        for (size_t j = 0; j < strlen(DEMO_STRING); ++j) {
            char c = toupper(DEMO_STRING[j]);
            if (c >= 'A' && c <= 'Z') {
                blink_morse(morse_dict[c - 'A']);
            }
            xtimer_usleep(LETTER_PAUSE);
        }
    }

    // After blinking, switch to input handling mode
    while (1) {
        gpio_set(BLUE_LED_PIN); // Turn on blue LED to indicate ready for input
        handle_input();
    }

    return 0;
}

void blink_led(gpio_t pin, uint32_t duration) {
    gpio_set(pin);
    xtimer_usleep(duration);
    gpio_clear(pin);
}

void blink_morse(uint8_t code) {
    for (int i = 7; i >= 0; i -= 2) {
        if ((code >> i) & 0x3) {
            uint32_t duration = ((code >> i) & 0x3) == 0x2 ? DASH_TIME : DOT_TIME;
            blink_led(BLUE_LED_PIN, duration);
            xtimer_usleep(DOT_TIME);
        }
    }
}
}

void blink_string(const char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            blink_morse(morse_dict[*str - 'A']);
            xtimer_usleep(LETTER_PAUSE); // Gap between letters
        }
        str++;
    }
}

char decode_morse(const char *code) {
    for (int i = 0; i < 26; ++i) {
        if (strcmp(code, morse_dict[i]) == 0) {
            return 'A' + i;
        }
    }
    return 0; // Not found
}

void handle_input(void) {
    char input_code[5] = {0}; // Buffer for one Morse code, max 4 elements + null terminator
    int pos = 0;
    uint64_t last_press_time = 0;

    while (1) {
        if (gpio_read(BUTTON_PIN) == 0) { // Button pressed
            gpio_clear(BLUE_LED_PIN); // Turn off blue LED during input
            uint64_t press_start_time = xtimer_now_usec();
            while (gpio_read(BUTTON_PIN) == 0); // Wait for button release
            uint64_t press_duration = xtimer_now_usec() - press_start_time;

            // Determine dot or dash
            if (press_duration < 500000UL) { // Less than 0.5s for dot
                input_code[pos++] = '.';
            } else {
                input_code[pos++] = '-';
            }

            if (pos >= 4) { // Buffer full, process input
                char decoded_char = decode_morse(input_code);
                if (decoded_char) {
                    printf("%c", decoded_char);
                    blink_string(&decoded_char);
                } else {
                    // Error, unrecognized code
                    gpio_set(RED_LED_PIN);
                    xtimer_sleep(5); // Keep red LED on for 5 seconds
                    gpio_clear(RED_LED_PIN);
                }
                memset(input_code, 0, sizeof(input_code)); // Clear input buffer
                pos = 0;
            }
            last_press_time = xtimer_now_usec();
        } else if (xtimer_now_usec() - last_press_time > END_INPUT_TIMEOUT && last_press_time != 0) {
            // End of input
            gpio_set(BLUE_LED_PIN); // Turn blue LED back on to indicate end of input
            break;
        }
    }
}
