#ifndef _FLASH_H_
#define _FLASH_H_

#define FLIGHT_PARAMETER_TABLE_NUM  60
#define PID_USE_NUM  	8
//FLASH��ʼ��ַ   W25Q128 16M ��������Ϊ 256 ���飨Block��
#define Nor_FLASH_ADDRESS    (0x0000) 	//W25Q128 FLASH�� ��ͨ��ʼ��ַ   ���ڸ�������

#define IMP_FLASH_ADDRESS    (0x1000) 	//W25Q128 FLASH�� ��Ҫ������ʼ��ַ ���ڸ�������
#include "DataType.h"

typedef struct
{
		uint8 No_0xFF[PID_USE_NUM];
}PID_Parameter_Flag;

typedef enum  // _e ��ʾö��
{
		VEHICLE_MODE_e,              //����ģʽ  rov/auv
		DEBUG_TOOL_e,                //Debug���� ano/vcan
		
		ROBOTIC_ARM_MAX_VALUE_e,    //��е�ۿ������ֵ  
		ROBOTIC_ARM_MIN_VALUE_e,   	//��е�۹ر�ֵ
		ROBOTIC_ARM_MED_VALUE_e, 		//��е�۵�ǰֵ
		ROBOTIC_ARM_SPEED_e,   			//��е��ת���ٶ�
	
		YUNTAI_MAX_VALUE_e,    	 //��̨�������ǰֵ
		YUNTAI_MIN_VALUE_e,	 		 //��̨���µ�ǰֵ	
		YUNTAI_MED_VALUE_e,   	 //��̨��ֵ
		YUNTAI_SPEED_e,  				 //��̨ת���ٶ�

		COMPASS_OFFSET_ANGLE_e,    //ָ���벹���Ƕ�
	
   	//���ƽ�������
		PROPELLER_RIGHT_UP_DIR_e,   // ����
		PROPELLER_LEFT_DOWN_DIR_e,  //����
		PROPELLER_LEFT_UP_DIR_e,    //����
		PROPELLER_RIGHT_DOWN_DIR_e, //����
		PROPELLER_LEFT_MED_DIR_e,   //����
		PROPELLER_RIGHT_MED_DIR_e,  //����
	
		PROPELLER_POWER_e,  //�ƽ�������ϵ��
	
		DEPTH_SENSOR_TYPE_e,  //��ȴ����� ����
	
		BATTERY_CAPACITY_e, //��ص������� 3s/4s/6s
	
		EXTRACTOR_VALUE_e,  //��ȡ�������̶�ֵ
	
		WORK_MODE_e,  //��ȡ�������̶�ֵ	
		
		/* �ƽ���ƫ��ֵ */
		PROPELLER_RIGHT_UP_ERROR_e,  //�����ƽ��� ƫ��ֵ
		PROPELLER_LEFT_DOWN_ERROR_e,
		PROPELLER_LEFT_UP_ERROR_e,
		PROPELLER_RIGHT_DOWN_ERROR_e,
		PROPELLER_LEFT_MED_ERROR_e,
		PROPELLER_RIGHT_MED_ERROR_e,
	/******************************************/
		PARAMEMER_MAX_NUMBER_e,    //������ֵ
	
}NORMAL_PARAMETER_TABLE;


typedef enum //FLASH ��ַaddressö��
{
		PID1_PARAMETER_KP,
		PID1_PARAMETER_KI,
		PID1_PARAMETER_KD,
		PID2_PARAMETER_KP,
		PID2_PARAMETER_KI,
		PID2_PARAMETER_KD,
		PID3_PARAMETER_KP,
		PID3_PARAMETER_KI,
		PID3_PARAMETER_KD,
		PID4_PARAMETER_KP,
		PID4_PARAMETER_KI,
		PID4_PARAMETER_KD,
		PID5_PARAMETER_KP,
		PID5_PARAMETER_KI,
		PID5_PARAMETER_KD,
		PID6_PARAMETER_KP,
		PID6_PARAMETER_KI,
		PID6_PARAMETER_KD,
		PID7_PARAMETER_KP,
		PID7_PARAMETER_KI,
		PID7_PARAMETER_KD,
		PID8_PARAMETER_KP,
		PID8_PARAMETER_KI,
		PID8_PARAMETER_KD,   //��ַö��
		
		
		PID9_PARAMETER_KP,
		PID9_PARAMETER_KI,
		PID9_PARAMETER_KD,   //��ַö��
		
				
		PID10_PARAMETER_KP,
		PID10_PARAMETER_KI,
		PID10_PARAMETER_KD,   //��ַö��
	

}IMPORTANT_PARAMETER_TABLE;

void Parameter_SelfCheck(uint32 *RealParameter,uint32 *TempParameter);
void Normal_Parameter_SelfCheck_With_Flash(void); //Flash�����Լ� ��Ϊ-1 �� 0 ��Ϊ ���������� 
/* FLASH ���� ��ֵͨ */
void Flash_Update(void);

void Capacity_Flash_Update(float array[],uint8 number);
int Normal_Parameter_Init_With_Flash(void);
															 
void Save_PID_Parameter(void);			
												 
#endif



