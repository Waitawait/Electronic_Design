/*
 * adc.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  ADC��ѹ�ɼ�   
 */
#define LOG_TAG    "adc"

#include "drv_adc.h"
#include <rtthread.h>
#include <elog.h>
#include <stdio.h>
#include "sys.h"
#include "filter.h"
/*---------------------- Constant / Macro Definitions -----------------------*/		

#define REFER_VOLTAGE       (3.3f)          // �ο���ѹ 3.3V,���ݾ��ȳ���100����2λС��
#define CONVERT_BITS        ((1 << 12)-1)   // ת��λ��Ϊ12λ 4095
#define Voltge_Parameter    11				      // ��ѹϵ�� 1/(200K/(200K+2M)) = 11

/*----------------------- Variable Declarations -----------------------------*/


/*----------------------- Function Implement --------------------------------*/



/* ð�� get��ѹ */
float get_voltage_value(void)
{
		uint8 i = 0;
		float voltage = 0.0f;
		uint32 adc_value[10] = {0};			
		for(i = 0;i < 10;i++){
				rt_thread_mdelay(10);
				adc_value[i] = get_adc2(ADC_Channel_10);//����
		}
	
		voltage = Bubble_Filter(adc_value) * REFER_VOLTAGE / CONVERT_BITS * Voltge_Parameter;	//��ѹ���㹫ʽ��voltage = adc����ֵ * ��������(3.3V/4096) *��ѹϵ��
		return voltage;
} 


///* ð�� get���� */
//float get_current_value(void)
//{

//		for(i = 0;i < 10;i++){

//				adc_value[i] = get_adc3(ADC_Channel_11);//����
//		}
//	
//		voltage = Bubble_Filter(adc_value) * REFER_VOLTAGE / CONVERT_BITS ;	//��ѹ���㹫ʽ��voltage = adc����ֵ * ��������(3.3V/4096) 
//		current = voltage * 6.5f;   // Is = Vo*1K/(Rs*Rl) = (Vo*1K)/(0.01*100K) = (1*Vo)��ϸ��INA16 Datasheet
//		return current;
//}

/* ð�� get���� */
float get_current_value(void)
{
		uint8 j = 0;
		uint32 adc1_value[10] = {0};
		float voltage1 = 0.0f,current = 0.0f;
		for(j = 0;j < 10;j++){
				rt_thread_mdelay(10);
				adc1_value[j] = get_adc2(ADC_Channel_11);//����
		}
	
		voltage1 = Bubble_Filter(adc1_value)  * REFER_VOLTAGE / CONVERT_BITS ;//��ѹ���㹫ʽ��voltage = adc����ֵ * ��������(3.3V/4096) 
		current=voltage1;   // Is = Vo*1K/(Rs*Rl) = (Vo*1K)/(0.01*100K) = (1*Vo)��ϸ��INA16 Datasheet
		return current;
}

//���ADCֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 get_adc2(u8 ch)   
{
		//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
		ADC_RegularChannelConfig(ADC2, ch, 1, ADC_SampleTime_480Cycles );	//ADC2,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    

		ADC_SoftwareStartConv(ADC2);		//ʹ��ָ����ADC2�����ת����������	
		 
		while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));//�ȴ�ת������

		return ADC_GetConversionValue(ADC2);	//�������һ��ADC2�������ת�����
}

//���ADC3ֵ
//ch: @ref ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_Channel_0~ADC_Channel_16
//����ֵ:ת�����
u16 get_adc1(u8 ch)   
{
		//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
		ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles );	//ADC3,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    

		ADC_SoftwareStartConv(ADC3);		//ʹ��ָ����ADC3�����ת����������	
		 
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

		return ADC_GetConversionValue(ADC3);	//�������һ��ADC3�������ת�����
}

//��ʼ��ADC																	   
int  adc_init(void)
{    
		GPIO_InitTypeDef  GPIO_InitStructure;
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef       ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOCʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); //ʹ��ADC2ʱ��

		 //��ʼ��ADC2ͨ��10 IO��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;//PC0 ͨ��10    PC1 ͨ��11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
		GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	 
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC2��λ

	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//��λ����	 
	
		ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
		ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
		
		ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
		ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1
		
		ADC_Init(ADC2, &ADC_InitStructure);//ADC2��ʼ��	

		ADC_Cmd(ADC2, ENABLE);//����AD2ת����	

		return 1;
}

