/*
 * DataProcess.c
 *
 *  Created on: 2019��8��7��
 *      Author: zengwangfa
 *      Notes:  FDC2214���ݴ���
 */


#include "DataProcess.h"
#include "FDC2214.h"
#include <math.h>
#include <rtthread.h>
#include "filter.h"
#include "led.h"
#include "HMI.h"
#include <string.h> 
#include "ioDevices.h"
#include "my2490.h"

unsigned int CH4_DATA;//ͨ��ֵ

float ShortValue4;//��·ֵ

float Cap_Division[100]= {0};/**/
int Cap_Probability[100] ={0}; //��ſ�����

float Cap_Value[10] ={0}; //��� 10�βɼ��ĵ���ֵ



PaperCountEngine_Type Paper = {
			.Finish_Flag = 2
};/*Paper���ݽṹ��*/

float KT_Board_Value_In_Flash = 0.0f; //KT��Flash��ֵ
float Fiber_Board_Value_In_Flash = 0.0f;//��ά�� Flash��ֵ



//�ж�data�Ƿ��� value �� range��
int is_in_range(float data,float value,float range)
{
		if( (data < (value+range) && data > (value-range))){
				return 1;
		}		
		return 0;
}

void Get_Paper(void){

		for(int i = 0;i < 10;i++){
				Cap_Value[i] = get_single_capacity();//��ȡ50������
		}	
    rt_enter_critical(); /* �߳����� */
		Paper.PaperNumber = ProbablityCapacitance(Cap_Value);	//�Ƚ����ݣ���ȡ���� ֽ����
    rt_exit_critical();		 
}


/* ���� ֽ�� ��� */
void Capcity_Paper_Detection(void)
{

		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/

		/*-----------------------��·��⡢���·ֱ��return-------------------------*/
		if(1 == Short_Circuit_Detection()){//�ж���·�򲻽������ݱȶ�
				if(1 == HMI_Work_Button){
						uart_send_my2490_now_status(my2490_number_array,Paper.PaperNumber);
						Buzzer_Set(&Beep,1,1);//��������һ��
						HMI_Work_Button = 0;
				}
				uart_send_hmi_paper_numer(Paper.PaperNumber);
				rt_thread_mdelay(5); //��ֱ�ӷ����ǣ���ʱ���Է���ȡ����
				return;
		}

		Get_Paper();/* ��ȡֽ������ */

		/*-----------------------��ȡ50�����ݡ�����������-------------------------*/	

		if(1 == HMI_Work_Button){

				Buzzer_Set(&Beep,1,1);//��������һ��
				uart_send_my2490_now_status(my2490_number_array,Paper.PaperNumber);
				HMI_Work_Button = 0;
		}

		if(0 == HMI_Work_Button){//������û�а��£�һֱ��ȡ����
				
		}
			
		uart_send_hmi_paper_numer(Paper.PaperNumber);	//��������
}

/* ��ӡ�� ֽ�ż�� */
void Printer_Paper_Detection(void)
{
		Get_Paper();/* ��ȡֽ������ */

}


/* ���� ��� */
void Material_Detection(void)
{
		static int is_material = 0;
	
		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/
		if(Material_Button == 1){//����ⰴť����
				if(fabs(Paper.Capacitance-KT_Board_Value_In_Flash) < fabs(Paper.Capacitance-Fiber_Board_Value_In_Flash) )	{
						is_material = 1;
				}
				else{
						is_material = 2;
				}
				Material_Button = 0;
		}
		uart_send_hmi_is_material(is_material);//1ΪKT�壬2Ϊ��ά��

}



/* ��У׼ʱ����ȡ������ ��ֵ */
float get_single_capacity(void)
{
		static unsigned int res_CH4_DATA = 0;
		static float res_ch4 = 0.0f;
		
		FDC2214_GetChannelData(FDC2214_Channel_3, &res_CH4_DATA);
		res_ch4 = Cap_Calculate(&res_CH4_DATA);

		return res_ch4;
}



/* ��·�ж� */
int Short_Circuit_Detection(void)
{
    //�������� ��·��ֵ��Χ�仯
		//���ߵ�ֵ�ǳ�С��ʱ���ж�Ϊ�ܵ�����
		if( ((is_in_range(FDC2214_Paper_Data[0],FDC2214_Data_In_Flash[0],50.0f)) && HMI_Status_Flag == 1) \
			||((is_in_range(Paper.Capacitance,FDC2214_Data_In_Flash[0],50.0f)) && HMI_Status_Flag == 2)  )
		{
				Paper.ShortStatus = 1;//�ж���·
				Paper.PaperNumber = 0; //�����·��Ϊ0
				Bling_Set(&Light_Red,100,50,0.5,0,77,0);//������ʾ��·
		}			

		else{
				Bling_Set(&Light_Green,100,50,0.5,0,78,0);//�̵���ʾ����·
				Paper.ShortStatus = 2; //�ж�����·
		}
		uart_send_hmi_is_short(); //�����Ƿ��·
		return Paper.ShortStatus;
}		

float CapacitanceDP= 0;
/*
Cap_Division �ָ�
arrey �����ԭʼ����
Number ����
*/
void DataSubsection(float Cap_Division[],float arrey[],int Number)
{

		static int rec = 1;
		
		for(int i = 2;i < 30;i++){
				CapacitanceDP = (arrey[i-1]-arrey[i]) /2.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;

		}
		for(int i=30;i<Number;i++){
				CapacitanceDP = 30*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		if(rec==1){
				Cap_Division[0] =arrey[1]+(arrey[1]-arrey[2]) /2.0f;
				rec = 0;
		}
}

int Probability_Max = 0;
/*
CompareArrey 
*/
uint8 ProbablityCapacitance(float CompareArrey[])	//���� ��Ҫ�Ƚϵ�����
{

		memset(Cap_Probability,0,sizeof(Cap_Probability));//��յ���ֵ��������
		for(int i=0;i<=70 ;i++ ){
				for(int j=0; j<10 ;j++){
						if( (CompareArrey[j] < Cap_Division[i])  && (CompareArrey[j] >= Cap_Division[i+1])){
								Cap_Probability[i]++;
						}
				}
		}
		for(int n = 0;n < 69;n++){
				if(Cap_Probability[n] > Cap_Probability[Probability_Max]){
						Probability_Max = (n + 1);
				}
				if(Cap_Probability[0] >= 25){	//1ֽ�ŵ����⴦��
						Probability_Max = 1;
				}	
		}
		return Probability_Max;
}

