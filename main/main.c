#include <stdio.h>
#include "led_strip_driver.h"
#include "colors.h"
#include <led_clock_driver.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";
static uint8_t buf[56 * 3];
static led_strip_t strip;
static led_clock_t led_clock;

static uint8_t buf_2[2 * 3];
static led_strip_t strip_2;

// Первый поток
void led_clock_task(void *pvParameters)
{
    led_color_t color = WHITE;
    uint8_t brightness = 0xFF;

    while (1){
        clock_set_time(&led_clock, time(NULL));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Второй поток
void sec_task(void *pvParameters)
{
    bool enabled = true;
    while (1)
    {
        ESP_LOGI(TAG, "NEW SEC %d", enabled);
        if (enabled)
        {
            led_set_color(&strip_2, WHITE);
            led_set_brightness(&strip_2, 0x80);
        }
        else
        {
            led_clear(&strip_2);
        }
        led_show(&strip_2);
        enabled = !enabled;
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "WS2812B LED Clock Example");
    int led_gpio = 3;   // 25;
    int led_gpio_2 = 1; // 25;
    // int led_count = 56; //179; //42

    led_init(&strip, buf, 56, led_gpio);
    led_init(&strip_2, buf_2, 2, led_gpio_2);

    led_clock_init(&led_clock, &strip, 2);

    // Запускаем первый поток
    xTaskCreate(led_clock_task, "Task1", 2048, NULL, 5, NULL);
    // Запускаем второй поток
    xTaskCreate(sec_task, "Task2", 2048, NULL, 5, NULL);
}