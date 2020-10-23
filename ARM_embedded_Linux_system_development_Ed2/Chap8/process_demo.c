// process_demo.c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t pid, pid_wait;
  int fd[2];
  char buff[64], *cmd = "exit";

  if (pipe(fd)) {									// �����ܵ�
    perror("Create pipe fail!");
    return 0;
  }

  pid = fork();
  if (-1==pid) {
    perror("Create process fail!");
    return 0;
  } else if (0==pid) {								// �ӽ���
    close(fd[1]);									// �ر�д����
    printf("wait command from parent!\n");
    while(1) {
      read(fd[0], buff, 64);
      if (0==strcmp(buff, cmd)) {
        printf("recv command ok!\n");
        close(fd[0]);
        exit(0);
      }
    }
  } else {										// ������
    printf("Parent process! child process id: %d\n", pid);
    close(fd[0]);									// �رն�����
    sleep(2);
    printf("Send command to child process.\n");
    write(fd[1], cmd, strlen(cmd)+1);					// д������
    close(fd[1]);
  }

  return 0;
}
