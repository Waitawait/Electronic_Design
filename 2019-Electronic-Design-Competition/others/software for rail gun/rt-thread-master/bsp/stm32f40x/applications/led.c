/*
 * led.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  LEDָʾ
 */
#define LOG_TAG    "led"

#include "led.h"
#include "flash.h"
#include "drv_ano.h"
#include "ioDevices.h"
#include <string.h>
#include <elog.h>
#include <drivers/pin.h>
#include <easyflash.h>
#include "sensor.h"
#include "EasyThread.h"
/*---------------------- Constant / Macro Definitions -----------------------*/

const uint8 inputdata[8] = {0x00,0x04,0x02,0x01,0x03,0x05,0x06,0x07};

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init �¼����ƿ�. */
extern struct rt_event init_event;
extern uint8 VehicleStatus;

Bling_Light Light_Red,Light_Green,Light_Blue;

uint8 Bling_Mode = 0;
/*----------------------- Function Implement --------------------------------*/

int led_thread_init(void)
{
    rt_thread_t led_tid;
		/*������̬�߳�*/
    led_tid = rt_thread_create("led",//�߳�����
                    led_thread_entry,				 //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    1024,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    15,										 	 //�߳����ȼ���priority��
                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (led_tid != RT_NULL){
				rt_pin_mode(LED_Red, 	 PIN_MODE_OUTPUT);//�������ģʽ	
				rt_pin_mode(LED_Green, PIN_MODE_OUTPUT);	
				rt_pin_mode(LED_Blue,  PIN_MODE_OUTPUT);
				rt_pin_mode(LED_Camera,PIN_MODE_OUTPUT); //OV2640 LED
			
				rt_pin_mode(Light_PIN, 	PIN_MODE_OUTPUT);	  //̽�յ�
				rt_pin_write(Light_PIN ,PIN_LOW);
				log_i("LED_Init()");
				//rt_event_send(&init_event, LED_EVENT);
				rt_thread_startup(led_tid);
		}
		return 0;
}
INIT_APP_EXPORT(led_thread_init);



/* led ��ѹָʾ��  */
void led_voltage_task(void)
{

		if(Sensor.PowerSource.Voltage >= Sensor.PowerSource.Capacity/FULL_VOLTAGE*STANDARD_VOLTAGE ){ //����ѹ���� ﮵�ر�׼��ѹʱ
				Bling_Set(&Light_Green,300,1100-10*Sensor.PowerSource.Percent,0.5,0,78,0); //����ԽС����˸Խ��
		}
		else if(Sensor.PowerSource.Voltage < Sensor.PowerSource.Capacity/FULL_VOLTAGE*STANDARD_VOLTAGE) //����ѹС��9Vʱ�������
		{
				Bling_Set(&Light_Red,300,200,0.5,0,77,0);
		}
		
}
	

/* ϵͳ��ʼ��led��˸״̬����ʾ7����ɫ�� -->[��ɫ���ı�> ��  ��  ��  ��  ��  ��  ��  ��] */
void system_init_led_blink(void)
{
		static uint8 i=0;/*��ɫ���ı�> ��   ��   ��   ��   ��   ��   ��   �� */

		do{
				if(inputdata[i] & 0x04){	
								LED_ON(LED_Red); }
				else{ 	LED_OFF(LED_Red);}
				
				if(inputdata[i] & 0x02){	
							LED_ON(LED_Green); }
				else{ LED_OFF(LED_Green);}
				
				if(inputdata[i] & 0x01){	
							 LED_ON(LED_Blue); }
				else{  LED_OFF(LED_Blue);}
				rt_thread_mdelay(300);//�ȴ�ϵͳ��ʼ��  ��ϣ�ϵͳ�ȶ����ڶ�ȡ����
		}while((i++) <= 7);//��˸8����ɫ
		
		ALL_LED_OFF();//�ر�RGB
}




void Light_Control(uint8 *action)//̽�յ�
{
		static uint8 Light_Mode_Count = 0;//̽�յ�ģʽ����
		// ̽�յ��������ػ��������ģʽ ������������ͨ����������
	
		switch(*action)
		{
				case 0x01:
						Light_Mode_Count ++;
						if(Light_Mode_Count <= 3){// ̽�յ�ǰ���ο���
								rt_pin_write(Light_PIN ,PIN_LOW);//�رռ̵���
								rt_thread_mdelay(500);//0.5s
								rt_pin_write(Light_PIN ,PIN_HIGH);//�򿪼̵���
						}
						else {
								Light_Mode_Count = 0;//̽�յ�ģʽ���� ����
								rt_pin_write(Light_PIN ,PIN_LOW);// ̽�յƵ��Ĵιر�
						}
						break;

				default:break;
		}
		*action = 0x00;//���������
}




/***************************************************
������: void Bling_Set(Bling_Light *Light,
uint32_t Continue_time,//����ʱ��
uint16_t Period,//����100ms~1000ms
float Percent,//0~100%
uint16_t  Cnt,
uint16_t Pin
,uint8_t Flag)
˵��:	״ָ̬ʾ�����ú���
���:	ʱ�䡢���ڡ�ռ�ձȡ��˿ڵ�
����:	��
��ע:	�����ʼ����ʼ������
****************************************************/
void Bling_Set(Bling_Light *Light,
               uint32 Continue_time,//����ʱ��
               uint32 Period,//����100ms~1000ms
               float Percent,//0~100%
               uint32  Cnt,
               uint32 Pin,
               uint8 Flag)
{
		Light->Bling_Contiune_Time=(Continue_time/5);//����ʱ��
		Light->Bling_Period=Period;//����
		Light->Bling_Percent=Percent;//ռ�ձ�
		Light->Pin=Pin;//����
		Light->Endless_Flag=Flag;//�޾�ģʽ
}

/***************************************************
������: void Bling_Process(Bling_Light *Light)//��˸�����߳�
˵��:	״ָ̬ʾ��ʵ��
���:	״̬�ƽṹ��     
����:	��
��ע:	�����ʼ����ʼ������
****************************************************/
void Bling_Process(Bling_Light *Light)//��˸�����߳� Cnt 
{
		if(Light->Bling_Contiune_Time>=1) { 
				Light->Bling_Contiune_Time--;
		}
		else {LED_ON(Light->Pin);}//��
		if(Light->Bling_Contiune_Time != 0//��ʱ��δ��0
				||Light->Endless_Flag==1)//�ж��޾�ģʽ�Ƿ���
		{
				Light->Bling_Cnt++;
				if(5*Light->Bling_Cnt>=Light->Bling_Period){
						Light->Bling_Cnt=0;//��������
				}
				if(5*Light->Bling_Cnt <= Light->Bling_Period * Light->Bling_Percent){	
						LED_ON(Light->Pin);   //��
				}
				else {LED_OFF(Light->Pin);}//��
		}
		else {	
				LED_OFF(Light->Pin);		//�ߵ�ƽ ��Ϩ��		
		}
}



/***************************************************
������: Bling_Working(uint16 bling_mode)
˵��:	״ָ̬ʾ��״̬��
���:	��ǰģʽ
����:	��
��ע:	�����ʼ����ʼ������
****************************************************/
void Bling_Working(uint8 bling_mode)
{
		if(0 == bling_mode)
		{
				Bling_Process(&Light_Red);
				Bling_Process(&Light_Green);
				Bling_Process(&Light_Blue);
		}
		else if(1 == bling_mode)//
		{
				Bling_Process(&Light_Red);
		}
		else if(2 == bling_mode)//
		{
				Bling_Process(&Light_Green);
		}
		else if(3 == bling_mode)//ȫ��
		{
				Bling_Process(&Light_Blue);
		}
		 
}



/* led on MSH���� */
int led_on(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_e("Error! Proper Usage: led_on r\n Species:r/g/b/c/l");
				result = -RT_ERROR;
				return result;
    }
		
		switch(*argv[1]){
				case 'r':LED_ON(LED_Red);break;
				case 'g':LED_ON(LED_Green);break;
				case 'b':LED_ON(LED_Blue);break;
				case 'c':LED_OFF(LED_Camera);break;
				case 'l':LED_OFF(Light_PIN);break;
				default:log_e("Error! Proper Usage: led_on R\n Species:r/g/b/c/l");break;
		}

		return result;
}
MSH_CMD_EXPORT(led_on,ag: led_on r  );


/* led off MSH���� */
int led_off(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
        log_e("Error! Proper Usage: led_off r\n Species:r/g/b/c/l");
				result = -RT_ERROR;
        goto _exit;
    }
		
		switch(*argv[1]){
			
				case 'r':LED_OFF(LED_Red);break;
				case 'g':LED_OFF(LED_Green);break;
				case 'b':LED_OFF(LED_Blue);break;
				case 'c':LED_ON(LED_Camera);break;
				case 'l':LED_ON(Light_PIN);break;
				default:log_e("Error! Proper Usage: led_off r\n Species:r/g/b/c/l");break;
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(led_off,ag:led_off r);


void ErrorStatus_LED(void)
{
		LED_ON(LED_Red);			//��ʼ��Ϊ�ߵ�ƽ ��Ϩ��
		LED_OFF(LED_Green);			
		LED_OFF(LED_Blue);
}

void ALL_LED_OFF(void)
{
		LED_OFF(LED_Red);			//��ʼ��Ϊ�ߵ�ƽ ��Ϩ��
		LED_OFF(LED_Green);			
		LED_OFF(LED_Blue);
}
