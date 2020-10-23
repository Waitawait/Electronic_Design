//
// HttpDemo.c
//
// ����: ʹ��HttpЭ�����վ��ȡһ��ҳ��
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

void sig_int(int sig);

/* ����HTTPͷ�ĺ��� */
int GetHttpHeader(char *buff, char *header);

/* ��ӡ������Ϣ */
#define PRINTERROR(s) \
    fprintf(stderr,"\nError at %s, errno = %d\n", s, errno)

/* ������ */
int main()
{
  int bytes_all = 0;
  char *host_name = "www.sohu.com";
  int nRet;
  int sock_fd;// socket���
  struct sockaddr_in server_addr;
  struct hostent *host_entry;
  char strBuffer[2048] = {0};//��ŷ��ص�����
  char strHeader[1024] = {0};//���Http������ͷ

  /* ��װSIGINT�ź���Ӧ���� */
  signal(SIGINT, sig_int);

  sock_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    PRINTERROR("socket()");
    return -1;
  }

  host_entry = gethostbyname(host_name);//��ȡ������Ӧ��IP��ַ
  server_addr.sin_port = htons(80);//���÷���˿ں�
  server_addr.sin_family = PF_INET;//����socket����
  server_addr.sin_addr = (*(struct in_addr*)*(host_entry->h_addr_list));

  /* ���ӵ������� */
  nRet = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
  if (nRet == -1){
    PRINTERROR("connect()");
    close(sock_fd);
    return -1;
  }

  /* ����Http������ͷ */
  sprintf(strBuffer, "GET / HTTP/1.1\r\n");
  strcat(strBuffer, "Accept */*\r\n");
  strcat(strBuffer, "Connection: Keep-Alive\r\n");

  /* ����Http���� */
  nRet = send(sock_fd, strBuffer, strlen(strBuffer), 0);
  if (nRet == -1) {
    PRINTERROR("send()");
    close(sock_fd);
    return -1;
  }

  /* ��ȡ���������ص�ҳ������ */
  while(1)
  {
    /* �ȴ�����������ҳ������ */
    nRet = recv(sock_fd, strBuffer, sizeof(strBuffer), 0);
    if (nRet == -1)
    {
      PRINTERROR("recv()");
      break;
    }

    bytes_all += nRet;//�ۼӷ���������ҳ�����ݵ��ֽ���

    if (0==GetHttpHeader(strBuffer, strHeader)) {
      printf("%s", strHeader);
    }

    /* ���������Ƿ�ر����� */
    if (nRet == 0) {//û�����ݷ��ر�ʾ�����Ѿ��ر�
      fprintf(stderr,"\n %d bytes received.\n", bytes_all);
      break;
    }

    /* ��ӡ���������ص����� */
    printf("%s", strBuffer);
  }

  /* �ر����� */
  close(sock_fd);

  return 0;

}

void sig_int(int sig)	// �ж��ź���Ӧ����
{
  printf("Ha ha, we get SIGINT!\n");
}

/* ��ȡHTTPЭ��ͷ */
int GetHttpHeader(char *buff, char *header)
{
  char *p, *q;
  int i=0;

  p = buff;								// ������ͷ
  q = header;							// Э��ͷ

  if (NULL==p)							// �������
    return -1;
  if (NULL==q)
    return -1;

  while('\0'!=(*p)) {						// ����Ƿ��ַ�������
    q[i] = p[i];
    if ((p[i]==0x0d)&&(p[i+1]==0x0a)&&
      (p[i+2]==0x0d)&&(p[i+3]==0x0a)) {		// �ж��Ƿ���ӽ�β
      q[i+1] = p[i+1];
      q[i+2] = p[i+2];
      q[i+3] = p[i+3];
      q[i+4] = 0;
      return 0;
    }
    i++;
  }
  return -1;
}
