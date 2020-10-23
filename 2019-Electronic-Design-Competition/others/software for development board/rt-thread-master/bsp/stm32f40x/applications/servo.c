/*
 * servo.c
 *
 *  Created on: 2019��3��30��
 *      Author: zengwangfa
 *      Notes:  ����豸
 */
#define  LOG_TAG    "servo"
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <elog.h>

#include "sys.h"

#include "DataType.h"
#include "Control.h"
#include "ioDevices.h"
#include "servo.h"
#include "propeller.h"
#include "flash.h"
#include "ret_data.h"
#include "rc_data.h"
#include "focus.h"
#include "DeviceThread.h"
#include "PID.h"
/*---------------------- Constant / Macro Definitions -----------------------*/

#define RoboticArm_MedValue  1500
#define YunTai_MedValue  		 2000

/*----------------------- Variable Declarations -----------------------------*/

ServoType RoboticArm = {
		 .MaxValue = 2000, 		//��е�� �������ֵ
		 .MinValue = 1000,	  //��е�� ����
		 .MedValue = 1500,
	   .Speed  = 5//��е�۵�ǰֵ
};  //��е��
ServoType  YunTai = {
		 .MaxValue = 1700, 		// �������ֵ
		 .MinValue = 1100,	  // ����
		 .MedValue = 1400,
	   .Speed  = 10//��̨ת���ٶ�
};      //��̨


uint16 propeller_power = 1500;



/*----------------------- Function Implement --------------------------------*/


/*******************************************
* �� �� ����Servo_Output_Limit
* ��    �ܣ�����������
* �������������ֵ������ṹ���ַ 
* �� �� ֵ��None
* ע    �⣺
********************************************/
void Servo_Output_Limit(ServoType *Servo)
{
		Servo->CurrentValue = Servo->CurrentValue > Servo->MaxValue ? Servo->MaxValue : Servo->CurrentValue ;//�����޷�
		Servo->CurrentValue = Servo->CurrentValue < Servo->MinValue ? Servo->MinValue : Servo->CurrentValue ;//�����޷�
	
}







/*  С����� ����*/
void Servo_Dir_Control(short offest) //PWMԽ�� ,����
{

	
		//YunTai.CurrentValue = YunTai.MedValue + (offest);
	
		Servo_Output_Limit(&YunTai); //�޷�
		TIM4_PWM_CH4_D15(YunTai.CurrentValue);  //���

}


/* ��̨���� 

��ǰx������x;��ǰy������y
*/
void yuntai_pid_control(float present_x,float expect_x,float persent_y,float expect_y) //
{

    /* ����� Ϊ X*/
		Total_Controller.Yaw_Angle_Control.FeedBack = present_x;  //��ǰ��ȷ���
		Total_Controller.Yaw_Angle_Control.Expect = expect_x ; //���������ң��������	
	
		/* ������ Ϊ Y*/
		Total_Controller.Pitch_Angle_Control.FeedBack = persent_y;  //��ǰ��ȷ���
		Total_Controller.Pitch_Angle_Control.Expect = expect_y ; //���������ң��������
	
	
		PID_Control(&Total_Controller.Pitch_Angle_Control);//�߶�λ�ÿ�����
		PID_Control(&Total_Controller.Yaw_Angle_Control);//�߶�λ�ÿ�����
	
}

/*���ƽ����� �޸� ���������ֵ��MSH���� */
static int pitch_yuntai(int argc, char **argv)
{

    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: Propoller_Test <0~100>");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 200000){

				PropellerPower.rightMiddle = atoi(argv[1]);

				log_i("Current propeller power:  %d",atoi(argv[1]) );
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(pitch_yuntai,ag: two_axis_yuntai 1000 1000);



/*���ƽ����� �޸� ���������ֵ��MSH���� */
static int yaw_yuntai(int argc, char **argv)
{

    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: Propoller_Test <0~100>");
				result = -RT_ERROR;
        goto _exit;
    }
		if(atoi(argv[1]) <= 200000){

				PropellerPower.leftMiddle = atoi(argv[1]);

				log_i("Current propeller power:  %d",atoi(argv[1]) );
		}
		
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(yaw_yuntai,ag: two_axis_yuntai 1000 1000);


/**
  * @brief  RoboticArm_Control(��е�ۿ���)
  * @param  ����ָ�� 0x00��������  0x01���ſ�  0x02���ر�
  * @retval None
  * @notice 
  */
void RoboticArm_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:RoboticArm.CurrentValue += RoboticArm.Speed;
									if(RoboticArm.CurrentValue >= RoboticArm.MaxValue){device_hint_flag |= 0x01;}//��е�۵�ͷ��־
									else {device_hint_flag &= 0xFE;}; //�����е�۵�ͷ��־

									break;
				case 0x02:RoboticArm.CurrentValue -= RoboticArm.Speed;
									if(RoboticArm.CurrentValue <= RoboticArm.MinValue){device_hint_flag |= 0x01;}//��е�۵�ͷ��־
									else {device_hint_flag &= 0xFE;}; //�����е�۵�ͷ��־

									break;
				default:break;
		}
		Servo_Output_Limit(&RoboticArm);//��е�۶���޷�
		TIM4_PWM_CH3_D14(RoboticArm.CurrentValue);
		*action = 0x00; //���������
}




/**
  * @brief  YunTai_Control(��̨����)
  * @param  ����ָ�� 0x00��������  0x01������  0x02������
  * @retval None
  * @notice 
  */

void YunTai_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:YunTai.CurrentValue += YunTai.Speed;  //����
						if(YunTai.CurrentValue <= YunTai.MaxValue){device_hint_flag |= 0x02;}//��̨��ͷ��־
						else {device_hint_flag &= 0xFD;}; //�����̨��ͷ��־

						break;  
						
				case 0x02:YunTai.CurrentValue -= YunTai.Speed; //����
						if(YunTai.CurrentValue >= YunTai.MinValue){device_hint_flag |= 0x02;}//��̨��ͷ��־
						else {device_hint_flag &= 0xFD;}; //�����̨��ͷ��־

						break;  

				case 0x03:YunTai.CurrentValue = YunTai.MedValue;break;   //����
				default: break;
		}
		Servo_Output_Limit(&YunTai);
		//TIM4_PWM_CH4_D15(YunTai.CurrentValue); 
		*action = 0x00; //���������
}







//short _test_value = 0;
///*����ȡ���� �޸� �����ԡ� MSH���� */
//static int xiquqi_value_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: robotic_arm_currentValue_set <0~3000>");
//				result = -RT_ERROR;
//        goto _exit;
//    }

//		if(atoi(argv[1]) <= 3000 ){		
//				_test_value = atoi(argv[1]);
//				log_i(" Value:  %d",_test_value);
//				TIM3_PWM_CH3_B0(_test_value);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(xiquqi_value_set,ag: xiquqi_value_set <0~3000>);





///*����̨����� �޸� ����ǰ�� MSH���� */
//static int robotic_arm_currentValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: robotic_arm_currentValue_set <0~3000>");
//				result = -RT_ERROR;
//        goto _exit;
//    }

//		if(atoi(argv[1]) <= 3000 ){		
//				RoboticArm.CurrentValue = atoi(argv[1]);
//				log_i(" Value:  %d",RoboticArm.CurrentValue);
//				TIM4_PWM_CH3_D14(RoboticArm.CurrentValue);


//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(robotic_arm_currentValue_set,ag: robotic_arm_currentValue_set <0~3000>);





///*����е�ۡ���� �޸� �ٶ�ֵ */
//static int robotic_arm_speed_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: RoboticArm_Speed 10");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 255 && atoi(argv[1]) > 0){
//				RoboticArm.Speed = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed! RoboticArm.Speed:  %d",RoboticArm.Speed);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(robotic_arm_speed_set,ag: robotic_arm_speed_set 10);


///*����е�ۡ���� �޸� ���������ֵ��MSH���� */
//static int robotic_arm_maxValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: RoboticArm_Maxvalue_set 1600");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 5000 && atoi(argv[1]) >= 1500){
//				RoboticArm.MaxValue = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed!  RoboticArm.MaxValue:  %d",RoboticArm.MaxValue);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(robotic_arm_maxValue_set,ag: robotic_arm_openvalue_set 2000);




///*����е�ۡ���� �޸� ���������ֵ�� MSH���� */
//static int robotic_arm_minValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: RoboticArm_minvalue_set 1150");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 3000 &&  atoi(argv[1]) >= 500){
//				RoboticArm.MinValue = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed!  RoboticArm.MinValue:  %d",RoboticArm.MinValue);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}

//		
//		
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(robotic_arm_minValue_set,ag: robotic_arm_closevalue_set 1500);


///*����е�ۡ���� �޸� �ٶ�ֵ */
//static int yuntai_speed_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: yuntai_speed_set 5");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 255 && atoi(argv[1]) > 0){
//				YunTai.Speed = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed! YunTai.Speed:  %d",YunTai.Speed);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai_speed_set,ag: yuntai_speed_set 5);

///*����̨����� �޸� ���������ֵ��MSH���� */
//static int yuntai_maxValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: YunTai_maxvalue_set 1600");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 5000){
//				YunTai.MaxValue = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed! YunTai.MaxValue:  %d",YunTai.MaxValue);
//		}
//		
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai_maxValue_set,ag: yuntai_maxvalue_set 2500);




///*����̨����� �޸� ���������ֵ�� MSH���� */
//static int yuntai_minValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: YunTai_minvalue_set 1150");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 3000){
//				YunTai.MinValue = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed! YunTai.MinValue:  %d",YunTai.MinValue);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}

//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai_minValue_set,ag: yuntai_arm_closevalue_set 1500);

///*����̨����� �޸� ���������ֵ�� MSH���� */
//static int yuntai_medValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 3000){
//				YunTai.MedValue = atoi(argv[1]);
//				Flash_Update();
//				log_i("Write_Successed! YunTai.MedValue):  %d",YunTai.MedValue);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}

//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai_medValue_set,ag: yuntai_arm_medvalue_set 2000);



///*����̨����� �޸� ����ǰ�� MSH���� */
//static int yuntai_currentValue_set(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 3000 && atoi(argv[1]) >= 500){
//				YunTai.CurrentValue = atoi(argv[1]);
//				log_i("Write_Successed! Current YunTai.CurrentValue:  %d",YunTai.CurrentValue);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}

//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai_currentValue_set,ag: yuntai_currentValue_set 1500);


///*����̨����� �޸� ����ǰ�� MSH���� */
//static int yuntai(int argc, char **argv)
//{
//    int result = 0;
//		ServoType servo;
//    if (argc > 1){
//        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		rt_kprintf("Set Min Value:");
//		scanf("%d",(int *)(&servo.MinValue));

//		if(servo.MinValue <= 7000 ){		
//				log_i("Write_Successed! servo.MinValue  %d",servo.MinValue);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}

//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(yuntai,ag: yuntai_currentValue_set 1500);


