


#include "sym_global.h"

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

extern int msgid;  /*global.c文件里面的变量需要extern*/
extern int shmid;
extern int semid;

extern int dev_camera_fd;
extern int dev_uart_fd;


pthread_t 	id_zigbee_rcv,
			id_uart_cmd,
			id_main_thread,
			id_camera,
			id_refresh;


void ReleaseResource (int signo)
{
	pthread_cancel (id_zigbee_rcv);
	pthread_cancel (id_uart_cmd);
	pthread_cancel (id_main_thread);
	pthread_cancel (id_camera);
	pthread_cancel (id_refresh);

	pthread_mutex_destroy (&mutex_linklist);
	pthread_mutex_destroy (&mutex_global);
	pthread_mutex_destroy (&mutex_refresh_updata);
	pthread_mutex_destroy (&mutex_refresh);
	pthread_mutex_destroy (&mutex_camera);
	pthread_mutex_destroy (&mutex_main_thread);
	pthread_mutex_destroy (&mutex_uart_cmd);
	pthread_mutex_destroy (&mutex_zigbee_rcv);
	pthread_mutex_destroy (&mutex_slinklist);

	pthread_cond_destroy (&cond_refresh_updata);
	pthread_cond_destroy (&cond_refresh);
	pthread_cond_destroy (&cond_camera);
	pthread_cond_destroy (&cond_main_thread);
	pthread_cond_destroy (&cond_uart_cmd);

	msgctl (msgid, IPC_RMID, NULL);
	shmctl (shmid, IPC_RMID, NULL);
	semctl (semid, 1, IPC_RMID, NULL);


	close (dev_camera_fd);
	close (dev_uart_fd);


	printf ("All quit\n");

	exit(0);
}




int main(int argc, char ** argv)
{





pthread_create (&id_zigbee_rcv, 0, pthread_zigbee_rcv, NULL);
//sleep (1);
pthread_create (&id_uart_cmd, 0, pthread_uart_send, NULL);
pthread_create (&id_main_thread, 0, pthread_main, NULL);
pthread_create (&id_camera, 0, pthread_camera, NULL);
pthread_create (&id_refresh, 0, pthread_refresh, NULL);



signal (SIGINT, ReleaseResource);  




pthread_join (id_zigbee_rcv, NULL);
printf ("g1\n");
pthread_join (id_uart_cmd, NULL);
printf ("g2\n");
pthread_join (id_main_thread, NULL);
printf ("g3\n");
pthread_join (id_camera, NULL);
printf ("g4\n");
pthread_join (id_refresh, NULL);
printf ("g5\n");
























}













































