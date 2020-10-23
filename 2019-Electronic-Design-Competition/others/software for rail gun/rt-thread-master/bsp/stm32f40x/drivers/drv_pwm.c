/*
 * pwm.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  PWM�豸
 */
#define LOG_TAG    "pwm"
#include "drv_pwm.h"
#include "sys.h"
#include "propeller.h"
#include "PropellerControl.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		



/*----------------------- Variable Declarations -----------------------------*/




/*----------------------- Function Implement --------------------------------*/

void PWM_Init(void)
{
		//TIM1_PWM_Init(1000-1,16-1);	//168M/16=10Mhz�ļ���Ƶ��,��װ��ֵ(��PWM����)20000������PWMƵ��Ϊ 1M/100=10KHz.  ������Ϊ500Hz��
		//TIM3_PWM_Init(20000-1,84-1);  //��ȡ��	
		TIM4_PWM_Init(20000-1,84-1);	//84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ(��PWM����)20000������PWMƵ��Ϊ 1M/20000=50Hz.  
		TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM1
		TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4

//		TIM1_PWM_CH1_E9 (0); //����	 E9	
//		TIM1_PWM_CH2_E11(0); //����	 E11
//		TIM1_PWM_CH3_E13(0); //����   E13
//		TIM1_PWM_CH4_E14(0); //����   E14
	
		TIM4_PWM_CH1_D12(0); //����   D12
		TIM4_PWM_CH2_D13(0); //����   D13
		TIM4_PWM_CH3_D14(0); //
		TIM4_PWM_CH4_D15(0); //
	
		//TIM3_PWM_CH3_B0(0);  //��ȡ��
		//TIM3_PWM_CH4_B1(0);
}

void Light_PWM_Init(void)
{

		TIM10_PWM_Init(100-1,16800-1);	//168M/1680=100Khz�ļ���Ƶ��,��װ��ֵ(��PWM����)100������PWMƵ��Ϊ 100K/100=1KHz. 
		TIM11_PWM_Init(100-1,16800-1);	
		TIM_Cmd(TIM10, ENABLE); //ʹ��TIM10
		TIM_Cmd(TIM11, ENABLE); //ʹ��TIM11
		TIM10_PWM_CH1_F6(0);
		TIM11_PWM_CH1_F7(0);
}

/*------------- TIM1 �ƽ���---------------*/

void TIM1_PWM_CH1_E9(short duty)
{
		TIM_SetCompare1(TIM1,duty);     //����	 E9	
}

void TIM1_PWM_CH2_E11(short duty)
{
		TIM_SetCompare2(TIM1,duty);    //����	 E11
}

void TIM1_PWM_CH3_E13(short duty)
{
		TIM_SetCompare3(TIM1,duty); 	    //����   E13
}

void TIM1_PWM_CH4_E14(short duty)
{
		TIM_SetCompare4(TIM1,duty);   //����   E14
}

/*------------- TIM3 ��ȡ��---------------*/

void TIM3_PWM_CH3_B0(short duty)
{
		TIM_SetCompare3(TIM3,duty);
}

void TIM3_PWM_CH4_B1(short duty)
{
		TIM_SetCompare4(TIM3,duty);	
}


/*------------- TIM4 �ƽ�������̨����е�� ---------------*/
void TIM4_PWM_CH1_D12(short duty)
{

		TIM_SetCompare1(TIM4,duty);  //����   D12
}

void TIM4_PWM_CH2_D13(short duty)
{
		TIM_SetCompare2(TIM4,duty); //����   D13
}

void TIM4_PWM_CH3_D14(short duty)
{
		TIM_SetCompare3(TIM4,duty);
}

void TIM4_PWM_CH4_D15(short duty)
{
		TIM_SetCompare4(TIM4,duty);
}

/*------------- TIM10 ̽�յ�---------------*/
void TIM10_PWM_CH1_F6(short duty)
{
			TIM_SetCompare1(TIM10,duty);
		
}
/*------------- TIM11 ---------------*/
void TIM11_PWM_CH1_F7(short duty)
{
			TIM_SetCompare1(TIM11,duty);
}


/*
��1����APB1��APB2��Ƶ��Ϊ1��ʱ��TIM1��TIM8~TIM11��ʱ��ΪAPB2��ʱ�ӣ�TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӣ�

��2�������APB1��APB2��Ƶ����Ϊ1����ôTIM1��TIM8~TIM11��ʱ��ΪAPB2��ʱ�ӵ�������TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������
		 ����ʱ�ӷ�������֪
		 ��Ϊϵͳ��ʼ��SystemInit�������ʼ��APB1����ʱ��Ϊ4��Ƶ��42M��APB2����ʱ��Ϊ2��Ƶ��84M��
		 ����TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M��
*/


void All_TIM_Enable(void)
{
		




}

void All_TIM_Disable(void)
{
		TIM_Cmd(TIM1, DISABLE);  //ʧ��TIM1
		TIM_Cmd(TIM3, DISABLE);  //ʧ��
		TIM_Cmd(TIM4, DISABLE);  //ʧ��
		TIM_Cmd(TIM10, DISABLE); //ʧ��
		TIM_Cmd(TIM11, DISABLE); //ʧ��
}

//TIM1 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��

void TIM1_PWM_Init(uint32 arr,uint32 psc)
{		 					 
		//�˲������ֶ��޸�IO������
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1ʱ��ʹ��    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTEʱ��	
		
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;//GPIOE
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
		GPIO_Init(GPIOE,&GPIO_InitStructure);              //��ʼ��P
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��1
		
		//��ʼ��TIM14 Channel1 PWMģʽ	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
		
		TIM_CtrlPWMOutputs(TIM1,ENABLE);
		
		TIM_Cmd(TIM1, DISABLE);  //ʧ��TIM1
				
}  

//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(uint32 arr,uint32 psc)//��ȡ��
{		 					 
		//�˲������ֶ��޸�IO������
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	//TIM3ʱ��ʹ��    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ��PORTFʱ��	
		
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);	
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
		GPIO_Init(GPIOB,&GPIO_InitStructure);               //��ʼ��PF7
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
		
		//��ʼ��TIM11 Channel PWMģʽ	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 3OC1
		TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 4OC1
				
		TIM_CtrlPWMOutputs(TIM3,ENABLE);		  
		
		TIM_Cmd(TIM3, DISABLE);  //ʧ��TIM3
				
} 

//TIM4 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM4_PWM_Init(uint32 arr,uint32 psc)
{		 					 
		//�˲������ֶ��޸�IO������
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM1ʱ��ʹ��    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTFʱ��	
		
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//GPIOD
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
		GPIO_Init(GPIOD,&GPIO_InitStructure);              //��ʼ��PF9
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��4
		
		//��ʼ��TIM4 Channel PWMģʽ	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1
		TIM_OC2Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1
		TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1		
		TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM4,ENABLE);
		
		TIM_Cmd(TIM4, DISABLE);  //ʧ��TIM4						  
} 



//TIM10 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM10_PWM_Init(uint32 arr,uint32 psc)//̽�յ�90K Hz -> F6
{		 					 
		//�˲������ֶ��޸�IO������
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  	//TIM10ʱ��ʹ��    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
		
		GPIO_PinAFConfig(GPIOF,GPIO_PinSource6,GPIO_AF_TIM10);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
		GPIO_Init(GPIOF,&GPIO_InitStructure);              //��ʼ��PF9
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);//��ʼ����ʱ��10
		
		//��ʼ��TIM4 Channel PWMģʽ	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM10, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM10,ENABLE);
		
		TIM_Cmd(TIM10, DISABLE);  //ʧ��TIM10					  
} 



//TIM11 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM11_PWM_Init(uint32 arr,uint32 psc)//̽�յ�90K Hz -> F7
{		 					 
		//�˲������ֶ��޸�IO������
		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  	//TIM11ʱ��ʹ��    
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 	//ʹ��PORTFʱ��	
		
		GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_TIM11);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//GPIOF
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
		GPIO_Init(GPIOF,&GPIO_InitStructure);               //��ʼ��PF7
			
		TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
		
		TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);//��ʼ����ʱ��11
		
		//��ʼ��TIM11 Channel PWMģʽ	 
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
		TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ե�
		TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
		TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
		
		TIM_OC1Init(TIM11, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 4OC1
		
		TIM_CtrlPWMOutputs(TIM11,ENABLE);		  
		
		TIM_Cmd(TIM11, DISABLE);  //ʹ��TIM10		
} 

