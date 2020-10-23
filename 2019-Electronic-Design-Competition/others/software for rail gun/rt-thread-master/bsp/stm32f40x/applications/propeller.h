#ifndef __PROPELLER_H
#define __PROPELLER_H

#include "DataType.h"


typedef struct //�ƽ�����ֵ�����ֵ����Сֵ
{
		int16 PowerMax;		
		int16 PowerMed;
		int16 PowerMin;	
	  int16 PowerDeadband;	//����ֵ
	
}PropellerParameter_Type;//�ƽ��������ṹ��

typedef struct //�ƽ�������
{
		int leftUp;  //����
		int rightUp;
		int leftDown; //����
		int rightDown;
		int leftMiddle; //����
		int rightMiddle;		

}PropellerDir_Type;

typedef struct//����ʱ�����ƽ�����ƫ���� 
{
		int16 leftUp;
		int16 rightUp;
		int16 leftDown;
		int16 rightDown;
		int16 leftMiddle;
		int16 rightMiddle;

}PropellerError_Type;//�ƽ���ƫ��ֵ

typedef struct
{
		int16 Adjust1;
		int16 Adjust2;
		int16 Adjust3;
		int16 Adjust4;
}Adjust_Parameter; //���ڲ���

typedef struct
{
	  float PowerPercent; //�ƽ��������ٷֱ�
	
		short leftUp;
		short rightUp;
		short leftDown;
		short rightDown;
		short leftMiddle;
		short rightMiddle;

}PropellerPower_Type;//�����ƽ�������

typedef enum
{
		RiseUp    = 1, //����
		Dive      = 2, //��Ǳ
		Stop      = 0
		
}ActionType_Enum; //����ָ��ö��

void Back_Wheel_Control(int duty);
void Propeller_Init(void);

void PWM_Update(PropellerPower_Type* power);//�ƽ���PWM ����
void Extractor_Control(uint8 *action);

extern PropellerDir_Type PropellerDir;
extern PropellerParameter_Type PropellerParameter; 
extern PropellerPower_Type  PropellerPower; //�ƽ������������
extern PropellerError_Type  PropellerError; //�ƽ���ƫ��ֵ

extern PropellerError_Type Forward;
extern PropellerError_Type Retreat;
extern PropellerError_Type TurnLeft;
extern PropellerError_Type TurnRight;
extern Adjust_Parameter AdjustParameter;

#endif




