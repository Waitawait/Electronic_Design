// echo_serv.c �C gcc �Co s echo_serv.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define EHCO_PORT 8080
#define MAX_CLIENT_NUM 10

int main()
{
  int sock_fd;
  struct sockaddr_in serv_addr;
  int clientfd;
  struct sockaddr_in clientAdd;
  char buff[101];
  socklen_t len;
  int closing =0;
  int n;

  /* ����socket */
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_fd==-1) {
    perror("create socket error!");
    return 0;
  } else {
    printf("Success to create socket %d\n", sock_fd);
  }

  /* ����server��ַ�ṹ */
  bzero(&serv_addr, sizeof(serv_addr));				// ��ʼ���ṹռ�õ��ڴ�
  serv_addr.sin_family = AF_INET;					// ���õ�ַ���������
  serv_addr.sin_port = htons(EHCO_PORT);			// ���ü����˿�
  serv_addr.sin_addr.s_addr = htons(INADDR_ANY);		// ���÷�������ַ
  bzero(&(serv_addr.sin_zero), 8);

  /* �ѵ�ַ���׽��ְ� */
  if(bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))!= 0) {
    printf("bind address fail! %d\n", errno);
    close(sock_fd);
    return 0;
  } else {
    printf("Success to bind address!\n");
  }

  /* �����׽��ּ��� */
  if(listen(sock_fd ,MAX_CLIENT_NUM) != 0) {
    perror("listen socket error!\n");
    close(sock_fd);
    return 0;
  } else {
    printf("Success to listen\n");
  }

  /* ���������Ӷ�Ӧ���׽��� */
  len = sizeof(clientAdd);
  clientfd = accept(sock_fd, (struct sockaddr*)&clientAdd, &len);
  if (clientfd<=0) {
    perror("accept() error!\n");
    close(sock_fd);
    return 0;
  }

  /* �����û����������� */
  while((n = recv(clientfd,buff, 100,0 )) > 0) {
    buff[n] = '\0'; // ���ַ������������
    printf("number of receive bytes = %d data = %s\n", n, buff);		// ��ӡ�ַ������Ⱥ�����
    fflush(stdout);
    send(clientfd, buff, n, 0);			// �����ַ������ݸ��ͻ���
    if(strncmp(buff, "quit", 4) == 0)		// �ж��Ƿ����˳�����
      break;
  }

  close(clientfd);						// �ر��½�������
  close(sock_fd);					// �رշ���˼�����socket

  return 0;
}
