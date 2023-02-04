 /* 
 * File:   application.c
 * Author: Mohamed_Nagy
 * https://www.linkedin.com/in/mohamednagyofficial/
 * Created on January 27, 2022, 8:41 PM
 */


#include "application.h"
#include "MCAL_Layer/ADC/hal_adc.h"
Std_ReturnType ret = E_NOT_OK;
volatile uint16 LM35;
void WelcomingMessage();
void adcInterruptHandeler();
volatile uint8 adcArray[7];

chr_lcd_4bit_t lcd_1 =
{
	.lcd_rs.port=PORTC_INDEX,
	.lcd_rs.pin=GPIO_PIN2,
	.lcd_rs.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_rs.logic=GPIO_LOW,
	
    .lcd_en.port=PORTC_INDEX,
	.lcd_en.pin=GPIO_PIN3,
	.lcd_en.direction=GPIO_DIRECTION_OUTPUT,
	.lcd_en.logic=GPIO_LOW,
	
    .lcd_data[0].port=PORTC_INDEX,
	.lcd_data[0].pin=GPIO_PIN4,
	.lcd_data[0].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[0].logic=GPIO_LOW,
	
    .lcd_data[1].port=PORTC_INDEX,
	.lcd_data[1].pin=GPIO_PIN5,
	.lcd_data[1].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[1].logic=GPIO_LOW,
	
    .lcd_data[2].port=PORTC_INDEX,
	.lcd_data[2].pin=GPIO_PIN6,
	.lcd_data[2].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[2].logic=GPIO_LOW,
	
    .lcd_data[3].port=PORTC_INDEX,
	.lcd_data[3].pin=GPIO_PIN7,
	.lcd_data[3].direction=GPIO_DIRECTION_OUTPUT,
	.lcd_data[3].logic=GPIO_LOW,
	
};


dc_motor_t motor_1 =
{
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN0,
    .dc_motor_pin[0].logic = GPIO_LOW,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,  
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN1,
    .dc_motor_pin[1].logic = GPIO_LOW,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT
};


dc_motor_t motor_2 =
{
    .dc_motor_pin[0].port = PORTD_INDEX,
    .dc_motor_pin[0].pin = GPIO_PIN2,
    .dc_motor_pin[0].logic = GPIO_LOW,
    .dc_motor_pin[0].direction = GPIO_DIRECTION_OUTPUT,  
    .dc_motor_pin[1].port = PORTD_INDEX,
    .dc_motor_pin[1].pin = GPIO_PIN3,
    .dc_motor_pin[1].logic = GPIO_LOW,
    .dc_motor_pin[1].direction = GPIO_DIRECTION_OUTPUT
};



adc_conf_t adc_1 =
{
    .ADC_InterruptHandler = adcInterruptHandeler,
    .acquisition_time = ADC_12_TAD,
    .adc_channel = ADC_CHANNEL_AN0,
    .conversion_clock = ADC_CONVERSION_CLOCK_FOSC_DIV_16,
    .result_format = ADC_RESULT_RIGHT,
    .voltage_reference = ADC_VOLTAGE_REFERENCE_DISABLED
};



int main()
{ 
    application_intialize();
    WelcomingMessage();
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 1, "Temperature is : ");
    while(1)
    {
        ret = ADC_StartConversion(&adc_1); 
        
    }    
    return (EXIT_SUCCESS);
}




void WelcomingMessage()
{
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 7, "Welcome.");
    __delay_ms(1000);
    ret = lcd_4bit_send_command(&lcd_1, _LCD_CLEAR);
}

void application_intialize()
{
    ret = lcd_4bit_intialize(&lcd_1);
    ret = dc_motor_initialize(&motor_1);
    ret = dc_motor_initialize(&motor_2);
    ret = ADC_Init(&adc_1);
}


void adcInterruptHandeler()
{
    ret = ADC_GetConversionResult(&adc_1, &LM35);
    LM35 *= 0.4888f;
    ret = convert_uint8_to_string(LM35, &adcArray);
    ret = lcd_4bit_send_string_pos(&lcd_1, 2, 18, &adcArray);
    if(LM35 < 20)
    {
        ret = dc_motor_move_right(&motor_1);
        ret = dc_motor_move_left(&motor_2);
    }
    else if(LM35 > 30)
    {
        ret = dc_motor_move_right(&motor_2);
        ret = dc_motor_move_left(&motor_1);
    }
    else
    {
        ret = dc_motor_stop(&motor_1);
        ret = dc_motor_stop(&motor_2);
    }
}