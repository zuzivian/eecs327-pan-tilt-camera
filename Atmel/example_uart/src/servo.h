/*
 * IncFile1.h
 *
 * Created: 5/24/2019 2:05:21 PM
 *  Author: Qiankai Cao
 */ 


#ifndef SERVO_H
#define SERVO_H

#include <asf.h>
#include <string.h>

/** PWM frequency in Hz */
#define PWM_FREQUENCY  50
/** PWM period value */
#define PERIOD_VALUE   1600
/** Initial duty cycle restrictions */
#define DUTY_VALUE_MIN  PERIOD_VALUE / 20
#define DUTY_VALUE_MAX  PERIOD_VALUE / 10
#define DUTY_VALUE_INIT (DUTY_VALUE_MIN + DUTY_VALUE_MAX) / 2

#define pwm_pwmh2_pin_0 IOPROT_CREATE_PIN_0
#define pwm_pwmh2_pin_1 IOPROT_CREATE_PIN_1

// functions
void servo_init(void);
void servo_set(pwm_channel_t *p_channel, uint32_t duty_value);

// Globals
uint32_t pan_angle;
uint32_t tilt_angle;

pwm_channel_t pwm_channel_cam_0;
pwm_channel_t pwm_channel_cam_1;

#endif /* INCFILE1_H_ */