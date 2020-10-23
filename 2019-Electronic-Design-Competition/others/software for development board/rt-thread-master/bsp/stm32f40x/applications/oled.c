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
#include "DataProcess.h"
#include "HMI.h"

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


/*--------------------------- Variable Declarations --------------------------------*/

volatile MENU_LIST_Enum MENU = DebugPage;//OLED��ʼҳ��Ϊ ״̬ҳ. volatile��һ���������η���
																				  //volatile ������ ����Ϊָ��ؼ��֣�ȷ������ָ�������������Ż���ʡ�ԣ���Ҫ��ÿ��ֱ�������ڴ��ж�ֵ��

/* OLED ���� ��ʼ��. */
Oled_Type oled = {	 
									 .pagenum = DebugPage,		 //ҳ�� pagenum
									 .pagechange = DebugPage, //�ݴ�ҳ�� ���ҳ���Ƿ�ı� pagechange
									 .pagechange_flag = 0,     //ҳ��ı��־λ pagechange flag
									 .pagename = //ҳ������ pagename
										{	
												"SwitchPage",
												"DebugPage",
												"WorkPage",
												"LockPage"} 							
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
                    10,								  //�߳����ȼ���priority��
                    10);							  //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (oled_tid != RT_NULL){
				OLED_Init();
				log_i("OLED_Init()");
				rt_thread_startup(oled_tid);

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
	  if(oled.pagenum > WorkPage || oled.pagenum <= DebugPage) oled.pagenum = DebugPage; //����ҳ�淶Χ ��Ϊ��һҳ
		if(oled.pagechange != oled.pagenum){ //��ҳ��ı�
				OLED_Clear(); //����
				//Buzzer_Set(&Beep,1,1);
				//rt_kprintf("Current Menu_Page: %s \n",oled.pagename[oled.pagenum-1]);
				oled.pagechange_flag = 1;
		}
		else {oled.pagechange_flag = 0;}
		oled.pagechange = oled.pagenum;
		
		//oled.pagenum = HMI_Status_Flag;//��ȡ״̬
		HMI_Status_Flag = oled.pagenum;
		switch(oled.pagenum){
//				case 0:{
//						MENU = SwitchPage;OLED_SwitchPage();		break;
//				}
				case 1:{
						MENU = DebugPage;OLED_DebugPage();		break;
				}
				case 2:{
						MENU = WorkPage;OLED_WorkPage();break;
				}
//				case 3:{
//						MENU = FuncPage;OLED_FuncSwitchPage();break;
//				}
//				case 4:{
//						MENU = PrintPage;OLED_PrintPage();break;
//				}
//				case 5:{
//						MENU = MaterPage;OLED_MaterPage();break;
//				}
				default:OLED_SwitchPage();	break;
		}
}

void OLED_SwitchPage(void)
{
		OLED_ChineseString(32,0,31,34,16);//ѡ��˵�
		
		OLED_ChineseString(0,32,35,36,16);//����ģʽ
		OLED_ChineseString(0,48,37,38,16);
		
		OLED_ChineseString(96,32,39,40,16);//����ģʽ
		OLED_ChineseString(96,48,37,38,16);	
	  OLED_Refresh_Gram();//������ʾ��OLED
}



float OperationTime = 0;
/*******************************************
* �� �� ����OLED_DebugPage
* ��    �ܣ�
* ���������none
* �� �� ֵ��none
* ע    �⣺У׼ҳ��
********************************************/
void OLED_DebugPage(void)
{
		static char str[50] = {0};
	
		OLED_ChineseString(0,0,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.5f",FDC2214_Paper_Data[HMI_Page_Number]);
		OLED_ShowString(32,0, (uint8 *)str,16);

		OLED_ChineseString(0,16,0,1,16);
		OLED_ChineseString(32,16,4,4,16);
		OLED_ChineseString(48,16,15,15,16);   //��ӡ��ǰҳ��
		sprintf(str,": %d   ",HMI_Page_Number);		  //��ӡ��ǰҳ��
		OLED_ShowString(72,16, (uint8 *)str,16);

		OLED_ChineseString(0,48,13,14,16);  		// ״̬
		sprintf(str,":");
		OLED_ShowString(32,48, (uint8 *)str,16); 		

		if(1 == Paper.ShortStatus){//����·
				OLED_ChineseString(40,48,11,12,16);
				OLED_ShowString(72,48, (uint8 *)"    ",16);
		}
		else{
				OLED_ChineseString(40,48,10,10,16);
				OLED_ChineseString(56,48,11,12,16);
		}
	
		OLED_Refresh_Gram();//������ʾ��OLED						
}

/*******************************************
* �� �� ����OLED_GyroscopePage
* ��    �ܣ���ʾ����ģ����������ٶȡ����ٶȡ�ŷ���ǡ��ų���
* ���������none
* �� �� ֵ��none
* ע    �⣺OLED�ڶ�ҳ ���������ҳ��
********************************************/
void OLED_WorkPage(void)
{
		static char str[50] = {0};
		

		OLED_ChineseString(0,0,13,14,16);  		// ״̬
		OLED_ShowString(32,0, (uint8 *)":",16); 
		
		if(1 == Paper.ShortStatus){//����·
				OLED_ChineseString(40,0,11,12,16);
				OLED_ShowString(72,0, (uint8 *)"    ",16);
		}
		else{
				OLED_ChineseString(40,0,10,10,16);
				OLED_ChineseString(56,0,11,12,16);
		}
		
		
		
		OLED_ChineseString(0,16,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.3f",Paper.Capacitance);
		OLED_ShowString(32,16, (uint8 *)str,16);

		OLED_ChineseString(0,32,23,26,16);   	//��ӡ��ǰ����
		OLED_ShowString(64,32, (uint8 *)":",16); 
		
		if(1 == Paper.Finish_Flag )	{			//��ȡֽ���������
		
				sprintf(str,"%3d",Paper.PaperNumber);  //����ֽ��ҳ��
				OLED_ShowString(72,32, (uint8 *)str,16); 
				Paper.Status = 1;									//��״̬�������
				Paper.Finish_Flag = 0; //����ֽ����ʾ
		}
		else if(2 == Paper.Finish_Flag){
				sprintf(str,"%3d",Paper.PaperNumber);  //����ֽ��ҳ��
				OLED_ShowString(72,32, (uint8 *)str,16); 
		
		}
		
		OLED_ChineseString(0,48,27,30,16);	  	//��ӡʹ��ʱ��
		sprintf(str,":%.3fs",OperationTime);
		OLED_ShowString(64,48, (uint8 *)str,16);
		
	  OLED_Refresh_Gram();//������ʾ��OLED
}


/* ��չ����ѡ�� ҳ��*/
void OLED_FuncSwitchPage(void)
{

		OLED_ChineseString(32,0,31,34,16);//ѡ��˵�
		
		OLED_Refresh_Gram();//������ʾ��OLED			
}


/* ��ӡ�� ҳ��*/
void OLED_PrintPage(void)
{
		static char str[20] = {0};
		
		OLED_ChineseString(0,16,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.3f",Paper.Capacitance);
		OLED_ShowString(32,16, (uint8 *)str,16);
		
		OLED_ChineseString(0,32,23,26,16);   	//��ӡ��ǰ����
		OLED_ShowString(64,32, (uint8 *)":",16); 
				
		sprintf(str,"%3d",Paper.PaperNumber);  //����ֽ��ҳ��
		OLED_ShowString(72,32, (uint8 *)str,16); 
		
		OLED_Refresh_Gram();//������ʾ��OLED			
		
}


/* ���ϼ�� ҳ��*/
void OLED_MaterPage(void)
{
		static char str[20] = {0};
		OLED_ChineseString(0,16,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.3f",Paper.Capacitance);
		OLED_ShowString(32,16, (uint8 *)str,16);
		

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



