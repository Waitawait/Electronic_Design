#include "stm32f10x.h"
#include "ultrasonic.h"
#include"delay.h"

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(ULTRA_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin =ULTRA_TRIG;       //���͵�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
 	GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);

	GPIO_InitStructure.GPIO_Pin =ULTRA_ECHO;		//���ص�ƽ����
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
 	GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);	
}
/********************************************************************************************************************************************
@f_name: void Ranging(void)
@brief:	 ��������࣬��ֵ5�Σ�ʵ�� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 float *p:��໺�����
@return: None
*********************************************************************************************************************************************/
void Ultra_Ranging(float *p)
{
	u8 i;
	u32 j;
	float Ultr_Temp;	
	for(i=0;i<5;i++)
	{
		TRIG_Send=1;
		delay_ms(10);
		TRIG_Send=0;
		while(!ECHO_Reci);
		while(ECHO_Reci)
		{
			delay_us(10);
			j++;
		}
		Ultr_Temp+=340/2*j*10;//  ģ�����ɲ��3m 
		j=0;
		delay_ms(60);//��ֹ�����źŶԻ����źŵ�Ӱ��
	}
	*p=Ultr_Temp/5/1000000; 	
}
