
#include "sym_global.h"

extern unsigned char dev_camera_mask;/*与原处不同*/
 /*如果不用extern 可以从别的地方传递过来吗*/
int dev_camera_fd;/*与原处不同*/
 extern pthread_mutex_t mutex_camera;
 
 extern pthread_cond_t cond_camera;

void *pthread_camera (void *arg)
{
	 unsigned char picture=0;

# if 1
	if ((dev_camera_fd = open (DEV_CAMERA, O_RDWR | O_NOCTTY | O_NDELAY))<0 )
		{
		perror ("dev_camera_fd\n");
		//return -1;
		}
	printf ("pthread_camera is ok\n");
#endif
	while(1)
		{
			pthread_mutex_lock (&mutex_camera);
			pthread_cond_wait (&cond_camera, &mutex_camera);
			picture = dev_camera_mask;
			pthread_mutex_unlock (&mutex_camera);
#if 1
			for (;picture>0;picture--)
				{
					system("echo one > /tmp/webcom");  //ok
					printf("picture = %d\n", picture);
					sleep(4);
				}
#endif
		}
}







