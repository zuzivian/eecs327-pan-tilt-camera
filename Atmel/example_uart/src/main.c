#include <asf.h>
#include "conf_board.h"
#include "conf_clock.h"
#include "wifi.h"
#include "camera.h"
#include "servo.h"
#include "timer_interface.h"


int main (void)
{
	sysclk_init();
	wdt_disable(WDT);
	board_init();
	configure_tc();
	
	gpio_set_pin_low(WIFI_RESET_PIN);
	delay_ms(100);
	gpio_set_pin_high(WIFI_RESET_PIN);
	delay_ms(10000);
	
 	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();
	
	servo_init();
	
	while (ioport_get_pin_level(WIFI_SET)==0) {
		if (setup_web) {
		
		usart_write_line(WIFI_USART, "setup web\r\n");
		setup_web=false;
		}
	}
	
	capture_init();
	g_ul_vsync_flag = false;
	
	while(1) {
		servo_set(&pwm_channel_cam_0, pan_angle);
		servo_set(&pwm_channel_cam_1, tilt_angle);
		
		if (setup_web){
			usart_write_line(WIFI_USART, "setup web\r\n");
			setup_web=false;  
		}
		
		 else if (ioport_get_pin_level(WIFI_SET)==1){
			usart_write_line(WIFI_USART, "poll all\r\n");
				
			if(!web_connected){
				usart_write_line(WIFI_USART, "get wl n s\r\n");
				delay_ms(1000);
			}
			else if (wifi_stream<2) {
				if (wifi_stream ==0) {
					char comm[1000];
					sprintf(comm, "tcpc 35.208.31.153 80\r\n");
					write_wifi_command(comm, 2);
					wifi_stream = 1;
				}
				delay_ms(1000);
			}
			else{
				start_capture();
				write_image_to_file();
			}
		}
	
		else{
			gpio_set_pin_low(WIFI_RESET_PIN);
			delay_ms(50);
			gpio_set_pin_high(WIFI_RESET_PIN);
		}
		
	}
}
			

