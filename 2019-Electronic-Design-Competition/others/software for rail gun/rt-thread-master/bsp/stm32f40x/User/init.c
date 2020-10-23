/**
 *                             _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     ���汣��        ����BUG
*/

#define LOG_TAG    "init"

#include "init.h"
#include <rthw.h>
#include <board.h>
#include <stdio.h>

#include <spi_flash.h>
#include <easyflash.h>
#include <elog_flash.h>
#include <spi_flash_sfud.h>
#include "flash.h"
/*----------------------- Variable Declarations -----------------------------*/


rt_spi_flash_device_t nor_flash;

extern struct rt_event init_event;/* ALL_init �¼����ƿ� */
/*----------------------- Function Implement --------------------------------*/

static rt_err_t exception_hook(void *context);
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line);

/**
 * ϵͳ����߳�
 * @param  parameter
 */
void thread_entry_sys_monitor(void* parameter)
{
    while (1)
    {
        IWDG_Feed(); //ι��,��ֹϵͳ�쳣����
				rt_thread_mdelay(500);
    }
}


/**
 * ϵͳ��easylogger �� easyflash ���ʼ���߳�
 * @param parameter parameter
 */
void sys_init_thread(void* parameter){
	
		rt_err_t result;

    if ((nor_flash = rt_sfud_flash_probe("W25Q128", "spi20")) == NULL) { /* ��ʼ�� nor_flash W25Q128 Flash �豸 */ 
				rt_kprintf("Error! No find W25Q128!");  //16MB Flash
        return;
    }

    easyflash_init();		/* ��ʼ�� EasyFlash ģ�� */
		elog_init_start();  /* ��ʼ����־ϵͳ */
		
    rt_hw_exception_install(exception_hook);	/* ����Ӳ���쳣���� */
    rt_assert_set_hook(rtt_user_assert_hook); /* ����RTT���Թ��� */
		
    result = rt_event_init(&init_event, "event", RT_IPC_FLAG_FIFO);  /* ��ʼ���¼����� */
    if (result != RT_EOK){
        log_e("init event failed.\n");
		}
		Normal_Parameter_Init_With_Flash(); //Flash������ʼ����ȡ

		//rt_kprintf("file:%s,function:%s,line:%d\n",__FILE__,__FUNCTION__,__LINE__); //��ӡ���� �ļ������������к�
}


/* ����Ӳ���쳣���� */
static rt_err_t exception_hook(void *context) {
    extern long list_thread(void);
    uint8_t _continue = 1;
	
		ErrorStatus_LED(); //�쳣״ָ̬ʾ�� ��ɫ
    rt_enter_critical();//��ֹ����

    list_thread();      //��ӡ�߳�
    while (_continue == 1);//�쳣����
    return RT_EOK;
}


/* ����RTT���Թ��� */
static void rtt_user_assert_hook(const char* ex, const char* func, rt_size_t line) {
	
		ErrorStatus_LED();  //�쳣״ָ̬ʾ�� ��ɫ
    rt_enter_critical();//��ֹ����
    elog_async_enabled(false); //�����첽���ģʽ
    elog_a("rtt", "(%s) has assert failed at %s:%ld.", ex, func, line);//��ӡ��ɶ���C��������

    while(1);//�쳣����
}

int rt_system_init(void)
{
    rt_thread_t sys_thread;  //ϵͳ���ֳ�ʼ���߳�
		rt_thread_t monitor_thread;  //�����߳�
				 
	  monitor_thread = rt_thread_create("monitor",
																			 thread_entry_sys_monitor, 
																			 NULL,
																			 1024,
																			 30,
																			 10);
							 
    sys_thread = rt_thread_create("sys_init",
																	 sys_init_thread, 
																	 NULL,
																	 1024,
																	 5,
																	 10);
	
    if (monitor_thread != NULL) {
        rt_thread_startup(monitor_thread);
    }
		else {
		  	rt_kprintf("monitoring error!");
		}
    if (sys_thread != NULL) {
        rt_thread_startup(sys_thread);
    }
		else {
				rt_kprintf("sys init error!");
		}
    return 0;
}
INIT_DEVICE_EXPORT(rt_system_init);



void rt_hw_us_delay(u32 us)
{
    rt_uint32_t delta;
    /* �����ʱ������ tick �� */
    us = us * (SysTick->LOAD/(1000000/RT_TICK_PER_SECOND));
    /* ��õ�ǰʱ�� */
    delta = SysTick->VAL;
    /* ѭ����õ�ǰʱ�䣬ֱ���ﵽָ����ʱ����˳�ѭ�� */
    while (delta - SysTick->VAL< us);
}

void rt_hw_ms_delay(u32 ms)
{
		rt_hw_us_delay(1000 * ms);
}

void delay_us(u32 nTimer)
{
	u32 i=0;
	for(i=0;i<nTimer;i++){
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
			__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}





