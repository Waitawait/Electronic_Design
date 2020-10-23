/* stty_echo.c  -  gcc -o stty_echo stty_echo.c */
#include     <stdio.h>		/*��׼�����������*/
#include     <stdlib.h>		/*��׼�����ⶨ��*/
#include     <unistd.h>		/*Unix ��׼��������*/
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>			/*�ļ����ƶ���*/
#include     <termios.h>		/*PPSIX �ն˿��ƶ���*/
#include     <errno.h>		/*����Ŷ���*/


#define STTY_DEV "/dev/ttyS0"
#define BUFF_SIZE 512

int main()
{
  int stty_fd, n;
  char buffer[BUFF_SIZE];
  struct termios opt;

  /* �򿪴����豸 */
  stty_fd = open(STTY_DEV, O_RDWR);
  if (-1==stty_fd) {
    perror("open device");
    return 0;
  }
  printf("Open device success, waiting user input ...\n");

  /* ȡ�õ�ǰ�������� */
  tcgetattr(stty_fd, &opt);
  tcflush(stty_fd, TCIOFLUSH);

  /* ���ò����� - 19200bps */
  cfsetispeed(&opt, B19200);
  cfsetospeed(&opt, B19200);

  /* ��������λ - 8λ����λ */
  opt.c_cflag &= ~CSIZE;
  opt.c_cflag |= CS8;

  /* ������żλ - ����żУ�� */
  opt.c_cflag &= ~PARENB;
  opt.c_iflag &= ~INPCK;


  /* ����ֹͣλ - 1λֹͣλ */
  opt.c_cflag &= ~CSTOPB;

  /* ���ó�ʱʱ�� - 15�� */
  opt.c_cc[VTIME] = 150;
  opt.c_cc[VMIN] = 0;

  /* ����д���豸 */
  if (0!=tcsetattr(stty_fd, TCSANOW, &opt)) {
    perror("set baudrate");
    return 0;
  }
  tcflush(stty_fd, TCIOFLUSH);

  /* ��ȡ���ݣ�ֱ�����յ�'quit'�ַ����˳� */
  while(1) {
    n = read(stty_fd, buffer, BUFF_SIZE);
    if (n<=0) {
      perror("read data");
      break;
    }
    buffer[n] = '\0';

    printf("%s", buffer);
    if (0==strncmp(buffer, "quit", 4)) {
      printf("user send quit!\n");
      break;
    }
  }
  printf("Program will exit!\n");

  close(stty_fd);
}
