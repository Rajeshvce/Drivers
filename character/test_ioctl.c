#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
 
#define READ_CHARACTER_DRIVER _IOR('c','1', int*)
#define WRITE_CHARACTER_DRIVER _IOW('c','2',int*)

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
 
        printf("Closing Driver\n");
        close(fd);
}
