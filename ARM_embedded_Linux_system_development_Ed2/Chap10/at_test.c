/* at_test.c  -  gcc -o at_test at_test.c */
#include <stdio.h>		/*��׼�����������*/
#include <stdlib.h>		/*��׼�����ⶨ��*/
#include <unistd.h>		/*Unix ��׼��������*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>		/*�ļ����ƶ���*/
#include <termios.h>		/*PPSIX �ն˿��ƶ���*/
#include <errno.h>		/*����Ŷ���*/
#include <iconv.h>


#define STTY_DEV "/dev/ttyS0"
#define BUFF_SIZE 512

int SetOption(int fd)
{
  struct termios opt;

  /* ȡ�õ�ǰ�������� */
  tcgetattr(fd, &opt);
  tcflush(fd, TCIOFLUSH);

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
  if (0!=tcsetattr(fd, TCSANOW, &opt)) {
    perror("set baudrate");
    return -1;
  }
  tcflush(fd, TCIOFLUSH);
  return 0;
}

int main()
{
  int stty_fd, n;
  iconv_t cd;
  char buffer[BUFF_SIZE];

  char phone[20] = "+8618912345678";			// �����ֻ�����
  char sms_number[20] = "+8613800200500";		// �������Ϣ���ĺ���
  char sms_gb2312[140] = "������죡";			// �������Ϣ����Ŷ
  char sms_utf8[140];
  char *sms_in = sms_gb2312;
  char *sms_out = sms_utf8;
  int str_len, i, tmp;
  int gb2312_len, utf8_len;

  /* �򿪴����豸 */
  stty_fd = open(STTY_DEV, O_RDWR);
  if (-1==stty_fd) {
    perror("open device");
    return 0;
  }
  printf("Open device success!\n");

  /* ���ô��ڲ��� */
  if (0!=SetOption(stty_fd)) {
    close(stty_fd);
    return 0;
  }

  /* ת���绰�� */
  if (phone[0] = '+') {// ȥ�����뿪ͷ��'+'
    for ( i=0; i<strlen(phone)-1; i++ )
      phone[i] = phone[i+1];
  }
  phone[i] = '\0';

  str_len = strlen(phone);
  if ((strlen(phone)%2)!=0) {					// ������볤�����������ں�����ַ�'F'
    phone[str_len] = 'F';
    phone[str_len+1] = '\0';
  }

  for (i=0;i<strlen(phone);i+=2) {					//�Ѻ������żλ����
    tmp = phone[i];
    phone[i] = phone[i+1];
    phone[i+1] = tmp;
  }


  /* ת������Ϣ���ĺ��� */
  if (sms_number[0] = '+') {// ȥ�����뿪ͷ��'+'
    for ( i=0; i<strlen(sms_number)-1; i++ )
      sms_number[i] = sms_number[i+1];
  }
  sms_number[i] = '\0';

  str_len = strlen(sms_number);
  if ((strlen(sms_number)%2)!=0) {				// ������볤�����������ں�����ַ�'F'
    sms_number[str_len] = 'F';
    sms_number[str_len+1] = '\0';
  }

  for (i=0;i<strlen(sms_number);i+=2) {			//�Ѻ������żλ����
    tmp = sms_number[i];
    sms_number[i] = sms_number[i+1];
    sms_number[i+1] = tmp;
  }

  str_len = strlen(sms_number);
  for (i=strlen(sms_number)+2;i!=0;i--)			// ���е��ַ�����ƶ������ֽ�
    sms_number[i] = sms_number[i-2];
  sms_number[str_len+3] = '\0';
  strncpy(sms_number, "91", 2);					// ��ͷд���ַ���"91"

  tmp = strlen(sms_number)/2;					// �����ַ�������

  str_len = strlen(sms_number);
  for (i=strlen(sms_number)+2;i!=0;i--)			// ���е��ַ�����ƶ������ֽ�
    sms_number[i] = sms_number[i-2];
  sms_number[str_len+3] = '\0';
  sms_number[0] = (char)(tmp/10) + 0x30;			// �ַ�������ת��Ϊ�ַ�����д�뿪ͷ
  sms_number[1] = (char)(tmp%10) + 0x30;


  /* ת������Ϣ���� */
  cd = iconv_open("utf-8", "gb2312");				//����ת������"gb2312"==>"utf-8"
  if (0==cd) {
    perror("create iconv handle!");
    close(stty_fd);
    return 0;
  }
  gb2312_len = strlen(sms_gb2312);				// �����ַ����ĳ���
  utf8_len = 140;
  if (-1==iconv(cd, &sms_in, &gb2312_len,
                    &sms_out, &utf8_len)) {		// ת���ַ�ΪUnicode����
    perror("convert code");
    close(stty_fd);
    return 0;
  }
  iconv_close(cd);

  /* ����ʹ��PDUģʽ */
  strcpy(buffer, "AT+CMGF=0\n");
  write(stty_fd, buffer, strlen(buffer));				// д����������
  n = read(stty_fd, buffer, BUFF_SIZE);
  if (n<=0) {
    perror("set pdu mode");
    close(stty_fd);
    return 0;
  }
  if (0!=strncmp(buffer, "OK", 2)) {				// �ж������Ƿ�ִ�гɹ�
    perror("set pdu mode");
    close(stty_fd);
    return 0;
  }

  /* ������Ϣ */
  sprintf(buffer, "AT+CMGS=%d\n", utf8_len);		// д�뷢����Ϣ����
  write(stty_fd, buffer, strlen(buffer));
  write(stty_fd, sms_utf8, utf8_len);				// д����Ϣ����
  printf("Send message OK!\n");

  close(stty_fd);
}
