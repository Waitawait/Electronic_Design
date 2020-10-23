#define LOG_TAG    "light"

#include "light.h"
#include <rtthread.h>
#include <elog.h>
#include "sys.h"
#include <stdlib.h>
#include "drv_pwm.h"
#include <drivers/pin.h>

short light_value = 0;

/*******************************************
* �� �� ����Light_Output_Limit
* ��    �ܣ��ƹ������������
* ����������ƹ�����ֵ 0~90%
* �� �� ֵ��None
* ע    �⣺
********************************************/
int Light_Output_Limit(short *value)
{
		*value = *value >= 90 ? 90 : *value ;//�޷�
		*value = *value <= 0  ? 0   : *value ;//�޷�
		return *value;
}



/**
  * @brief  Light_Control(̽�յƿ���)
  * @param  ����ָ�� 0x00��������  0x01������  0x02������
  * @retval None
  * @notice 
  */

void Search_Light_Control(uint8 *action)
{
		switch(*action)
		{
				case 0x01:light_value += 5; //����
									break;  		
				case 0x02:light_value -= 5; //�䰵
									break;  
				default: break;
		}
		Light_Output_Limit(&light_value);
		TIM10_PWM_CH1_F6(light_value);
		TIM11_PWM_CH1_F7(light_value);
		*action = 0x00; //���������
}




/*��̽�յơ� �޸� ����ǰ�� MSH���� */
static int light(int argc, char **argv)
{
    int result = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: YunTai_medvalue_set 2000");
				result = -RT_ERROR;
        goto _exit;
    }

		if(atoi(argv[1]) <= 100 ){		
				light_value = atoi(argv[1]) ;
				log_i("Light Value:  %d",light_value);
		}
		else {
				log_e("Error! The value is out of range!");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(light,ag: light <0~100>);


