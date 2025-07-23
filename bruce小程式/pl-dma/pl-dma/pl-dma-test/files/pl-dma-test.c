/*
* Copyright (C) 2013 - 2016  Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in this
* Software without prior written authorization from Xilinx.
*
*/

#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h> 	
#include <unistd.h>    		
#include <time.h>
#include <sys/ioctl.h>	
#include <sys/sendfile.h>
#include <sys/mman.h>

#include "os_wrap.h"
#include "AF_AXI_DMA_ctrl.h"

int getch(void)
{
  int ch;
  struct termios oldt;
  struct termios newt;
  
  tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
  newt = oldt; /* copy old settings to new settings */
  newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
  
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */
  
  ch = getchar(); /* standard getchar call */
  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
  
  return ch; /*return received char */
}

int main(int argc, char **argv)
{
	void *ip_addr = NULL;
	char ch;
	int quit=0;
	int bwrite = 0; 
	unsigned long data;
	AF_AXI_DMA_ctrl_t af_axi_dma_ctrl;

	AF_DMA_Initial(&af_axi_dma_ctrl, 0x87000000, 0, 0);
	
//	if ((ip_addr = prepare_memory(0x87000000)) == NULL)
//	{
//		fprintf(stderr, "prepare_memory failed\n");
//		exit(1);
//	}
	ip_addr = af_axi_dma_ctrl.IP_address;
	
	while(!quit)
	{
		ch = getch();
		switch (ch)
		{
			case 'q':
				quit = 1;
				break;
			case 'w':
				printf("write test: enter 1/2/4/8\n");
				break;
			case 'r':
				data = bus_rd32(ip_addr+4);
				printf("read: value=%x\n", data);
				break;
			case '1':
			case '2':
			case '4':
			case '8':
				data = ch - 0x30; 
				bus_wr32(ip_addr+0, data);
			default:
				printf("\n'w' for write test, 'r' for read test, 'q' for quit\n");
				break; 
		}
	}

	if(ip_addr)
		detroy_memory(ip_addr);
    return 0;
}
