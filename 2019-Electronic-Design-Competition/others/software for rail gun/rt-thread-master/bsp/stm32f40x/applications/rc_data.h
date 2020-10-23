#ifndef __RC_DATA_H_
#define __RC_DATA_H_

#include "DataType.h"

typedef struct{

		int16 X;    //ҡ�� X��Y��ģ���� ��X������Ϊ��ͷ����
		int16 Y;
		int16 Z;    //ZΪ��ֱ����
		int16 Fx;   //ת���� X�����
		int16 Fy;   //ת���� Y�����
		int16 Yaw;  //ƫ��
		int16 Force;//����
		float Angle;//�����ĽǶ�
}Rocker_Type;

typedef struct{
		uint16  THR;
		uint16  YAW;
		uint16  ROL;
		uint16  PIT;
	
}ReceiveData_Type;

typedef struct{
		uint8  All_Lock;       //�ܿ���  
		uint8  Depth_Lock;     //����ȡ�	 0x01 ���������0x02 �˹����� 
		uint8  Direction_Lock; //������	 0x01 ����������0x02 ��ˮ��
		uint8  Move;      		 //���˶���	 0x01 ǰ����    0x02 ���ˡ�  0x00 ��������Ĭ�ϣ�
		uint8  Translation;    //��ƽ�ơ�	 0x01 ��ƽ�ơ�  0x02 ��ƽ�ơ�0x00 ��������Ĭ�ϣ�
	  uint8  Vertical;       //����ֱ��	 0x01 ���ϡ�    0x02 ���¡�  0x00 ��������Ĭ�ϣ�
		uint8  Rotate;         //����ת��	 0x01 ������    0x02 ������  0x00��ʾ��������Ĭ�ϣ�
		uint16 Power;          //�����š�	 0x00~0xff (0~255) ��ʾ�����Ŵ�С��4��λ�ɵ���LB�ӵ���LT�������ɷֱ�����4����λ���Ŵ�С
		uint8  Light;          //���ƹ⡿	 0x01 �򿪡�    0x00 �رգ�Ĭ�ϣ�
		uint8  Focus;          //���۽���  0x01 �۽���		0x02 �Ž���  0x11 �Ŵ�0x12 ��С��0x00��ʾ��������Ĭ�ϣ� 
		uint8  Yuntai;         //����̨��	 0x01 ���ϡ�    0x02 ���¡�  0x00��ʾ��������Ĭ�ϣ�
		uint8  Arm;            //����е�ۡ�0x01 �ſ���    0x02 �رա�  0x00��ʾ��������Ĭ�ϣ�
		uint8  Raspi;          //����ݮ�ɡ�0x01 ������ݮ�ɿ�����0x00Ϊ�޿�����Ӧ��Ĭ�ϣ�
		uint8  Reserve;        //����е�ۡ�0x01 �ſ���    0x02 �رա�  0x00��ʾ��������Ĭ�ϣ�
}ControlCmd_Type;



extern Rocker_Type Rocker;
extern ControlCmd_Type ControlCmd;
extern ReceiveData_Type ReceiveData;
void Control_Cmd_Get(ControlCmd_Type *cmd); //���������ȡ
void Control_Cmd_Clear(ControlCmd_Type *cmd);//������������
void Remote_Control_Data_Analysis(uint8 Data); //�������ݽ���

// is_raspi_start(��ݮ���Ƿ񿪻�)
uint8 is_raspi_start(void);
uint8 get_button_value(ControlCmd_Type *cmd);
#endif 




