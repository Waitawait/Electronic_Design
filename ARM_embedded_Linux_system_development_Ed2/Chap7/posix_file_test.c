/* ע��POSIX�����ļ�����ʹ�ò�ͬ��ͷ�ļ� */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <errno.h>


extern int errno;

int main()
{
	int fd;							// ע�⣬�ļ�������������ֵ
	char buf[64] = "this is a posix file!(line1)\n";
	off_t curr_pos;
	int file_mode;
	
	fd = open("./posix.data", O_CREAT|O_RDWR|O_EXCL, S_IRWXU); 
//��һ�������ڵ��ļ����������ļ���Ȩ�����û��ɶ�дִ��
	if (-1==fd) {  // ����ļ����Ƿ�ɹ�
		switch (errno) {
			case EEXIST:				// �ļ��Ѵ���
				printf("File exist!\n");
				break;
			default:					// ��������
				printf("open file fail!\n");
				break;
		}
		return 0;
	}
	
	write(fd, buf, strlen(buf));				// ���ַ���д���ļ�
	
	curr_pos = lseek(fd, 0, SEEK_CUR);		// ȡ�õ�ǰ�ļ�ƫ����λ��
	printf("File Point at: %d\n", curr_pos);
	
	lseek(fd, 0, SEEK_SET);				// ���ļ�ƫ�����ƶ����ļ���ͷ
	
	strcpy(buf, "File Pointer Moved!\n");
	write(fd, buf, strlen(buf));				// ���µ�����д���ļ�
	
	file_mode = fcntl(fd, F_GETFL);		// ��ȡ�ļ�״̬���
	if (-1!=file_mode) {
		switch (file_mode&O_ACCMODE) {	// ����ļ�״̬
			case O_RDONLY:
				printf("file mode is READ ONLY\n");
				break;
			case O_WRONLY:
				printf("file mode is WRITE ONLY\n");
				break;
			case O_RDWR:
				printf("file mode is READ & WRITE\n");
				break;
		}
	}
	
	close(fd);
	
	return 0;
	
}
