#include     <stdio.h>		/*��׼�����������*/             
#include     <stdlib.h>		/*��׼�����ⶨ��*/               
#include     <unistd.h>		/*Unix ��׼��������*/            
#include     <sys/types.h>                                 
#include     <sys/stat.h>                                  
#include     <fcntl.h>		/*�ļ����ƶ���*/                 
#include     <termios.h>		/*PPSIX �ն˿��ƶ���*/         
#include     <errno.h>		/*����Ŷ���*/                   
                                                           
int main()                                                 
{                                                          
	int fd;                                                  
                                                           
	fd = open( "/dev/ttyS0", O_RDWR);	// ʹ�ö�д��ʽ�򿪴���
	if (-1 == fd){                                           
		perror("open ttyS0");                                  
		return 0;                                              
	}                                                        
	printf("Open ttyS0 OK!\n");                              
	                                                         
	close(fd);		// �رմ���                                
	return 0;                                                
}                                                          