/*
 * uart.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  �����豸��д����
 */
#define LOG_TAG    "uart"

#include "gyroscope.h"
#include <elog.h>
#include <string.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_ano.h"
#include "uart.h"
#include "rc_data.h"
#include "focus.h"
#include "HMI.h"
/*---------------------- Constant / Macro Definitions -----------------------*/

char GYRO_UART_NAME[]    = "uart2";  //JY901 uart2

char DEBUG_UART_NAME[]  =  "uart3" ; //WIFI uart3

char CONTROL_UART_NAME[] = "uart4";  //CP2101 uart4

char FOCUS_UART_NAME[]  =  "uart5";  //�佹����ͷ

#define Query_JY901_data 0     /* "1"Ϊ���Բ�ѯ  "0"Ϊ������ȡ */

#if Query_JY901_data
char recv_buffer[128]; 				//����2�������ݻ������,
unsigned char recv_data_p = 0x00;  //  ����2��������ָ��
#endif

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init �¼����ƿ�. */
extern struct rt_event init_event;

rt_device_t control_uart_device;	
rt_device_t debug_uart_device;	
rt_device_t gyro_uart_device;	
rt_device_t focus_uart_device;	

struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* ���ò��� */

struct rt_semaphore control_rx_sem;/* ���ڽ�����Ϣ���ź��� */
struct rt_semaphore gyro_rx_sem;/* ���ڽ�����Ϣ���ź��� */
struct rt_semaphore debug_rx_sem;/* ���ڽ�����Ϣ���ź��� */
struct rt_semaphore focus_rx_sem;/* ���ڽ�����Ϣ���ź��� */

uint8 uart_startup_flag = 0; //debug���� ��ʼ����ɱ�־λ
/*------------------------------------------ Control Uart ------------------------------------------------*/
/* �������ݻص����� */
static rt_err_t control_uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&control_rx_sem);

    return RT_EOK;
}

static void control_thread_entry(void *parameter)
{
    unsigned char ch;

		while (1)
		{
				/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
				while (rt_device_read(control_uart_device, 0, &ch, 1) != 1)
				{
				  /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
						rt_sem_take(&control_rx_sem, RT_WAITING_FOREVER);
				}
				Remote_Control_Data_Analysis(ch);
		}
}

/*------------------------------------------ Gyro Uart ------------------------------------------------*/
/* �������ݻص����� */
static rt_err_t gyroscope_uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&gyro_rx_sem);

    return RT_EOK;
}

static void gyroscope_thread_entry(void *parameter)
{
    unsigned char ch;

		while (1)
		{
				/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
				while (rt_device_read(gyro_uart_device, 0, &ch, 1) != 1)
				{
				  /* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
						rt_sem_take(&gyro_rx_sem, RT_WAITING_FOREVER);
				}
#if Query_JY901_data //���ߵ��� ��ѯģʽ	
				recv_buffer[recv_data_p] = ch;
				recv_data_p++;
				if(recv_data_p>127)recv_data_p = 0;
#else 
				CopeSerial2Data(ch); //��������ģʽ ɸѡ���ݰ�
#endif
		}
}

/*------------------------------------------ Debug Uart ------------------------------------------------*/
/* �������ݻص����� */
static rt_err_t debug_uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&debug_rx_sem);

    return RT_EOK;
}


static void debug_thread_entry(void *parameter)
{
    unsigned char ch;

		while (1)
		{
				/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
				while (rt_device_read(debug_uart_device, 0, &ch, 1) != 1)
				{
						/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
						rt_sem_take(&debug_rx_sem, RT_WAITING_FOREVER);
				}
				ANO_DT_Data_Receive_Prepare(ch);

		}
}

/*------------------------------------------ Focus Uart ------------------------------------------------*/
/* �������ݻص����� */
static rt_err_t focus_uart_input(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&focus_rx_sem);

    return RT_EOK;
}


static void focus_thread_entry(void *parameter)
{
    unsigned char ch;

		while (1)
		{
				/* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
				while (rt_device_read(focus_uart_device, 0, &ch, 1) != 1)
				{
						/* �����ȴ������ź������ȵ��ź������ٴζ�ȡ���� */
						rt_sem_take(&focus_rx_sem, RT_WAITING_FOREVER);
				}
				//Camera_Focus_Data_Analysis(ch);
				HMI_Data_Analysis(ch);
		}
}


/*----------------------- Function Implement --------------------------------*/
int device_uart_init(void)
{
		uint8 uart_init_flag = 1;
		/* ���� �߳̾�� */
		rt_thread_t control_uart_tid;
		rt_thread_t gyroscope_uart_tid;
		rt_thread_t debug_uart_tid;
		rt_thread_t focus_uart_tid;
	
	  /* ����ϵͳ�еĴ����豸 */

		control_uart_device = rt_device_find(CONTROL_UART_NAME); 
		gyro_uart_device = rt_device_find(GYRO_UART_NAME);       
		debug_uart_device = rt_device_find(DEBUG_UART_NAME);
	  focus_uart_device = rt_device_find(FOCUS_UART_NAME);

		list_serial_devices();
	
    if (control_uart_device != RT_NULL){		
			
					/* �Զ�д�Լ��жϽӴ򿪴����豸 */
				rt_device_open(control_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX );
				config.baud_rate = BAUD_RATE_115200;
				config.data_bits = DATA_BITS_8;
				config.stop_bits = STOP_BITS_1;
				config.parity = PARITY_NONE;
			
				/* ���豸��ſ��޸Ĵ������ò��� */
				rt_device_control(debug_uart_device, RT_DEVICE_CTRL_CONFIG, &config);
				rt_sem_init(&control_rx_sem, "control_sem", 0, RT_IPC_FLAG_FIFO);
				/* ���ý��ջص����� */
				rt_device_set_rx_indicate(control_uart_device, control_uart_input);
		}	
		
    if (gyro_uart_device != RT_NULL){		
			
					/* �Զ�д�Լ��жϽӴ򿪴����豸 */
				rt_device_open(gyro_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);
				config.baud_rate = BAUD_RATE_115200;
				config.data_bits = DATA_BITS_8;
				config.stop_bits = STOP_BITS_1;
				config.parity = PARITY_NONE;
			
				rt_device_control(gyro_uart_device, RT_DEVICE_CTRL_CONFIG, &config);
				rt_sem_init(&gyro_rx_sem, "gyro_sem", 0, RT_IPC_FLAG_FIFO);
				/* ���ý��ջص����� */
				rt_device_set_rx_indicate(gyro_uart_device, gyroscope_uart_input);
		}
		
		if(debug_uart_device != RT_NULL){
				rt_device_open(debug_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX); 
				config.baud_rate = BAUD_RATE_115200;
				config.data_bits = DATA_BITS_8;
				config.stop_bits = STOP_BITS_1;
				config.parity = PARITY_NONE;
				
				log_v("Debug_Uart_Baud Rate:%d",config.baud_rate);
				/* ���豸��ſ��޸Ĵ������ò��� */
				rt_device_control(debug_uart_device, RT_DEVICE_CTRL_CONFIG, &config);
			
				rt_sem_init(&debug_rx_sem, "debug_sem", 0, RT_IPC_FLAG_FIFO);
				/* ���ý��ջص����� */
				rt_device_set_rx_indicate(debug_uart_device, debug_uart_input);
		}
		
		if (focus_uart_device != RT_NULL){		
			
					/* �Զ�д�Լ��жϽӴ򿪴����豸 */
				rt_device_open(focus_uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_DMA_RX);

				rt_sem_init(&focus_rx_sem, "focus_sem", 0, RT_IPC_FLAG_FIFO);
				/* ���ý��ջص����� */
				rt_device_set_rx_indicate(focus_uart_device, focus_uart_input);
		}
    /* ���� ���� serial �߳� */
		control_uart_tid = rt_thread_create("control_uart",
																		control_thread_entry,
																		RT_NULL, 
																		512, 
																		5,
																		10);
    /* ���� ���� serial �߳� */
		gyroscope_uart_tid = rt_thread_create("gyro_uart",
																			gyroscope_thread_entry,
																			RT_NULL, 
																			512, 
																			15,
																			10);
		
		/* ���� ���� serial �߳� */
		debug_uart_tid = rt_thread_create("debug_uart",
																	debug_thread_entry,
																	RT_NULL, 
																	512, 
																	20,
																	10);
																			
		/* ���� �佹 serial �߳� */
		focus_uart_tid = rt_thread_create("focus_uart",
																	focus_thread_entry,
																	RT_NULL, 
																	512, 
																	15,
																	10);
		/* �����ɹ��������߳� */
    if (control_uart_tid != RT_NULL){
				rt_thread_startup(control_uart_tid);
				uart_init_flag <<= 1;
    }
    /* �����ɹ��������߳� */
    if (gyroscope_uart_tid != RT_NULL){
				rt_thread_startup(gyroscope_uart_tid);
				uart_init_flag <<= 1;
    }
		
		/* �����ɹ��������߳� */
    if (debug_uart_tid != RT_NULL){
				rt_thread_startup(debug_uart_tid);
				uart_init_flag <<= 1;
    }
		
				/* �����ɹ��������߳� */
    if (focus_uart_tid != RT_NULL){
				rt_thread_startup(focus_uart_tid);
				uart_init_flag <<= 1;
    }
		
		if(0x10 == uart_init_flag){ //�ȴ����д����豸����ʼ����� ��uart_startup_flag
				uart_startup_flag = 1;
				log_i("Uart_Init()");
			  //rt_event_send(&init_event, GYRO_EVENT); //�����¼�  ��ʾ��ʼ�����
		}
		else {
				uart_startup_flag = 0;
			  log_e("Uart_Init have some errors.");
		}
		return 0;
}
INIT_APP_EXPORT(device_uart_init);


int list_serial_devices(void)
{
		log_v("  ");
		log_v("name             serial number");
    log_v("---------------  -------------");
		log_v("control serial:      %s", control_uart_device);	
		log_v("gyroscope serial:    %s", gyro_uart_device);
		log_v("debug serial:        %s", debug_uart_device);
		log_v("console serial:      %s", RT_CONSOLE_DEVICE_NAME);	
		log_v("focus serial:        %s", focus_uart_device);
		rt_kprintf("\r\n");
		return 0;
}
MSH_CMD_EXPORT(list_serial_devices,list serial devices to used );
