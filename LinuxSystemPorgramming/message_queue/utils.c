#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <stdint.h>
#include <sys/wait.h>

#include "utils.h"

int mq_get(char id)
{
	int msqid;
	key_t key;
	
	if ((key = ftok(MSG_FILE, id)) < 0) 
	{
		perror("ftok error");
		return -1;
	}

	if ((msqid = msgget(key, IPC_CREAT|0777)) == -1) 
	{
		perror("msgget error");
		return -1;
	}
	
	return msqid;
}


int log_count(int count)
{
	char command[128];
	
	sprintf(command, "echo %d > %s", count, STORAGE_LOG_FILE);
	printf("==%s\n", command);
	system(command);
}	
