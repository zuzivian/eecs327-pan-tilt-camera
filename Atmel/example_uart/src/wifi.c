/*
 * wifi.c
 *
 * Created: 1/17/2017 9:47:03 AM
 *  Author: Ilya
 */ 

#include "servo.h"
#include "wifi.h"
#include "timer_interface.h"
#include "camera.h"
#include <string.h>

volatile uint32_t received_byte_wifi = 0;
volatile bool new_rx_wifi = false;
volatile unsigned int input_pos_wifi = 0;
volatile bool wifi_comm_success = false;
volatile bool image_pass = false;
volatile uint32_t setup_web = false;
volatile uint32_t wifi_stream = 0;


volatile bool web_connected = false;

void WIFI_USART_HANDLER(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);
	}
}

void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	wifi_comm_success = true;
	process_data_wifi();
	//for (int jj=0;jj<MAX_INPUT_WIFI;jj++) input_line_wifi[jj] = 0;
	memset(input_line_wifi, 1, MAX_INPUT_WIFI);
	input_pos_wifi = 0;
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	setup_web = true;
}

void configure_usart_wifi(void)
{
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);
	
	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);
	
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_MSK, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_MSK);
}

void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_SET_ID);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_SET_PIO, WIFI_SET_ID, WIFI_SET_PIN_MSK, WIFI_SET_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_SET_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_SET_PIO, WIFI_SET_PIN_MSK);
}


void process_incoming_byte_wifi(uint8_t inByte)
{	
	input_line_wifi[input_pos_wifi++] = inByte;
}

void write_wifi_command(char* comm, int cnt){
	usart_write_line(WIFI_USART, comm);
}

void write_image_to_file(void){
	int len = find_image_len();
	if (len==0)
	{
		return 0;
	}
	else {
		
		char comm[200];
		sprintf(comm, "write 0 %d -c\r\nPOST / HTTP/1.1\r\nContent-Type: image/jpeg\r\nContent-Length: %5d\r\n\r\n", len+45+23, len);

		write_wifi_command(comm, 2);
		for(size_t i = start_idx; i <= start_idx+len; i++){
 			usart_putchar(WIFI_USART, image_buff[i]);
		}
		char comm2[100];
		sprintf(comm2, "read 0 200\r\n");
		write_wifi_command(comm2, 2);
	}		
}

void process_data_wifi()
{
	if(!web_connected && strstr(input_line_wifi, "2")){
		web_connected=true;
	}
	else if (wifi_stream==1 && strstr(input_line_wifi, "[Opened: 0]")) {
		wifi_stream = 2;
	}
	else if (wifi_stream==1 && strstr(input_line_wifi, "[Open failed]")) {
		wifi_stream = 0;
	}
	else if(cmd_ptr = strstr(input_line_wifi, "&&&")) {
		// "&&&12345,12345"
		pan_angle = atoi(cmd_ptr + 3) + DUTY_VALUE_INIT;
		cmd_ptr = strstr(input_line_wifi, ",,,");
		tilt_angle = atoi(cmd_ptr + 3) + DUTY_VALUE_INIT;
		char com[50];
		sprintf(com, "set sy c e on\r\n%d,%d\r\nset sy c e off\r\n", pan_angle, tilt_angle);
		write_wifi_command(com, 0);
	}
	wifi_comm_success = false;
}