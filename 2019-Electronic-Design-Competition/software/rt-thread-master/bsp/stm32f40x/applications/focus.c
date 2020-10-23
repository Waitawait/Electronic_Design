/*
 * focus.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  �佹����ͷ�豸(����ͨ��)
 */

#include "focus.h"
#include <rtthread.h>
#include <elog.h>
#include "uart.h"
#include "ret_data.h"
#include "Control.h"
/*---------------------- Constant / Macro Definitions -----------------------*/
#define OPENMV_LEN 15

/*----------------------- Variable Declarations -----------------------------*/
uint8 focus_data_ok = 0;

uint8 addFocus_Data[6] = {0xAA,0x55,0x02,0x01,0x00,0x02}; //�Ŵ�
uint8 minFocus_Data[6] = {0xAA,0x55,0x02,0x02,0x00,0x03}; //��С

uint8 addZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x01,0x02}; //�۽�
uint8 minZoom_Data[6]  = {0xAA,0x55,0x02,0x00,0x02,0x03}; //�Ž�

uint8 Camera_Clear_Data[6] = {0xAA,0x55,0x02,0x88,0x88,0x11}; //�ָ���ʼֵ
uint8 Camera_Stop_Data[6]  = {0xAA,0x55,0x02,0x00,0x00,0x01}; //ֹͣ
/*----------------------- Function Implement --------------------------------*/
uint8 focus_data[20] = {0};

int target_blob_x = 0;
int target_blob_y = 0;
int persent_blob_x = 0;
int persent_blob_y = 0;

/**
  * @brief  Focus_Zoom_Camera(����ͷ�佹���Ŵ�)
  * @param  �����ַ�����action [0x01�۽���0x02�Ž���0x11�Ŵ�0x12��С]
  * @retval None
  * @notice 
  */
void Focus_Zoom_Camera_Control(uint8 *action)
{		
		switch(*action){
				case 0x01:rt_device_write(focus_uart_device, 0,addFocus_Data,   6); break;
				case 0x02:rt_device_write(focus_uart_device, 0,minFocus_Data,   6); break;
				case 0x11:rt_device_write(focus_uart_device, 0,addZoom_Data,    6); break;
				case 0x12:rt_device_write(focus_uart_device, 0,minZoom_Data,    6); break;
				case 0x88:rt_device_write(focus_uart_device, 0,Camera_Clear_Data,    6); break; //�ָ���ʼֵ
				default  :break; //����Ϊ�������� ֹͣ����
		}
		*action = 0x00;
}



/**
  * @brief  Camera_Focus_Data_Analysis(�佹����ͷ�������ݽ���)
  * @param  �����ַ����� uint8 Data
  * @retval None
  * @notice �ӵ��ĸ��ֽڿ�ʼΪ�����ַ�
  */
void Camera_Focus_Data_Analysis(uint8 Data) //�������ݽ���
{
		static uint8 RxCheck = 0;	  //βУ����
		static uint8 RxCount = 0;	  //���ռ���
		static uint8 i = 0;	   		  //

		focus_data[RxCount++] = Data;	//���յ������ݴ��뻺������
	
	
		if(RxCount <= (OPENMV_LEN)){ //�������ݳ���δ������ͷ�Ͱ���3���ֽ�,+4)  
				if(focus_data[0] == 0xAA){ //���յ���ͷ0xAA
						if(RxCount > 4){
								if(focus_data[1] == 0x55){ //���յ���ͷ0x55
										if(RxCount >= focus_data[3]+5){ //���յ����ݰ�����λ����ʼ�ж�ʲôʱ��ʼ����У��
												for(i = 0;i <= (RxCount-2);i++){ //�ۼӺ�У��
														RxCheck += focus_data[i];
												}
			
												if(RxCheck == focus_data[RxCount-1]){
														focus_data_ok = 1; //�������ݰ��ɹ�
												}
												else {focus_data_ok = 0;}
												
												RxCheck = 0; //�����������
												RxCount = 0;	
										}
								}
								else {focus_data_ok = 0;RxCount = 0;focus_data_ok = 0;} //���ղ��ɹ�����
						}
				}
				else {focus_data_ok = 0;RxCount = 0;focus_data_ok = 0;} //���ղ��ɹ�����
		}
		else {focus_data_ok = 0;RxCount = 0;focus_data_ok = 0;} //���ղ��ɹ�����

		
	
		if(1 == focus_data[4]){//Ŀ��Ϊ1
				target_blob_x = ((focus_data[8] <<8) | focus_data[9] );
				target_blob_y = ((focus_data[10]<<8) | focus_data[11]);
		}
		else if(2 == focus_data[4]){//��ǰΪ2
				persent_blob_x = ((focus_data[8] <<8) | focus_data[9] );
				persent_blob_y = ((focus_data[10]<<8) | focus_data[11]);		
		}

}

int get_target_x(void) //0Ϊ��,1Ϊ��
{
		return target_blob_x;
}


int get_target_y(void)
{
		return target_blob_y;
}

int get_persent_x(void) //0Ϊ��,1Ϊ��
{
		return persent_blob_x;
}


int get_persent_y(void)
{
		return persent_blob_y;
}






