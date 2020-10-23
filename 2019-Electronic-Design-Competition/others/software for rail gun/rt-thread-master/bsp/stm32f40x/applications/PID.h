#ifndef __PID_H_
#define __PID_H_

#include "DataType.h"
#include "filter.h"

typedef struct
{
		uint8 Err_Limit_Flag :1;//ƫ���޷���־   λ����
		uint8 Integrate_Limit_Flag :1;//�����޷���־
		uint8 Integrate_Separation_Flag :1;//���ַ����־
		float Expect;//����
		float FeedBack;//����ֵ
		float Err;//ƫ��
		float Last_Err;//�ϴ�ƫ��
		float Err_Max;//ƫ���޷�ֵ
		float Integrate_Separation_Err;//���ַ���ƫ��ֵ
		float Integrate;//����ֵ
		float Integrate_Max;//�����޷�ֵ
		float Kp;//���Ʋ���Kp  12
		float Ki;//���Ʋ���Ki  13
		float Kd;//���Ʋ���Kd  14
		float Control_OutPut;//�����������
		float Last_Control_OutPut;//�ϴο����������
		float Control_OutPut_Limit;//����޷�
		/***************************************/
		float Pre_Last_Err;//���ϴ�ƫ��
		float Adaptable_Kd;//����Ӧ΢�ֲ���
		float Last_FeedBack;//�ϴη���ֵ
		float Dis_Err;//΢����
		float Dis_Error_History[5];//��ʷ΢����
		float Err_LPF;
		float Last_Err_LPF;
		float Dis_Err_LPF;
		float Last_Dis_Err_LPF;
		float Pre_Last_Dis_Err_LPF;
		Butter_BufferData Control_Device_LPF_Buffer;//��������ͨ�����������
		float Scale_Kp;
		float Scale_Ki;
		float Scale_Kd;
}PID_Controler;


typedef struct
{
		PID_Controler Roll_Gyro_Control;  //���ٶ�
		PID_Controler Pitch_Gyro_Control;
		PID_Controler Yaw_Gyro_Control;
	
		PID_Controler Roll_Angle_Control; //���Ƚ�
		PID_Controler Pitch_Angle_Control;
		PID_Controler Yaw_Angle_Control;
	
		PID_Controler High_Speed_Control;  //�߶�
		PID_Controler High_Position_Control;
	

}AllControler;

typedef enum
{

		Roll_Gyro_Controler=0,  //���ٶ�
		Pitch_Gyro_Controler=1,
		Yaw_Gyro_Controler=2,
	
		Roll_Angle_Controler=3,  //�Ƕ�
		Pitch_Angle_Controler=4, 
		Yaw_Angle_Controler=5,
	
		High_Speed_Controler=6,	
		High_Position_Controler=7

}Controler_Label;


typedef struct
{
  float Last_Time;
  float Now_Time;
  float Time_Delta;
  uint16 Time_Delta_INT;//��λms
}Testime;

extern AllControler Total_Controller;


void Total_PID_Init(void);
float	PID_Control(PID_Controler *Controler);//���θ߶�λ�ÿ�����
float PID_Control_Yaw(PID_Controler *Controler);
float PID_Control_Div_LPF(PID_Controler *Controler);
float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter);

#endif




