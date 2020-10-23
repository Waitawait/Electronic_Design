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
#include "DataProcess.h"
#include "flash.h"
#include <string.h>
#include "ioDevices.h"
#include <easyflash.h>
#include "flash.h"
#include "my2490.h"

/*---------------------- Constant / Macro Definitions -----------------------*/

#define HMI_LEN 5

/*----------------------- Variable Declarations -----------------------------*/

uint8 hmi_data_ok = 0;
int HMI_Status_Flag = 0;//������ �趨״̬��־λ ������ 1��or������2��
int HMI_Data = 0;
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
uint8 him_uart_reboot_cmd[7] = {0x72,0x65,0x73,0x74,0xFF,0xFF,0xFF};

uint8 him_uart_material_cmd[12] = {0x76,0x61,0x33,0x2E,0x76,0x61,0x6C,0x3D,0x30,0xff,0xff,0xff};

uint8 him_uart_money_cmd[12] = {0x76,0x61,0x34,0x2E,0x76,0x61,0x6C,0x3D,0x30,0xff,0xff,0xff};

uint8 him_ret_status = 0;

uint8 hmi_data[10] = {0};

float FDC2214_Paper_Data[100]  = {0.0f};
float FDC2214_Data_In_Flash[100] = {0.0f};//д��

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

		him_uart_cmd[8] = *cmd + 0x30; //����+0x30  ת�ɶ�Ӧ��ASCII ��Ӧд��
		rt_device_write(focus_uart_device, 0,him_uart_cmd	, sizeof(him_uart_cmd));
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
    static char adjust_str[50];
    static char adjust_str_len = 0;
		static uint8 number = 0;
		number = HMI_Page_Number % 5;
		if(HMI_Page_Number !=0 && 0 == number){
				number = 5;
		}
		sprintf(adjust_str,"t%d.txt=\"%d: %.4f\"",number,HMI_Page_Number,FDC2214_Data_In_Flash[HMI_Page_Number]);//���͸������� t1.txt="xxxx"
		
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);	
}


void uart_send_hmi_30_90_flash_data(void)  //���͸�hmi ǰ5��У׼����
{ 	
		static uint8 end[3] = {0xFF,0xFF,0xFF};
		char adjust_str[35];
		char adjust_str_len = 0;
		
		sprintf(adjust_str,"n1.val=%d",Div_Parameter.Div_30_40);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);	
		
		sprintf(adjust_str,"n2.val=%d",Div_Parameter.Div_40_50);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);			

		sprintf(adjust_str,"n3.val=%d",Div_Parameter.Div_50_60);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);			
		
		sprintf(adjust_str,"n4.val=%d",Div_Parameter.Div_60_70);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);		
		
		sprintf(adjust_str,"n5.val=%d",Div_Parameter.Div_70_80);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);		
		
		sprintf(adjust_str,"n6.val=%d",Div_Parameter.Div_80_90);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);		

		sprintf(adjust_str,"n7.val=%d",Div_Parameter.Div_90_100);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);	
}


char adjust_str[35];
char adjust_str_len = 0;
void uart_send_hmi_now_level(void)  //���͸�hmi ��ǰ�ȼ�
{ 	
		static uint8 end[3] = {0xFF,0xFF,0xFF};

		sprintf(adjust_str,"n0.val=%d",Level);//���͸������� t1.txt="xxxx"
		adjust_str_len =  strlen(adjust_str);
		rt_device_write(focus_uart_device, 0,adjust_str	,adjust_str_len);	
		rt_device_write(focus_uart_device, 0,end	, 3);	
		
}


void uart_send_hmi_is_short(void)  //���͸�hmi �Ƿ��·
{ 	
		him_uart_short_cmd[8] = 0x30 + Paper.ShortStatus;
		
		rt_device_write(focus_uart_device, 0,him_uart_short_cmd	, sizeof(him_uart_short_cmd));//��HMI���Ͷ�·��Ϣ

}

void uart_send_hmi_is_material(uint8 material)  //���͸�hmi ��ʲô����
{ 	
		him_uart_material_cmd[8] = 0x30 + material;				
		
		rt_device_write(focus_uart_device, 0,him_uart_material_cmd	, sizeof(him_uart_material_cmd));//��HMI���Ͷ�·��Ϣ
}

void uart_send_hmi_is_money(uint8 money)  //���͸�hmi ��ʲô����
{ 	
		him_uart_money_cmd[8] = 0x30 + money;				
		
		rt_device_write(focus_uart_device, 0,him_uart_money_cmd	, sizeof(him_uart_money_cmd));//��HMI���Ͷ�·��Ϣ
}


float Data_Diff_Value = 0.0f; //FLash �������εĲ�ֵ
uint32 _cnt = 0;
/* ���͸������� д���״̬
01:д��ɹ�
02:д��ʧ��
03:����д��
04:��ʾhmi����
*/
void FDC2214_Data_Adjust(void)//����У׼ �洢
{
		static char str[30] = {0};

		_cnt ++;
		FDC2214_Paper_Data[HMI_Page_Number] = get_single_capacity(); /* ��ȡ���� ��ֵ*/

		if(HMI_Page_Number > 1 && 0x01 == HMI_Debug_Write_Button){
				Data_Diff_Value = FDC2214_Paper_Data[HMI_Page_Number] - FDC2214_Paper_Data[HMI_Page_Number-1];
		}
		if(0x01 == HMI_Debug_Write_Button){//ֻ�а��²�д��
				FDC2214_Data_In_Flash[HMI_Page_Number] = FDC2214_Paper_Data[HMI_Page_Number] ;//���� ��Ӧҳ ����ֵ����
				Flash_Update();/* FLASH д�� */
				sprintf(str,"pagenum:%d,cap:%f\n",HMI_Page_Number,FDC2214_Data_In_Flash[HMI_Page_Number]);
				rt_kprintf(str);
				Paper.Status = 0x01; //д��ɹ�
				uart_send_hmi_writer_status(&Paper.Status);//����״̬��Ϣ
				HMI_Debug_Write_Button = 0; //д��״̬����

		}
		else if(0x00 == HMI_Debug_Write_Button && _cnt >= 200){
				_cnt = 0;
				Paper.Status = 0x04; //���	
				uart_send_hmi_writer_status(&Paper.Status);//����״̬��Ϣ	
		}
		Short_Circuit_Detection();  //��·���
		if(HMI_Page_Number != 0){//������0 ����
				uart_send_hmi_adjust_data(HMI_Page_Number);
		}
}


uint8 last_level = 0;
/**
  * @brief  HMI_Data_Analysis(�������������ݽ���)
  * @param  �����ַ����� uint8 Data
  * @retval None
  * @notice �ӵ��ĸ��ֽڿ�ʼΪ�����ֽ�
  */
void HMI_Data_Analysis(uint8 Data) //�������ݽ���
{

		static uint8 i = 0;	   		  
		static uint8 RxCheck = 0;	  //βУ����
		static uint8 RxCount = 0;	  //���ռ���

/*================================= ����Ϊ���ݽ��� ==================================*/					
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

/*================================= ����Ϊ���ݽ�������Ӧ�Ĵ����� ==================================*/					
		if(1 == hmi_data_ok){
				HMI_Status_Flag = hmi_data[3];//��ȡ ����ģʽλ(ҳ��)
				HMI_Data = hmi_data[4];//��ȡ ���ص�����
				switch(HMI_Status_Flag){
				
						case 0x01:
											HMI_Debug_Write_Button = 0; 	//��У׼ҳ�桿ֻ�ǵ�������ҳ��
											break;	
/*----------------------- ��ȡУ׼ģʽ --------------------------------*/			
						case 0x02:
											HMI_Page_Number = HMI_Data;//��У׼ҳ�桿 ��ȡ ָ����У׼ҳ��
											HMI_Debug_Write_Button = 1; 	//д��Flash
											break;	
/*----------------------- ��ȡ����ģʽ������������ --------------------------------*/			
						case 0x03:
											if(0x01 == HMI_Data){
													HMI_Work_Button = 1;         	//������ҳ�桿 ����������ť
											}
											else if(0x02 == HMI_Data){
													HMI_Work_Button = 2;         	//������ҳ�桿 �������								
											}
											break;				
						case 0x04:
											break;//��չ����ҳ�棬����빤��ģʽѡ���Ӧ�ĺ���				
						case 0x05:
											break;//��ӡ��ҳ�棬����빤��ģʽѡ���Ӧ�ĺ���			
						
/*----------------------- ��ȡ���ϼ�⹦�� --------------------------------*/			
					 case 0x06:
										if( 0x01 == HMI_Data){
												Material_Button = 1; 
										}
										else if(0x02 == HMI_Data){
												Material_Debug_Write_Button = 1;
										}
										else if(0x03 == HMI_Data){
												Material_Debug_Write_Button = 2;
										}
										else{
												Material_Button = 0; //�������
										}
										break;	
/*----------------------- ��ȡ�����Ҳ������� --------------------------------*/				
					 case 0x07:
										if( 0x01 == HMI_Data){
												Money_Button = 1; //�������
										}
										else if(HMI_Data >= 2){
												Money_Debug_Write_Button = HMI_Data -1;
										}
										else{
												Money_Button = 0; //�������
										}
										
/*----------------------- ��ȡ�ֶ������������ϲ������ԣ����÷ֶγ���ϵ���ķ����� --------------------------------*/										
					 case 0x08:
										uart_send_hmi_30_90_flash_data();//������ν���,���ʹ���Flash��ֵ
										break;
					 case 0x09:
										Div_Parameter.Div_30_40 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break;	
					 case 0x0A:
										Div_Parameter.Div_40_50 = HMI_Data;//��ȡ��ֵ
										Flash_Update();

										break;		
					 case 0x0B:
										Div_Parameter.Div_50_60 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break; 
					 case 0x0C:
										Div_Parameter.Div_60_70 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break;  
					 case 0x0D:
										Div_Parameter.Div_70_80 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break;  
					 case 0x0E:
										Div_Parameter.Div_80_90 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break;  
					 

/*----------------------- �ӱ����ܣ����ڲ����������ŵ�ʱ�򣬽������������ϸ��� --------------------------------*/				 
					 case 0xFE:
										uart_send_hmi_now_level();//����ȼ�ҳ��
										rt_thread_mdelay(100);
										uart_send_hmi_now_level();//����ȼ�ҳ��
										break;  
					 case 0xFF:
										uart_send_my2490_now_sounds();//������ڣ�����
								
										Level = HMI_Data;//��ȡ�ȼ�
										Flash_Update();
										break;  
					 case 0x10:
										Div_Parameter.Div_90_100 = HMI_Data;//��ȡ��ֵ
										Flash_Update();
										break;  
				}
				
			

		}
		hmi_data_ok = 0;
}







