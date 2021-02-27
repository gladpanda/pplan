#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <termios.h>
#include <stdlib.h>
/*
	串口初始化函数
	/dev/ttySAC1
	/dev/ttySAC2
	/dev/ttySAC3
	以上的端口号 -. com
	
	speed ——》 波特率 -》 9600
*/

int init_serial(char *com,int speed)
{
	int fd = open(com,O_RDWR);
	if(-1 == fd)
	{
		perror("open serial error");
		exit(-1);//直接结束程序
	}
	struct termios t;
	t.c_cflag |= (CREAD | CLOCAL);//本地模式，可接受数据
	t.c_cflag &= ~CSTOPB;//停止位1
	t.c_cflag &= ~CSIZE;//清空数据位
	t.c_cflag |= CS8;//设置8位数据位
	t.c_cflag &= ~PARENB;//不要奇偶校检
	t.c_cflag &= ~CRTSCTS;//不要流控
	//设置波特率
	switch(speed)
	{
		case 9600:
			cfsetspeed(&t, 9600);
			break;
		case 19200:
			cfsetspeed(&t, 19200);
			break;
		case 115200:
			cfsetspeed(&t, 115200);
			break;
	}
	//清空输入缓存区内容
	tcflush(fd,TCIFLUSH);

	//设置串口属性
	tcsetattr(fd,TCSANOW,&t);
	return fd;
	
}

int main(void)
{

		int dis;
		while (1)
		{
		int fd=init_serial("/dev/ttyUSB0",9600);
		//计算距离
		unsigned char s[2];
		char b=0x55;
		write(fd,&b,1);
		usleep(50000);
		read(fd,&s,2);
		close(fd);
		dis=s[0]*256+s[1];
		printf("distance=%dcm\n",dis/10);		
		}
		return dis/10;
}