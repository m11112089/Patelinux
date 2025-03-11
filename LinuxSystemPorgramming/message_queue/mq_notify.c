#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

#include "utils.h"
#define CMD_IMG_ETHERNET_COUNTER 4
int main(int argc, char *argv[])
{
	int msqid;
	key_t key;
	struct msg_form msg;
	int cmd = 0;

	// if(argc > 1)
	// 	cmd = atoi(argv[1]);
		
	if ((msqid = mq_get(MQ_ID0)) == -1) 
	{
		perror("msgget error");
		exit(1);
	}
	printf("mq_notify running\n");
	msg.m_data.cmd = CMD_IMG_ETHERNET_COUNTER;	// vis-ctrl.h: CMD_IMG_ETHERNET_COUNTER = 4
	msg.m_type = IMG_TO_CTRL;
	msgsnd(msqid, &msg, sizeof(m_data_t), 0);

}
