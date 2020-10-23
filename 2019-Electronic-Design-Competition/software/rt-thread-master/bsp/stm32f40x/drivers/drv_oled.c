#include "oled.h"
#include "stdlib.h"
 	 
#include "init.h"
#include "oledfont.h" 	

/*---------------------- Constant / Macro Definitions -----------------------*/			   
#define OLED_CS 	PGout(9)  //δ����
#define OLED_RST  PGout(11)	
#define OLED_RS 	PGout(10)
//#define OLED_WR 	PAout(4)	 	  
//#define OLED_RD 	PDout(7)
 
//ʹ��4�ߴ��нӿ�ʱʹ�� 
#define OLED_SCLK 	PGout(13)    //D0
#define OLED_SDIN 	PGout(12) 	 //D1
		     
//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1	//8080����
//ͨ��ƴ�յķ�����OLED���һ��8λ����
//data:Ҫ���������
void OLED_Data_Out(u8 data)
{
	u16 dat=data&0X0F;
	GPIOC->ODR&=~(0XF<<6);		//���6~9
	GPIOC->ODR|=dat<<6;			//D[3:0]-->PC[9:6]
	GPIO_Write(GPIOC,dat<<6);
	PCout(11)=(data>>4)&0X01;	//D4
	PBout(6)=(data>>5)&0X01;	//D5
	PEout(5)=(data>>6)&0X01;	//D6
	PEout(6)=(data>>7)&0X01;	//D7 
} 
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	OLED_Data_Out(dat);	    
 	OLED_RS=cmd;
	OLED_CS=0;	   
	OLED_WR=0;	 
	OLED_WR=1;
	OLED_CS=1;	  
	OLED_RS=1;	 
} 	    	    
#else
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; //д���� 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
#endif
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON`	
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	

void Half_OLED_Clear(void)
{	
	u8 i,n;  
	for(i=0;i<8;i++)
			for(n=0;n<64;n++)
					OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}


//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


void OLED_ShowMyChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	//chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
  for(t=0;t<csize;t++)
  {   
		if(size==12)temp=my_font_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=my_font_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//��ʾͼƬ
//x,y:�������  
//p_w:ͼƬ����λ���أ�
//p_h:ͼƬ�ߣ���λ���أ�
//*p:ͼƬ��ʼ��ַ 
void OLED_ShowPicture(u8 x,u8 y,const u8 *p,u8 p_w,u8 p_h)
{	
	u8 temp,i,col,row;
	u8 y0=y;
	u8 width=p_w;
	if(x+p_w>128)width=128-p_w;//ʵ����ʾ���
	u8 high=p_h;
	if(y+p_h>64)high=64-p_h;//ʵ����ʾ�߶�
	u8 exp_col_bytes=(p_h/8+((p_h%8)?1:0));//��ʾһ�е��ֽ���
	u8 act_col_bytes=(high/8+((high%8)?1:0));//ʵ����ʾһ�е��ֽ���
	
	for(row=0;row<width;row++)//��++
	{
		for(col=0;col<act_col_bytes;col++)//��ʾһ��
		{   
			temp = p[col+row*exp_col_bytes];
			for(i=0;i<8;i++)
			{
				if(temp&0x80)OLED_DrawPoint(x,y,1);
				else OLED_DrawPoint(x,y,0);
				temp<<=1;
				y++;
				if((y-y0)==high)
				{
					y=y0;
					x++;
					break;
				}		
			} 
		}
	}		
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	


/*************************************************************************/
/*                
��ʾһ������                          
x,y:�������
num :�ֿ��еڼ�������
size:�����С
mode��ģʽ
*/ 
/*************************************************************************/
void OLED_ShowGBK(u8 x, u8 y,  u8 num, u8 size,u8 mode)
{
	u8 temp,t,t1;
	u8 y0=y;
	//u8 size = 16;
	u8 csize=(size/8 + ((size%8)?1:0)) * size;     //�õ�����һ���ַ���Ӧ������ռ���ֽ���
			  
	for(t=0;t<csize;t++)
	{  
		  //  ��ֻ������16��12������   û��������������
		if(size==12)      temp = gbk_1212[num][t];    //����1212����
		else if(size==16) temp = gbk_1616[num][t];    //����1616����
//      else if(size==24)temp=asc2_2412[chr][t];    //����2412����
		else return;                                //û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}    
	} 	
}
void OLED_ChineseString(uint8 x,uint8 y,uint8 First,uint8 Final,uint8 size)
{
	for(int i= First;i<=Final;i++)
	{
		OLED_ShowGBK(x+size*(i-First),y,i,size,1);
	}
}








//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	 		 
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PORTA~E,PORTGʱ��
  
#if OLED_MODE==1		//ʹ��8080����ģʽ		
	
	//GPIO��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;	
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��	
 
	
 	OLED_WR=1;
	OLED_RD=1; 
#else					//ʹ��4��SPI ����ģʽ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��
	
	OLED_SDIN=1;
	OLED_SCLK=1;	
#endif
 
	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=0;
	//rt_thread_mdelay(100);
	OLED_RST=1; 
	
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
	
}  







//��ʼ��IO ����1
//bound:������
void uart6_init(u32 bound)
{
   //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); //ʹ��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART1ʱ��

		//����1��Ӧ���Ÿ���ӳ��
		GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9����ΪUSART1
		GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10����ΪUSART1

		//USART1�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOA9��GPIOA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��PA9��PA10

		//USART1 ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//����������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
		USART_Init(USART6, &USART_InitStructure); //��ʼ������1

		USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���1 

		USART_ClearFlag(USART6, USART_FLAG_TC);
        
#if EN_USART6_RX        
		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�

		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����1�ж�ͨ��
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;                //�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
        
}



void USART6_Sends(u8 *buf,u8 len)  
{  
    u8 t;  
    for(t=0;t<len;t++)   
    {             
        USART_SendData(USART6,buf[t]);  
        while(USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);    
    }      
       
}  





















