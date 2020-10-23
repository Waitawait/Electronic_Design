// shm_write.c --> gcc -o w shm_write.c
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
  int shmid;							 // ���干���ڴ�id
  char *ptr;
  char *shm_str = "string in a share memory";

  shmid = shmget(0x90, 1024, SHM_W|SHM_R|IPC_CREAT|IPC_EXCL); // ���������ڴ�
  if (-1==shmid)
    perror("create share memory");

  ptr = (char*)shmat(shmid, 0, 0);			// ͨ�������ڴ�id��ù����ڴ��ַ
  if ((void*)-1==ptr)
    perror("get share memory");

  strcpy(ptr, shm_str);					// ���ַ���д�빲���ڴ�
  shmdt(ptr);

  return 0;
}
