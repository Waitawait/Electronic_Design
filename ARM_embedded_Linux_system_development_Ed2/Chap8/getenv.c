// filename  getenv.c �C ��ȡ������������
#include <stdio.h>
#include <stdlib.h>
 
int main()
{
  char *env_path = "PATH";		// �����ȡ�Ļ�����������
  char *env_value = NULL;		// ��������ֵ
 
  env_value = getenv(env_path);	// ʹ��ϵͳ������ȡָ����������
  if (NULL==env_value)			// ����Ƿ��ȡ��������ֵ
    printf("Not found!\n");
  printf("Get Env PATH:\n%s", env_value);	// ���PATH����������ֵ
  return 0;
}
