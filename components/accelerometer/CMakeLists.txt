idf_component_register(SRCS "accelerometr.c"
                        SRCS "leds.c"
                        SRCS "pwm_notes.c"
                        SRCS "pentatonic_mode.c"
                        SRCS "chromatic_mode.c"
                        REQUIRES include
                        REQUIRES oled
                        INCLUDE_DIRS ".")