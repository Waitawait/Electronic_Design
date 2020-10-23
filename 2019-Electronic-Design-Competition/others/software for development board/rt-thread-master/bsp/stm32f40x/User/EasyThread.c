#include "EasyThread.h"
#include <rtthread.h>
#include <math.h>
#include "elog.h"

/**** �豸���� ****/
#include "ioDevices.h"
#include "oled.h"
#include "led.h"
#include "flash.h"
#include "drv_ano.h"



/*******************************************
* �� �� ����ioDevices_thread_entry
* ��    �ܣ�IO�豸�߳�����
* ���������none
* �� �� ֵ��none
* ע    �⣺none
********************************************/
void ioDevices_thread_entry(void* parameter)// --- ���������� ---
{
		uint8 boma_value = get_boma_value();	//��ʼ���õ���ǰ����״̬;	//�ݴ沦��״̬ �жϲ���״̬�Ƿ�ı�
	
		Buzzer_Set(&Beep,1,1);
    while (1){
				Key_Read();
				Buzzer_Process(&Beep); //��������������
				rt_thread_mdelay(10);
    }
}


/*******************************************
* �� �� ����oled_thread_entry
* ��    �ܣ�OLED��Ļ�߳�����
* ���������none
* �� �� ֵ��none
* ע    �⣺�˵���Խ�� ˢ������Խ��
********************************************/
void oled_thread_entry(void* parameter)
{
		//Boot_Animation();	//��������
		OLED_Clear();
		while(1){	
				menu_define();//�˵�����ѡ��
				rt_thread_mdelay(1000/pow(MENU+2,2)); //�˵���Խ�� ˢ������Խ��
		}
}



/*******************************************
* �� �� ����led_thread_entry
* ��    �ܣ�LED�߳�����
* ���������none
* �� �� ֵ��none
* ע    �⣺
********************************************/
void led_thread_entry(void *parameter)
{	

		system_init_led_blink();			
		rt_thread_mdelay(1000);		
    while (1)
    {			

				Bling_Working(0);
				rt_thread_mdelay(10); //10ms
    }
}
