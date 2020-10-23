/*
 * PID.c
 *
 *  Created on: 2019��2��30��
 *      Author: 
 *      Notes:  PID������
 */
#define LOG_TAG    "PID"
 
#include "PID.h"
#include <math.h>

/*----------------------- Variable Declarations -----------------------------*/

AllControler Total_Controller; //�ܿ�����PID
Butter_Parameter Control_Device_Div_LPF_Parameter;


/*
1ƫ���޷���־��  2�����޷���־��3���ַ����־��   4������
5����            6ƫ�        7�ϴ�ƫ�       8ƫ���޷�ֵ��
9���ַ���ƫ��ֵ��10����ֵ       11�����޷�ֵ��    12���Ʋ���Kp��
13���Ʋ���Ki��   14���Ʋ���Kd�� 15�������������  16�ϴο����������
17������޷��ȣ� 18����ֿ���ʱ�Ļ�������
*/
const float Control_Unit[18][20]=
{
  /*                                          Kp      Ki      Kd                     */
  /*1  2  3  4  5  6   7  8    9   10   11    12      13      14    15  16   17   18*/
   //
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 1000,  0.50   ,2.5000  ,1.80  ,0  ,0 , 5000,  1 ,  1 ,  1 },//Roll_Gyro;������ٶ�   1
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,500 ,0  ,0 , 1000,  0.75   ,3.5000  ,1.80  ,0  ,0 , 5000,  1 ,  1 ,  1 },//Pitch_Gyro;�������ٶ�  2
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,250 ,0  ,0 , 1000,  1.00   ,0.5000  ,0.00  ,0  ,0 , 3000,  1 ,  1 ,  1 },//Yaw_Gyro;ƫ�����ٶ�    3
		
	{0  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 1000,   4.00   ,0.0000  ,0.00  ,0  ,0 , 3000,  1 ,  1 ,  1 },//Pitch_Angle;�����Ƕ�   4
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,45  ,0  ,0 , 1000 , 5.00   ,0.0000  ,0.00  ,0  ,0 , 3000,  1 ,  1 ,  1 },//Yaw_Angle;ƫ����		   5
	{0  ,1 ,0 ,0 ,0 ,0 , 0 ,30  ,0  ,0 , 1000,   4.00   ,0.0000  ,0.00  ,0  ,0 , 3000,  1 ,  1 ,  1 },//Roll_Angle;�����      6
		
  /*                                         Kp        Ki      Kd            */
  /*1  2  3  4  5  6   7  8   9   10   11    12        13      14  15  16  17    18*/
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,400 ,0  ,0 ,1000 ,  5.0     ,0.300   ,0.1  ,0  ,0 ,5000,  1 ,  1 ,  1 },//High_Speed;���������ٶ� 7 
  {0  ,1 ,0 ,0 ,0 ,0 , 0 ,200 ,0  ,0 ,1000 ,  0.5     ,0.000   ,0    ,0  ,0 ,4000,  1 ,  1 ,  1 },//High_Position;���θ߶�λ�� 8


};

/**
  * @brief  PID_Init(PID������ʼ������)
  * @param  PID_Controler PID��������Controler_Label PID������
  * @retval None
  * @notice 
  */
void PID_Init(PID_Controler *Controler,Controler_Label Label)
{
		Controler->Err_Limit_Flag=(uint8)(Control_Unit[Label][0]);//1ƫ���޷���־
		Controler->Integrate_Limit_Flag=(uint8)(Control_Unit[Label][1]);//2�����޷���־
		Controler->Integrate_Separation_Flag=(uint8)(Control_Unit[Label][2]);//3���ַ����־
		Controler->Expect=Control_Unit[Label][3];//4����
		Controler->FeedBack=Control_Unit[Label][4];//5����ֵ
		Controler->Err=Control_Unit[Label][5];//6ƫ��
		Controler->Last_Err=Control_Unit[Label][6];//7�ϴ�ƫ��
		Controler->Err_Max=Control_Unit[Label][7];//8ƫ���޷�ֵ
		Controler->Integrate_Separation_Err=Control_Unit[Label][8];//9���ַ���ƫ��ֵ
		Controler->Integrate=Control_Unit[Label][9];//10����ֵ
		Controler->Integrate_Max=Control_Unit[Label][10];//11�����޷�ֵ
		Controler->Kp=Control_Unit[Label][11];//12���Ʋ���Kp
		Controler->Ki=Control_Unit[Label][12];//13���Ʋ���Ki
		Controler->Kd=Control_Unit[Label][13];//14���Ʋ���Ki
		Controler->Control_OutPut=Control_Unit[Label][14];//15�����������
		Controler->Last_Control_OutPut=Control_Unit[Label][15];//16�ϴο����������
		Controler->Control_OutPut_Limit=Control_Unit[Label][16];//17�ϴο����������
		Controler->Scale_Kp=Control_Unit[Label][17];
		Controler->Scale_Ki=Control_Unit[Label][18];
		Controler->Scale_Kd=Control_Unit[Label][19];  
}


void Total_PID_Init(void)
{
		PID_Init(&Total_Controller.Pitch_Angle_Control,Pitch_Angle_Controler);
		PID_Init(&Total_Controller.Pitch_Gyro_Control,Pitch_Gyro_Controler);
		PID_Init(&Total_Controller.Roll_Angle_Control,Roll_Angle_Controler);
		PID_Init(&Total_Controller.Roll_Gyro_Control,Roll_Gyro_Controler);
		PID_Init(&Total_Controller.Yaw_Angle_Control,Yaw_Angle_Controler);
		PID_Init(&Total_Controller.Yaw_Gyro_Control,Yaw_Gyro_Controler);
		PID_Init(&Total_Controller.High_Position_Control,High_Position_Controler);
		PID_Init(&Total_Controller.High_Speed_Control,High_Speed_Controler);
}

float PID_Control(PID_Controler *Controler)
{

		/*******ƫ�����*********************/
		Controler->Last_Err = Controler->Err;//�����ϴ�ƫ��
		Controler->Err = Controler->Expect-Controler->FeedBack;//������ȥ�����õ�ƫ��
		if(Controler->Err_Limit_Flag == 1)//ƫ���޷��ȱ�־λ
		{
			if(Controler->Err >= Controler->Err_Max)   Controler->Err = Controler->Err_Max;
			if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
		}
		/*******���ּ���*********************/
		if(Controler->Integrate_Separation_Flag==1)//���ַ����־λ
		{
			if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
					Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
		}
		else
		{
			Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
		}
		/*******�����޷�*********************/
		if(Controler->Integrate_Limit_Flag == 1)//�������Ʒ��ȱ�־
		{
			if(Controler->Integrate >= Controler->Integrate_Max)
				Controler->Integrate = Controler->Integrate_Max;
			if(Controler->Integrate <= -Controler->Integrate_Max)
				Controler->Integrate = -Controler->Integrate_Max ;
		}
		/*******���������*********************/
		Controler->Last_Control_OutPut = Controler->Control_OutPut;//���ֵ����
		Controler->Control_OutPut = 
																  Controler->Scale_Kp * Controler->Kp * Controler->Err//����
																+ Controler->Integrate//����
																+ Controler->Kd * (Controler->Err - Controler->Last_Err);//΢��
		/*******������޷�*********************/
		if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
			Controler->Control_OutPut = Controler->Control_OutPut_Limit;
		if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
			Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
		/*******���������*********************/
		
		return Controler->Control_OutPut;
}

float PID_Control_Yaw(PID_Controler *Controler)
{

		/*******ƫ�����*********************/
		Controler->Last_Err = Controler->Err;//�����ϴ�ƫ��
		Controler->Err = Controler->Expect - Controler->FeedBack;//������ȥ�����õ�ƫ��  FeedBack
		/***********************ƫ����ƫ���+-180����*****************************/
		//if(Controler->Err < -180)  Controler->Err = Controler->Err + 360;
		//if(Controler->Err > 180)  Controler->Err = Controler->Err - 360;
		
		if(Controler->Err_Limit_Flag == 1)//ƫ���޷��ȱ�־λ
		{
			if(Controler->Err >= Controler->Err_Max)   Controler->Err =  Controler->Err_Max;
			if(Controler->Err <= -Controler->Err_Max)  Controler->Err = -Controler->Err_Max;
		}
		/*******���ּ���*********************/
		if(Controler->Integrate_Separation_Flag == 1)//���ַ����־λ
		{
				if(fabs(Controler->Err) <= Controler->Integrate_Separation_Err)
				Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
		}
		else{
				Controler->Integrate += Controler->Scale_Ki * Controler->Ki * Controler->Err;
		}
		
		/*******�����޷�*********************/
		if(Controler->Integrate_Limit_Flag==1)//�������Ʒ��ȱ�־
		{
				if(Controler->Integrate >= Controler->Integrate_Max)
						Controler->Integrate = Controler->Integrate_Max;
				if(Controler->Integrate <= -Controler->Integrate_Max)
						Controler->Integrate = -Controler->Integrate_Max ;
		}
		/*******���������*********************/
		Controler->Last_Control_OutPut = Controler->Control_OutPut;//���ֵ����   
		Controler->Control_OutPut = 
																	Controler->Scale_Kp * Controler->Kp * Controler->Err//����  λ��ʽ
																+ Controler->Integrate														//����
																+ Controler->Kd * (Controler->Err - Controler->Last_Err);//΢��
		/*******������޷�*********************/
		if(Controler->Control_OutPut >= Controler->Control_OutPut_Limit)
				Controler->Control_OutPut = Controler->Control_OutPut_Limit;
		if(Controler->Control_OutPut <= -Controler->Control_OutPut_Limit)
				Controler->Control_OutPut = -Controler->Control_OutPut_Limit;
		/*******���������*********************/
		return Controler->Control_OutPut;
}


float PID_Control_Div_LPF(PID_Controler *Controler)
{
		uint8  i=0;
		float tempa,tempb,tempc,max,min;//���ڷ������˲�
		/*******ƫ�����*********************/
		Controler->Last_Err=Controler->Err;//�����ϴ�ƫ��
		Controler->Err=Controler->Expect-Controler->FeedBack;//������ȥ�����õ�ƫ��
		Controler->Dis_Err=Controler->Err-Controler->Last_Err;//ԭʼ΢��
		
		/******************************************/
		//��ֵ�˲�����֤�õ����ݲ����䣬����������Ծʱ��΢������쳣
		tempa=Controler->Pre_Last_Dis_Err_LPF;
		tempb=Controler->Last_Dis_Err_LPF;
		tempc=Controler->Dis_Err;
		max = tempa > tempb ? tempa:tempb;
		max = max > tempc ? max:tempc;
		min = tempa < tempb ? tempa:tempb;
		min = min < tempc ? min:tempc;
		if(tempa > min && tempa < max)    Controler->Dis_Err = tempa;
		if(tempb > min  && tempb < max )  Controler->Dis_Err = tempb;
		if(tempc > min  &&  tempc < max)  Controler->Dis_Err = tempc;
		Controler->Pre_Last_Dis_Err_LPF = Controler->Last_Dis_Err_LPF;
		Controler->Last_Dis_Err_LPF = Controler->Dis_Err;
		/*****************************************/
		
		for(i=4;i>0;i--)//���ֵ�ͨ��΢�����
		{
			Controler->Dis_Error_History[i]=Controler->Dis_Error_History[i-1];
		}
		Controler->Dis_Error_History[0]=Control_Device_LPF(Controler->Dis_Err,
																											 &Controler->Control_Device_LPF_Buffer,
																											 &Control_Device_Div_LPF_Parameter);//������˹��ͨ��õ���΢����,20hz
		
		if(Controler->Err_Limit_Flag==1)//ƫ���޷��ȱ�־λ
		{
			if(Controler->Err>=Controler->Err_Max)   Controler->Err= Controler->Err_Max;
			if(Controler->Err<=-Controler->Err_Max)  Controler->Err=-Controler->Err_Max;
		}
		/*******���ּ���*********************/
		if(Controler->Integrate_Separation_Flag==1)//���ַ����־λ
		{
			if(my_abs(Controler->Err)<=Controler->Integrate_Separation_Err)
				Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err;
		}
		else
		{
				Controler->Integrate+=Controler->Scale_Ki*Controler->Ki*Controler->Err;
		}
		/*******�����޷�*********************/
		if(Controler->Integrate_Limit_Flag==1)//�������Ʒ��ȱ�־
		{
			if(Controler->Integrate>=Controler->Integrate_Max)
				Controler->Integrate=Controler->Integrate_Max;
			if(Controler->Integrate<=-Controler->Integrate_Max)
				Controler->Integrate=-Controler->Integrate_Max ;
		}
		/*******���������*********************/
		Controler->Last_Control_OutPut=Controler->Control_OutPut;//���ֵ����
		Controler->Control_OutPut=Controler->Scale_Kp*Controler->Kp*Controler->Err//����
			+Controler->Integrate//����
				//+Controler->Kd*Controler->Dis_Err;//΢��
				+Controler->Kd*Controler->Dis_Error_History[0];//΢������Դ�ڰ�����˹��ͨ�˲���
		/*******������޷�*********************/
		if(Controler->Control_OutPut>=Controler->Control_OutPut_Limit)
			Controler->Control_OutPut=Controler->Control_OutPut_Limit;
		if(Controler->Control_OutPut<=-Controler->Control_OutPut_Limit)
			Controler->Control_OutPut=-Controler->Control_OutPut_Limit;
		/*******���������*********************/
		return Controler->Control_OutPut;
}



float Control_Device_LPF(float curr_inputer,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
{
		/* ���ٶȼ�Butterworth�˲� */
		/* ��ȡ����x(n) */
		Buffer->Input_Butter[2]=curr_inputer;
		/* Butterworth�˲� */
		Buffer->Output_Butter[2]=
			Parameter->b[0] * Buffer->Input_Butter[2]
				+Parameter->b[1] * Buffer->Input_Butter[1]
		+Parameter->b[2] * Buffer->Input_Butter[0]
						-Parameter->a[1] * Buffer->Output_Butter[1]
							-Parameter->a[2] * Buffer->Output_Butter[0];
		/* x(n) ���б��� */
		Buffer->Input_Butter[0]=Buffer->Input_Butter[1];
		Buffer->Input_Butter[1]=Buffer->Input_Butter[2];
		/* y(n) ���б��� */
		Buffer->Output_Butter[0]=Buffer->Output_Butter[1];
		Buffer->Output_Butter[1]=Buffer->Output_Butter[2];
		return (Buffer->Output_Butter[2]);
}
