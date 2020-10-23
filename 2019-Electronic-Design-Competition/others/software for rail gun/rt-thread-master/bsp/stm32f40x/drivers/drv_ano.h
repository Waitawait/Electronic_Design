#ifndef __DRV_ANO_
#define __DRV_ANO_

#include "DataType.h"
#include "flash.h"

extern Vector3f_pid PID_Parameter[PID_USE_NUM];

void ANO_SEND_StateMachine(void);//��������ѭ������

void ANO_Data_Send_Version(int hardwareType,int hardwareVER,int softwareVER,int protocolVER,int bootloaderVER);//���Ͱ汾��Ϣ
void ANO_Data_Send_Status(int roll,int pitch,int yaw,int depth);//���ͻ�����Ϣ����̬������״̬��

void ANO_Data_Send_Voltage_Current(float volatge,float current);//���͵�ѹ����

void ANO_DT_Data_Receive_Prepare(uint8 data);//ANO����վ���ݽ���
void ANO_DT_Data_Receive_Anl(uint8 *data_buf,uint8 num);//ANO���ݽ���


void Save_Or_Reset_PID_Parameter(void);











#endif




