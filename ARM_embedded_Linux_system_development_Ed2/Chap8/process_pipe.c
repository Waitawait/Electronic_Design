#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  int fd[2];
  pid_t pid;
  char buf[64] = "I'm parent process!\n";		// ������Ҫд��ܵ�����Ϣ
  char line[64];

  if (0!=pipe(fd)) {						// �����ܵ��������
    fprintf(stderr, "Fail to create pipe!\n");
    return 0;
  }

  pid = fork();							// ��������
  if (pid<0) {
    fprintf(stderr, "Fail to create process!\n");
    return 0;
  } else if (0==pid) {						// ������
    close(fd[0]);							// �رն��ܵ���ʹ�ø�����ֻ����ܵ�д������
    write(fd[1], buf, strlen(buf));				// д���ݵ��ܵ�
    close(fd[1]);							// �ر�д�ܵ�
  } else {								// �ӽ���
    close(fd[1]);							// �ر�д�ܵ���ʹ���ӽ���ֻ�ܴӹܵ���ȡ����
    read(fd[0], line, 64);					// �ӹܵ���ȡ����
    printf("DATA From Parent: %s", line);
    close(fd[0]);							// �رն��ܵ�
  }

  return 0;
}
