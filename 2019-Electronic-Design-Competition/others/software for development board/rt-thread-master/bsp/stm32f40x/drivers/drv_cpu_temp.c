/*
 * File      : board.h

 */
#define LOG_TAG "drv_cpu_temp"

#include "board.h"
#include <rtthread.h>
#include <elog.h>
#include <string.h>
#include <stdio.h>
#include "drv_cpu_temp.h"
#include "filter.h"

/**
 * ��ȡADCֵ����
 *
 * @param ch ͨ��
 * @return ADC��ֵ
 */
static uint16_t get_adc(u8 ch) {  //���� STM32F40x �� STM32F41x �������¶ȴ������ڲ����ӵ�ͨ�� ADC1_IN16
    ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_112Cycles );//�ֲ��251ҳ
    ADC_SoftwareStartConv(ADC1);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
    return ADC_GetConversionValue(ADC1);
}

/**
 * ��ȡ CPU �¶Ⱥ���
 *
 * @return CPU�¶�ֵ
 */
float get_cpu_temp(void) {

    uint32_t adc[10] = { 0 }, adc_max = 0, adc_min = 0;
    uint32_t adc_value = 0, adc_sum = 0;
    double temperature = 0.0;

    for (uint8_t t = 0; t < 10; t++) {
        adc[t] = get_adc(ADC_Channel_16);
        adc_sum = adc_sum + adc[t];
        rt_thread_delay(rt_tick_from_millisecond(5));
    }
    adc_max = adc[0];
    adc_min = adc[0];
    for (uint8_t t = 1; t < 10; t++) {
        if (adc_max < adc[t]) {
            adc_max = adc[t];
        }
        if (adc_min > adc[t]) {
            adc_min = adc[t];
        }
    }

    adc_value = ((adc_sum - adc_max - adc_min) / 8);//��ȥ�����Сֵ ȡƽ��
    temperature = (float) adc_value * (3.3 / 4096);
    temperature = (temperature - 0.76) / 0.0025 + 25;
	
    return (temperature);
}



/**
 * CPU �¶� ADC Ӳ����ʼ��
 */
int stm32_cpu_temperature_init(void) {
  
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
    ADC_TempSensorVrefintCmd(ENABLE);

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_112Cycles);
    ADC_Cmd(ADC1, ENABLE);

    return 0;
}
INIT_BOARD_EXPORT(stm32_cpu_temperature_init);
