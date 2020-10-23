/*********************************************************************************
*      notes.c    ���Աʼ�
*                                     	  0.log_a[A]������(Assert)        
* File          : notes.c             	  1.log_e[E]������(Error)
* Version       : V1.0          		  	  2.log_w[W]������(Warn)
* Author        : zengwangfa				   		3.log_i[I]����Ϣ(Info)
																					4.log_d[D]������(Debug)
* History       :													5.log_v[V]����ϸ(Verbose)
* Date          : 2019.01.25							����log�����ȼ��������
*******************************************************************************/

/*

����ʼǣ�
  1.�ٵ�����ֵ=1�������붼���£�Ϊ����ģʽ
����ģʽ��
				//��������ͷ
				����LEDϵͳ���� ��˸ָʾ�ƣ���LED Լ0.5s��˸��
				�������� ������������ָʾ

    �ڵ�����ֵ!=1 Ϊ ����ģʽ
����ģʽ��
				�ر�����ģʽ�µļ�������
				
------------------------------------------------------		

	2.FLASH��ȡ�� ��ͨ������PID����
	

void test_env(void) {
    uint32_t i_boot_times = NULL;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    // get the boot count number from Env 
    c_old_boot_times = ef_get_env("boot_times");
    RT_ASSERT(c_old_boot_times);
    i_boot_times = atol(c_old_boot_times);//atoi
    // boot count +1 
    i_boot_times ++;
    rt_kprintf("The system now boot %d times\n", i_boot_times);
    // interger to string 
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    // set and store the boot count number to Env 
    ef_set_env("boot_times", c_new_boot_times);
    ef_save_env();
}







Notes:

�ص㣺���뻷����Ϊc99���ɰ汾Keil5ϵͳĬ��Ϊc89��c89����ֻ�ܽ�������ں�������ǰ��

������c99������
				Options -> C/C++ -> Misc Controls ->�������롾--c99�� ->���漴��



1.JY901 ��λ������Ư��

�� ʹ��ǰ��ҪУ׼
�� У׼ʱ��������Χ�ų�����
�� ʹ��ǰ ��Ҫ�Ѳ����ʵ���115200
ʹ�ó�������ָ��:
		http://wiki.wit-motion.com/doku.php?id=wt901��������



2.RT-Thread�����������ͣ�

sample:
		//���ݴ����string��       ��ΪRT-Thread rt_kprintf()�����޷���������ͣ�����ֽ����ݴ����String�ͷ���.
		char str[100];
		sprintf(str,"Time:20%d-%d-%d %d:%d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		rt_kprintf(str);




3.W25Q128 ID��ȡ����ȷ��

SPI��ʼ������ʱ:
		< SPI_BaudRatePrescaler_64 >  -> ���岨����Ԥ��Ƶ��ֵ  ����̫��Ҳ����̫С.



4.�ڴ�ռ�ü�¼��

Code�Ǵ���ռ�õĿռ�;
RO-data�� Read Only ֻ�������Ĵ�С����const��;
RW-data�ǣ�Read Write�� ��ʼ���˵Ŀɶ�д�����Ĵ�С;
ZI-data�ǣ�Zero Initialize�� û�г�ʼ���Ŀɶ�д�����Ĵ�С��ZI-data���ᱻ������������Ϊ���ᱻ��ʼ��;


Program Size: Code=157602 RO-data=27178 RW-data=1684 ZI-data=105348    ��2019.3.4��


��2019.3.9��
    Total RO  Size (Code + RO Data)               190236 ( 185.78kB)
    Total RW  Size (RW Data + ZI Data)            108336 ( 105.80kB)
    Total ROM Size (Code + RO Data + RW Data)     190652 ( 186.18kB)

��2019.3.23��
		Total RO  Size (Code + RO Data)               194360 ( 189.80kB)
		Total RW  Size (RW Data + ZI Data)            108392 ( 105.85kB)
		Total ROM Size (Code + RO Data + RW Data)     194788 ( 190.22kB)
	
��2019.4.1��	
		Total RO  Size (Code + RO Data)               199912 ( 195.23kB)
    Total RW  Size (RW Data + ZI Data)            108880 ( 106.33kB)
    Total ROM Size (Code + RO Data + RW Data)     200360 ( 195.66kB)
		
��2019.4.3��
    Total RO  Size (Code + RO Data)               201512 ( 196.79kB)
    Total RW  Size (RW Data + ZI Data)            109024 ( 106.47kB)
    Total ROM Size (Code + RO Data + RW Data)     201964 ( 197.23kB)
		
��2019.4.18��		
    Total RO  Size (Code + RO Data)               168260 ( 164.32kB)
    Total RW  Size (RW Data + ZI Data)             26048 (  25.44kB)
    Total ROM Size (Code + RO Data + RW Data)     168664 ( 164.71kB)

��2019.4.21��
    Total RO  Size (Code + RO Data)               169468 ( 165.50kB)
    Total RW  Size (RW Data + ZI Data)             26200 (  25.59kB)
    Total ROM Size (Code + RO Data + RW Data)     169888 ( 165.91kB)
		
��2019.05.05��	
		Total RO  Size (Code + RO Data)               188180 ( 183.77kB)
    Total RW  Size (RW Data + ZI Data)             27120 (  26.48kB)
    Total ROM Size (Code + RO Data + RW Data)     188648 ( 184.23kB)
		
*/

