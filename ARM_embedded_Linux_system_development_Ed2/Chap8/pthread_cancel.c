#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* thread_func(void *arg)								// �̺߳���
{
  int *val = arg;
  printf("Hi, I'm a thread!\n");
  if (NULL!=arg) {									// ���������Ϊ�գ���ӡ��������
    while(1)
      printf("argument set: %d\n", *val);
  }
}

int main()
{
  pthread_t tid;										// �߳�ID
  int t_arg = 100;									// ���̴߳���Ĳ���ֵ

  if (pthread_create(&tid, NULL, thread_func, &t_arg))		// �����߳�
    perror("Fail to create thread");

  sleep(1);											// ˯��1�룬�ȴ��߳�ִ��
  printf("Main thread!\n");
  pthread_cancel(tid);									// ȡ���߳�

  return 0;
}
