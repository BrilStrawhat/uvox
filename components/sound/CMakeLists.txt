idf_component_register(SRCS "pentatonic_mode.c"
                            "chromatic_mode.c"
                            "pwm_notes.c"
                        REQUIRES include buttons oled
                        INCLUDE_DIRS ".")