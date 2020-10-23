/*
 * servo.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  �������豸����ȴ�������CPU������ADC��ѹ��
 */
 
 #define LOG_TAG    "sensor"
#include "sensor.h"
#include <rtthread.h>
#include <elog.h>
#include <string.h>
#include "drv_MS5837.h"
#include "gyroscope.h"
#include "flash.h"
#include "stdio.h"
#include "drv_i2c.h"
#include "drv_adc.h"
#include "drv_cpu_temp.h"
#include "drv_cpuusage.h"
#include "filter.h"
#include "drv_spl1301.h"

/*----------------------- Variable Declarations -----------------------------*/
char *Depth_Sensor_Name[3] = {"MS5837","SPL1301","null"};

Sensor_Type Sensor;//����������
		float temp_current = 0.0f;

/*----------------------- Function Implement --------------------------------*/
/**
  * @brief  sensor_lowSpeed_thread_entry(���ٻ�ȡ������������)
  * @param  void* parameter
  * @retval None
  * @notice 
  */
void sensor_lowSpeed_thread_entry(void* parameter)
{
		uint8 cpu_usage_major, cpu_usage_minor; //����λ��С��λ

		rt_thread_mdelay(1000);//�ȴ�3sϵͳ�ȶ��ٻ�ȡ����

		while(1)
		{
			
				Sensor.CPU.Temperature = get_cpu_temp();           //��ȡCPU�¶�
				Sensor.PowerSource.Voltage = get_voltage_value();  //��ȡ��Դ��ѹֵ
//				if(Sensor.PowerSource.Voltage > 6.0f ){							//��δ�����Դʱ����������ֵ
//						
//						Sensor.PowerSource.Current = get_current_value();  //��ȡINA169����ֵ
//						temp_current = Sensor.PowerSource.Current ;
//						Sensor.PowerSource.Current = KalmanFilter(&Sensor.PowerSource.Current);
//						 //����ֵ ���п������˲����ÿ������˲�����r��ֵ���ͺ�����Խϴ�
//				}
				cpu_usage_get(&cpu_usage_major, &cpu_usage_minor); //��ȡCPUʹ����
				Sensor.CPU.Usage = cpu_usage_major + (float)cpu_usage_minor/100;
			

				rt_thread_mdelay(100);
		}
}

/**
  * @brief  sensor_highSpeed_thread_entry(���ٻ�ȡ������������)
  * @param  void* parameter
  * @retval None
  * @notice 
  */


void sensor_highSpeed_thread_entry(void* parameter)
{

		rt_thread_mdelay(1000);//�ȴ�3sϵͳ�ȶ��ٻ�ȡ����
	
		while(1)
		{
			
				//JY901_Convert(&Sensor.JY901); //JY901����ת��
				//Depth_Sensor_Data_Convert();  //�������ת��

				rt_thread_mdelay(20);
		}
}



/*******************************�̳߳�ʼ��*******************************************/
int sensor_thread_init(void)
{
    rt_thread_t sensor_lowSpeed_tid; //���ٻ�ȡ�Ĵ���������ӿ�
	  rt_thread_t sensor_highSpeed_tid;//���ٻ�ȡ�Ĵ���������ӿ�
		/*������̬�߳�*/
    sensor_lowSpeed_tid = rt_thread_create("sensor",  //�߳�����
                    sensor_lowSpeed_thread_entry,		 //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    2048,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    30,										 	 //�߳����ȼ���priority��
                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

	  sensor_highSpeed_tid = rt_thread_create("sensor",  //�߳�����
                    sensor_highSpeed_thread_entry,		 //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    2048,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    10,										 	 //�߳����ȼ���priority��
                    3);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (sensor_lowSpeed_tid != RT_NULL && sensor_highSpeed_tid != RT_NULL){
			



				if(adc_init()){ log_i("Adc_Init()");}//ADC��ѹ�ɼ���ʼ��

//				rt_thread_startup(sensor_lowSpeed_tid);
//				rt_thread_startup(sensor_highSpeed_tid);
		}
		return 0;
}
//INIT_APP_EXPORT(sensor_thread_init);


void Depth_Sensor_Data_Convert(void)//��ȴ���������ת��
{
		static uint32 value[10] = {0};
		static uint8 ON_OFF = 0; //��������
		static uint8 i = 0;
		
		if(SPL1301 == Sensor.DepthSensor.Type){  //��� SPL1301
				spl1301_get_raw_temp();
				spl1301_get_raw_pressure();//����������ת��
				
				if(ON_OFF == 0){
						ON_OFF = 1;
						Sensor.DepthSensor.Init_PessureValue = get_spl1301_pressure();//��ȡ��ʼ������
				}
				for(i = 0;i < 10;i++){
						value[i++] = get_spl1301_pressure();//��ȡ1������
				}
				Sensor.DepthSensor.Temperature = get_spl1301_temperature();
				Sensor.DepthSensor.PessureValue = Bubble_Filter(value);
								 				                              /* �����ֵ ��λΪcm   ����ϵ��Ϊ 1.3 ��λ/cm */
				Sensor.DepthSensor.Depth = ((Sensor.DepthSensor.PessureValue - Sensor.DepthSensor.Init_PessureValue)/20);		
			 
		}
		else if(MS5837 == Sensor.DepthSensor.Type){ //ʹ��MS5837
			
				if(ON_OFF == 0 ){
						ON_OFF = 1; //��������																							       
						Sensor.DepthSensor.Init_PessureValue = get_ms5837_pressure();//��ȡ��ʼ������
				}		 
						
				Sensor.DepthSensor.PessureValue = get_ms5837_pressure();
				Sensor.DepthSensor.Temperature  = get_ms5837_temperature();

				//����״̬����ȴ�������ѹ��ֵ��ӦԽ��Խ��
				if(Sensor.DepthSensor.Init_PessureValue - Sensor.DepthSensor.PessureValue >= 1 && \
					 Sensor.DepthSensor.Init_PessureValue - Sensor.DepthSensor.PessureValue <= 5 ){	//����ȴ����� ��ǰֵ�𽥱�С�����ж�Ϊ����Ư�ƣ����ֵ���ڵ�ǰֵ
			
						Sensor.DepthSensor.Init_PessureValue = Sensor.DepthSensor.PessureValue;
				}
					 				                              /* �����ֵ ��λΪcm   ����ϵ��Ϊ 1.95 ��λ/cm */
				Sensor.DepthSensor.Depth = ((Sensor.DepthSensor.PessureValue - Sensor.DepthSensor.Init_PessureValue)/1.95f);			
		}

}


/* ��ӡ��������Ϣ */
void print_sensor_info(void)
{
		log_i("    variable        |  value");
		log_i("--------------------|-----------");
	
		log_i("      Roll          |  %+0.3f",Sensor.JY901.Euler.Roll);
		log_i("      Pitch         |  %+0.3f",Sensor.JY901.Euler.Pitch);
		log_i("      Yaw           |  %+0.3f",Sensor.JY901.Euler.Yaw);
		log_i("--------------------|-----------");
		log_i("      Acc.x         |  %+0.3f",Sensor.JY901.Acc.x);
		log_i("      Acc.y         |  %+0.3f",Sensor.JY901.Acc.y);//
		log_i("      Acc.z         |  %+0.3f",Sensor.JY901.Acc.z);//
		log_i("--------------------|-----------");
		log_i("      Gyro.x        |  %+0.3f",Sensor.JY901.Gyro.x);
		log_i("      Gyro.y        |  %+0.3f",Sensor.JY901.Gyro.y);//	
		log_i("      Gyro.z        |  %+0.3f",Sensor.JY901.Gyro.z);//	
		log_i("  JY901_Temperature |  %+0.3f",Sensor.JY901.Temperature);//					

	
		log_i("--------------------|-----------");
		log_i("     Voltage        |  %0.3f",Sensor.PowerSource.Voltage); //��ѹ
		log_i("     Current        |  %0.3f",Sensor.PowerSource.Current); //����
		log_i("--------------------|-----------");
		log_i("  Depth Sensor Type |  %s",Depth_Sensor_Name[Sensor.DepthSensor.Type]); //��ȴ���������
		log_i(" Water Temperature  |  %0.3f",Sensor.DepthSensor.Temperature);    //ˮ��
		log_i("sensor_Init_Pressure|  %0.3f",Sensor.DepthSensor.Init_PessureValue); //��ȴ�������ʼѹ��ֵ	
		log_i("   sensor_Pressure  |  %0.3f",Sensor.DepthSensor.PessureValue); 		 //��ȴ�������ǰѹ��ֵ	
		log_i("     Depth          |  %0.3f",Sensor.DepthSensor.Depth); 									 //���ֵ
		log_i("--------------------|-----------");	
		log_i("    CPU.Usages      |  %0.3f",	Sensor.CPU.Temperature); //CPU�¶�
		log_i("   CPU.Temperature  |  %0.3f",	Sensor.CPU.Usage); 			 //CPUʹ����

		

}
MSH_CMD_EXPORT(print_sensor_info, printf gysocope & PowerSource & pressure);



/*����ȴ������� �޸� �����͡�MSH���� */
static int set_depth_sensor_type(int argc, char **argv) //ֻ���� 0~3.0f
{
    int result = 0;
    if (argc != 2){ //6���ƽ���
        log_e("Error! Proper Usage: set_depth_sensor_type <ms5837/spl1301/null>");
				result = -RT_ERROR;
        goto _exit;
    }
		
	  if( !strcmp(argv[1],"ms5837") ) {
				 
				Sensor.DepthSensor.Type = MS5837; //
				Flash_Update();
	
				log_i("Sensor.DepthSensor.Type :%s",Depth_Sensor_Name[Sensor.DepthSensor.Type]);
				log_i("Please reboot now");		
		}
	  else if( !strcmp(argv[1],"spl1301") ) {
				 
				Sensor.DepthSensor.Type = SPL1301; //
				Flash_Update();

				log_i("Sensor.DepthSensor.Type :%s",Depth_Sensor_Name[Sensor.DepthSensor.Type]);
				log_i("Please reboot now");				
		}	

	  else if( !strcmp(argv[1],"null") ) {
				 
				Sensor.DepthSensor.Type = DS_NULL; //����ȴ�����
				Flash_Update();

				log_i("Sensor.DepthSensor.Type :%s",Depth_Sensor_Name[Sensor.DepthSensor.Type]);
				log_i("Please reboot now");				
		}				
		else {
				log_e("Error! Input Error!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_depth_sensor_type,depth_sensor_type_set <ms5837/spl1301/null> );



/*����Դ������ �޸� ��������MSH���� */
static int set_battery_capacity(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){ //6���ƽ���
        log_e("Error! Proper Usage: set_battery_capacity <2s/3s/4s/6s> % ");
				result = -RT_ERROR;
        goto _exit;
    }
		
	  if( !strcmp(argv[1],"2s") ) {
				 
				Sensor.PowerSource.Capacity = 4.2*2; // 2s->8.4v������ѹ
				Flash_Update();
	
				log_i("Sensor.PowerSource.Capacity :%f v",Sensor.PowerSource.Capacity);
		}		
	  else if( !strcmp(argv[1],"3s") ) {
				 
				Sensor.PowerSource.Capacity = 4.2*3; // 3s->12.6v������ѹ
				Flash_Update();
	
				log_i("Sensor.PowerSource.Capacity :%f v",Sensor.PowerSource.Capacity);
		}
	  else if( !strcmp(argv[1],"4s") ) {
				 
				Sensor.PowerSource.Capacity = 4.2*4; // 4s->16.8v������ѹ
				Flash_Update();

				log_i("Sensor.PowerSource.Capacity :%f v",Sensor.PowerSource.Capacity);
		}		
		
		else if( !strcmp(argv[1],"6s") ) {
				 
				Sensor.PowerSource.Capacity = 4.2*6; // 6s->25.2v������ѹ
				Flash_Update();

				log_i("Sensor.PowerSource.Capacity :%f v",Sensor.PowerSource.Capacity);
		}		
		else {
				log_e("Error! Input Error!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_battery_capacity,set_battery_capacity <2s/3s/4s/6s> );








