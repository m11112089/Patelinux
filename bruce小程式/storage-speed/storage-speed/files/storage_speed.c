#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include <time.h>

#define RESERVED_MEMORY_OFFSET  0x32000000
#define MAP_SIZE 0x07000000 // 112MB
unsigned char *data_ptr;

void usage()
{
	printf("usage: storage_speed -l locate -s file_size -c count [-f]\n");
	printf("\texample storage_speed -l /run/media/mmcblk0p2/ -s 22020096 -c 100\n");
}
int main(int argc, char **argv) {
	int fd;
	int length=0; 
	int file_no = 0;
	unsigned char *reserved_memory;
	FILE *fhandle;
	struct timespec tp1,tp2;
	long t_diff;
	int i;
	int file_size, loop_count;
	char *filepath;
	char filename[40];
    int cmd_opt = 0;
    int bsync=0;
    
    i=0;
    while(1)
	{
        cmd_opt = getopt(argc, argv, "l:s:c:f");
        if (cmd_opt == -1)
            break;

        switch (cmd_opt)
        {
            case 'l':
				filepath = optarg;
				i++;
                break;
            case 's':
				file_size = atoi(optarg);
				i++;
                break;
            case 'c':
				loop_count = atoi(optarg);
				i++;
                break;
            case 'f':
				bsync = 1;
                break;
            default:
                usage();
                exit(-1);
                break;
        }
    }
    if (i != 3)
    {
		usage();
		exit(-1);
	}
	else
		printf("filepath=%s file_size=%d loop_count=%d [%s]\n", filepath, file_size, loop_count, bsync ?"true" :"false");
		
#if 1
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	reserved_memory = (char *) mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, RESERVED_MEMORY_OFFSET);
    close(fd);
	if (reserved_memory == MAP_FAILED) {
		printf("Failed to creating mapping.\n");
		printf("ERRNO: %s\n", strerror(errno));
		return -1;
	}

    printf("Memory mapped at address %p mode(%d).\n", reserved_memory);
    do{
		if(length % (1024 * 1024) == 0)
			printf(".");
			reserved_memory[length] = length % 256;
			
	    length++;
    }  while (length < file_size);
    printf("\nInit content finished\n");

	for(i=0; i<loop_count; i++)	
	{
		clock_gettime(CLOCK_MONOTONIC_RAW, &tp1) ;
		sprintf(filename, "%sfile-%d", filepath, file_no++);
		fhandle=fopen(filename,"wb");
		if(fhandle == NULL){
			printf("%s create failed.\n", filename);
			continue;
		}
		fwrite(reserved_memory, 1, file_size, fhandle);
		if(bsync)
			fsync(fhandle);
			
        fclose(fhandle);

		clock_gettime(CLOCK_MONOTONIC_RAW, &tp2) ;
		t_diff = ((tp2.tv_sec-tp1.tv_sec)*(1000*1000*1000) + (tp2.tv_nsec-tp1.tv_nsec)) / 1000 ;
		printf("%lu usec passed\n",t_diff) ;
	}
	
ERROR_EXIT:
    if (munmap(reserved_memory, MAP_SIZE) == -1)
        printf("Can't unmap memory from user space.\n");

#endif	
	printf("end testing\n");
    return 0;
}
