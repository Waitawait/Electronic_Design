/*
 * camera.c
 *
 *  Created on: 2019��2��30��
 *      Author: zengwangfa
 *      Notes:  camera�豸
 */
#define LOG_TAG    "camera"

#include "led.h"
#include "key.h"
#include "drv_ov2640.h" 
#include "drv_dcmi.h" 
#include <rtthread.h>
#include <elog.h>
#include <string.h>
#include <stdlib.h>
#include <elog.h>

/*---------------------- Constant / Macro Definitions -----------------------*/

#define jpeg_buf_size 20 * 1024   //31*1024  ����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)

/*----------------------- Variable Declarations -----------------------------*/
/* ALL_init �¼����ƿ�. */
extern struct rt_event init_event;
extern rt_device_t debug_uart_device;	

u8 ov2640_mode=1;						//����ģʽ:0,RGB565ģʽ;1,JPEGģʽ

__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG���ݻ���buf
volatile u32 jpeg_data_len=0; 	//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;			//JPEG���ݲɼ���ɱ�־ 
																//0,����û�вɼ���;
																//1,���ݲɼ�����,���ǻ�û����;
																//2,�����Ѿ����������,���Կ�ʼ��һ֡����
/*----------------------- Function Implement --------------------------------*/

//JPEG�ߴ�֧���б�
const u16 jpeg_img_size_tbl[][2]=
{
		176,144,	//QCIF
		160,120,	//QQVGA
		352,288,	//CIF
		320,240,	//QVGA
		640,480,	//VGA
		800,600,	//SVGA
		1024,768,	//XGA
		1280,1024,	//SXGA
		1600,1200,	//UXGA
}; 

//��Ч����
enum OV_Effect{ 
		OV_Normal,  //0:��ͨģʽ 
		OV_Negative,//1,��Ƭ
		OV_BW,			//2,�ڰ� 
		OV_Redish,  //3,ƫ��ɫ
		OV_Greenish,//4,ƫ��ɫ
		OV_Bluish,	//5,ƫ��ɫ
		OV_Antique  //6,����	
};

//��С����
enum OV_Size{ 
		OV_QCIF,  
		OV_QQVGA,
		OV_CIF,		
		OV_QVGA,  
		OV_VGA,
		OV_SVGA,	
		OV_XGA,
		OV_SXGA,
		OV_UXGA  //6,����	
};
const char *EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 
const char *JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEGͼƬ 9�ֳߴ� 


//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
		if(ov2640_mode)//ֻ����JPEG��ʽ��,����Ҫ������.
		{
				if(0 == jpeg_data_ok)	//jpeg���ݻ�δ�ɼ���
				{	
						DMA_Cmd(DMA2_Stream1, DISABLE);//ֹͣ��ǰ���� 
						while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������  
						jpeg_data_len = jpeg_buf_size - DMA_GetCurrDataCounter(DMA2_Stream1);//�õ��˴����ݴ���ĳ���

						jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
				}
				if(2 == jpeg_data_ok)	//��һ�ε�jpeg�����Ѿ���������
				{
						DMA2_Stream1->NDTR=jpeg_buf_size;	
						DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//���䳤��Ϊjpeg_buf_size*4�ֽ�
						DMA_Cmd(DMA2_Stream1, ENABLE);			//���´���
						jpeg_data_ok=0;										  //�������δ�ɼ�
				}
		}
} 

//JPEG����,ͨ��WIFI���͸�����.
void camera_thread_entry(void* paramter)
{
	  u32 i,jpgstart,jpglen; 
		u8 headok=0;
		u8 *p;
		//u8 effect=0,saturation=2,contrast=2;
		u8 size=1;			//Ĭ����QVGA 320*240�ߴ�

		log_v("JPEG Size:%s ",JPEG_SIZE_TBL[size]);
		
		OV2640_JPEG_Mode();		//JPEGģʽ
		My_DCMI_Init();			//DCMI����
		DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI DMA����   
		OV2640_OutSize_Set(jpeg_img_size_tbl[size][0],jpeg_img_size_tbl[size][1]);//��������ߴ� 
		DCMI_Start(); 		//��������
	
		while(1)
		{
	
				if(1 == jpeg_data_ok)	//�Ѿ��ɼ���һ֡ͼ����
				{
				  	p = (u8*)jpeg_buf;
						jpglen = 0;	//����jpg�ļ���СΪ0
						headok = 0;	//���jpgͷ���
						for(i = 0;i < jpeg_data_len*4;i++)	//dma����1�ε���4�ֽ�,���Գ���4.
						{
								if((p[i]==0xFF)&&(p[i+1]==0xD8))//�ҵ�FF D8
								{
										jpgstart = i;
										headok = 1;	//����ҵ�jpgͷ(FF D8)
								}
								if((p[i]==0xFF)&&(p[i+1]==0xD9)&&headok)//�ҵ�ͷ�Ժ�,����FF D9
								{
										jpglen = i - jpgstart + 2;
										//rt_kprintf("jpglen:%d\n",jpglen);
										break;
								}
						} 
						
						if(jpglen)	//������jpeg���� 
						{
								p += jpgstart;			  //ƫ�Ƶ�0XFF,0XD8�� 
								for(i = 0;i < jpglen;i++)	//��������jpg�ļ�
								{
//										while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);	//ѭ������,ֱ���������  		
//										USART_SendData(USART3,p[i]); 		
								}  
						}	
						jpeg_data_ok=2;	//���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��.
				}
				rt_thread_mdelay(1);
	
		}    
} 



int camera_thread_init(void)
{
		static u8 ErrorCount = 0;//�������  ���ڷ�ֹ����ͷ��ʼ������
    rt_thread_t camera_tid;
		/*������̬�߳�*/
    camera_tid = rt_thread_create("camera",	 //�߳�����
                    camera_thread_entry,		 //�߳���ں�����entry��
                    RT_NULL,							   //�߳���ں���������parameter��
                    512,										 //�߳�ջ��С����λ���ֽڡ�byte��
                    20,										 	 //�߳����ȼ���priority��
                    10);										 //�̵߳�ʱ��Ƭ��С��tick��= 100ms

    if (camera_tid != RT_NULL){
				if(boma_value_get() == System_NORMAL_STATUS)//����ģʽ�� �����û��OV-Camera
				{
						while(OV2640_Init() && ErrorCount < 1)//��ʼ��OV2640
						{
								ErrorCount ++;
								log_e("OV2640_Init_Error\r\n");
						}
								log_i("OV2640_Init()");
								rt_event_send(&init_event, CAM_EVENT);
								rt_thread_startup(camera_tid);
				}
				else {log_w("Not Open OV2640 Camera!");}
		}
		return 0;
}
INIT_APP_EXPORT(camera_thread_init);




/* OV2640 ��Ч���� */
static int ov2640_set_effect(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
			log_e("Proper Usage: ov2640_set_effect bw / normal");//�÷�: ���á��ڰס�����ͨ��
				result = -RT_ERROR;
        goto _exit;
    }

		if( !strcmp(argv[1],"normal") ){ //����Ϊ��ͨ
				OV2640_Special_Effects(OV_Normal);//������Ч
				log_i("OV_2640 Effcet: normal");
		}
		else if( !strcmp(argv[1],"bw") ){ //���úڰ�
				OV2640_Special_Effects(OV_BW);//������Ч
				log_i("OV_2640 Effcet: B&W");
		}
		else {
				log_e("Proper Usage: ov2640_set_effect bw / normal");
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(ov2640_set_effect,ov2640 set effect [bw / normal]);


/* OV2640 �ֱ������� */
static int ov2640_set_size(int argc,char **argv)
{
		int result = 0;
    if (argc != 2){
				log_e("Proper Usage: ov2640_set_size 0/1/2/3/4/5");//�÷�: ���÷ֱ���
				rt_kprintf("0.QCIF:176*144\n1.QQVGA:160*120\n2.CIF:352*288\n3.QVGA:320*240\n4.VGA:640*480\n5.SVGA:800*600\n");//�÷�: ���÷ֱ���
				result = -RT_ERROR;
        goto _exit;
    }

		if( atoi(argv[1]) < 6 ){ //����Ϊ��ͨ
				OV2640_OutSize_Set(jpeg_img_size_tbl[atoi(argv[1])][0],jpeg_img_size_tbl[atoi(argv[1])][1]);//��������ߴ� 
				log_i("Success! OV_2640 Size: %s",JPEG_SIZE_TBL[atoi(argv[1])]);
		}
		else {
				log_e("Proper Usage: ov2640_set_size 0/1/2/3/4/5");
				rt_kprintf("0.QCIF:176*144\n1.QQVGA:160*120\n2.CIF:352*288\n3.QVGA:320*240\n4.VGA:640*480\n5.SVGA:800*600\n");//�÷�: ���÷ֱ���
		}
_exit:
    return result;
}
MSH_CMD_EXPORT(ov2640_set_size,ov2640 set size [QQVGA...]);


