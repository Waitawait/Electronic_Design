#ifndef __SERVO_H
#define __SERVO_H

#include "drv_pwm.h"

typedef struct  /*  */
{
		short MaxValue;		//��� �������ֵ
		short MinValue;	  //��� ����
		short MedValue;   //��� ��ֵ
		short CurrentValue; //�����ǰֵ
		uint8 Speed;		  //����ٶ�ֵ
}ServoType;


void Servo_Dir_Control(short offest); //PWMԽ�� ,����
void RoboticArm_Control(uint8 *action);

void YunTai_Control(uint8 *action);
void DirectionProportion(int Mode);
/* ��̨���� 

��ǰx������x;��ǰy������y
*/
void yuntai_pid_control(float present_x,float expect_x,float persent_y,float expect_y) ;//

extern ServoType RoboticArm ;//��е��

extern ServoType YunTai; //��̨

extern int DirectionMode;


#endif
