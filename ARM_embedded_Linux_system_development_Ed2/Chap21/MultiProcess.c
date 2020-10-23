// MultiProcess.c
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
  pid_t   pid;				// �����ӽ���ID

  pid = fork();
  if (pid <0) {				// �����ӽ���
    printf("fork err\n");
    exit(-1);
  } else if (pid == 0) {
    /* child process */
    //sleep(60);

    int a = 10;
    int b = 100;
    int c = 0;
    int d;

    d = b/a;
    printf("d = %d\n", d);
    d = a/c;
    printf("d = %d\n", d);		// ��0����

    exit(0);
  } else {
    /* parent process */
    sleep(4);
    wait(-1);				// �ȴ��ӽ��̽���
    exit(0);
  }

  return 0;
}
