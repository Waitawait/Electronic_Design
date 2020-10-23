/*
 * DeviceThread.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  �豸��������
 */

#include "DeviceThread.h"
#include <rtthread.h>
#include <elog.h>
#include <math.h>
#include <stdlib.h>
#include "propeller.h"
#include "servo.h"
#include "light.h"
#include "rc_data.h"
#include "Control.h"
#include "PropellerControl.h"
#include "focus.h"
#include "debug.h"
#include "timer.h"
#include "gyroscope.h"
#include "PID.h"
#include "DataProcess.h"
#include "HMI.h"
#include "drv_i2c.h"
#include "FDC2214.h"
#include "my2490.h"
#include "nbiot.h"



void fdc2214_thread_entry(void *parameter)//�ߵ�ƽ1.5ms ������20ms  ռ�ձ�7.5% volatil
{

		rt_thread_mdelay(1000);

		rt_thread_mdelay(2000);
		uart_send_hmi_reboot();//��HMI��������λ
		uart_send_hmi_reboot();//�������� ȷ������
		DataSubsection(Cap_Division,FDC2214_Data_In_Flash);//��ȡFlash�е����ݣ��ָ�� ��������
	
		while(1)
		{
				switch(HMI_Status_Flag){
/*--------------------------- �����ݴ������� --------------------------------*/
						case 0x01:/* У׼ģʽ */
											FDC2214_Data_Adjust(); //����У׼	
											break;
						case 0x02:/* У׼ģʽ */
											FDC2214_Data_Adjust(); //����У׼
											break;
						case 0x03:/* ����ģʽ */
											Capcity_Paper_Detection(); //ֽ�Ż�ȡ��������
											break;			
						case 0x04:/* ��չ����ҳ��ѡ�� */
											rt_thread_mdelay(100);
											break;							
/*------------------------------ ��չ ���� ---------------------------------*/					
						case 0x05:/* ��ӡ�� ֽ�ż�� */
											Printer_Paper_Detection();
											break;															
						case 0x06:/* ���� ��� */
											Material_Detection();
											break;
						case 0x07:/* ֽ�Ҽ�� */
											Money_Detection();
											break;
						case 0x08:/* �ֶβ��� */
											rt_thread_mdelay(100);
											break;
						case 0xFF:
											/* Ԥ���������� */
											rt_thread_mdelay(100);
											break;
				}
				rt_thread_mdelay(3);
	
		}
	
}


int fdc2214_thread_init(void)
{
    rt_thread_t fdc2214_tid;
		/*������̬�߳�*/
    fdc2214_tid = rt_thread_create("fdc2214",//�߳�����
                    fdc2214_thread_entry,		 //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    2048,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    8,										 	 //�߳����ȼ���priority��
                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 1ms
 
    if (fdc2214_tid != RT_NULL){
			
				IIC_Init(); /* ��ʼ�� */
				rt_thread_mdelay(100);
				FDC2214_Init();
			
				rt_thread_startup(fdc2214_tid);
		}

		return 0;
}
INIT_APP_EXPORT(fdc2214_thread_init);

































