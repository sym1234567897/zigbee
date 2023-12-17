

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include <syscall.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include "sym_uart_cache.h"
#include <arpa/inet.h>



#define ENDPOIT_NUM 		4
#define msg_leng 32
#define LEN_ENV 20  //struct getEnvMsg 

#define MSG_LED				1L  
#define MSG_BEEP			2L
#define MSG_CAMERA			3L
#define MSG_ZIGBEE			4L
#define DEV_ZIGBEE 			"/dev/ttyUSB0"
#define DEV_CAMERA			"/dev/video0"


extern void *pthread_uart_send(void *);
extern void *pthread_zigbee_rcv (void *);			//数据接收线程
extern void *pthread_main (void *);	
extern void *pthread_camera (void *);
extern void *pthread_refresh (void *);


struct getEnvMsg
{
	unsigned char tem[2];
	unsigned char hum[2];
	unsigned char ep_no;	
	unsigned char x;
	unsigned char y;
	unsigned char z;

	unsigned short hongwai;
	unsigned short lux;			//光照  2个字节
	unsigned short rsv2;
	unsigned short gas;			//烟雾
	unsigned short rsv3;
	unsigned short adc;			
};


struct msg
{
	long type;  /*给哪个线程*/
	long msgtype;/*给哪个设备的*/
	unsigned char text[msg_leng];
};





struct env_info
{
	unsigned char storage_status;				// 0:open 1:close
	unsigned char led_status;
	unsigned char buzzer_status;
	unsigned char fan_status;
	unsigned char seg_status;
	signed char x;
	signed char y;
	signed char z;
	char samplingTime[20];
	float temperature;
	float temperatureMIN;
	float temperatureMAX;
	float humidity;
	float humidityMIN;
	float humidityMAX;
	unsigned int hongwai;
	unsigned int illumination;
	float illuminationMIN;
	float illuminationMAX;
	unsigned int gas;
	float adc;
	float adcMIN;
};

struct env_info_array
{
	struct env_info env_no[ENDPOIT_NUM];	
};

struct env_info_array all_info_RT; //用于对应共享内存中数据  原本在global.c里面


/*#include "link_list.h"*/
//#ifndef __LIST_QUEUE_H____
//#define __LIST_QUEUE_H____

typedef struct msg_pack
{
	char msg_type;
	char text[27];
}link_datatype;

typedef struct _node_
{
	link_datatype data;
	struct _node_ *next;
}linknode, *linklist;

extern linklist CreateEmptyLinklist ();
extern int EmptyLinklist (linklist h);
extern linklist GetLinknode (linklist h);
extern int InsertLinknode (link_datatype x);

//#endif





























