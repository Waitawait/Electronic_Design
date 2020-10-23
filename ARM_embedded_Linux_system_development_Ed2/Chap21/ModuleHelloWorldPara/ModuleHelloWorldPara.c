/* �ں�ģ��: ModuleHelloWorldPara.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");

static int initValue = 0;											// ģ����� initValue = <int value>
static char *initName = NULL;									// ģ����� initName = <char*>
module_param(initValue, int, S_IRUGO);
module_param(initName, charp, S_IRUGO);

/* init function */
static int hello_init(void)
{
  printk(KERN_ALERT "initValue = %d initName = %s\n", initValue, initName);	// ��ӡ����ֵ
  printk(KERN_ALERT "(init)Hello,World!\n");
  return 0;
}

/* exit function */
static void hello_exit(void)
{
  printk(KERN_ALERT "(exit)Hello,World!\n");
}

module_init(hello_init);
module_exit(hello_exit);
