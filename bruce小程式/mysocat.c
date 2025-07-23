#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>

#define IMG_PATH	"img.bin"

unsigned char *image_ptr=NULL;
int image_size;

int prepare_memory(const char *filepath)
{	
    struct stat statbuf;
    int fd;
    int err;
    
    fd = open(filepath, O_RDONLY);
    if(fd < 0){
        printf("\n\"%s \" could not open\n",
               filepath);
        return -1;
    }

    err = fstat(fd, &statbuf);
    if(err < 0){
        printf("\n\"%s \" could not open\n",
                       filepath);
        return -2;
    }

	image_size = statbuf.st_size;
    image_ptr = mmap(NULL,statbuf.st_size,
            PROT_READ|PROT_WRITE,MAP_PRIVATE,
            fd,0);
    if(image_ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return -3;
    }
    close(fd);
}

void assert(int b)
{
	if(b)
	{
		printf("ASSERT:%s%s(%d)\n", __FILE__, __FUNCTION__, __LINE__);
		exit(1);
	}
}

void detroy_memory()
{
	int err;
	
	if(image_ptr)
	{
		err = munmap(image_ptr, image_size);
		if(err != 0)
			printf("UnMapping Failed\n");
    }
}

int socat(char *server_ip, int server_port)
{
	int sockfd;
	struct sockaddr_in servaddr;
	int len;
	
	// socket create and varification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(server_ip);
	servaddr.sin_port = htons(server_port);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	} else{
		printf("connected to the server..\n");
	}

	len = write(sockfd,image_ptr,image_size);
	printf("read %d bytes from socket\n",len);
	close(sockfd);	
}

int main(int argc, char *argv[])
{	
	if(prepare_memory(IMG_PATH) != 0)
		exit(0);
		
	socat("127.0.0.1", 5001);
	
	detroy_memory();
}
