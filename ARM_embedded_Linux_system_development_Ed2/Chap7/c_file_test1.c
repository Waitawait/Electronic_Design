// ���ļ���д�ɿ�����
#include <stdio.h>
int main()
{
	int buf[1024] = {0};
	int *p;
    int i;
	FILE *fp = fopen("./blk_file.dat", "rb+");
	if (NULL == fp)
		return -1;
	fwrite(buf, sizeof(int), 1024, fp);			// ��1024�����ݿ�д���ļ���fp��ÿ�����ݿ�4���ֽ�
	
	/* �޸�buf�����ݣ�����ȡ��Ƚ� */
	for (i=0;i<16;i++)
		buf[i] = -1;

	p = &buf[0]; // ����ָ��pָ��buf�������ļ���ȡ����ʹ��
	fread(p, sizeof(int), 1024, fp);			// ���ļ���ȡ1024�����ݿ鵽buf��ÿ�����ݿ�4���ֽ�
	
	/* ��ӡ���ļ���ȡ�Ķ��������� */
	for (i=0;i<1024;i++)
		printf("buf[%d] = %d\n", i, buf[i]);
	
	fclose(fp);							// �������˹ر��ļ�
	
	return 0;
}
