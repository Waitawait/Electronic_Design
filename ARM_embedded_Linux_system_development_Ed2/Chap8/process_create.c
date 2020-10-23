#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t pid;

  pid = fork();					// ��������
  if (-1==pid) {					// ��������ʧ��
    printf("Error to create new process!\n");
    return 0;
  }
  else if (pid==0) {				// �ӽ���
    printf("Child process!\n");
  } else {						// ������
    printf("Parent process! Child process ID: %d\n", pid);
  }

  return 0;
}
