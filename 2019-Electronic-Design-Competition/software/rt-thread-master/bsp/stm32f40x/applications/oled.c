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
									 .pagenum = SwitchPage,		 //ҳ�� pagenum
									 .pagechange = SwitchPage, //�ݴ�ҳ�� ���ҳ���Ƿ�ı� pagechange
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
                    12,								  //�߳����ȼ���priority��
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
	  if(oled.pagenum >= OLED_Page_MAX || oled.pagenum <= SwitchPage) oled.pagenum = SwitchPage; //����ҳ�淶Χ ��Ϊ��һҳ
		if(oled.pagechange != oled.pagenum){ //��ҳ��ı�
				OLED_Clear(); //����
				//Buzzer_Set(&Beep,1,1);//��������һ��
				//rt_kprintf("Current Menu_Page: %s \n",oled.pagename[oled.pagenum-1]);//��ӡ��ǰҳ��
				oled.pagechange_flag = 1;
		}
		else {oled.pagechange_flag = 0;}
		oled.pagechange = oled.pagenum;
		
		oled.pagenum = HMI_Status_Flag;//��ȡ״̬

		switch(oled.pagenum){
				case 0:{
						MENU = SwitchPage;OLED_SwitchPage();break;//ģʽѡ��ҳ�棨��DeviceThread.c�������Ӧ��
				}
				case 1:{
						MENU = DebugPage;OLED_DebugPage();break;//����ҳ�棨��DeviceThread.c�������Ӧ��
				}
				case 2:{
						MENU = DebugPage;OLED_DebugPage();break;//����ҳ�棨��DeviceThread.c�������Ӧ��
				}
				case 3:{
						MENU = WorkPage;OLED_WorkPage();break;//��������ҳ�棨��DeviceThread.c�������Ӧ��
				}	
				case 4:{
						MENU = FuncPage;OLED_FuncSwitchPage();break;//��չ����ѡ����DeviceThread.c�������Ӧ��
				}
				case 5:{
						MENU = PrintPage;OLED_PrintPage();break;//��ӡ�����ܣ���DeviceThread.c�������Ӧ��
				}
				case 6:{
						MENU = MaterPage;OLED_MaterPage();break;//���ϼ�⹦�ܣ���DeviceThread.c�������Ӧ��
				}
				case 7:{
						MENU = RMBPage; OLED_RMBDectionPage();break;//�����Ҽ�⹦�ܣ���DeviceThread.c�������Ӧ��
				
				}
				default:OLED_SwitchPage();break;//ģʽѡ��ҳ�棨��DeviceThread.c�������Ӧ��
		}
}

void OLED_SwitchPage(void)
{
		OLED_ChineseString(32,0,31,34,16);//ѡ��˵�
		
		OLED_ChineseString(0,32,35,36,16);//����ģʽ
		OLED_ChineseString(0,48,37,38,16);
		
		OLED_ChineseString(48,32,41,42,16);//����ģʽ
		OLED_ChineseString(48,48,43,44,16);
		
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
		sprintf(str,": %.5f     ",FDC2214_Paper_Data[HMI_Page_Number]);
		OLED_ShowString(32,0, (uint8 *)str,16);

		OLED_ChineseString(0,16,0,1,16);
		OLED_ChineseString(32,16,4,4,16);
		OLED_ChineseString(48,16,15,15,16);   //��ӡ��ǰҳ��
		sprintf(str,": %d   ",HMI_Page_Number);		  //��ӡ��ǰҳ��
		OLED_ShowString(72,16, (uint8 *)str,16);

		OLED_ChineseString(0,48,13,14,16);  		// ״̬
		OLED_ShowString(32,48, (uint8 *)":",16); 		

		sprintf(str,"D: %.3f",Data_Diff_Value);
		OLED_ShowString(0,32,(uint8 *)str,16); 			
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
		OLED_ChineseString(32,0,41,44,16);

		OLED_ChineseString(0,32,48,49,16);		//ֽ�м��
		OLED_ChineseString(0,48,50,51,16);
		
		OLED_ChineseString(48,32,52,53,16);		//����ʶ��
		OLED_ChineseString(48,48,54,55,16);
		
		OLED_ChineseString(96,32,67,68,16);		//��ֵ���
		OLED_ChineseString(96,48,69,70,16);
	
		OLED_Refresh_Gram();//������ʾ��OLED			
}


/* ��ӡ�� ҳ��*/
void OLED_PrintPage(void)
{
		static uint8 PrinterFlag = 1;
		static char str[20] = {0};
		OLED_ChineseString(32,0,45,47,16);          	//��ӡ��
		
		OLED_ChineseString(0,32,0,1,16);				//��ӡ��ʣ��ֽ��
		OLED_ChineseString(32,32,56,57,16);
		OLED_ShowString(64,32,(uint8 *)":",16);
		OLED_ChineseString(108,32,24,24,16);
		sprintf(str,"%d   ",Paper.PaperNumber);  //����ֽ��ҳ��
		OLED_ShowString(72,32, (uint8 *)str,16); 
		
		if(PrinterFlag == 1){
				OLED_ChineseString(32,48,58,61,16);				//ȱֽ����
		}
		else {
				OLED_ChineseString(32,48,62,62,16);
		}
		
		if(Paper.PaperNumber <=5){
				PrinterFlag = 1;
		}
		else{
				PrinterFlag =0;
		}
		
		OLED_Refresh_Gram();//������ʾ��OLED			
		
}


/* ���ϼ�� ҳ��*/
void OLED_MaterPage(void)
{
		static uint8 MaterialFlag = 0;
		static char str[20] = {0};
		OLED_ChineseString(32,0,52,55,16);  //����ʶ��
			
		OLED_ChineseString(0,32,0,1,16);				//��ǰ����
		OLED_ChineseString(32,32,52,53,16);
		OLED_ShowString(64,32,(uint8 *)":",16);
	
		OLED_ChineseString(0,16,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.3f",Paper.Capacitance);
		OLED_ShowString(32,16, (uint8 *)str,16);
		if(MaterialFlag == 0)	{						//��
		
				OLED_ChineseString(72,32,63,63,16);
		}
		if(MaterialFlag == 1){							//KT��
				OLED_ShowString(72,32,(uint8 *)"KT",16);
				OLED_ChineseString(72,32,66,66,16);
		}
		if(MaterialFlag == 2)	{						//��ά��
		
				OLED_ChineseString(72,32,64,66,16);
		}

		OLED_Refresh_Gram();//������ʾ��OLED			

}



/*******************************************
* �� �� RMBDection
* ��    �ܣ�RMB������
* ���������none
* �� �� ֵ��none
* ע    �⣺OLEDѡ����� 
********************************************/

void OLED_RMBDectionPage()
{
		static char str[10];
		static char rmb_value =0;
	
		if(0 == RMB_Value){
				rmb_value = 0;
		}
		else if(1 == RMB_Value){
				rmb_value = 100;
		}
		else if(2 == RMB_Value){
				rmb_value = 50;
		}
		else if(3 == RMB_Value){
				rmb_value = 10;
		}
		else if(4 == RMB_Value){
				rmb_value = 5;
		}		
		OLED_ShowString(20,0, (uint8 *)"RMB",16);//RMB ��ֵ���
		OLED_ChineseString(48,0,67,70,16);
		
		
		OLED_ChineseString(0,16,2,3,16);  		//��ӡ��ǰ����
		sprintf(str,": %.3f",Paper.Capacitance);
		OLED_ShowString(32,16, (uint8 *)str,16);
		
		OLED_ChineseString(0,32,69,70,16);
		OLED_ChineseString(32,32,67,68,16);
		sprintf(str,":%d  ",rmb_value);
		OLED_ShowString(64,32,(uint8 *)str,16);
		
		OLED_Refresh_Gram();//������ʾ��OLED			

}


/*******************************************
* �� �� CoefficientDebug
* ��    �ܣ�����ҳ��-����ϵ��
* ���������none
* �� �� ֵ��none
* ע    �⣺ 
********************************************/
void OLED_CoefficientDebug(void)
{
		static char str[10];
			
		OLED_ShowString(12,0, (uint8 *)"35",16);
		OLED_ShowString(44,0, (uint8 *)"45",16);
		OLED_ShowString(76,0, (uint8 *)"55",16);
		OLED_ShowString(108,0, (uint8 *)"65",16);
		
		sprintf(str,"%d%%",Div_Parameter.Div_30_40);							//���ӿڡ� Valueдϵ��
		OLED_ShowString(4,0, (uint8 *)str,16);
		sprintf(str,"%d%%",Div_Parameter.Div_40_50);
		OLED_ShowString(36,0, (uint8 *)str,16);
		sprintf(str,"%d%%",Div_Parameter.Div_50_60);
		OLED_ShowString(68,0, (uint8 *)str,16);
		sprintf(str,"%d%%",Div_Parameter.Div_60_70);
		OLED_ShowString(100,0, (uint8 *)str,16);
	
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



