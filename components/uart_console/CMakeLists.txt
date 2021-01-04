idf_component_register( SRCS "uart_console.c" "command_execution.c" "simple_simbols_handler.c"
		         		REQUIRES mx_function include leds wifi sound buttons
                        INCLUDE_DIRS ".")
