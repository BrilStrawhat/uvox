idf_component_register(SRCS "accelerometer.c" "leds.c" "pwm_notes.c" "pentatonic_mode.c" "chromatic_mode.c"
                        REQUIRES include oled buttons uart_console test_fireware main
                        INCLUDE_DIRS ".")

