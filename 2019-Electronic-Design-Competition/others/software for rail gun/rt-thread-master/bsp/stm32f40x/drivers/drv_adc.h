#ifndef __ADC_CONVERT_H_
#define __ADC_CONVERT_H_

#include "DataType.h"



//��ʼ��ADC															   
int adc_init(void);

uint16 get_adc2(uint8 ch);  //get adcͨ��ֵ

uint16 get_adc1(uint8 ch); //get adc3ͨ��ֵ

float get_voltage_value(void); //get ��ѹֵ

float get_current_value(void);// get����ֵ 

#endif


