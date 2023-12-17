
#include "sym_global.h"

extern int dev_uart_fd;  /*在sym_global.c里面定义了*/
extern pthread_mutex_t mutex_linklist;
extern pthread_cond_t cond_refresh;

char check;
char flag=0;   /*这种用来判断的flag 最好用char  ，我记得我曾经用过int，由于系统的问题会误判*/
void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd,&options);
	options.c_cflag |= ( CLOCAL | CREAD );
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB; 
	options.c_iflag |= IGNPAR;
	options.c_iflag &= ~(ICRNL | IXON);
	options.c_oflag = 0;
	options.c_lflag = 0;

	cfsetispeed(&options,B115200);
	cfsetospeed(&options,B115200);
	tcsetattr(fd,TCSANOW,&options);
}

void *pthread_zigbee_rcv (void * arg)
{
	link_datatype buf;
	int len;

#if 1
	dev_uart_fd = open(DEV_ZIGBEE,O_RDWR);
	if (dev_uart_fd<0)
		{
			perror("dev_uart");
			return (void *) -1;
		}

	serial_init(dev_uart_fd);
	printf ("pthread_transfer is ok\n");
#endif
	
	while(1)
	{
		   /*read 阻塞函数    ，因为他是阻塞函数 所以可以调用while*/
		memset(&buf,0,sizeof(link_datatype));
		read(dev_uart_fd, &check ,1);
		if(check == 's')
			{
			 check =0;
			 read(dev_uart_fd, &check ,1);
			 if(check == 't')
			 	{
					check =0;
					read(dev_uart_fd, &check ,1);
					if(check == ':')
			 		{
			 			check = 0;
						read (dev_uart_fd, &check, 1);
						if (check == 'e')
							{
							 buf.msg_type='e';
							 //usleep(1);
							 if ((len =read(dev_uart_fd, buf.text,LEN_ENV))!=LEN_ENV)
							 	{
							 		/*如果没有读到20个   就接着读*/
									for (int i=len;i<LEN_ENV;i++)
										{
										 read (dev_uart_fd,buf.text+i,1);
										}
									
							 	}
							 flag =1;
							}
			 		}
				}
			}
		if(flag ==1)
			{
			flag =0;
			  /*把数据存入node 临界操作*/
			pthread_mutex_lock (&mutex_linklist);
			if(InsertLinknode(buf)==-1)
				{
					printf("NONMEM\n");
					pthread_mutex_unlock (&mutex_linklist);/*失败要及时释放锁*/
				}
			pthread_cond_signal (&cond_refresh);
			pthread_mutex_unlock (&mutex_linklist);
			
				
			}
	}
	return 0;
}















