#ifndef __PWM_H_
#define __PWM_H_

#include "DataType.h"

void PWM_Init(void);


void Light_PWM_Init(void);





void TIM1_PWM_Init(uint32 arr,uint32 psc);//�ƽ���
void TIM3_PWM_Init(uint32 arr,uint32 psc);//��ȡ��
void TIM4_PWM_Init(uint32 arr,uint32 psc);//�ƽ�������е�ۡ���̨

void TIM10_PWM_Init(uint32 arr,uint32 psc);
void TIM11_PWM_Init(uint32 arr,uint32 psc);

void TIM1_PWM_CH1_E9(short duty); //����	 E9	

void TIM1_PWM_CH2_E11(short duty);//����	 E11

void TIM1_PWM_CH3_E13(short duty);//����   E13

void TIM1_PWM_CH4_E14(short duty);//����   E14


/*------------- TIM3 ��ȡ��---------------*/

void TIM3_PWM_CH3_B0(short duty); //��ȡ��

void TIM3_PWM_CH4_B1(short duty);


/*------------- TIM4 �ƽ�������̨����е�� ---------------*/
void TIM4_PWM_CH1_D12(short duty);//����   D12

void TIM4_PWM_CH2_D13(short duty);//����   D13

void TIM4_PWM_CH3_D14(short duty);//��е�� D14

void TIM4_PWM_CH4_D15(short duty);//��̨   D15



/*------------- TIM10 ̽�յ�---------------*/
void TIM10_PWM_CH1_F6(short duty); //̽�յ� F6

/*------------- TIM11 ̽�յ�---------------*/
void TIM11_PWM_CH1_F7(short duty); //̽�յ� F7

#endif

