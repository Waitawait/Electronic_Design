#include <stdio.h>
 
int main()
{
	FILE *fp = NULL;						// �����ļ�ָ��
	char *buf[3] = {						// ���������ַ�������д���ļ�ʹ��
	        "This is first line!\n",
	        "Second Line!\n",
	        "OK, the last line!\n"};
	char tmp_buf[3][64], *p;				// �����ַ������棬����ȡ�ļ�ʹ��
	int i;
	
	fp = fopen("chap7_demo.dat", "rb+");		// ʹ�ö�д��ʽ���ļ������Ұ��ļ�������0
	if (NULL==fp) {
    printf("error to open file!\n");
    return -1;
	}
	
	// �������ַ���д���ļ�
	for (i=0;i<3;i++)
	        fputs(buf[i], fp);
	
	fseek(fp, 0, SEEK_SET);				// ���ļ�ָ�����õ��ļ���ͷ���൱��rewind(fp)
	
	// ���ļ���ȡ�����ַ���������
	for (i=0;i<3;i++) {
            p = tmp_buf[i];
            fgets(p, 64, fp);
            printf("%s", p);				// ��ӡ�ն�ȡ�������ַ�������Ļ
	}
	
	fclose(fp);							// �����ǹر��ļ�
	
	return 0;
}
