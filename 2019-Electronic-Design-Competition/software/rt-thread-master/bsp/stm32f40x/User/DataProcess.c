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
#include "flash.h"
#include "ioDevices.h"
#include "nbiot.h"

unsigned int CH4_DATA;//ͨ��ֵ

float ShortValue4;//��·ֵ

float Cap_Division[110]= {0};/**/

float Cap_Value[10] ={0}; //��� 10�βɼ��ĵ���ֵ

int Cap_Probability[110] ={0}; //��ſ�����

PaperCountEngine_Type Paper = {
			.Finish_Flag = 2
};/*Paper���ݽṹ��*/

float KT_Board_Value_In_Flash = 0.0f; //KT��Flash��ֵ
float Fiber_Board_Value_In_Flash = 0.0f;//��ά�� Flash��ֵ

int RMB_Value = 0;

Div_Parameter_Type Div_Parameter = {30,30,30,30,30,30,30};//����ֶε�ϵ��
Money_Type Money;

int Level = 0;//�ȼ� 0~100

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
    rt_enter_critical(); /* ���������� */
		Paper.PaperNumber = ProbablityCapacitance(Cap_Value);	//�Ƚ����ݣ���ȡ���� ֽ����
    rt_exit_critical();	 /* ���������� */	 
}


/* ���� ֽ�� ��� */
void Capcity_Paper_Detection(void)
{
		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/

		/*-----------------------��·��⡢���·ֱ��return-------------------------*/
		if(0x01 == Short_Circuit_Detection()){//�ж���·�򲻽������ݱȶ�
				if(0x01 == HMI_Work_Button){
						uart_send_my2490_now_status(my2490_number_array,Paper.PaperNumber);
						Buzzer_Set(&Beep,1,1);//��������һ��
						HMI_Work_Button = 0;
						Nb_Iot_Send_data(Paper.PaperNumber,(uint16)Paper.Capacitance,Paper.ShortStatus);
				}
				
				uart_send_hmi_paper_numer(Paper.PaperNumber);
				rt_thread_mdelay(5); //��ֱ�ӷ����ǣ���ʱ���Է���ȡ����
				return;
		}
		
		/*-----------------------��ȡ50�����ݡ�����������-------------------------*/	


		if(0x01 == HMI_Work_Button){
				Buzzer_Set(&Beep,1,1);//��������һ��
				Get_Paper();/* ��ȡֽ������ */
				Get_Paper();/* ��ȡֽ������ */
				uart_send_hmi_paper_numer(Paper.PaperNumber);	//��������
				uart_send_my2490_now_status(my2490_number_array,Paper.PaperNumber);

				rt_thread_mdelay(200);
				uart_send_hmi_paper_numer(Paper.PaperNumber);	//��������
				HMI_Work_Button = 0;
				Nb_Iot_Send_data(Paper.PaperNumber,(uint16)Paper.Capacitance,Paper.ShortStatus);//NB-IOT������Ϣ
		}

		else if( (0x02 == HMI_Work_Button )) {//����־λΪ2���� ʵʱ���
				Get_Paper();/* ��ȡֽ������ */
				uart_send_hmi_paper_numer(Paper.PaperNumber);	//��������
		}
			

}

/* ��ӡ�� ֽ�ż�� */
void Printer_Paper_Detection(void)
{

		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/
		if(Short_Circuit_Detection() != 1){
				Get_Paper();/* ��ȡֽ������ */	
		}
	

	
		if(Paper.PaperNumber < 5 ){//С��5��һֱ����
				Buzzer_Set(&Beep,1,1);
		}
		rt_thread_mdelay(100);
		uart_send_hmi_paper_numer(Paper.PaperNumber);	//��������

}

int is_material = 0;
/* ���� ��� */
void Material_Detection(void)
{

		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/
		if(0x01 == Material_Debug_Write_Button){
				KT_Board_Value_In_Flash = Paper.Capacitance;
		}
		else if(0x02 == Material_Debug_Write_Button){
				Fiber_Board_Value_In_Flash = Paper.Capacitance;
		}
		if(0x00 != Material_Debug_Write_Button){
				Buzzer_Set(&Beep,1,1);		
				Flash_Update();				
		}
		Material_Debug_Write_Button = 0;
		
		if(Material_Button == 1){//����ⰴť����
				Buzzer_Set(&Beep,1,1);//��������һ��
				if( is_in_range(Paper.Capacitance,KT_Board_Value_In_Flash,15) ){//�Ƚϸ�����˭
						is_material = 1; //KT��
				}
				else if( is_in_range(Paper.Capacitance,Fiber_Board_Value_In_Flash,15) ) {
						is_material = 2; //��ά��
				}
				else{
					  is_material = 0; //��
				}
				Material_Button = 0;
		}
		if(1 == Short_Circuit_Detection()){
				is_material = 0; //��
		}
		uart_send_hmi_is_material(is_material);//1ΪKT�壬2Ϊ��ά��
		rt_thread_mdelay(5);

}




/* ֽ�� ��� */
void Money_Detection(void)
{

	
		Paper.Capacitance = get_single_capacity();/* ��ȡ���� ����ֵ*/
		if(0x01 == Money_Debug_Write_Button){
				Money.RMB_100 = Paper.Capacitance;
		}
		else if(0x02 == Money_Debug_Write_Button){
				Money.RMB_50 = Paper.Capacitance;
		}
		else if(0x03 == Money_Debug_Write_Button){
				Money.RMB_10 = Paper.Capacitance;
		}
		else if(0x04 == Money_Debug_Write_Button){
				Money.RMB_5 = Paper.Capacitance;
		}
		if(0x00 != Money_Debug_Write_Button){
				Flash_Update();				
				Buzzer_Set(&Beep,1,1);		
		}
		Money_Debug_Write_Button = 0;
		if(Money_Button == 1){//����ⰴť���� 
				Buzzer_Set(&Beep,1,1);//��������һ��
				if( is_in_range(Paper.Capacitance,Money.RMB_100,40) )	{//�Ƚϸ�����˭
						RMB_Value = 1; //100Ԫ
				}
				else if( is_in_range(Paper.Capacitance,Money.RMB_50,40) ) {
						RMB_Value = 2; //50Ԫ
				}
				else if( is_in_range(Paper.Capacitance,Money.RMB_10,40) ) {
						RMB_Value = 3; //50Ԫ
				}
				else if( is_in_range(Paper.Capacitance,Money.RMB_5,40) ) {
						RMB_Value = 4; //50Ԫ
				}
				else{
					  RMB_Value = 0; //��
				}
				Money_Button = 0;
		}
		
		if(Short_Circuit_Detection() == 1){
				RMB_Value = 0; //��
		}
		uart_send_hmi_is_money(RMB_Value);//1ΪKT�壬2Ϊ��ά��	
		rt_thread_mdelay(5);


}

/**
  * @brief  get_single_capacity ��ȡ������ֵ
  * @param  None
  * @retval ����ֵ
  * @notice 
  */
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
		if( //((is_in_range(FDC2214_Paper_Data[0],FDC2214_Data_In_Flash[0],50.0f)) && HMI_Status_Flag == 1) \
			//||((is_in_range(Paper.Capacitance,FDC2214_Data_In_Flash[0],50.0f)) && HMI_Status_Flag == 2) 
			//||((is_in_range(Paper.Capacitance,FDC2214_Data_In_Flash[0],50.0f)) && HMI_Status_Flag == 4) )
				(FDC2214_Paper_Data[0] <= (FDC2214_Data_In_Flash[0]+50) && HMI_Status_Flag == 1)  \
				 || (Paper.Capacitance <= (FDC2214_Data_In_Flash[0]+50) && HMI_Status_Flag == 2) \
				 || (Paper.Capacitance <= (FDC2214_Data_In_Flash[0]+50) && HMI_Status_Flag == 3) 
		
		)
		{
				Paper.ShortStatus = 0x01;//�ж���·
				Paper.PaperNumber = 0; //�����·��Ϊ0
				Bling_Set(&Light_Red,100,50,0.5,0,77,0);//�����ʾ��·
		}			
		else{
				Bling_Set(&Light_Green,100,50,0.5,0,78,0);//�̵���ʾ����·
				Paper.ShortStatus = 0x02; //�ж�����·
		}
		uart_send_hmi_is_short(); //�����Ƿ��·
		return Paper.ShortStatus;
}		

float CapacitanceDP= 0;

/**
  * @brief  DataSubsection ����ֵ�ָ��ȡ����
  * @param  Cap_Division�����зֺ��ֵ��arrey���ȴ���ĵ���ֵ
  * @retval 
  * @notice 
  */
void DataSubsection(float Cap_Division[],float arrey[])
{

		static int rec = 1;
		
		for(int i = 2;i <= 30;i++){
				CapacitanceDP = (arrey[i-1]-arrey[i]) /2.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;

		}
		for(int i=31;i<=40;i++){
				CapacitanceDP = Div_Parameter.Div_30_40*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		for(int i=41;i<=50;i++){
				CapacitanceDP = Div_Parameter.Div_40_50*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		
		for(int i=51;i<=60;i++){
				CapacitanceDP = Div_Parameter.Div_50_60*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		for(int i=61;i<=70;i++){
				CapacitanceDP = Div_Parameter.Div_60_70*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		for(int i=71;i<=80;i++){
				CapacitanceDP = Div_Parameter.Div_70_80*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}
		for(int i=81;i<=90;i++){
				CapacitanceDP = Div_Parameter.Div_80_90*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}				
		for(int i=91;i<=100;i++){
				CapacitanceDP = Div_Parameter.Div_90_100*(arrey[i-1]-arrey[i]) /100.0f;
				Cap_Division[i-1]= arrey[i-1]-CapacitanceDP;
		}				
		if(rec==1){
				Cap_Division[0] =arrey[1]+(arrey[1]-arrey[2]) /2.0f;
				rec = 0;
		}
}

int Probability_Max = 0;
/**
  * @brief  ProbablityCapacitance ���ݷָ������ȡ���ܵ�ֽ������
  * @param  50�����ֵ��ַ
  * @retval ����ֵ
  * @notice �����ٷֱȷ��ض�Ӧ��ֽ���������ֻ�ǿ���ֵ
  */
uint8 ProbablityCapacitance(float CompareArrey[])	//���� ��Ҫ�Ƚϵ�����
{

		memset(Cap_Probability,0,sizeof(Cap_Probability));//��յ���ֵ��������
	
		for(int i=0;i <= Level;i++ ){
				for(int j=0; j<10 ;j++){
						if( (CompareArrey[j] < Cap_Division[i])  && (CompareArrey[j] >= Cap_Division[i+1])){
								Cap_Probability[i]++;
						}
				}
		}
		for(int n = 0;n < Level ;n++){
				if(Cap_Probability[n] > Cap_Probability[Probability_Max]){
						Probability_Max = (n + 1);
				}
				if(Cap_Probability[0] >= 5){	//ֽ���������⴦��
						Probability_Max = 1;
				}	
		}
		
		if(0 == Cap_Probability[Probability_Max-1]){
				Probability_Max = 0;
		}
		return Probability_Max;
}

