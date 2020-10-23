/*
 * timer.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  ��ʱ��
 */
#define LOG_TAG    "timer"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "flash.h"
#include <rtthread.h>
#include <elog.h>
#include "drv_MS5837.h"
#include "drv_ano.h"
#include "Control.h"
#include "gyroscope.h"
#include "debug.h"
#include "ret_data.h"
#include "PropellerControl.h"

/*---------------------- Constant / Macro Definitions -----------------------*/



/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/


//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,DISABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}

void TIM3_ENABLE(void)
{
	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3

}

//static int cnt = 0;
//int timer3_cnt = 0;

//��ʱ��3�жϷ�����
//void TIM3_IRQHandler(void)
//{

//		if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//		{
//			
//				ANO_SEND_StateMachine();
////				cnt ++;
////				if(cnt >= timer3_cnt){
////					  cnt = 0;
////						Two_Axis_Yuntai_Control();
////				}

//			
//		}
//		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}


//static int timer3_speed(int argc, char **argv)
//{
//    int result = 0;
//    if (argc != 2){
//        log_e("Error! Proper Usage: speed <0~100>");
//				result = -RT_ERROR;
//        goto _exit;
//    }
//		if(atoi(argv[1]) <= 1000){
//				timer3_cnt = atoi(argv[1]);
//		}
//		else {
//				log_e("Error! The value is out of range!");
//		}
//_exit:
//    return result;
//}
//MSH_CMD_EXPORT(timer3_speed,ag: speed  <0~100>);





/**
  * @brief  timer1_out(��ʱ���жϺ���)
  * @param  void* parameter
  * @retval None
  * @notice Attention! -> ��ʱ���в��ܴ�����ʱ�����ͷ��̵߳Ĳ���
  */
static void timer1_out(void* parameter)// ��ʱ��1��ʱ����  ����JY901ģ������ת��
{


		/* �����������������󣬽������л��������̣߳�����Ӧ�ж� */
		rt_enter_critical();
				

	
//		get_speed(&Sensor.JY901.Acc.x,&Sensor.JY901.Speed.x);//�õ�x�ٶ�,���������⣬�ۻ�ЧӦ����
//		get_zspeed(); 
		//Angle_Control(); //�Ƕȿ���


		/* ���������� */
		rt_exit_critical();

	
}



int timer1_init(void)
{
		/* ��ʱ���Ŀ��ƿ� */
		static rt_timer_t timer1;
    /* ������ʱ��1 */
    timer1 = rt_timer_create("timer1",  /* ��ʱ�������� timer1 */
                        timer1_out, 		  /* ��ʱʱ�ص��Ĵ����� */
                        RT_NULL, 			  /* ��ʱ��������ڲ��� */
                        1,      			  /* ��ʱ���ȣ���OS TickΪ��λ����5��OS Tick   --> 50MS*/  
                        RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER); /* �����Զ�ʱ�� */
    /* ������ʱ�� */
    if (timer1 != RT_NULL){ 
				//TIM3_Int_Init(10000-1,84-1); //84M/84 = 1M,  1M/10000 = 100Hz  = 10MS
				//rt_timer_start(timer1);
				
		}

    return 0;
}
//INIT_APP_EXPORT(timer1_init);

