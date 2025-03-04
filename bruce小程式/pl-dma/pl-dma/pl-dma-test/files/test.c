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
    int ch;
    // struct termios oldt;
    // struct termios newt;
    // tcgetattr(STDIN_FILENO, &oldt); /*store old settings */
    // newt = oldt; /* copy old settings to new settings */
    // newt.c_lflag &= ~(ICANON | ECHO); /* make one change to old settings in new settings */
    // tcsetattr(STDIN_FILENO, TCSANOW, &newt); /*apply the new settings immediatly */


    while (1)
    {
        // ch = getch();
        ch = getchar();
        if (ch == 'q')
        {
            break;
        }
        else
        {
            printf("You pressed %c\n", ch);
        }   

    }
    // tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /*reapply the old settings */
}