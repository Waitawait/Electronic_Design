#ifndef __DEBUG_
#define __DEBUG_

#include "DataType.h"


/* VCANɽ����λ������ BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize);
void Vcan_Send_Data(void);

void Debug_Mode(int button);

extern char *debug_tool_name[3];

extern uint8 debug_tool ; //ɽ�� / ������λ�� ���Ա�־λ

#endif


