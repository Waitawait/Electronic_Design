#ifndef __MY2490_H_
#define __MY2490_H_
#include "DataType.h"

void uart_send_my2490_now_status(uint8 *cmd_array,uint32 number);//���ڷ��͸� MY2490��ǰֽ������

extern uint8 my2490_number_array[7] ; //my2490��Ӧ��Ŀ
extern uint8 my2490_louder_array[7] ; //my2490  ����+
extern uint8 my2490_lower_array[7]  ; //my2490 ����-

void uart_send_my2490_now_sounds(void);//���ڷ��͸� ������Ч
void Volunm_Max(void);  //ʹ�������
#endif
