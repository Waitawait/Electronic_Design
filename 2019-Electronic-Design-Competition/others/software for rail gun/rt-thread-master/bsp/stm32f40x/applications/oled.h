#ifndef __OLED_H_
#define __OLED_H_

#include "DataType.h"

void Boot_Animation(void);//��������
void menu_define(void); //�˵�����
void OLED_StatusPage(void);
void OLED_PicturePage(void);
void OLED_LockPage(void);
void OLED_GyroscopePage(void);
void draw_circle(uint8 x,uint8 y,uint8 r); //Բ��(x,y),�뾶r
void draw_line(uint8 x0,uint8 y0,float k,uint8 dot); //���̶���(x0,y0),б��k  dot:0,���;   1,���	  
void draw_fill_circle(uint8 x0,uint8 y0,uint8 r,uint8 dot);//д��ʵ��Բ��(x0,y0),�뾶r

void OLED_Clear(void);

typedef struct {
		uint8 pagenum;  				//ҳ��
		uint8 pagechange;
	  uint8 pagechange_flag;
		char *pagename[10]; //ҳ��
}Oled_Type;


typedef enum
{
		StatusPage = 1, //ҳ��ӵ�һҳ��ʼ
		GyroscopePage,
		LockPage,   
		PicturePage,
	
		OLED_Page_MAX   //ҳ�����ֵ
	
}MENU_LIST_Enum; //�˵�ö��

extern char *WorkModeName[2]; //����������ʾ�� ����ģʽ
extern char *VehicleModeName[2];
extern Oled_Type oled;
extern volatile MENU_LIST_Enum MENU;

#endif


