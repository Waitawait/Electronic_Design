#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void bye(void)					// �˳�ʱ�ص��ĺ���
{
  printf("That was all, folks\n");
}

void bye1(void)					// �˳�ʱ�ص��ĺ���
{
  printf("This should called first!\n");
}

int main()
{
  long a;
  int i;

  i = atexit(bye);					// �����˳��ص���������鷵�ؽ��
  if (i != 0) {
    fprintf(stderr, "cannot set exit function bye\n");
    return EXIT_FAILURE;
  }

  i = atexit(bye1);				// �����˳��ص���������鷵�ؽ��
  if (i!=0) {
    fprintf(stderr, "cannot set exit function bye1\n");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
