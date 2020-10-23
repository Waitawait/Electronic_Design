#ifndef __IO_DEVICES_H
#define __IO_DEVICES_H

#include "DataType.h"
typedef struct 
{
		uint8 time;
		uint8 count;
		uint8 number;	
}Buzzer_Type;

uint8 get_boma_value(void);
uint8 is_wifi_connect(void);

void key_down(void *args);

void Buzzer_Init(void);
	
void buzzer_once(void);

/* countΪ��Ĵ���  length���ʱ�䳤��  */
void Buzzer_Set(Buzzer_Type* buzzer,uint8 count,uint8 length);

void Buzzer_Process(Buzzer_Type * buzzer);

extern Buzzer_Type Beep;

void work_button(void *args);

/* �������²��������� */
void next_oled(void *args)  ;


/* �������²��������� */
void paper_add(void *args)  ;

/* �������²��������� */
void paper_min(void *args)  ;


void Key_Read(void);

#endif



