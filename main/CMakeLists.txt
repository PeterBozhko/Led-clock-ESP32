idf_component_register(SRCS "led_clock_driver.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES ws2812b)