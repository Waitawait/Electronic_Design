/*
 * HMI.c
 *
 *  Created on: 2019��8��7��
 *      Author: zengwangfa
 *      Notes:  ������ͨ��
 */

#include "HMI.h"
#include "DataType.h"
#include <rtthread.h>
#include <stdio.h>
#include "uart.h"
//#include "DataProcess.h"
#include "flash.h"
#include <string.h>
#include <math.h>
/*---------------------- Constant / Macro Definitions -----------------------*/

#define HMI_LEN 5

/*----------------------- Variable Declarations -----------------------------*/

uint8 hmi_data_ok = 0;
int HMI_Status_Flag = 0;//������ �趨״̬��־λ ������ 1��or������2��
int HMI_Page_Number = 0;//���������͵�У׼  ֽ����

int HMI_Debug_Write_Button = 0; //д������Flag
int HMI_Work_Button = 0; /* ��� ȷ�� ��ť*/
int Material_Button = 0; /* ���� ��� ȷ�ϰ�ť*/
int Material_Debug_Write_Button = 0; /* ���� д������*/

int Money_Button = 0; /* ֽ�� ��� ȷ�ϰ�ť*/
int Money_Debug_Write_Button = 0; /* ֽ��  д������*/
//76 61 30 2E 76 61 6C 3D 31 ff ff ff
//76 61 30 2E 76 61 6C 3D 31 ff ff ff   va0.val=1
uint8 him_uart_cmd[12] = {0x76,0x61,0x30,0x2E,0x76,0x61,0x6C,0x3D,0x31,0xFF,0xFF,0xFF};    // 01 д��ɹ�  02 д��ʧ��   03 ����д��... 04 ��ʾhmi����
//76 61 31 2E 76 61 6C 3D 31 30 30 ff ff ff
//76 61 31 2E 76 61 6C 3D 31 30 30 ff ff ff  va1.val=100
uint8 him_uart_nmber_cmd[14] = {0x76,0x61,0x31,0x2E,0x76,0x61,0x6C,0x3D,0x31,0x30,0x30,0xff,0xff,0xff};

//76 61 32 2E 76 61 6C 3D 31 ff ff ff   va2.val=1
uint8 him_uart_short_cmd[12] = {0x76,0x61,0x32,0x2E,0x76,0x61,0x6C,0x3D,0x30,0xff,0xff,0xff};
// rest (HMI��λָ��)72 65 73 74 ff ff ff
uint8 him_uart_reboot_cmd[7] = {0x72,0x65,0x73,0x74,0xff,0xff,0xff};

uint8 him_ret_status = 0;
uint8 hmi_data[10] = {0};

float FDC2214_Paper_Data[100]  = {0.0f};
float FDC2214_Data_In_Flash[100] = {0.0f};//д��



int Rail_Gun_Distance = 250;
int Rail_Gun_Angle = 0;
/*----------------------- Function Implement --------------------------------*/

void uart_send_hmi_reboot(void)
{
		rt_device_write(focus_uart_device, 0,him_uart_reboot_cmd	, 7);
}

/* ���͸������� д���״̬
01:д��ɹ�
02:д��ʧ��
03:����д��
04:��ʾhmi����
*/
void uart_send_hmi_writer_status(uint8 *cmd)//���͸� hmiд���״̬
{

		*cmd += 0x30; //����+0x30  ת�ɶ�Ӧ��ASCII ��Ӧд��
		rt_device_write(focus_uart_device, 0,him_uart_cmd	, sizeof(him_uart_cmd));

		*cmd = 0; //��������
}



void uart_send_hmi_paper_numer(uint8 N_number)  //���͸�hmi ֽ������
{ 	

		him_uart_nmber_cmd[8]  = (N_number/100%10) + 0x30;  //��λ
		him_uart_nmber_cmd[9]  = (N_number/10%10) + 0x30; //ʮλ
		him_uart_nmber_cmd[10] = (N_number/1%10) + 0x30;//��λ
	
		rt_device_write(focus_uart_device, 0,him_uart_nmber_cmd	, sizeof(him_uart_nmber_cmd));	

}


void uart_send_hmi_adjust_data(uint8 N_number)  //���͸�hmi ǰ5��У׼����
{ 	
		static uint8 end[3] = {0xFF,0xFF,0xFF};
    static char adjust_str[35];
    static char adjust_str_len = 0;
		static uint8 number = 0;
		number = HMI_Page_Number % 5;
		if(HMI_Page_Number !=0 && 0 == number){
				number = 5;
		}
		sprintf(adjust_str,"t%d.txt=\"%d: %.4f\"",number,HMI_Page_Number,FDC2214_Data_In_Flash[HMI_Page_Number]);//���͸������� t1.txt="xxxx"
		
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_kprintf(adjust_str);
		rt_kprintf("sizeof(str):%d", sizeof(adjust_str));
		rt_device_write(focus_uart_device, 0,end	, 3);	

}





/**
  * @brief  HMI_Data_Analysis(�������������ݽ���)
  * @param  �����ַ����� uint8 Data
  * @retval None
  * @notice �ӵ��ĸ��ֽڿ�ʼΪ�����ֽ�
  */
void HMI_Data_Analysis(uint8 Data) //�������ݽ���
{

		static uint8 i = 0;	   		  //
		static uint8 RxCheck = 0;	  //βУ����
		static uint8 RxCount = 0;	  //���ռ���

		hmi_data[RxCount++] = Data;	//���յ������ݴ��뻺������
	
	
		if(RxCount <= (HMI_LEN+4)){ //�������ݳ���δ������ͷ�Ͱ���3���ֽ�,+4)  
				if(hmi_data[0] == 0xAA){ //���յ���ͷ0xAA
						if(RxCount > 3){
								if(hmi_data[1] == 0x55){ //���յ���ͷ0x55
										if(RxCount >= hmi_data[2]+4){ //���յ����ݰ�����λ����ʼ�ж�ʲôʱ��ʼ����У��
												for(i = 0;i <= (RxCount-2);i++){ //�ۼӺ�У��
														RxCheck += hmi_data[i];
												}
			
												if(RxCheck == hmi_data[RxCount-1]){
														hmi_data_ok = 1; //�������ݰ��ɹ�
												}
												else {hmi_data_ok = 0;}
												
												RxCheck = 0; //�����������
												RxCount = 0;	
										}
								}
								else {hmi_data_ok = 0;RxCount = 0;hmi_data_ok = 0;} //���ղ��ɹ�����
						}
				}
				else {hmi_data_ok = 0;RxCount = 0;hmi_data_ok = 0;} //���ղ��ɹ�����
		}
		else {hmi_data_ok = 0;RxCount = 0;hmi_data_ok = 0;} //���ղ��ɹ�����

		
		if(1 == hmi_data_ok){
				HMI_Status_Flag = hmi_data[3];//��ȡ ����ģʽλ
				
				if( 0x01 == hmi_data[3] && hmi_data[4] != 0xFF){			
						Rail_Gun_Distance = hmi_data[4];
						if(Rail_Gun_Distance < 200){
								Rail_Gun_Distance = 255 + hmi_data[4];
						}
				}
						
				if( 0x02 ==  hmi_data[3]){
						Rail_Gun_Angle = hmi_data[4] ;
						if(Rail_Gun_Angle > 128){
								Rail_Gun_Angle = (hmi_data[4]) - 255;
						}
						
				}	
				
				if(0x03 ==  hmi_data[3]){
						HMI_Work_Button = 0x01;
				}				
		}
		hmi_data_ok = 0;
}







