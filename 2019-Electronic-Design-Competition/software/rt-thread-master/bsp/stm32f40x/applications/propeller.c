/*
 * servo.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  �ƽ����豸
 */
 #define LOG_TAG    "propeller"
 
 
#include <stdlib.h>
#include <string.h>
#include "sys.h"
#include "propeller.h"
#include <elog.h>
#include <rtthread.h>

#include "flash.h"
#include "rc_data.h"
#include "drv_pwm.h"
#include "DeviceThread.h"

/*----------------------- Variable Declarations -----------------------------*/

uint8 Propeller_Init_Flag = 0;

PropellerParameter_Type PropellerParameter = {//��ʼ���ƽ�������ֵ���޷���
		 .PowerMax = 1750,//�������ֵ
		 .PowerMed = 1500,//��ֵ
		 .PowerMin = 1250,//������Сֵ�������������
	
	   .PowerDeadband = 10	//����ֵ
}; 

PropellerDir_Type    PropellerDir = {1,1,1,1,1,1};     //�ƽ�������Ĭ��Ϊ1
PropellerPower_Type  PropellerPower = {0,0,0,0,0,0,0}; //�ƽ�������������
PropellerError_Type  PropellerError = {0,0,0,0,0,0};   //�ƽ���ƫ��ֵ

PropellerPower_Type power_test; //�����õı���

PropellerError_Type Forward   = {0,0,0,0,0,0};
PropellerError_Type Retreat   = {0,0,0,0,0,0};
PropellerError_Type TurnLeft  = {0,0,0,0,0,0};
PropellerError_Type TurnRight = {0,0,0,0,0,0};
Adjust_Parameter AdjustParameter = {1,1,1,1};


/*----------------------- Function Implement --------------------------------*/




void Back_Wheel_Control(int duty)
{
		if(duty >= 0)
		{

				PropellerPower.rightUp = duty;
				PropellerPower.rightDown = 0;
		}
		else{
				PropellerPower.rightUp = 0;
				PropellerPower.rightDown = abs(duty);
			
		
		}
		TIM1_PWM_CH1_E9 (PropellerPower.rightUp);     //����	 E9	
		TIM1_PWM_CH4_E14(PropellerPower.rightDown);   //����   E14

}








//PropellerPower_Type power_test_msh; //�����õı���

///*���ƽ����� �޸� ���������ֵ��MSH���� */
//static int Propeller_Test(int argc, char **argv)
//{

//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: Propoller_Test <0~100>");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 2000){


//				//PropellerPower.rightUp     =  atoi(argv[1]);


//				PropellerPower.rightDown   =  atoi(argv[1]);




//				log_i("Current propeller power:  %d",atoi(argv[1]) );
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(Propeller_Test,ag: Propoller_Test <0~100>);



///*���ƽ����� �޸� ���������ֵ��MSH���� */
//static int propeller_maxvalue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: propeller_maxvalue_set 1600");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 2000){
//				PropellerParameter.PowerMax = atoi(argv[1]);
//				Flash_Update();
//				log_i("Current propeller max_value_set:  %d",PropellerParameter.PowerMax);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(propeller_maxvalue_set,ag: propeller set 1600);


///*���ƽ����� �޸� ���������ֵ��MSH���� */
//static int propeller_medvalue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: propeller_medvalue_set 1500");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 1500){
//				PropellerParameter.PowerMed = atoi(argv[1]);
//				Flash_Update();
//				log_i("Current propeller med_value_set:  %d",PropellerParameter.PowerMed);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(propeller_medvalue_set,ag: propeller set 1500);

///*���ƽ����� �޸� ���������ֵ��MSH���� */
//static int propeller_minvalue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: propeller_minvalue_set 1600");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 1500){
//				PropellerParameter.PowerMin = atoi(argv[1]);
//				Flash_Update();
//				log_i("Current propeller min_value_set:  %d",PropellerParameter.PowerMin);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(propeller_minvalue_set,ag: propeller set 1200);



///*���ƽ����� �޸� ������MSH���� */
//static int propeller_dir_set(int argc, char **argv) //ֻ���� -1 or 1
//{
//    int result = 0;
//    if (argc != 7){ //6���ƽ���
//				log_i("Propeller: rightUp      leftDown     leftUp     rightDown     leftMiddle    rightMiddle");   //���־ֻ���� 1  or  -1 
//        log_e("Error! Proper Usage: propeller_dir_set <1 1 1 1 1 1>  ");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		
//		if(abs(atoi(argv[1])) == 1 && abs(atoi(argv[2])) == 1  && abs(atoi(argv[3])) == 1  && \
//			 abs(atoi(argv[4])) == 1  && abs(atoi(argv[5])) == 1  && abs(atoi(argv[6])) == 1  ) {
//				 
//				PropellerDir.rightUp     = atoi(argv[1]);
//				PropellerDir.leftDown    = atoi(argv[2]);
//				PropellerDir.leftUp      = atoi(argv[3]);
//				PropellerDir.rightDown   = atoi(argv[4]);
//				PropellerDir.leftMiddle  = atoi(argv[5]);
//				PropellerDir.rightMiddle = atoi(argv[6]);
//				 
//				log_i("Propeller: rightUp      leftDown     leftUp     rightDown     leftMiddle    rightMiddle");   //���־ֻ���� 1  or  -1 
//				log_i("Propeller:    %d           %d          %d          %d            %d             %d",\
//				 PropellerDir.rightUp,PropellerDir.leftDown,PropellerDir.leftUp,PropellerDir.rightDown,PropellerDir.leftMiddle ,PropellerDir.rightMiddle);
//				Flash_Update();//FLASH����
//				rt_kprintf("\n");

//		}
//		
//		else {
//				log_e("Error! Input Error!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(propeller_dir_set,propeller <1 1 1 1 1 1>);




//uint8 is_range(short value)
//{
//		return abs(value) < 100?1:0;
//}

///*���ƽ����� �޸� ��ƫ��ֵ��MSH���� */
//static int propeller_error_set(int argc, char **argv){ 
//    int result = 0;
//    if (argc != 7){ //6���ƽ���
//				log_i("Propeller: rightUp      leftDown     leftUp     rightDown     leftMiddle    rightMiddle");   //���־ֻ���� 1  or  -1 
//        log_e("Error! Proper Usage: propeller_dir_set <-100~+100>  ");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		
//		if(is_range(atoi(argv[1])) && is_range(atoi(argv[2]))  && is_range(atoi(argv[3]))   && \
//			 is_range(atoi(argv[4])) && is_range(atoi(argv[5])) && is_range(atoi(argv[6]))  ) {
//				 
//				PropellerError.rightUp     = atoi(argv[1]);
//				PropellerError.leftDown    = atoi(argv[2]);
//				PropellerError.leftUp      = atoi(argv[3]);
//				PropellerError.rightDown   = atoi(argv[4]);
//				PropellerError.leftMiddle  = atoi(argv[5]);
//				PropellerError.rightMiddle = atoi(argv[6]);
//				 
//				log_i("Propeller: rightUp      leftDown     leftUp     rightDown     leftMiddle    rightMiddle");
//				log_i("Propeller:    %d           %d          %d          %d            %d             %d",\
//				 PropellerError.rightUp,PropellerError.leftDown,PropellerError.leftUp,PropellerError.rightDown,PropellerError.leftMiddle ,PropellerError.rightMiddle);
//				Flash_Update();//FLASH����
//				rt_kprintf("\n");

//		}
//		
//		else {
//				log_e("Error! Input Error!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(propeller_error_set,propeller_error_set <-100~+100>);
