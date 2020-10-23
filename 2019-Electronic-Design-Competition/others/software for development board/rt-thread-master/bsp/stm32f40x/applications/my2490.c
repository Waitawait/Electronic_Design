#include "my2490.h"
#include <rtthread.h>
#include "uart.h"
#include <elog.h>
#include <stdlib.h>
//uint8 VolunmMax[6] = {0x7E,0X04,0X31,0X1E,0X2B,0XEF};//�������
uint8 VolunmMax[6] = {0x7E,0X04,0X31,0X0A,0X3F,0XEF};//�������
uint8 my2490_number_array[7] = {0X7E,0X05,0X41,0X00,0X00,0X45,0XEF}; //my2490��Ӧ��Ŀ

void uart_send_my2490_now_status(uint8 *cmd_array,uint32 number)//���ڷ��͸� MY2490��ǰ��Ϣ
{
		if(cmd_array[0] == 0x7E){//��ʱ����ѡ��ʱ
				cmd_array[4] = number+1;
				cmd_array[5] = (cmd_array[1] ^ cmd_array[2] ^ cmd_array[3] ^cmd_array[4]);
				rt_device_write(debug_uart_device, 0,cmd_array,7);
		}
		else if(cmd_array[0] == 0x00){//����״̬

		}
}




void Volunm_Max(void)  //ʹ�������
{

		rt_device_write(debug_uart_device, 0,VolunmMax,6);
	
}


/* led off MSH���� */
int set_my2940_vol(int argc, char **argv)
{
    int result = 0;
		int res = 0;
    if (argc != 2){
        log_e("Error! Proper Usage: set_my2940_vol <0~30>");
				result = -RT_ERROR;
        goto _exit;
    }
		res = atoi(argv[1]);
		if(res <= 30){
				VolunmMax[3] = res;
				VolunmMax[4] = (VolunmMax[1]^VolunmMax[2]^VolunmMax[3]);
				rt_device_write(debug_uart_device, 0,VolunmMax,6);
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(set_my2940_vol,set_my2940_vol <0~30>);


