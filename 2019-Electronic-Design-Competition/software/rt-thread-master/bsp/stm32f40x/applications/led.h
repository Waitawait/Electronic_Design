#ifndef __LED_H
#define __LED_H

#include "DataType.h"
/*---------------------- Constant / Macro Definitions -----------------------*/
//RGB�����ź�
#define LED_Red 		77 // PD8
#define LED_Green   78 // PD9
#define LED_Blue 		79 // PD10

//OV Camera �����
#define LED_Camera 	141  // PE0 �ߵ�ƽ����

//̽�յ�
#define Light_PIN  	114  //PD0 

//RGB�ƹ������ӵ�+3.3V����ƽ 0�� 1��
#define LED_ON(led_pin) 						rt_pin_write(led_pin ,PIN_LOW )
#define LED_OFF(led_pin) 						rt_pin_write(led_pin ,PIN_HIGH)
#define LED_Turn(led_pin,status) 		rt_pin_write(led_pin ,status =! status) //ȡ��





typedef struct
{
  uint32 Bling_Contiune_Time;//��˸����ʱ��
  uint32 Bling_Period;//��˸����
  float  Bling_Percent;//��˸ռ�ձ�
  uint32  Bling_Cnt;//��˸������
  uint32 Pin;//����
  uint8 Endless_Flag;//�޾�ģʽ
}Bling_Light;






void led_voltage_task(void);
void system_init_led_blink(void);

void Bling_Set(Bling_Light *Light,
               uint32 Continue_time,//����ʱ��
               uint32 Period,//����100ms~1000ms
               float Percent,//0~100%
               uint32  Cnt,
               uint32 Pin,
							 uint8 Flag);
							 
							 
							 
void Bling_Process(Bling_Light *Light);
void Bling_Working(uint8 bling_mode);
							 
void ErrorStatus_LED(void);
void ALL_LED_OFF(void);							 
void Light_Control(uint8 *action);							 

extern Bling_Light Light_Red,Light_Green,Light_Blue;
extern uint8 Bling_Mode;
						 
							 
#endif


