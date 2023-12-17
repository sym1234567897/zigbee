#include "sym_global.h"
#include "sem.h"  /*把信号灯的操作 封装成了函数*/



#define N 1024

extern pthread_mutex_t mutex_refresh;     /*定义在.h里面不需要 extern 定义在.c里面才需要 extern*/
extern pthread_mutex_t mutex_refresh_updata;
extern pthread_mutex_t mutex_global;
extern pthread_mutex_t mutex_slinklist;
extern pthread_cond_t cond_refresh;
extern pthread_cond_t cond_refresh_updata;
extern pthread_mutex_t mutex_linklist;


extern char qt_status;
linklist envlinkHead, envlinkTail;   /*这个我放进了sym_global.h  不需要extern*/

struct shm_addr
{
	char cgi_status;
	char qt_status;
	struct env_info_array rt_status;
};

void getEnvPackage (link_datatype *buf)
{
	struct getEnvMsg pack;   /*在sym_global.h 里面定义了*/
	float temh;//,teml;
	int ep_no;
	
	memcpy (&pack, buf->text, LEN_ENV);
	ep_no = pack.ep_no;

	pthread_mutex_lock (&mutex_global);
	struct env_info current = all_info_RT.env_no[ep_no];
	pthread_mutex_unlock (&mutex_global);

	current.storage_status = 1;
	current.x = pack.x;
	current.y = pack.y;
	current.z = pack.z;
	
	temh =  pack.tem[1];
	//teml =  pack.tem[0];
	current.temperature = temh;

	temh =  pack.hum[1];
	//teml =  pack.hum[0];
	current.humidity = temh;
	
	current.illumination = (unsigned short)pack.lux;//光强

	current.gas = (unsigned short)pack.gas;//烟雾

	current.hongwai = pack.hongwai;
	
	printf ("ep_no = %d tem = %0.2f hum = %0.2f ill = %d  gas = %d hongwai=%d\n", 
			ep_no,current.temperature, current.humidity, current.illumination, current.gas,current.hongwai);

	//checkEnv (sto_no, &current);	

	pthread_mutex_lock (&mutex_global);
	all_info_RT.env_no[ep_no] = current;
	pthread_mutex_unlock (&mutex_global);

	return ;
}


 void *pthread_refresh (void *arg)
{

	/*共享内存和信号量一般联合操作   ，key也好像可以共有，也可以创建两个*/
	key_t key_info;
	int shmid, semid;
	linklist node;
	link_datatype buf;
	struct shm_addr *shm_buf;
	
	if((key_info =ftok("./",'i'))<0)
		{
			perror ("key_info\n");
			exit(-1);
		}
	
	if ((semid = semget (key_info, 1, IPC_CREAT | IPC_EXCL |0666)) < 0)
	{
		if (errno == EEXIST)
		{
			semid = semget (key_info, 1, 0666);
		}
		else
		{
			perror ("semget");
			exit (-1);
		}
	}
	else
	{
		init_sem (semid, 0, 1);
	}

	if ((shmid =shmget(key_info, N , IPC_CREAT | IPC_EXCL | 0666))<0)
		{
		 if (errno == EEXIST)
		 	{
		 		shmid = shmget (key_info, N, 0666);
				shm_buf = (struct shm_addr*) shmat (shmid,NULL,0);/*给共享内存 映射地址*/
				if (shm_buf ==(void *)-1)
					{
					perror("shmat");
					exit (-1);
					}
		 	}
		 else
		 	{
		 		perror("shmget");
				exit (-1);
		 	}
		}
	else
		{
			shm_buf = (struct shm_addr*) shmat (shmid,NULL,0);/*给共享内存 映射地址*/
			if (shm_buf ==(void *)-1)
				{
				perror("shmat");
				exit (-1);
				}
		}
	bzero (shm_buf, sizeof (struct shm_addr));  /*首次分配的空间 理应清零一下*/

	while(1)
		{
			pthread_mutex_lock (&mutex_refresh);
			pthread_cond_wait (&cond_refresh, &mutex_refresh);
			while(1)
				{
					
					pthread_mutex_lock (&mutex_linklist);
					if ((node = GetLinknode (envlinkHead)) == NULL)
					{
						pthread_mutex_unlock (&mutex_linklist);
						break;  /*while 读完了 就会从这里退出*/
					}
					pthread_mutex_unlock (&mutex_linklist);
					buf = node->data;
					free (node);
					node = NULL;
					if (buf.msg_type == 'e')
						{
						 	getEnvPackage(&buf);
						}
					
				}
			/*对共享内存操作  ：把链表里面的node的data取出来利用getEnvPackage重新构造，再存入共享内存 */
			sem_p(semid,0);/*0代表信号灯的编号，这次创建的信号量里面只包含了一个信号灯  ，p是-1 不允许别人操作*/
			pthread_mutex_lock (&mutex_global);
			shm_buf->rt_status = all_info_RT;
			pthread_mutex_unlock (&mutex_global);
			sem_v (semid, 0);
			
			pthread_mutex_unlock (&mutex_refresh);
		}
}








