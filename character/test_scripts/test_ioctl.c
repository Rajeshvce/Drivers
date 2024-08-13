#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
 
#define READ_CHARACTER_DRIVER   _IOR('c','1', int*)
#define WRITE_CHARACTER_DRIVER  _IOW('c','2',int*)
#define WRITE_PUT               1
#define READ_GET                0

int main()
{
        int fd;
        int val,number;
 
        printf("\nOpening Driver\n");
        fd = open("/dev/my_cdev", O_RDWR);
        if(fd < 0) {
                printf("Cannot open device file...\n");
                return 0;
        }
 
        printf("Enter the Value to send\n");
        scanf("%d", &val);
        printf("Writing Value to Driver\n");
        ioctl(fd, WRITE_CHARACTER_DRIVER, (int*) &val); 
 
        printf("Reading Value from Driver\n");
        ioctl(fd, READ_CHARACTER_DRIVER, (int*) &number);
        printf("Value is %d\n", number);
 
        int val1,number1;
        printf("Enter the value to send using the put user\n");
        scanf("%d", &val1);
        printf("Writing value to Driver\n");
        ioctl(fd, WRITE_PUT, (int*) &val1);

        printf("Reading the written value using get_user\n");
        ioctl(fd, READ_GET, (int*) &number1);
        printf("value is %d\n", number1);
        
        printf("Closing Driver\n");
        close(fd);
}
