#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#pragma once

#include "driver/rmt_tx.h"

typedef union  //0xRRGGBB
{
    struct {
        union {
            uint8_t b;     ///< Blue channel value
            uint8_t blue;  ///< @copydoc b
        };
        union {
            uint8_t g;      ///< Green channel value
            uint8_t green;  ///< @copydoc g
        };
        union {
            uint8_t r;    ///< Red channel value
            uint8_t red;  ///< @copydoc r
        };
    };
    /// Access the red, green, and blue data as an array.
    /// Where:
    /// * `raw[2]` is the red value
    /// * `raw[1]` is the green value
    /// * `raw[0]` is the blue value

    uint8_t raw[3]; 

} led_color_t;

// Главная структура состояния драйвера (контекст)
typedef struct {
    uint8_t *buffer;          // Указатель на внешний массив байтов (размер = num_leds * 3)
    uint16_t num_leds;        // Количество светодиодов
    uint8_t brightness;       // Общая яркость (0..255)
    
    // Поля, специфичные для RMT (заполняются внутри led_init)
    rmt_channel_handle_t tx_chan;
    rmt_encoder_handle_t led_encoder;
    rmt_transmit_config_t tx_config;
} led_strip_t;

esp_err_t led_init(led_strip_t *strip, uint8_t *buffer, uint16_t num_leds, int gpio_num);
    
void led_set_pixel(led_strip_t *strip, led_color_t color, int index);
void led_set_color(led_strip_t *strip, led_color_t color);
void led_set_brightness(led_strip_t *strip, uint8_t brightness);
void led_show(led_strip_t *strip);
void led_clear(led_strip_t *strip);
esp_err_t led_del(led_strip_t *strip);

#endif 