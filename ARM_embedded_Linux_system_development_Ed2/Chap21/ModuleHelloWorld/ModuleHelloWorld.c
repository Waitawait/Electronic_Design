/* �ں�ģ��: ModuleHelloWorld.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");				// �����ں�ģ���ȨЭ��
MODULE_AUTHOR("GongLei");				// �����ں�ģ������

/* init function */
static int __init hello_init(void)				// ģ���ʼ������
{
  printk(KERN_ALERT "(init)Hello,World!\n");	// ��ӡһ����Ϣ
  return 0;
}

/* exit function */
static void __exit hello_exit(void)				// ģ���˳��������
{
  printk(KERN_ALERT "(exit)Hello,World!\n");	// ��ӡһ����Ϣ
}

module_init(hello_init);						// ����ģ���ʼ������
module_exit(hello_exit);					// ����ģ���˳�ʱ�������
