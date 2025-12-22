#include <stdio.h>
#include "ws2812b.h"
#include "colors.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "main";


void set_color_brightness(WS2812B_led_color_t *color, int brightness){
    color->r = color->r & brightness;
    color->g = color->g & brightness;
    color->b = color->b & brightness;
}

void app_main(void)
{
    ESP_LOGI(TAG, "WS2812B LED Clock Example");
    int led_gpio = 25;// 25;
    int led_count = 14; //179; //42
    init_led_strip(led_gpio);
    WS2812B_led_color_t color = WHITE;
    int brightness = 0xFF;
    set_color_brightness(&color, brightness);
    setSymbol(color, 8);
    vTaskDelay(100);
    while(1){
        color = WHITE;
        brightness += 0x44;
        if (brightness == 0xFF) brightness = 0x11;
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "WHITE : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);
        color = RED;
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "RED : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);
        color = GREEN;
        ESP_LOGI(TAG, "GREEN");
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "GREEN : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);
        color = BLUE;
        ESP_LOGI(TAG, "BLUE");
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "BLUE : %d %d %d", color.r, color.g, color.b);    
        setColorByPixel(color);
        vTaskDelay(200);
        color = YELLOW;
        ESP_LOGI(TAG, "YELLOW");
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "YELLOW : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);
        color = CYAN;   
        ESP_LOGI(TAG, "CYAN");
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "CYAN : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);
        color = MAGENTA;
        ESP_LOGI(TAG, "MAGENTA");
        set_color_brightness(&color, brightness);
        ESP_LOGI(TAG, "MAGENTA : %d %d %d", color.r, color.g, color.b);
        setColorByPixel(color);
        vTaskDelay(200);   
    }
}