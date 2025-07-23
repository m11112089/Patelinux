#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <fcntl.h>
#include <getopt.h>

#define dprintf printf
//#define dprintf 
#define PATH_ORIGINAL_PRIFIX "./original/imag-00000-%d"
#define PATH_VCU_PREFIX "./vcu/imag-00000-%d.h265"
#define MAX_PATH 256

#define AIRS_HDR	0x12000000
#define VIS_HDR		0x12000020

static inline uint8_t *put32(uint8_t *ptr, uint32_t data)
{
	*ptr++ = (data & 0xFF000000) >> 24;
	*ptr++ = (data & 0x00FF0000) >> 16;
	*ptr++ = (data & 0x0000FF00) >> 8;
	*ptr++ = (data & 0x000000FF);
	return ptr;
}

static inline uint8_t *put16(uint8_t *ptr, uint32_t data)
{
	*ptr++ = (data & 0x0000FF00) >> 8;
	*ptr++ = (data & 0x000000FF);
	return ptr;
}

uint8_t *get_ddr_memory(int size)
{
	return ((uint8_t *) malloc(sizeof(char) * size));
}

void free_ddr_memory(uint8_t *ptr)
{
	free(ptr);	
}


static uint16_t get_information(char *filename)
{
	#define CD_ENABLE 0x0001
	#define CD_CLOUDY 0x0002
	
	uint16_t info=0;
	FILE *fp;
	int covery=0;
	char buff[9];
	char filepath[MAX_PATH];

	sprintf(filepath, "%s.cd", filename);
	fp = fopen(filepath, "rb");
	if (!fp)
		return 0;
	
	info |= CD_ENABLE;

	bzero(buff, 9);
	fread(buff, sizeof(char), 8, fp);
	
	covery = atoi(buff);
	if(covery > 65)
		info |= CD_CLOUDY;
	
	dprintf("covery=%d info=%d\n", covery, info);
	return info;
}

uint8_t *pack_header(uint8_t *ptr, uint8_t mode, uint32_t uct)
{	
	ptr = put32(ptr, AIRS_HDR);
	ptr = put32(ptr, VIS_HDR);
	*ptr++ = mode;
	ptr = put32(ptr, uct);
}

uint8_t *pack_block(uint8_t *ptr, char *filename, uint32_t index, uint32_t data_lenth)
{
	uint16_t info = get_information(filename);
	FILE *fp;
	uint32_t nbytes = data_lenth;
	int nread;
	
	ptr = put32(ptr, index);
	ptr = put16(ptr, info);
	ptr = put32(ptr, data_lenth);
	
	fp = fopen(filename, "rb");
	if (!fp)
		return NULL;
	while (nbytes)
	{
		if( !(nread = fread(ptr, sizeof(char), nbytes, fp)) )
		{
			fclose(fp);
			dprintf("%s read failed\n", filename);
			return NULL;
		}
			
		nbytes -= nread;
		ptr += nread;
		dprintf("%s read %d bytes\n", filename, nread);
	}
	fclose(fp);
	return ptr;
}

void usage(const char app_name[])
{
	dprintf("usage:%s -m mode -U UCT -p file_path_prefix\n", app_name);
	dprintf("  example: %s -m 1 -U 1 -p '/images/imag-\%%d'\n", app_name);
	exit(0);
}

int main (int argc, char *argv[])
{
	char filepath[MAX_PATH];
	struct stat statbuf;
	int i=0;
	int iret;
	uint64_t offset=0;
	uint8_t *ptr, *start;
	uint32_t total;
	uint8_t mode;
	uint32_t UTC;
	uint8_t *file_prefix;
	uint8_t need=0;
	
    int opt;

	while ((opt = getopt (argc, argv, "m:U:p:")) != -1)
	{
		switch (opt)
		{
			case 'm':
				mode = atoi(optarg) & 0xFF;
				need |= 1;
				break;
			case 'U':
				UTC = atoi(optarg) & 0xFFFFFFFF;
				need |= 2;
				break;
			case 'p':
				file_prefix = optarg;
				need |= 4;
				break;
			default: /* '?' */
				usage (argv[0]);
		}
	}
	if( need != 7 )
		usage(argv[0]);
	
	start = ptr = get_ddr_memory(32 * 1024 * 1024);

	ptr = pack_header(ptr, 1, 1);
	for(;;i++)
	{
		sprintf(filepath, file_prefix, i);
		
		iret = stat(filepath, &statbuf);
        if ( iret == -1 )
		{
			total = ptr - start;
			FILE *fp = fopen("./spi_data.bin", "wb");
			int nwrite;

			if(fp)
			{
				nwrite=fwrite(start, 1, total, fp);
				dprintf("finished total:%ld bytes write %ld bytes to ./spi_data.bin\n", total);
				fclose(fp);
			}
			else
				dprintf("backup to file failed\n"); 
			
			break;
		} 
		else
		{
			dprintf("%s: %d\n", filepath, statbuf.st_size);
			ptr = pack_block(ptr, filepath, i, statbuf.st_size);
			if(ptr == NULL)
			{
				dprintf("Fatal error !!!\n");
				break;
			}
		}
	}
	
	free_ddr_memory(start);
	
	printf("%d", total);
}
