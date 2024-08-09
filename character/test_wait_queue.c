#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define DEVICE_NAME "/dev/my_cdev"

int main() {
    int fd;
    pid_t pid;
    int write_val = 1; // Value to write to the driver
    int read_val;      // Value read from the driver

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
        if (read(fd, &read_val, sizeof(read_val)) < 0) {
            perror("Read failed");
        } else {
            printf("Child process (PID %d) read value: %d\n", getpid(), read_val);
        }
        close(fd);
        return 0;
    } else {
        // Parent process: Sleep briefly to ensure child sleeps first, then write
        sleep(2); // Ensure the child has time to sleep
        printf("Parent process (PID %d) writing to wake up the child...\n", getpid());
        if (write(fd, &write_val, sizeof(write_val)) < 0) {
            perror("Write failed");
        }
        // Wait for the child process to finish
        wait(NULL);
        close(fd);
        printf("Parent process completed.\n");
        return 0;
    }
}
