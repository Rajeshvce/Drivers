#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define DEVICE_NAME "/dev/my_cdev"

int main() {
    int fd;
    pid_t pid;
    int8_t write_val[1024]; // Value to write to the driver
    int8_t read_val[1024];      // Value read from the driver

    // Open the device file
    printf("Opening Driver\n");
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Cannot open device file");
        return 1;
    }

    // Fork a child process
    pid = fork();
    if (pid < 0) {
        perror("Failed to fork");
        close(fd);
        return 1;
    }

    if (pid == 0) {
        // Child process: Try to read from the device and go to sleep
        printf("Child process (PID %d) trying to read...\n", getpid());
        sleep(10); // as it a test application to test the wait queue wait to make the writing operation
        if (read(fd, read_val, strlen(read_val)+1) < 0) {
            perror("Read failed");
        } else {
            printf("Child process (PID %d) read value: %s\n", getpid(), read_val);
        }
        close(fd);
        return 0;
    } else {
        // Parent process: Sleep briefly to ensure child sleeps first, then write
        //sleep(2); // Ensure the child has time to sleep
        printf("Enter the value for writing"); 
        scanf("  %[^\t\n]s", write_val);
        printf("Parent process (PID %d) writing to wake up the child...\n", getpid());
        if (write(fd, write_val, strlen(write_val)+1) < 0) {
            perror("Write failed");
        }
        // Wait for the child process to finish
        wait(NULL);
        close(fd);
        printf("Parent process completed.\n");
        return 0;
    }
}
