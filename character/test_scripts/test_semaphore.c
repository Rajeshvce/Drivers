#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>
#include <errno.h>

#define DEVICE_NAME "/dev/my_cdev"

int main() {
    int fd;
    pid_t pid1, pid2;
    int8_t write_val1[1024]; // Value to write from process 1
    int8_t write_val2[1024]; // Value to write from process 2

    // Open the device file
    printf("Opening Driver\n");
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Cannot open device file");
        return 1;
    }

    // Get input values for both processes
    printf("Enter write_val1 for Process 1: ");
    scanf(" %[^\t\n]s", write_val1);

    printf("Enter write_val2 for Process 2: ");
    scanf(" %[^\t\n]s", write_val2);

    // Fork the first child process
    pid1 = fork();
    if (pid1 < 0) {
        perror("Failed to fork");
        close(fd);
        return 1;
    }

    if (pid1 == 0) {
        // Child process 1: Try to write to the device
        printf("Child process 1 (PID %d) trying to write...\n", getpid());
        if (write(fd, write_val1, strlen(write_val1)+1) < 0) {
            perror("Write failed");
        } else {
            printf("Child process 1 (PID %d) wrote value: %s\n", getpid(), write_val1);
        }
        close(fd);
        return 0;
    } else {
        // Fork the second child process
        pid2 = fork();
        if (pid2 < 0) {
            perror("Failed to fork");
            close(fd);
            return 1;
        }

        if (pid2 == 0) {
            // Child process 2: Try to write to the device
            printf("Child process 2 (PID %d) trying to write...\n", getpid());
            if (write(fd, write_val2, strlen(write_val2)+1) < 0) {
                perror("Write failed");
            } else {
                printf("Child process 2 (PID %d) wrote value: %s\n", getpid(), write_val2);
            }
            close(fd);
            return 0;
        } else {
            // Parent process: Wait for both children to finish
            wait(NULL);
            wait(NULL);
            close(fd);
            printf("Parent process completed.\n");
            return 0;
        }
    }
}
