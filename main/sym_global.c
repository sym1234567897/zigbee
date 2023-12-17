
#include "sym_global.h"

int  dev_uart_fd; //"ttyUSB0"  /*很多地方都要用  ，所以在这里定义，再别的地方大家用extern调用吧*/
char qt_status;

int msgid; //消息队列       /*变量定义在.c   ，我忘记了*/
int shmid; //共享内存
int semid; //信号量


//条件变量
pthread_cond_t cond_zigbee_rcv;
pthread_cond_t cond_uart_cmd;
pthread_cond_t cond_main_thread;
pthread_cond_t cond_camera;
pthread_cond_t cond_refresh;
pthread_cond_t cond_refresh_updata;

//互斥锁
pthread_mutex_t mutex_slinklist;
pthread_mutex_t mutex_zigbee_rcv;
pthread_mutex_t mutex_uart_cmd;
pthread_mutex_t mutex_main_thread;
pthread_mutex_t mutex_camera;
pthread_mutex_t mutex_refresh;
pthread_mutex_t mutex_refresh_updata;
pthread_mutex_t mutex_global;   //对应all_info_RT
pthread_mutex_t mutex_linklist;


/*link_list.c中的文件*/


linklist envlinkHead, envlinkTail;
linklist CreateEmptyLinklist()
{
	linklist h;
	h = (linklist)malloc (sizeof (linknode));
	envlinkTail = h;
	h->next = NULL;
	return h;
}

int EmptyLinklist (linklist h)
{
	return NULL == h->next;
}

linklist GetLinknode(linklist h)
{
	if (1 == EmptyLinklist (h))	
	{
		return NULL;
	}
	linklist p = h->next;
	h->next = p->next;
	if (p->next == NULL)
		envlinkTail = h;
	return p;
}

int InsertLinknode(link_datatype x)
{
	linklist q = (linklist)malloc (sizeof (linknode));
	if (NULL == q)
	{
		printf ("InsertLinknode Error\n");
		return -1;
	}
	envlinkTail->next = q;
	envlinkTail = q;
	q->data = x;
	q->next = NULL;

	return 0;
}














