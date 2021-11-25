#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

/************** 
* firstdrvtest on
* firstdrvtest off
**************/
int main(int argc, char **argv)
{
    int fd;
    int val = 1;
    fd = open("/dev/led_gpio0", O_RDWR);  
    if (fd < 0)
    {
        printf("can't open!\n");
    }
    if (argc != 2)      
    {
        printf("Usage :\n");
        printf("%s <on|off>\n", argv[0]);
        return 0;
    }
    if (strcmp(argv[1], "on") == 0)    
    {
        val = 1;
		printf("cmd led on\n");
    }
	else if (strcmp(argv[1], "off") == 0)
    {
        val = 0;
		printf("cmd led off\n");
    }
	else
	{
		printf("Usage :\n");
        printf("%s <on|off>\n", argv[0]);
		return -1;
	}
    
    write(fd, &val, 4);
    return 0;
}

