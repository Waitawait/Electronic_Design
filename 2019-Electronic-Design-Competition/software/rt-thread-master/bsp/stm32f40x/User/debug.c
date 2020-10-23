/*
 * debug.c
 *
 *  Created on: 2019��2��5��
 *      Author: zengwangfa
 *      Notes:  ������������λ��
 */
#define LOG_TAG    "debug"

#include <rtthread.h>
#include <string.h>
#include <elog.h>
#include "PID.h"
#include "debug.h"
#include "drv_ano.h"
#include "filter.h"
#include "drv_cpu_temp.h"
#include "gyroscope.h"
#include "drv_MS5837.h"
#include "PID.h"
#include "oled.h"
#include "propeller.h"
#include <stdlib.h>
#include "rc_data.h"
#include "DataProcess.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations. -----------------------------*/

extern rt_device_t debug_uart_device;	
extern rt_device_t control_uart_device;
extern uint8 uart_startup_flag;
extern float  Yaw;


enum 
{
		DEBUG_NULL = 0,
		PC_VCAN,
		PC_ANO,
		//������
		
}PC_TOOL;//��λ������

char *debug_tool_name[3]={"NULL","VCAN","ANO"};

uint8 debug_tool = PC_ANO; //ɽ�� / ������λ�� ���Ա�־λ

extern float target_pit ; //�����Ƕ�
extern float target_yaw ;

extern float persent_pit ;//��ǰ�Ƕ�
extern float persent_yaw ;

/*-----------------------Debug Thread Begin-----------------------------*/
void debug_send_thread_entry(void* parameter)
{
		rt_thread_mdelay(3000);//�ȴ� �����豸��ʼ�����
	
		while(uart_startup_flag)//��debug_uart��ʼ����Ϻ� �Ž�����λ��ͨ��
		{		
				rt_thread_mdelay(10);
//				switch(debug_tool)//ѡ����λ��
//				{

//						case PC_VCAN: Vcan_Send_Data();break;
//						case PC_ANO :	ANO_SEND_StateMachine();break;
//						default :break;
//				}
				 Vcan_Send_Data();
			  

		}
}



int Debug_thread_init(void)
{
	  rt_thread_t debug_send_tid;
		/*������̬�߳�*/
    debug_send_tid = rt_thread_create("debug",			 //�߳�����
                    debug_send_thread_entry, //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    4096,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    15,										 	 //�߳����ȼ���priority��
                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (debug_send_tid != RT_NULL){
				log_i("Debug_Init()");
				rt_thread_startup(debug_send_tid);
		}
		return 0;
}
INIT_APP_EXPORT(Debug_thread_init);

/*-----------------------Debug Thread End-----------------------------*/




#define CMD_WARE    3
/* VCANɽ����λ������ BEGIN */
void Vcan_Send_Cmd(void *wareaddr, unsigned int waresize)
{

    static uint8 cmdf[2] = {CMD_WARE, ~CMD_WARE};    //���ڵ��� ʹ�õ�ǰ����
    static uint8 cmdr[2] = {~CMD_WARE, CMD_WARE};    //���ڵ��� ʹ�õĺ�����

    rt_device_write(control_uart_device, 0,cmdf, 2);    //�ȷ���ǰ����
    rt_device_write(control_uart_device, 0,(uint8 *)wareaddr, waresize);    //��������
    rt_device_write(control_uart_device, 0,cmdr, 2);    //���ͺ�����
}



void Vcan_Send_Data(void)
{   

		static float list[8]= {0};

		list[0] = Paper.Capacitance;  //������ Pitch
		list[1] = 0; 	  //ƫ���� Yaw

		list[2] = 0;    //CPU�¶� temp
		list[3] = 0;//
		list[4] = 0;//MS_TEMP;//get_vol();
		list[5] = 0;//MS5837_Pressure;	
		list[6] = 0;	//camera_center;
		list[7] = 100;
		
		Vcan_Send_Cmd(list,sizeof(list));
}
/* VCANɽ����λ������  END */




/* debug �������߹ر� */
static int debug(int argc, char **argv)
{
    int result = 0;

    if (argc != 2){
				log_e("Proper Usage: debug <on/off>");//�÷�:������λ������
				result = -RT_ERROR; 
				goto _exit;  
    }
		if( !strcmp(argv[1],"on") ){ //����Ϊ ɽ����λ�� strcmp ����������� ����0
				uart_startup_flag = 1;
				log_v("debug open\r\n");
		}
		else if( !strcmp(argv[1],"off") ){ //����Ϊ ɽ����λ�� strcmp ����������� ����0
				uart_startup_flag = 0;
				log_v("debug off\r\n");
		}
		else {
				log_e("Proper Usage: debug <on/off>");//�÷�:������λ������
		}

_exit:
    return result;
}
MSH_CMD_EXPORT(debug,ag: debug on);



/* debug ������λ������ */
static int set_debug_tool(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("set_dubug_tool <vcan/ano/null>");//�÷�:������λ������
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"vcan") ){ //����Ϊ ɽ����λ�� strcmp ����������� ����0
				debug_tool = PC_VCAN;
				log_v("Debug Tool:VCAN\r\n");
		}

		else if( !strcmp(argv[1],"ano") ){ //����Ϊ ������λ��
				debug_tool = PC_ANO;
				log_v("Debug Tool:ANO\r\n");
		}
		else if( !strcmp(argv[1],"null") ){ //����Ϊ ɽ����λ��
				debug_tool = DEBUG_NULL;
				log_v("Debug Tool:NULL\r\n");
		}
		else {
				log_e("set_dubug_tool <vcan/ano/null>");//�÷�:������λ������
				goto _exit;
		}
		Flash_Update();
_exit:
    return result;
}
MSH_CMD_EXPORT(set_debug_tool,set_dubug_tool <vcan|ano|null>);




