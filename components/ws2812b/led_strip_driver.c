#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip_driver.h"
#include "led_strip_encoder.h"
#include <string.h>

static const char *TAG = "led_strip_driver (WS2812B)";

#define MEM_BLOCK_SYMBOLS 48         // memory block size, 64 * 4 = 256 Bytes
#define TIMER_RESOLUTION_HZ 10000000 // 10MHz tick resolution, i.e., 1 tick = 0.01 µs
#define TRANS_QUEUE_DEPTH 4          // set the number of transactions that can pend in the background
#define COLOR_DELAY 10               // ms

void send_bytes(led_strip_t *strip, uint8_t *bytes, size_t size)
{
    ESP_ERROR_CHECK(rmt_transmit(strip->tx_chan, strip->led_encoder, bytes, size, &strip->tx_config));
    ESP_ERROR_CHECK(rmt_tx_wait_all_done(strip->tx_chan, portMAX_DELAY));
    vTaskDelay(pdMS_TO_TICKS(COLOR_DELAY));
}

void init_led_blink(led_strip_t *strip)
{
    size_t total_bytes = strip->num_leds * 3;

    memset(strip->buffer, 0x44, total_bytes);
    led_show(strip);
    vTaskDelay(100);

    led_clear(strip);
    led_show(strip);
    vTaskDelay(100);

    memset(strip->buffer, 0x44, total_bytes);
    led_show(strip);
    vTaskDelay(100);

    led_clear(strip);
    led_show(strip);
}

esp_err_t led_init(led_strip_t *strip, uint8_t *buffer, uint16_t num_leds, int gpio_num)
{
    if (!strip || !buffer)
        return ESP_ERR_INVALID_ARG;

    strip->buffer = buffer;
    strip->num_leds = num_leds;

    led_clear(strip);

    ESP_LOGI(TAG, "Create led strip channel");

    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,         // select source clock
        .gpio_num = gpio_num,                   // GPIO number
        .mem_block_symbols = MEM_BLOCK_SYMBOLS, // memory block size, 64 * 4 = 256 Bytes
        .resolution_hz = TIMER_RESOLUTION_HZ,   // 10 MHz tick resolution, i.e., 1 tick = 0.01 µs
        .trans_queue_depth = TRANS_QUEUE_DEPTH, // set the number of transactions that can pend in the background
        .flags.invert_out = false,              // do not invert output signal
        .flags.with_dma = false,                // do not need DMA backend
    };

    strip->tx_config.loop_count = 0; // no transfer loop

    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &strip->tx_chan));

    ESP_LOGI(TAG, "Install led strip encoder");

    led_strip_encoder_config_t encoder_config = {
        .resolution = TIMER_RESOLUTION_HZ,
    };

    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &strip->led_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(strip->tx_chan));

    init_led_blink(strip);

    return ESP_OK;
}

void led_set_pixel(led_strip_t *strip, led_color_t color, int index)
{
    if (!strip || index >= strip->num_leds)
        return;

    size_t offset = index * 3;

    strip->buffer[offset] = color.g;
    strip->buffer[offset + 1] = color.r;
    strip->buffer[offset + 2] = color.b;
}

void led_set_color(led_strip_t *strip, led_color_t color)
{
    for (int i = 0; i < strip->num_leds; i++)
    {
        led_set_pixel(strip, color, i);
    }
}

void led_set_brightness(led_strip_t *strip, uint8_t brightness)
{
    if (!strip)
        return;
    strip->brightness = brightness;
}

void led_show(led_strip_t *strip)
{
    if (!strip)
        return;

    size_t total_bytes = strip->num_leds * 3;

    if (strip->brightness == 255)
    {
        send_bytes(strip, strip->buffer, total_bytes);
        return;
    }

    // Если яркость изменена, масштабируем «на лету» без порчи исходного буфера
    // (Примечание: при использовании DMA масштабирование лучше делать во временный буфер)

    uint8_t *scaled = malloc(sizeof(uint8_t) * total_bytes);

    for (size_t i = 0; i < total_bytes; i++)
    {
        // Быстрое целочисленное масштабирование (деление на 256 через сдвиг)
        scaled[i] = (uint8_t)(((uint16_t)strip->buffer[i] * strip->brightness) >> 8);
    }
    send_bytes(strip, scaled, total_bytes);
    free(scaled);
}

void led_clear(led_strip_t *strip)
{
    if (!strip || !strip->buffer)
        return;
    memset(strip->buffer, 0, strip->num_leds * 3);
}

esp_err_t led_del(led_strip_t *strip)
{
    if (!strip)
    {
        return ESP_ERR_INVALID_ARG;
    }

    esp_err_t ret = ESP_OK;

    // 1. Ждём завершения всех активных транзакций и выключаем канал
    if (strip->tx_chan)
    {
        rmt_tx_wait_all_done(strip->tx_chan, portMAX_DELAY);

        ret = rmt_disable(strip->tx_chan);
        if (ret != ESP_OK)
        {
            return ret;
        }
    }

    // 2. Удаляем энкодер (освобождаем память)
    if (strip->led_encoder)
    {
        ret = rmt_del_encoder(strip->led_encoder);
        if (ret != ESP_OK)
        {
            return ret;
        }
        strip->led_encoder = NULL;
    }

    // 3. Удаляем сам канал RMT
    if (strip->tx_chan)
    {
        ret = rmt_del_channel(strip->tx_chan);
        if (ret != ESP_OK)
        {
            return ret;
        }
        strip->tx_chan = NULL;
    }

    // 4. Сбрасываем указатель на буфер (саму память буфера не трогаем, так как её выделил пользователь)
    strip->buffer = NULL;
    strip->num_leds = 0;

    return ESP_OK;
}
