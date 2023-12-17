#ifndef __SEM_H__
#define __SEM_H__

#include "sym_global.h"
/*信号量的值与相应资源的使用情况有关，当它的值大于 0 时，表示当前可用的资源数的数量；当它的值小于 0 时，其绝对值表示等待使用该资源的进程个数。信号量的值仅能由 PV 操作来改变*/
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
								(Linux-specific) */
};

int init_sem(int semid, int num, int val)
{
	union semun myun;
	myun.val = val;
	if(semctl(semid, num, SETVAL, myun) < 0)
	{
		perror("semctl");
		exit(1);
	}
	return 0;
}

int sem_p(int semid, int num)
{   /*p操作 减资源操作*/
	struct sembuf mybuf;
	mybuf.sem_num = num;
	mybuf.sem_op = -1;
	mybuf.sem_flg = SEM_UNDO;
	if(semop(semid, &mybuf, 1) < 0)
	{
		perror("semop");
		exit(1);
	}

	return 0;
}

int sem_v(int semid, int num)
{  /*v操作 加资源操作*/
	struct sembuf mybuf;
	mybuf.sem_num = num;
	mybuf.sem_op = 1;
	mybuf.sem_flg = SEM_UNDO;
	if(semop(semid, &mybuf, 1) < 0)
	{
		perror("semop");
		exit(1);
	}

	return 0;
}

#endif
