#include <stdio.h>
#include <stdlib.h>
#include "dht.h"
#include "dht_params.h"
#include "fmt.h"

int main(void) {
    dht_params_t my_params;
    my_params.pin = GPIO_PIN(PORT_A, 1);
    my_params.in_mode = DHT_PARAM_PULL;

    printf("DHT sensor pin: %d\n", (int)my_params.pin);

    dht_t dev;
    if (dht_init(&dev, &my_params) == DHT_OK) {
        printf("DHT sensor connected\n");
        int16_t temperature, humidity;
        if (dht_read(&dev, &temperature, &humidity) == DHT_OK) {
            char temp_c_s[10];
            size_t n = fmt_s16_dfp(temp_c_s, temperature, -1);
            temp_c_s[n] = '\0';

            // Конвертация температуры в Фаренгейты
            int16_t temperature_f = (temperature * 9) / 5 + 320; // Добавляем 320 для смещения, так как значение температуры умножено на 10
            char temp_f_s[10];
            n = fmt_s16_dfp(temp_f_s, temperature_f, -1);
            temp_f_s[n] = '\0';

            char hum_s[10];
            n = fmt_s16_dfp(hum_s, humidity, -1);
            hum_s[n] = '\0';
            printf("Temperature: %s°C, %s°F, Humidity: %s%%\n",
                   temp_c_s, temp_f_s, hum_s);
        } else {
            printf("Failed to read data from DHT sensor\n");
        }
    } else {
        printf("Failed to connect to DHT sensor\n");
    }
    return 0;
}
