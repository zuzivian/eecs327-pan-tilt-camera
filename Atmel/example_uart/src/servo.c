/*
 * servo.c
 *
 * Created: 5/24/2019 2:07:44 PM
 *  Author: Qiankai Cao
 */ 

#include "pwm.h"
#include "servo.h"

uint32_t pan_angle = DUTY_VALUE_INIT;
uint32_t tilt_angle = DUTY_VALUE_INIT;

void servo_init(void) {
	
	/************************************************************************/
	/* PWM time configuration                                               */
	/************************************************************************/
	pio_configure_pin(pwm_pwmh2_pin_0, PIO_TYPE_PIO_PERIPH_A);
	pio_configure_pin(pwm_pwmh2_pin_1, PIO_TYPE_PIO_PERIPH_A);
	//enable the peripheral clock for the PWM hardware
	pmc_enable_periph_clk(ID_PWM);
	
	pwm_channel_disable(PWM, PWM_CHANNEL_0);
	pwm_channel_disable(PWM, PWM_CHANNEL_1);
	
	pwm_clock_t pwm_clock_setting = {
		.ul_mck = sysclk_get_cpu_hz(),
		.ul_clkb = 0,
		.ul_clka = PWM_FREQUENCY * PERIOD_VALUE //PWM_FREQUENCY * PERIOD_VALUE
	};
	
	pwm_init(PWM, &pwm_clock_setting);
	
	/************************************************************************/
	/* Setting for channel                                                  */
	/************************************************************************/

	pwm_channel_cam_0.alignment = PWM_ALIGN_LEFT;				// output signal align to the left
	pwm_channel_cam_0.polarity = PWM_HIGH;
	pwm_channel_cam_0.ul_prescaler = PWM_CMR_CPRE_CLKA;		// CMR is clock source OPTION_REG
	pwm_channel_cam_0.ul_period	= PERIOD_VALUE;
	pwm_channel_cam_0.ul_duty = DUTY_VALUE_INIT;
	pwm_channel_cam_0.channel = PWM_CHANNEL_0;
	
	pwm_channel_cam_1.alignment = PWM_ALIGN_LEFT;				// output signal align to the left
	pwm_channel_cam_1.polarity = PWM_HIGH;
	pwm_channel_cam_1.ul_prescaler = PWM_CMR_CPRE_CLKA;		// CMR is clock source OPTION_REG
	pwm_channel_cam_1.ul_period = PERIOD_VALUE;
	pwm_channel_cam_1.ul_duty = DUTY_VALUE_INIT;
	pwm_channel_cam_1.channel = PWM_CHANNEL_1;
	
	pwm_channel_init(PWM,&pwm_channel_cam_0);
	pwm_channel_init(PWM,&pwm_channel_cam_1);
	
	
	/************************************************************************/
    /* open pwm channel                                                     */
    /************************************************************************/
    pwm_channel_enable(PWM, PWM_CHANNEL_0);  
	pwm_channel_enable(PWM, PWM_CHANNEL_1);      
}

void servo_set(pwm_channel_t *p_channel, uint32_t duty_value) {
	if (duty_value > DUTY_VALUE_MAX)
		duty_value = DUTY_VALUE_MAX;
	if (duty_value < DUTY_VALUE_MIN)
		duty_value = DUTY_VALUE_MIN;
	pwm_channel_update_duty(PWM, p_channel, duty_value);
}