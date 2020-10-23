#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  pid_t pid, pid_wait;
  int status;

  pid = fork();							// �����ӽ���
  if (-1==pid) {							// ����Ƿ񴴽��ɹ�
    printf("Error to create new process!\n");
    return 0;
  }
  else if (pid==0) {						// �ӽ���
    printf("Child process!\n");
  } else {								// ������
    printf("Parent process! Child process ID: %d\n", pid);
    pid_wait = waitpid(pid, &status, 0);		// �ȴ�ָ�����̺ŵ��ӽ���
    printf("Child process %d returned!\n", pid_wait);
  }

  return 0;
}
