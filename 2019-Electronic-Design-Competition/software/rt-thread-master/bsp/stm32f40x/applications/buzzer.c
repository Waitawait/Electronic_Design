/*
 * buzzer.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  ������ָʾ
 */
#define LOG_TAG    "buzzer"


#include <rtthread.h>
#include <elog.h>
#include <drivers/pin.h>
/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/

/* ALL_init �¼����ƿ� */
extern struct rt_event init_event;



/*----------------------- Function Implement --------------------------------*/

//void buzzer_thread_entry(void *parameter)
//{
//    while (1)
//    {
//	
//				rt_thread_mdelay(10);
//    }
//}


//int buzzer_thread_init(void)
//{
//    rt_thread_t buzzer_tid;
//		/*������̬�߳�*/
//    buzzer_tid = rt_thread_create("buzzer",	 //�߳�����
//                    buzzer_thread_entry,		 //�߳���ں�����entry��
//                    RT_NULL,							   //�߳���ں���������parameter��
//                    1024,										 //�߳�ջ��С����λ���ֽڡ�byte��
//                    25,										 	 //�߳����ȼ���priority��
//                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

//    if (buzzer_tid != RT_NULL){

//				rt_thread_startup(buzzer_tid);
//				//rt_event_send(&init_event, BUZZ_EVENT);
//		}
//		return 0;
//}
//INIT_APP_EXPORT(buzzer_thread_init);



