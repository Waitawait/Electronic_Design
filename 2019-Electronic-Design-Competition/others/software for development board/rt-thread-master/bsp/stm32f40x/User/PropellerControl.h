#ifndef __PROPELLERCONTROL_H
#define __PROPELLERCONTROL_H

#include "DataType.h"
#include "rc_data.h"

void AUV_Depth_Control(Rocker_Type *rc);

void ROV_Depth_Control(Rocker_Type *rc);

void ROV_Rotate_Control(Rocker_Type *rc);

void robot_upDown(float *depth_output);
void Horizontal_Propeller_Power_Clear(void);//ˮƽ������������
void Propller_Stop(void);  //�ƽ���ͣת

void robotForward(void);  //ǰ��
void robotBackAway(void); //����

void turnRight(uint16 power); //��ת
void turnLeft(uint16 power);  //��ת

void Propeller_Output(void);/* �ƽ�������ʵ��� */

extern float Expect_Depth;
#endif



