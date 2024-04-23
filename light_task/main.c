#include <stdio.h>
#include "periph/adc.h"
#include "xtimer.h"

int main(void) {
    // Инициализация ADC для канала PA0
    if (adc_init(ADC_LINE(7)) < 0) {
        printf("Ошибка инициализации ADC на линии PA0\n");
        return 1;
    } else {
        printf("ADC инициализирован на линии PA0\n");
    }

    while (1) {
        // Пробное измерение
        int sample = adc_sample(ADC_LINE(0), ADC_RES_10BIT);
        if (sample < 0) {
            printf("Ошибка при чтении с ADC\n");
        } else {
            printf("Значение ADC: %d\n", sample);
        }

        xtimer_sleep(1);  // Пауза 1 секунда
    }

    return 0;
}
