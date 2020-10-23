/*
 * oled.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  OLED״̬������ָʾ
 */
#define LOG_TAG    "oled"

#include "oled.h"
#include <rtthread.h>
#include <math.h>
#include <elog.h>
#include <stdio.h>
#include "sys.h"

#include "led.h"
#include "rc_data.h"
#include "drv_cpu_temp.h"
#include "drv_cpuusage.h"
#include "drv_oled.h"
#include "drv_adc.h"
#include "sensor.h"

#include "ioDevices.h"

#include "gyroscope.h"
#include "sensor.h"
#include "filter.h"

#include "EasyThread.h"
/* �Զ���OLED ����ϵ����: 

	127 ��y
			---------
			|	      |
			|				|
			|				|
			|				|
			|				|
			|				|
		(0,0)-----��x  
							63
*/
/*---------------------- Static / Constant / Macro Definitions ---------------------*/
static float slope = 0.0; //����������ϵ�� ����б�� slope
static uint32 total_mem,used_mem,max_used_mem;

/*--------------------------- Variable Declarations --------------------------------*/

char *VehicleModeName[2] = {"4_Axis","6_Axis"}; //����������ʾ�� ģʽ�ַ� 0->AUV  1->ROV
char *WorkModeName[2] = {"Work","Debug"}; //����������ʾ�� ����ģʽ
volatile MENU_LIST_Enum MENU = StatusPage;//OLED��ʼҳ��Ϊ ״̬ҳ. volatile��һ���������η���
																				  //volatile ������ ����Ϊָ��ؼ��֣�ȷ������ָ�������������Ż���ʡ�ԣ���Ҫ��ÿ��ֱ�������ڴ��ж�ֵ��

extern int Rail_Gun_Distance;
extern int Rail_Gun_Angle ;
extern struct rt_event init_event;/* ALL_init �¼����ƿ� */


/* OLED ���� ��ʼ��. */
Oled_Type oled = {	 
									 .pagenum = StatusPage,		 //ҳ�� pagenum
									 .pagechange = StatusPage, //�ݴ�ҳ�� ���ҳ���Ƿ�ı� pagechange
									 .pagechange_flag = 0,     //ҳ��ı��־λ pagechange flag
									 .pagename = //ҳ������ pagename
										{	
												"StatusPage",
												"GyroscopePage",
												"LockPage",
												"PicturePage"} 							
};
/*----------------------- Function Implement --------------------------------*/

/* OLED �̳߳�ʼ�� */
int oled_thread_init(void)
{
    rt_thread_t oled_tid;
		/*������̬�߳�*/
    oled_tid = rt_thread_create("oled", //�߳�����
                    oled_thread_entry,	//�߳���ں�����entry��
                    RT_NULL,				    //�߳���ں���������parameter��
                    2048,							  //�߳�ջ��С����λ���ֽڡ�byte��
                    20,								  //�߳����ȼ���priority��
                    10);							  //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (oled_tid != RT_NULL){
				OLED_Init();
				log_i("OLED_Init()");
				rt_thread_startup(oled_tid);
				//rt_event_send(&init_event, OLED_EVENT);
				oled.pagechange = oled.pagenum;  //��ʼ���ݴ�ҳ��
		}
		return 0;
}
INIT_APP_EXPORT(oled_thread_init);

/*******************************************
* �� �� ����menu_define
* ��    �ܣ��˵�ѡ��
* ���������none
* �� �� ֵ��none
* ע    �⣺ҳ���л�ʱ����������һ��
********************************************/
void menu_define(void) //�˵�����
{
	if(oled.pagenum >= OLED_Page_MAX || oled.pagenum < StatusPage) oled.pagenum = StatusPage; //����ҳ�淶Χ ��Ϊ��һҳ
	if(oled.pagechange != oled.pagenum){ //��ҳ��ı�
			Buzzer_Set(&Beep,1,1);
			rt_kprintf("Current Menu_Page: %s \n",oled.pagename[oled.pagenum-1]);
			OLED_Clear(); //����
			oled.pagechange_flag = 1;
	}
	else {oled.pagechange_flag = 0;}
	oled.pagechange = oled.pagenum;
	
//	if(ControlCmd.All_Lock == LOCK){// �����벻��ǿ�ƽ��� ����ҳ��
//			oled.pagenum = LockPage;
//	}
	switch(oled.pagenum){
			case 1:{
					MENU = StatusPage;	 OLED_StatusPage();		break;
			}
			case 2:{
					MENU = GyroscopePage;OLED_GyroscopePage();break;
			}
			case 3:{
					MENU = LockPage;		 OLED_LockPage(); 	  break; //��������
			}
			case 4:{
					MENU = PicturePage;	 OLED_PicturePage(); break;
			}	
	}
}


/*******************************************
* �� �� ����OLED_StatusPage
* ��    �ܣ���ʾϵͳ״̬ҳ��
* ���������none
* �� �� ֵ��none
* ע    �⣺ϵͳ��һҳ ��״̬ҳ��
********************************************/
void OLED_StatusPage(void)
{
		static char str[50];
		
		if(is_raspi_start()){
				OLED_ShowMyChar(100,0,0,16,1); //3G����ͼ��2
		}
		else {OLED_ShowMyChar(119,0,2,16,1);} //���ͼ��0
		if(is_wifi_connect()){
				OLED_ShowMyChar(119,0,1,16,1);} //Wifiͼ��1
		else {OLED_ShowMyChar(119,0,2,16,1);} //���ͼ��0
	
		sprintf(str,"Mode:[%s-NO.%d]",VehicleModeName[VehicleMode],get_boma_value()); //��ȡ����ΪROV or AUV
		OLED_ShowString(0,0, (uint8 *)str,12); 
	
		sprintf(str,"Vol:%.2fV",Sensor.PowerSource.Voltage);//��ѹ
		OLED_ShowString(0,16,(uint8 *)str,12); 
		
		sprintf(str,"Cur:%.3f",Sensor.PowerSource.Current);//����
		OLED_ShowString(70,16,(uint8 *)str,12); 	
		
	  sprintf(str,"CPU:%.2f%% ",Sensor.CPU.Usage);//%�ַ���ת���ַ���%%  %����ַ��������������ƥ���ԭ��
		OLED_ShowString(0,32,(uint8 *)str,12); 
		
		sprintf(str,"Temp:%.1fC ",Sensor.CPU.Temperature);//��ʾ���¶�
		OLED_ShowString(70,32,(uint8 *)str,12);
		
		sprintf(str,"Dis:%d  Ang:%d",Rail_Gun_Distance,Rail_Gun_Angle);//��ʾ���¶�
		OLED_ShowString(0,48,(uint8 *)str,12);
		OLED_Refresh_Gram();//������ʾ��OLED
}


/*******************************************
* �� �� ����OLED_GyroscopePage
* ��    �ܣ���ʾ����ģ����������ٶȡ����ٶȡ�ŷ���ǡ��ų���
* ���������none
* �� �� ֵ��none
* ע    �⣺OLED�ڶ�ҳ ���������ҳ��
********************************************/
void OLED_GyroscopePage(void)
{
		char str[100];
		sprintf(str,"Acc:%.2f %.2f %.2f  ",Sensor.JY901.Acc.x,Sensor.JY901.Acc.y,Sensor.JY901.Acc.z);
		OLED_ShowString(0,0,(uint8 *)str,12); 	
		
		sprintf(str,"Gyro:%.1f %.1f %.1f ",Sensor.JY901.Gyro.x,Sensor.JY901.Gyro.y,Sensor.JY901.Gyro.z);
		OLED_ShowString(0,16,(uint8 *)str,12); 	
		
		sprintf(str,"Ang:%.1f %.1f %.1f  ",Sensor.JY901.Euler.Roll,Sensor.JY901.Euler.Pitch,Sensor.JY901.Euler.Yaw);
		OLED_ShowString(0,32,(uint8 *)str,12); 	
		 
		sprintf(str,"Mag:%d %d %d  ",Sensor.JY901.Mag.x,Sensor.JY901.Mag.y,Sensor.JY901.Mag.z);
		OLED_ShowString(0,48,(uint8 *)str,12); 
		
	  OLED_Refresh_Gram();//������ʾ��OLED
}
/*******************************************
* �� �� OLED_LockPage
* ��    �ܣ���ʾ
* ���������none
* �� �� ֵ��none
* ע    �⣺OLED����ҳ 
********************************************/
void OLED_LockPage(void)
{		

		static char str[30] = {0};   //�ݴ�OLED�ַ���
		static uint8 vol_box = 0; 	 //��ѹ�� ������
		static uint8 vol_percent = 0;//��ѹ�ٷֱ�
		
		if(Sensor.PowerSource.Capacity != 0){ //�ж���0
				vol_box = (Sensor.PowerSource.Voltage-(Sensor.PowerSource.Capacity*STANDARD_VOLTAGE/FULL_VOLTAGE))*12/
								  (Sensor.PowerSource.Capacity/(2*FULL_VOLTAGE)); //oled������ʾ = ��ʵ��ѹֵ*12��/����������ĵ�ѹ
			
				vol_percent = (Sensor.PowerSource.Voltage-(Sensor.PowerSource.Capacity*STANDARD_VOLTAGE/FULL_VOLTAGE))/
					            (Sensor.PowerSource.Capacity/(2*FULL_VOLTAGE))*100;  //�����ٷֱ� = ����ʵ��ѹֵ-��ȫ��ѹֵ��*100%
			
				vol_box = vol_box > 12 ? 12 : vol_box; 						  //��ѹ������ �޷�
				vol_box = vol_box <= 0  ? 0  : vol_box;
				vol_percent = vol_percent > 100 ? 100 : vol_percent;//��ѹ�ٷֱ��޷�
				vol_percent = vol_percent <= 0   ? 0   : vol_percent;
			
				Sensor.PowerSource.Percent = vol_percent;
		}		
		else{ //���δ�趨,�򲻼��� <��ѹ�������> �� <�����ٷֱ�> ,����ʾ�趨����������� 
				log_w("not yet set_battery_capacity parameter!");
				Buzzer_Set(&Beep,1,1);	
				rt_thread_mdelay(5000); //5s
		}
		
		if(is_raspi_start()){//��ݮ���Ƿ���������������
				Buzzer_Set(&Beep,3,1);
				OLED_ShowPicture(0,28,raspberry_logo,28,33);//��ʾ��ݮ��LOGO
		}

		sprintf(str,"Vol:%.2fV  \r\n",Sensor.PowerSource.Voltage);//��ѹ
		OLED_ShowString(0,0, (uint8 *)str,12);
		
		sprintf(str,"Cur:%.2f A  \r\n",Sensor.PowerSource.Current);//����
		OLED_ShowString(0,12,(uint8 *)str,12); 	
		
		sprintf(str,"%d%% ",vol_percent);//��ǰ�����ٷֱ� %
		OLED_ShowString(80,0, (uint8 *)str,12);

		OLED_ShowPicture(107,0,bmp_battery[vol_box],10,16);//��ʾ����
		OLED_ShowPicture(49,43-15,bmp_lock[ControlCmd.All_Lock-1],30,30);//����
		
		OLED_Refresh_Gram();//������ʾ��OLED
}

/*******************************************
* �� �� ����OLED_PicturePage
* ��    �ܣ���ʾOLED��������ͼ��ҳ��
* ���������none
* �� �� ֵ��none
* ע    �⣺OLED����ҳ ��ͼ��ҳ��
********************************************/
void OLED_PicturePage(void)
{
		static uint8 y=0;
		static char str[100];
		static int Angle_x = 0,Angle_y = 0;
		
		draw_fill_circle(31+Angle_x,31+Angle_y,6,0); //���ʵ��Բ������ˢ������
		draw_line(31,31,slope,0); //�����һ�λ����� ����ˢ��
	
		OLED_Refresh_Gram();//������ʾ��OLED
	
		Angle_x = Sensor.JY901.Euler.Roll/5;
		Angle_y = Sensor.JY901.Euler.Pitch/5;
		if(fabs(Sensor.JY901.Euler.Yaw) != 90) //90 deg��Ч
				slope = tan((float)Deg2Rad(Sensor.JY901.Euler.Yaw));  //ת�������� ���㶫��������ϵ�� ����б��slope
	
		for(y = 28;y <= 36;y++){ //��Բ�����ײ���ȱʧ��
				OLED_DrawPoint(y,0,1);
				OLED_DrawPoint(y,63,1);
		}

		draw_line(31,31,slope,1);
		
		sprintf(str,"Rol:%3.1f  ",Sensor.JY901.Euler.Roll); //�����Roll 
		OLED_ShowString(65,0, (uint8 *)str,12);
		
		sprintf(str,"Pit:%3.1f  ",Sensor.JY901.Euler.Pitch);//������Pitch
		OLED_ShowString(65,16, (uint8 *)str,12);
		
		sprintf(str,"Yaw:%3.1f  ",Sensor.JY901.Euler.Yaw); //ƫ����Yaw
		OLED_ShowString(65,32, (uint8 *)str,12);
		
		sprintf(str,"k:%.1f   ",slope);
		OLED_ShowString(65,48,(uint8 *)str,12); 
		

		OLED_ShowString(29,2 ,(uint8 *)"N",12);
		OLED_ShowString(29,51,(uint8 *)"S",12);
		OLED_ShowString(3	,28,(uint8 *)"W",12);
		OLED_ShowString(55,28,(uint8 *)"E",12);
		
		draw_circle(31,31,32);//���̶�Բ
		draw_fill_circle(31+Angle_x,31+Angle_y,6,1); //��ʵ��Բ
	
		OLED_Refresh_Gram();//������ʾ��OLED						
}



/*******************************************
* �� �� ����Boot_Animation
* ��    �ܣ���������
* ���������none
* �� �� ֵ��none
* ע    �⣺none
* ˵    ��������ETA����LOGO
********************************************/
void Boot_Animation(void)
{
		static uint8 x=0,y=0;

		for(x = 63;x>=18;x--){
				OLED_DrawPoint(108-0.7*x,x,1);//��б�� x,y����
				OLED_DrawPoint(17 +0.7*x,x,1);
				y = 64-x;
				OLED_DrawPoint(64-0.7*y,y,1);
				OLED_DrawPoint(64+0.7*y,y,1);
				rt_thread_mdelay(20);
			  OLED_Refresh_Gram();//������ʾ��OLED
		}
		
		for(x = 30;x <= 94;x++){
				OLED_DrawPoint(125-x,47,1);
				OLED_DrawPoint(x,18,1);
				rt_thread_mdelay(20);
			  OLED_Refresh_Gram();//������ʾ��OLED
		}

		OLED_ShowString(60,20,(uint8 *)"E",16);
	  OLED_Refresh_Gram();//������ʾ��OLED
		rt_thread_mdelay(1000);
		
}



/*******************************************
* �� �� ����draw_fill_circle
* ��    �ܣ��������Բ
* ���������(x0,y0)Բ�����ꡢrΪ�뾶
						dotΪ1ʱ ��ʵ��Բ��dotΪ0ʱ ���ʵ��Բ
* �� �� ֵ��none
* ע    �⣺none
********************************************/
void draw_fill_circle(uint8 x0,uint8 y0,uint8 r,uint8 dot)//д��ʵ��Բ��(x0,y0),�뾶r
{	
		uint8 x = 0,y = 0,R = 0;
		for(x = x0-r;x <= x0+r;x++){
				for(y = y0-r; y <= y0+r ;y++ ){
						R = sqrt(pow(r,2)-pow(x-x0,2))+y0; //Բ����  x,y����		
						if( (y >= y0 && y <= R) || (y < y0 && y >= 2*y0-R )|| 0 == dot ) {  //�������� Բ������	
								OLED_DrawPoint(y,x,dot);
						}	
				}
		}
}

/*******************************************
* �� �� ����draw_circle
* ��    �ܣ����ƿ���Բ
* ���������(x0,y0)Բ�����ꡢrΪ�뾶
* �� �� ֵ��none
* ע    �⣺none
********************************************/
void draw_circle(uint8 x0,uint8 y0,uint8 r) //Բ��(x0,y0),�뾶r
{
		uint8 x,y;

		for(x = 0;x <= 63;x++){
				y = sqrt(pow(r,2)-pow(x-x0,2))+y0; //Բ����  x,y����
				OLED_DrawPoint(y,x,1);      //�ϰ�Բ
				OLED_DrawPoint(63-y,x,1);  //�°�Բ
		}
		
}


/*******************************************
* �� �� ����draw_line
* ��    �ܣ���һ���̶����߶�
* ���������(x0,y0)Ϊ�̶������ꡢkΪб��
* �� �� ֵ��none
* ע    �⣺ʹ�ò�ͬ����ϵ Ϊ�˽�������� xӳ��yʱֻ�ܶ��һ�Ĺ�ϵ
						���߶γ���ΪԲ���̵İ뾶�����߶γ���������Բ��
********************************************/
void draw_line(uint8 x0,uint8 y0,float k,uint8 dot) //���̶���(x0,y0),б��k   dot:0,���;1,���	  
{
		uint8 x,y;
/* ���º���ʹ�ø�����ϵ: 
	
	                127 ��y
			-----------------
			|								|
			|								|
			|								|
			|								|
		��---------------(0,0)x  
			63
*/
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(20,2)-pow(x-31,2))+31+1; //Բ����  x,y����
			
				if( (Sensor.JY901.Euler.Yaw >-135 && Sensor.JY901.Euler.Yaw <-90 ) ||(Sensor.JY901.Euler.Yaw >90 && Sensor.JY901.Euler.Yaw < 145 ) || 0 == dot ){ //�ϰ�Բ
						if(  ((x-x0)/k+y0) >= 31 && ((x-x0)/k+y0) < y ) {  //�������� Բ������
								OLED_DrawPoint(x,((x-x0)/k+y0),dot);}
				}
				
				if( (Sensor.JY901.Euler.Yaw < -45 && Sensor.JY901.Euler.Yaw > -90) || (Sensor.JY901.Euler.Yaw < 90 && Sensor.JY901.Euler.Yaw> 45) || 0 == dot  ){ //�ϰ�Բ
						if(  ((x-x0)/k+y0) <= 31 && ((x-x0)/k+y0)> 63-y ) {  //�������� Բ������
								OLED_DrawPoint(x,((x-x0)/k+y0),dot);}
				}
		}
/* ���º���ʹ�ø�����ϵ: 
	127 ��y
			---------
			|	      |
			|				|
			|				|
			|				|
			|				|
			|				|
			|				|
		(0,0)-----��x  
							63
*/
		for(x = 0;x <= 63;x++){
				y = sqrt(pow(20,2)-pow(x-31,2))+31+1; //Բ����  x,y����
		
				if( (Sensor.JY901.Euler.Yaw>=-45 && Sensor.JY901.Euler.Yaw <= 0) || (Sensor.JY901.Euler.Yaw >=-180 && Sensor.JY901.Euler.Yaw <= -135)  || 0 == dot ){  // Sensor.JY901.Angle[2] < 0
						if( (k*(x-x0)+y0) >= 31 && (k*(x-x0)+y0) < y ) {  //�������� Բ������   �ϰ�Բ
								OLED_DrawPoint((k*(x-x0)+y0),x,dot);}
				}
				
				if( (Sensor.JY901.Euler.Yaw > 0 && Sensor.JY901.Euler.Yaw <= 45) || (Sensor.JY901.Euler.Yaw >=135 && Sensor.JY901.Euler.Yaw <= 180)  || 0 == dot ){  // Sensor.JY901.Angle[2] < 0
						if(((k*(x-x0)+y0)< 31 && (k*(x-x0)+y0) > 63-y)) {  //�������� Բ������  �°�Բ
								OLED_DrawPoint((k*(x-x0)+y0),x,dot);} 
				}
		}

}



/* OLED ��һҳ */
void next_oled_page(void)
{
		oled.pagenum ++;
}
MSH_CMD_EXPORT(next_oled_page,next_oled_page[page++]);

/* OLED ��һҳ */
void last_oled_page(void)
{
		oled.pagenum --;
}
MSH_CMD_EXPORT(last_oled_page,last_oled_page[page--]);


void get_slope(void)
{
		char str[100];
		sprintf(str,"k= %f\n",slope);
		rt_kprintf(str);
}
MSH_CMD_EXPORT(get_slope,get_slope[k]);


void get_cpuusage(void)
{
		rt_memory_info(&total_mem, &used_mem, &max_used_mem);
    rt_kprintf("Total_Mem:%ld  Used_Mem:%ld  Max_Used_Mem:%ld\n",total_mem,used_mem,max_used_mem);

}
MSH_CMD_EXPORT(get_cpuusage,get_cpuusage);
//STM32F405RGT6   Total_Mem:131048  Used_Mem:22044  Max_Used_Mem:22368

