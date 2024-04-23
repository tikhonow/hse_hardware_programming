#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/i2c.h"
#include "bmx280.h"
#include "bmx280_params.h"
#include "dht.h"
#include "dht_params.h"
#include "fmt.h"
#include "debug.h"

#define C_TO_F(c) (((c) * 9 / 5) + 32)

static const bmx280_params_t my_bmx280_params = {
        .i2c_dev = I2C_DEV(0),
        .i2c_addr = 0x76,
        .t_sb = BMX280_SB_0_5,
        .filter = BMX280_FILTER_OFF,
        .run_mode = BMX280_MODE_FORCED,
        .temp_oversample = BMX280_OSRS_X1,
        .press_oversample = BMX280_OSRS_X1,
        .humid_oversample = BMX280_OSRS_X1,
};

int main(void) {
    dht_params_t my_dht_params;
    my_dht_params.pin = GPIO_PIN(PORT_A, 1);
    my_dht_params.in_mode = DHT_PARAM_PULL;

    dht_t dev_dht;
    bmx280_t dev_bmx280;

    if (dht_init(&dev_dht, &my_dht_params) != DHT_OK) {
        printf("Failed to connect to DHT sensor\n");
    }

    if (bmx280_init(&dev_bmx280, &my_bmx280_params) != BMX280_OK) {
        printf("Failed to init BME280 sensor\n");
        return 1;
    }

    //int16_t last_temp_dht = 0, last_hum_dht = 0;

    while (1) {
        int16_t temperature, humidity;
        if (dht_read(&dev_dht, &temperature, &humidity) == DHT_OK) {
            char temp_s[10], hum_s[10];
            size_t n = fmt_s16_dfp(temp_s, temperature, -1);
            temp_s[n] = '\0';
            n = fmt_s16_dfp(hum_s, humidity, -1);
            hum_s[n] = '\0';
            printf("DHT - Temperature: %s°C / %d°F, Humidity: %s%%\n", temp_s, C_TO_F(temperature / 10), hum_s);
            //last_temp_dht = temperature;
            //last_hum_dht = humidity;
        } else {
            printf("Failed to read data from DHT sensor\n");
        }

        int16_t temp_bme = bmx280_read_temperature(&dev_bmx280);
        uint32_t press = bmx280_read_pressure(&dev_bmx280);
        uint16_t hum_bme = bme280_read_humidity(&dev_bmx280);

        printf("BME280 - Temperature: %d.%02d°C / %d.%02d°F, Pressure: %lu Pa, Humidity: %d.%02d%%\n",
               temp_bme / 100, abs(temp_bme % 100), C_TO_F(temp_bme) / 100, abs((C_TO_F(temp_bme) - C_TO_F(0)) % 100), press, hum_bme / 102, (hum_bme % 102) * 10 / 102);

        //float temp_diff = ((float)temp_bme / 100) - ((float)last_temp_dht / 10);
        //float hum_diff = ((float)hum_bme / 1024) - ((float)last_hum_dht / 10);
        //printf("Difference - Temperature: %.2f°C, Humidity: %.2f%%\n", temp_diff, hum_diff);

        xtimer_sleep(10);
    }

    return 0;
}
