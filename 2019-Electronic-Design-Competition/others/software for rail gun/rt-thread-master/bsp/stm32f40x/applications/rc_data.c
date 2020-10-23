/*
 * rc_data.c
 *
 *  Created on: 2019��4��5��
 *      Author: zengwangfa
 *      Notes:  ң�������ݽ��� Remote Control Receiving Data
 */
#define LOG_TAG    "rc_data"

#include "rc_data.h"
#include "led.h"
#include "PropellerControl.h"
#include <string.h>

/*---------------------- Constant / Macro Definitions -----------------------*/

#define MAX_DATA_LENS 16  //��Ч���ݰ����ȡ������� ��ͷ������λ��У��λ��

/*----------------------- Variable Declarations -----------------------------*/

ReceiveData_Type ReceiveData = {
		.THR = 1500,
		.YAW = 1500,
		.ROL = 1500,
	  .PIT = 1500
};

ControlCmd_Type ControlCmd = { //��λ����������
										.Power = 0,
										.All_Lock = LOCK //��ʼ������Ϊ����״̬
};

Rocker_Type Rocker; // ҡ�����ݽṹ��

uint32 Frame_Conut = 0;//���ݰ� ֡����
uint8 Frame_EndFlag = 0; //�������ݰ�������־ 
uint8 RC_Control_Data[30] = {0};
uint8 Receive_Data_OK = 0;
uint8 Control_RxCheck = 0;	  //βУ����
uint8 Control_RxCount = 0;	  //���ռ���

/*----------------------- Function Implement --------------------------------*/
/**
  * @brief  Remote_Control_Data_Analysis(�������ݽ���)
  * @param  �����ַ����� uint8 Data
  * @retval None
  * @notice �ӵ��ĸ��ֽڿ�ʼΪ�����ַ�
  */
void Remote_Control_Data_Analysis(uint8 Data) //�������ݽ���
{

		static uint8 i = 0;	
	
		RC_Control_Data[Control_RxCount++] = Data;	//���յ������ݴ��뻺������
		if(Control_RxCount <= (MAX_DATA_LENS+8)){ //�������ݳ���δ������ͷ�Ͱ���3���ֽ�,+4)          
				if(RC_Control_Data[0] == 0xAA){ //���յ���ͷ0xAA
						if(Control_RxCount > 3){
								if(RC_Control_Data[1] == 0x55){ //���յ���ͷ0x55
										if(RC_Control_Data[2] > MAX_DATA_LENS+4){Receive_Data_OK = 0;Control_RxCount = 0;return;} //���ճ���Э���ַ���
											
										if(Control_RxCount >= RC_Control_Data[2]+4){ //���յ����ݰ�����λ����ʼ�ж�ʲôʱ��ʼ����У��
												for(i = 0;i <= (Control_RxCount-2);i++){ //�ۼӺ�У��
														Control_RxCheck += RC_Control_Data[i];
												}
												if(Control_RxCheck == RC_Control_Data[Control_RxCount-1]){
														Receive_Data_OK = 1; //�������ݰ��ɹ�
														Frame_EndFlag = 1;
														Frame_Conut ++;
												}
												else {Receive_Data_OK = 0;}
												 
												Control_RxCheck = 0; //�����������
												Control_RxCount = 0;	
										}
								}
								else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);return;} //���ղ��ɹ�����
						}
				}
				else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);;return;} //���ղ��ɹ�����
		}
		else {Receive_Data_OK = 0;Control_RxCount = 0;Receive_Data_OK = 0;Control_Cmd_Clear(&ControlCmd);return;} //���ղ��ɹ�����
}

		

//�����ֻ�ȡ
void Control_Cmd_Get(ControlCmd_Type *cmd) //���������ȡ
{
		if(1 == Receive_Data_OK){
				cmd->Depth_Lock     = RC_Control_Data[3]; //�������
				cmd->Direction_Lock = RC_Control_Data[4]; //��������
			
				cmd->Move					  = RC_Control_Data[5]; //ǰ��
				cmd->Translation	  = RC_Control_Data[6]; //����ƽ��
				cmd->Vertical 			= RC_Control_Data[7]; //��ֱ
				cmd->Rotate 				= RC_Control_Data[8]; //��ת
				
			  cmd->Power 				  = RC_Control_Data[9];  //��������  �ƽ�������ϵ��
				cmd->Light 				  = RC_Control_Data[10]; //�ƹ����
				cmd->Focus 				  = RC_Control_Data[11]; //�佹����ͷ����
				cmd->Yuntai 				= RC_Control_Data[12]; //��̨����
				cmd->Arm						= RC_Control_Data[13]; //��е�ۿ���
				cmd->Raspi          = RC_Control_Data[14]; //��ݮ������λ
				cmd->All_Lock       = RC_Control_Data[18];
			
				Receive_Data_OK = 0x00;//�����־λ

		}
}

/* 
	 ��ע�⡿�������տ�������ָ�������� ����״ָ̬��
		��ˣ����ڲ���mesetֱ�����ṹ��Ϊ0
*/
void Control_Cmd_Clear(ControlCmd_Type *cmd) //memset(&addr,0,sizeof(addr));
{
		//cmd->All_Lock       = 0; 
		//cmd->Depth_Lock     = 0; //�������
		//cmd->Direction_Lock = 0; //��������
		//cmd->Raspi          = 0; //��ݮ������λ
		//cmd->Power 				  = 0;  //��������  �ƽ�������ϵ��
		cmd->Move					  = 0; //ǰ��
		cmd->Translation	  = 0; //����ƽ��
		cmd->Vertical 			= 0; //��ֱ
		cmd->Rotate 				= 0; //��ת
		

		cmd->Light 				  = 0; //�ƹ����
		cmd->Focus 				  = 0; //�佹����ͷ����
		cmd->Yuntai 				= 0; //��̨����
		cmd->Arm						= 0; //��е�ۿ���

		//memset(cmd,0,sizeof(*cmd));//�ڴ�������0 �������á�
}



/**
	* @brief  is_raspi_start(��ݮ���Ƿ�����)
  * @param  None
  * @retval 1 ������   0������
  * @notice 
  */
uint8 is_raspi_start(void)
{
		return ControlCmd.Raspi; 
}
			
uint8 up_down_ret_test = 0;
/**
	* @brief  get_up_down_Key(��ݮ���Ƿ�����)
  * @param  None
  * @retval
  * @notice 
  */
uint8 get_up_down_key(ControlCmd_Type *cmd)// 0 - 50 - 100 -150 -255
{
		static uint8 last_cmd_power = 0;

		if(cmd->Power > last_cmd_power  )//���
		{
				up_down_ret_test =  1;	//����
		}
		else if(cmd->Power < last_cmd_power )//��С
		{
				up_down_ret_test =  2; //����
		}
		
		last_cmd_power = cmd->Power;
		cmd->Power = 0;//����
		return up_down_ret_test;
}
	



uint8 get_button_value(ControlCmd_Type *cmd)
{
	
		if(1 == cmd->Focus)	{
				cmd->Focus = 0;
				return 1;
		}
		if(2 == cmd->Focus)	{
				cmd->Focus = 0;
				return 2;
		}
		if(0x12 == cmd->Focus)	{
				cmd->Focus = 0;
				return 3;
		}
		if(0x11 == cmd->Focus)	{
				cmd->Focus = 0;
				return 4;
		}		
//		if(1 == get_up_down_key(cmd)){
//  			up_down_ret_test = 0;
//				return 5;
//		}
//		if(2 == get_up_down_key(cmd)){
//				up_down_ret_test = 0;
//				return 6;
//		}	
	
		if(2 == cmd->Light ){
				cmd->Light = 0;
				return 7;
		}	
		if(1 == cmd->Light ){
				cmd->Light = 0;
				return 8;
		}	
	
		if(1 == cmd->Yuntai ){
				cmd->Yuntai = 0;
				return 9;
		}	
		if(2 == cmd->Yuntai ){
				cmd->Yuntai = 0;
				return 10;
		}	
	
		if(1 == cmd->Arm ){
				cmd->Arm = 0;
				return 11;
		}	
		if(2 == cmd->Arm ){
				cmd->Arm = 0;
				return 12;
		}		
		return 0;
}













	
	
	
