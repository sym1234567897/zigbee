



#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <stdio.h>
#include <sys/msg.h>
#include <errno.h>
#include "sym_global.h"
#include <sys/msg.h>


uart_cache_list zigbee_cache_head, zigbee_cache_tail;
struct msg msgbuf;
ssize_t  msgsize;

unsigned char dev_camera_mask;
unsigned char zigbee_temp;
extern int msgid;  /*global.c文件里面的变量需要extern*/

extern pthread_cond_t cond_camera;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_uart_cmd;
extern pthread_cond_t cond_sqlite;

extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_uart_cmd;
extern pthread_mutex_t mutex_camera;
extern pthread_mutex_t mutex_slinklist;


void *pthread_main(void *arg){

key_t key;  /*创建消息队列*/
ssize_t msgsize;
struct msg msgbuf;
key = ftok("/app",1);
if (key <0){
	perror("ftok");
}
if ((msgid= msgget(key, IPC_CREAT | IPC_EXCL | 0666))<0){
 if (errno== EEXIST){
		msgid = msgget(key,0666);
		return 0;   /*为什么 消息队列已经存在就需要直接返回呢？*/
 	}
 	else {
	perror("msgger");
	exit(-1);  /*创建消息队列失败  ，进程直接退出*/
	}
}

/*创建消息链表*/
zigbee_cache_head = CreateEmptyCacheList();
zigbee_cache_tail = zigbee_cache_head;
printf("pthread_main is ok\n");

while(1)
{
	bzero(&msgbuf, sizeof(msgbuf));
	printf("wait for msg\n");
	msgsize = msgrcv(msgid, &msgbuf, sizeof(msgbuf.text),1L,0);
	/*msg_sz发送结构体中除long int 的大小，也就是struct msgbuf中的char mtext的大小*/
	//msgsize = msgrcv (msgid, &msgbuf, sizeof (msgbuf) - sizeof (long), 1L, 0);
	printf ("  Get %ldL msg [%ld]\n", msgbuf.msgtype,msgsize);
	printf ("  text[0] = %#x\n", msgbuf.text[0]);
	switch(msgbuf.msgtype)
		{
		case MSG_CAMERA:
			{
				pthread_mutex_lock (&mutex_camera);
				dev_camera_mask = msgbuf.text[0];
				printf("  camera：msgbuf.text = %x\n",msgbuf.text[0]);
				pthread_cond_signal(&cond_camera);
				pthread_mutex_unlock (&mutex_camera);
				break;
			}
		case MSG_ZIGBEE:
			{
				pthread_mutex_lock (&mutex_uart_cmd);
				zigbee_temp = msgbuf.text[0];
				printf("  zigbee：msgbuf.text = %x\n",msgbuf.text[0]);
				InsertCacheNode (&zigbee_cache_tail, &zigbee_temp);
				pthread_cond_signal(&cond_uart_cmd);
				pthread_mutex_unlock (&mutex_uart_cmd);
				break;
			}
		default :
			break;
		}
	
}


}



