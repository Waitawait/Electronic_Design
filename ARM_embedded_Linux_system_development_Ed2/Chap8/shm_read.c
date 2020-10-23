// shm_read.c --> gcc -o r shm_read.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int shmid;								// ���干���ڴ�id
  char *ptr;

  shmid = shmget(0x90, 1024, SHM_W|SHM_R|IPC_EXCL);   // ����key��ù����ڴ�id
  if (-1==shmid)
    perror("create share memory");

  ptr = shmat(shmid, 0, 0);						// ͨ�������ڴ�id��ù����ڴ��ַ
  if ((void*)-1==ptr)
    perror("get share memory");

  printf("string in share memory: %s\n", ptr);		// ��ӡ�����ڴ��е�����

  shmdt(ptr);
  return 0;
}
