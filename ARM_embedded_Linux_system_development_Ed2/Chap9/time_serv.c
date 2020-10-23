// time_serv.c - gcc -o s time_serv.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define TIME_PORT 9090
#define DATA_SIZE 256

int main()
{
  int sock_fd;
  struct sockaddr_in local;
  struct sockaddr_in from;
  int fromlen, n;
  char buff[DATA_SIZE];
  time_t cur_time;

  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);		// �����׽���
  if (sock_fd<=0) {
    perror("create socket error!");
    return 0;
  }
  perror("Create socket");

  /* ����Ҫ�󶨵�IP�Ͷ˿� */
  local.sin_family=AF_INET;
  local.sin_port=htons(TIME_PORT);// �����˿�
  local.sin_addr.s_addr=INADDR_ANY;//����

  /* �󶨱������׽��� */
  if (0!=bind(sock_fd,(struct sockaddr*)&local,sizeof(local))) {
    perror("bind socket error!");
    close(sock_fd);
    return 0;
  }
  printf("Bind socket");

  fromlen =sizeof(from);
  printf("waiting request from client...\n");

  while (1)
  {
    n = recvfrom(sock_fd, buff, sizeof(buff), 0, (struct sockaddr*)&from, &fromlen);	// ��������
    if (n<=0) {
      perror("recv data!\n");
      close(sock_fd);
      return 0;
    }
    buff[n]='\0';									// �����ַ���������
    printf("client request: %s\n", buff);					// ��ӡ���յ����ַ���

    if (0==strncmp(buff, "quit", 4))					// �ж��Ƿ��˳�
      break;

    if (0==strncmp(buff, "time", 4)) {					// �ж��Ƿ�����ʱ��
      cur_time = time(NULL);
      strcpy(buff, asctime(gmtime(&cur_time)));			// ���ɵ�ǰʱ���ַ���
      sendto(sock_fd, buff,sizeof(buff), 0,(struct sockaddr*)&from,fromlen);	// ����ʱ����ͻ���
    }

  }
  close(sock_fd);								// �ر��׽���
  return 0;
}
