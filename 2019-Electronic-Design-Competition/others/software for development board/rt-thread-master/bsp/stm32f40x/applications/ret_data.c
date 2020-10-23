/*
 * ret_data.c
 *
 *  Created on: 2019��4��20��
 *      Author: zengwangfa
 *      Notes:  �������ݰ� Return Data
 */
#define LOG_TAG    "ret_data"

#include "ret_data.h"
#include "sensor.h"
#include <rtthread.h>
#include "uart.h"
#include "gyroscope.h"

/*----------------------- Variable Declarations -----------------------------*/

uint8 Return_Data[22] = {0};
uint8 device_hint_flag;		//�豸��ʾ�ַ�

/*----------------------- Function Implement --------------------------------*/
//void return_computer_thread_entry(void* parameter)
//{
//		static uint8 begin_buff[3] = {0xAA,0x55,0x00};
//		
//		rt_thread_mdelay(5000);//�ȴ�5s �豸��ʼ����ɺ�ʼ����
//		
//		while(uart_startup_flag){ //��debug_uart��ʼ����Ϻ� �Ž�����λ��ͨ��
//			
//				Convert_Return_Computer_Data(&Sensor); //ת��������λ��������
//				Send_Buffer_Agreement(begin_buff,Return_Data,22); //�������ݰ�Э��
//				rt_thread_mdelay(100);
//		}
//}

//int return_computer_thread_init(void)
//{
//    rt_thread_t return_computer_tid;
//		/*������̬�߳�*/
//    return_computer_tid = rt_thread_create("return_data",//�߳�����
//													return_computer_thread_entry,				 //�߳���ں�����entry��
//													RT_NULL,							   //�߳���ں���������parameter��
//													2048,										 //�߳�ջ��С����λ���ֽڡ�byte��
//													15,										 	 //�߳����ȼ���priority��
//													10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

//    if (return_computer_tid != RT_NULL){

//				rt_thread_startup(return_computer_tid);
//		}
//		return 0;
//}
//INIT_APP_EXPORT(return_computer_thread_init);




///**
//  * @brief  get_decimal(�õ�����������ͷ��λС����100��)
//  * @param  ���������� data
//  * @retval ͷ��λС����100��
//  * @notice 
//  */
//uint8 get_decimal(float data){ //�õ������� ��1λС��λ

//		return (uint8)((float)(data - (int)data)*100);
//}

///**
//  * @brief  Convert_Return_Computer_Data(ת��������λ�������ݰ�)
//  * @param  None
//  * @retval None
//  * @notice 
//  */
//void Convert_Return_Computer_Data(Sensor_Type *sensor) //������λ������ ת��
//{
//		static short temp_Roll = 0; //�ݴ�����
//		static short temp_Pitch = 0;
//		static short temp_Yaw = 0;
//	
//		temp_Roll  = (short)((sensor->JY901.Euler.Roll+180) *100);  //����ת��:���Ƕ�����תΪ��ֵ���Ŵ�100��
//		temp_Pitch = (short)((sensor->JY901.Euler.Pitch+180)*100);
//		temp_Yaw   = (short)((sensor->JY901.Euler.Yaw+180)*100);
//	
//		Return_Data[0] = sensor->PowerSource.Voltage; //������
//		Return_Data[1] = get_decimal(sensor->PowerSource.Voltage);//С����100��
//	
//		Return_Data[2] = (int)sensor->CPU.Temperature; //������
//		Return_Data[3] = get_decimal(sensor->CPU.Temperature) ;//С����100��
//	
//		Return_Data[4] = (int)sensor->DepthSensor.Temperature; //������
//		Return_Data[5] = get_decimal(sensor->DepthSensor.Temperature);//С����100��	
//	
//		Return_Data[6] = (int)(sensor->DepthSensor.Depth) >> 16; //��8λ
//		Return_Data[7] = (int)(sensor->DepthSensor.Depth) >> 8 ;//��8λ
//		Return_Data[8] = (int)(sensor->DepthSensor.Depth) ; //��8λ
//	
//		Return_Data[9]  = temp_Yaw>> 8 ; // Yaw ��8λ
//		Return_Data[10] = (uint8)temp_Yaw; //��8λ
//	
//		Return_Data[11] = temp_Pitch >> 8;// Pitch ��8λ
//		Return_Data[12] = (uint8)temp_Pitch;//��8λ
//	
//		Return_Data[13] = temp_Roll >> 8; // Roll ��8λ
//		Return_Data[14] = (uint8)temp_Roll; //��8λ

//		Return_Data[15] = (uint8)sensor->JY901.Speed.x;//x�ẽ��
//		Return_Data[16] = 0x02;//device_hint_flag;  //�豸��ʾ�ַ�
//}

///**
//  * @brief  Calculate_Check_Byte(����õ����У��λ)
//  * @param  ��ʼ��ͷ*begin_buff�����ݰ�*buff�����ݰ�����len
//  * @retval None
//  * @notice 
//  */
//uint8 Calculate_Check_Byte(uint8 *begin_buff,uint8 *buff,uint8 len)
//{
//		uint8 Check_Byte = 0;
//		uint8 i = 0;
//		for(i = 0;i < 3;i++){
//				Check_Byte += begin_buff[i];
//		}
//		for(i = 0;i < len;i++){
//			 	Check_Byte += buff[i];
//		}
//		return Check_Byte;
//}

///**
//	* @brief  Send_Buffer_Agreement(�������ݰ���Э��)
//  * @param  ��ʼ��ͷ*begin_buff�����ݰ�*buff�����ݰ�����len
//  * @retval None
//  * @notice 
//  */
//void Send_Buffer_Agreement(uint8 *begin_buff,uint8 *buff,uint8 len)
//{
//		uint8 Check_Byte = Calculate_Check_Byte(begin_buff ,buff ,len); //����У��λ
//		
//		begin_buff[2] = len; //����λ
//		rt_device_write(control_uart_device,0,begin_buff, 3);    //���Ͱ�ͷ
//		rt_device_write(control_uart_device,0,buff, len);    		 //�������ݰ�
//		rt_device_write(control_uart_device,0,&Check_Byte, 1);   //����У��λ
//}





















