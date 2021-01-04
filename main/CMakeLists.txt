idf_component_register(SRCS "main.c"
                        INCLUDE_DIRS "."
                        REQUIRES include accelerometer wifi uart_console oled leds)
