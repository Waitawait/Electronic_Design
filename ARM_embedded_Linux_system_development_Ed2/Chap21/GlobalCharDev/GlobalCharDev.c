/* GlobalCharDev.c */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("GongLei");

#define DEV_NAME "GlobalChar"

static ssize_t GlobalRead(struct file *, char *, size_t, loff_t*);
static ssize_t GlobalWrite(struct file *, const char *, size_t, loff_t*);

static int char_major = 0;
static int GlobalData = 0;  // "GlobalChar" �豸��ȫ�ֱ���

//��ʼ���ַ��豸������file_operations�ṹ��
struct file_operations globalchar_fops =
{
  .read = GlobalRead,
  .write = GlobalWrite
};


/* ģ���ʼ������ */
static int __init GlobalChar_init(void)
{
  int ret;

  ret = register_chrdev(char_major, DEV_NAME, &globalchar_fops); 	// ע���豸����
  if (ret<0) {
    printk(KERN_ALERT "GlobalChar Reg Fail!\n");
  } else {
    printk(KERN_ALERT "GlobalChar Reg Success!\n");
    char_major = ret;
    printk(KERN_ALERT "Major = %d\n", char_major);
  }
  return ret;
}

/* ģ��ж�غ��� */
static void __exit GlobalChar_exit(void)
{
  unregister_chrdev(char_major, DEV_NAME); 						// ע���豸����
  return;
}

/* �豸���������� */
static ssize_t GlobalRead(struct file *filp, char *buf, size_t len, loff_t *off)
{
  if (copy_to_user(buf, &GlobalData, sizeof(int))) {		// ���ں˿ռ临��GlobalData���û��ռ�
    return -EFAULT;
  }
  return sizeof(int);
}

/* �豸����д���� */
static ssize_t GlobalWrite(struct file *filp, const char *buf, size_t len, loff_t *off)
{
  if (copy_from_user(&GlobalData, buf, sizeof(int))) {	// ���û��ռ临��GlobalData���ں˿ռ�
    return -EFAULT;
  }
  return sizeof(int);
}

module_init(GlobalChar_init);
module_exit(GlobalChar_exit);
