#include "s3c2410.h"
#include "platform/smdk2410.h"

#define DELAYTIME 0x5000

void delay(unsigned long n);

int main(void)
{
  unsigned char i;
  unsigned long led[4] = {0xd0, 0x70, 0xe0, 0xb0};	// ����LED����I/O�ӿڵĵ�ַ

  GPFCON = vGPFCON;						// ��ʼ��GPIO���ƼĴ���

  while (1) {
    for (i=0; i<4; i++) {
      GPFDAT = led[i];						// ��GPIO�ӿ�д��ߵ�ƽ
      delay(DELAYTIME);
    }
  }

  return 0;
}

/* delay some time */
void delay(unsigned long n)
{
  while (n--) {
    ;
  }
}
