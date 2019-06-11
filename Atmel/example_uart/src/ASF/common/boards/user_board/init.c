/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "camera.h"

void board_init(void)
{
	
	
	ioport_init();
	
	
	ioport_set_pin_dir(WIFI_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_RESET_PIN, false);
	
	ioport_set_pin_dir(CAM_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CAM_RESET_PIN, true);
	
	ioport_set_pin_dir(WIFI_SET, IOPORT_DIR_INPUT);
	ioport_set_pin_level(WIFI_SET, IOPORT_MODE_PULLDOWN);
	
	ioport_set_pin_dir(WIFI_COMM, IOPORT_DIR_INPUT); 
	ioport_set_pin_level(WIFI_COMM, IOPORT_MODE_PULLDOWN);

	
	// Pull CTS pin of WiFi low to enable communication
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
	
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	/*gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);*/

	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	
	
	
	
	
	

	
}
