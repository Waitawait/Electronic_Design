// c_memory_test.c
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *p_str1, *p_str2;								// ��������char*ָ��

    /* ʹ��malloc()���������ڴ� */
     p_str1 = (char*)malloc(32);
     if (NULL==p_str1) {								// ����ڴ�����Ƿ�ɹ�
         printf("Alloc p_str1 memory ERROR!\n");
         return -1;
     }   
 
     /* ʹ��calloc()���������ڴ� */
     p_str2 = (char*)calloc(32, sizeof(char));
     if (NULL==p_str2) {								// ����ڴ��Ƿ����ɹ�
         printf("Alloc p_str2 memory ERROR!\n");
         free(p_str1);									// ע�⣬������Ҫ�ͷ�p_str1ռ�õ��ڴ�
         return -1;
     }   
 
     strcpy(p_str1, "This is a simple sentence.");				// p_str1д��һ���ַ���
     strcpy(p_str2, p_str1);								// p_str2д����p_str1��ͬ���ַ���
 
     /* ��ӡp_str1�Ľ�� */
     printf("p_str1 by malloc():\n");
     printf("p_str1 address: 0x%.8x\n", p_str1);				// p_str1���ڴ��ַ
     printf("p_str1: %s(%d chars)\n", p_str1, strlen(p_str1));		// p_str1������
 
     /* ��ӡp_str2�Ľ�� */
     printf("p_str2 by calloc():\n");
     printf("p_str2 address: 0x%.8x\n", p_str2);				// p_str2���ڴ��ַ
     printf("p_str2: %s(%d chars)\n", p_str2, strlen(p_str2));		// p_str2������
 
     /* Ϊp_str1���·����ڴ棨��С�� */
     p_str1 = (char*)realloc(p_str1, 16);
     if (NULL==p_str1) {								// ����ڴ������
         printf("Realloc p_str1 memory ERROR!\n");
         free(p_str2);									// ע�⣬��Ҫ�ͷ�p_str2ռ�õ��ڴ�
         return -1;
     }   
     p_str1[15] = '\0';									// д�ַ���������
 
     /* Ϊp_str2���·����ڴ棨���� */
     p_str2 = (char*)realloc(p_str2, 128);
     if (NULL==p_str2) {								// ����ڴ������
         printf("Realloc p_str2 memory ERROR!\n");
         free(p_str1);									// ע�⣬��Ҫ�ͷ�p_str1ռ�õ��ڴ�
         return -1;
     }   
     strcat(p_str2, "The second sentence in extra memory after realloced!");
 
     /* ��ӡp_str1�Ľ�� */
     printf("p_str1 after realloced\n");
     printf("p_str1 address: 0x%.8x\n", p_str1);				// p_str1���ڴ��ַ
     printf("p_str1: %s(%d chars)\n", p_str1, strlen(p_str1));		// p_str1������
 
     /* ��ӡp_str2�Ľ�� */
     printf("p_str2 after realloced:\n");
     printf("p_str2 address: 0x%.8x\n", p_str2);				// p_str2���ڴ��ַ
     printf("p_str2: %s(%d chars)\n", p_str2, strlen(p_str2));		// p_str2������
 
     /* ע�⣬���Ҫ�ͷ�ռ�õ��ڴ� */
     free(p_str1);										// �ͷ�p_str1ռ�õ��ڴ�
     free(p_str2);										// �ͷ�p_str2ռ�õ��ڴ�
 
     return 0;
 }
