//test.c
int g_var_a =0;					// �����ȫ���ѳ�ʼ��������
char g_var_b;					// �����BSS����δ��ʼ��ȫ�ֱ�����

int main()
{
	int var_a;					// �����ջ��
	char var_str[] = ��string1��;		// �����ջ��
	char *p_str1, *p_str2;		// �����ջ��
	char *p_str3 = ��string2��;		// ������ѳ�ʼ�������� ptr_str3�����ջ��
	static int var_b = 100;		// ȫ�־�̬���ݣ�������ѳ�ʼ����

	p_str1 = (char*)malloc(1024);	// �Ӷ�������1024�ֽ��ڴ�
	p_str2 = (char*)malloc(2048);	// �Ӷ�������2048�ֽ��ڴ�
	free(p_str1);
	free(p_str2);

	return 0;
}
