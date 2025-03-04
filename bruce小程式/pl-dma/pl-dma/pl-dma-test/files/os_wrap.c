#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __linux__
#ifdef __linux__

#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

#include "os_wrap.h"

int get_devpath(void *match, char *store, int mode)
{
	FILE *fp;
	char filename[128];
	char buff[64];
	int i=0;
	int iret = -1;
    struct stat state;
	while(1)
	{
		unsigned long value = 0;

		sprintf(filename, "/sys/class/uio/uio%d", i);

		if (stat(filename, &state) != 0)
			break; 

		sprintf(filename, "/sys/class/uio/uio%d/maps/map0/addr", i);
		fp = fopen(filename, "r");
		fread(buff, 1, 64, fp);

		value = strtol(buff, NULL, 16);
		if(value == (unsigned long) match)
		{
			sprintf(store, "/dev/uio%d", i);
			iret=0;
			break;
		}
		i++;
	}
	
	return iret;
}

void *prepare_memory(void * base_addr)
{	
    struct stat statbuf;
	char devpath[64];
    void *mm_ptr;
    int fd;
    int err;

	
	if(get_devpath(base_addr, &devpath[0], 0) == -1)
		return -1;
    printf("devpath = %s\n", devpath);
    fd = open(devpath, O_RDWR);
    if(fd < 0){
        printf("\n\"%s \" could not open\n",
               devpath);
        return NULL;
    }

    mm_ptr = mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE,MAP_SHARED, fd,0);
    if(mm_ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return NULL;
    }
    
    close(fd);
    return mm_ptr;
}

void detroy_memory(void *base_addr)
{
	int err;
	
	if(base_addr)
	{
		err = munmap(base_addr, MAP_SIZE);
		if(err != 0)
			printf("UnMapping Failed\n");
    }
}

#endif
