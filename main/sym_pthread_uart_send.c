#include "sym_global.h"
#include "sym_uart_cache.h"

uart_cache_list zigbee_cache_head, zigbee_cache_tail;  /*我没有用extern  ，因为我直接把sym_uart_cache.h拿来了*/

/*extern 的用处可能是因为uart_cache_list 这个结构体是在别处定义的*/
unsigned char dev_uart_mask ;/*没有用extern*/

extern int dev_uart_fd;  /*别的地方需要初始化串口操作，并打开串口*/
extern pthread_cond_t cond_uart_cmd;

extern pthread_mutex_t mutex_uart_cmd;


void *pthread_uart_send (void *arg)
{

	uart_cache_list uart_cache_p =NULL;  /*创建的指针 初始化的时候 就指向NULL*/
	unsigned char *uart_data = NULL;
	while(1)
	{
		pthread_mutex_lock (&mutex_uart_cmd);
		pthread_cond_wait (&cond_uart_cmd, &mutex_uart_cmd);
		while ((uart_cache_p = GetCacheNode (zigbee_cache_head, &zigbee_cache_tail)) != NULL)
			{
					pthread_mutex_unlock (&mutex_uart_cmd); /*已经获得了一个链表里面的元素 第一次是释放一下pthread_cond_signal获得的锁，别占用太久*/
					uart_data=(unsigned char *)uart_cache_p->data;  /*data 是void * 类型，但存的是unsigned char msgbuf.text*/
					dev_uart_mask = *uart_data;
					write(dev_uart_fd,&dev_uart_mask,1);
					free(uart_data);
					uart_data =NULL;
					free(uart_cache_p);
					uart_cache_p =NULL;
					usleep (200000);  /*不睡会怎么样？*/
					pthread_mutex_lock (&mutex_uart_cmd);/*这所以这里加锁 是因为第二个while的第一行解锁了 */
			}
		pthread_mutex_unlock (&mutex_uart_cmd);

		}
	
}



























