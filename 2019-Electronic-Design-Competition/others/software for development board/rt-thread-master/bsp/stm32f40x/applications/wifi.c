/*
 * wifi.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  wifi�豸
 */
#include "init.h"
#include "wifi.h"
#include <rtdevice.h>
#include <elog.h>
/*---------------------- Constant / Macro Definitions -----------------------*/
//#define WIFI_UART_NAME        "uart3"   // uart3 WIFI 

/*----------------------- Variable Declarations -----------------------------*/
//extern rt_device_t wifi_uart_device;	
//extern struct rt_semaphore wifi_rx_sem;/* ���ڽ�����Ϣ���ź��� */

//u8 wifi_recv_buffer[128] = {0}; //wifi���ݰ�������


/*----------------------- Function Implement --------------------------------*/

//void Query_WiFi_Connected(void)
//{
//		rt_device_write(wifi_uart_device, 0, "AT+WSLK\r\n", sizeof("AT+WSLK\r\n")); //��ѯSTA������Link״̬
//	
//}

///* ���� ����ģ�� ���ٶ�У׼ */
//void query_wifi_connect(void)
//{
//		  Query_WiFi_Connected();
//			log_i("Query_WiFi_Connected\r\n");
//}
//MSH_CMD_EXPORT(query_wifi_connect,query_wifi_connect);


///* �������ݻص����� */
//rt_err_t wifi_uart_input(rt_device_t dev, rt_size_t size)
//{
//    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
//    rt_sem_release(&wifi_rx_sem);

//    return RT_EOK;
//}


//static void wifi_thread_entry(void *parameter)
//{
//    u8 i = 0,ch;
//		
//		while (1)
//		{
//				/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
//				while (rt_device_read(wifi_uart_device, 0, &ch, 1) != 1)
//				{
//						/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
//						rt_sem_take(&wifi_rx_sem, RT_WAITING_FOREVER);
//				}
//				wifi_recv_buffer[i++] = ch;
//		}
//}





//int wifi_uart_init(void)
//{
//		rt_thread_t wifi_tid;
//		/* ���� serial �߳� */
//		wifi_tid = 	rt_thread_create("wifi_thread",
//																	wifi_thread_entry,
//																	RT_NULL, 
//																	1024, 
//																	15,
//																	10);
//    /* �����ɹ��������߳� */
//    if (wifi_tid != RT_NULL){
//				log_i("WiFi_Init()");
//				rt_thread_startup(wifi_tid);

//    }
//		return 0;
//}
//INIT_APP_EXPORT(wifi_uart_init);






















