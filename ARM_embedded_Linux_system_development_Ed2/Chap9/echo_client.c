// echo_client �C gcc �Co c echo_client.c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define EHCO_PORT 8080
#define MAX_COMMAND 5

int main()
{
  int sock_fd;
  struct sockaddr_in serv_addr;

  char *buff[MAX_COMMAND] = {"abc", "def", "test", "hello", "quit"};
  char tmp_buf[100];
  socklen_t len;
  int n, i;

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

  /* ���ӵ������ */
  if (-1==connect(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
    perror("connect() error!\n");
    close(sock_fd);
    return 0;
  }
  printf("Success connect to server!\n");

  /* ���Ͳ����ջ�������� */
  for (i=0;i<MAX_COMMAND;i++) {
    send(sock_fd, buff[i], 100, 0);						// �������ݸ������
    n = recv(sock_fd, tmp_buf, 100, 0);					// �ӷ���˽�������
    tmp_buf[n] = '\0';  // ���ַ�����ӽ�����־
    printf("data send: %s receive: %s\n", buff[i], tmp_buf);		// ��ӡ�ַ���
    if (0==strncmp(tmp_buf, "quit", 4))					// �ж��Ƿ����˳�����
      break;
  }

  close(sock_fd);									// �ر��׽���

  return 0;
}