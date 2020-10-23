#ifndef __HMI_H_
#define __HMI_H_

#include "DataType.h"

void HMI_Data_Analysis(uint8 Data); //�������ݽ���
uint32 get_set_page_number(void);

uint32 get_set_status(void);

/* �������͸�HMI�� */
void uart_send_hmi_writer_status(uint8 *cmd);//���͸� hmiд���״̬

void uart_send_hmi_paper_numer(uint8 N_number);  //���͸�hmi ֽ������

void uart_send_hmi_is_short(void); //���͸�hmi �Ƿ��·

void uart_send_hmi_reboot(void);//��him��λ

void uart_send_hmi_is_material(uint8 material);  //���͸�hmi ��ʲô����

void uart_send_hmi_is_money(uint8 money);  //���͸�hmi ��ʲô����
/* -------------  */
extern int HMI_Status_Flag ;//������ �趨״̬��־λ ������ 1��or������2��

extern int HMI_Page_Number ;//���������͵�У׼  ֽ����

extern float FDC2214_Data_In_Flash[100];
extern float FDC2214_Paper_Data[100];

extern int HMI_Status_Flag;
extern int Material_Button; 
extern int Material_Debug_Write_Button; /* ���� д������*/

extern int Money_Button; 
extern int Money_Debug_Write_Button; /* ���� д������*/

extern float Data_Diff_Value; //FLash �������εĲ�ֵ

extern int HMI_Work_Button ; /* ��� ȷ�� ��ť*/
#endif


