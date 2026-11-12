#ifndef COLORS_H
#define COLORS_H
  // @const Colors
#pragma once
#include "led_strip_driver.h"

static const led_color_t WHITE   = {.r = 0xFF, .g = 0xFF, .b = 0xFF};
static const led_color_t RED     = {.r = 0xFF, .g = 0x00, .b = 0x00};
static const led_color_t GREEN   = {.r = 0x00, .g = 0xFF, .b = 0x00};
static const led_color_t BLUE    = {.r = 0x00, .g = 0x00, .b = 0xFF};
static const led_color_t YELLOW  = {.r = 0xFF, .g = 0xFF, .b = 0x00};
static const led_color_t CYAN    = {.r = 0x00, .g = 0xFF, .b = 0xFF};
static const led_color_t MAGENTA = {.r = 0xFF, .b = 0x00, .g = 0xFF};
#endif