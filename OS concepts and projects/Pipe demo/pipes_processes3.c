#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
        exit(1);
    }

    int fd1[2];  // Pipe between cat and grep
    int fd2[2];  // Pipe between grep and sort

    if (pipe(fd1) == -1) {
        perror("Pipe 1 Failed");
        return 1;
    }
    if (pipe(fd2) == -1) {
        perror("Pipe 2 Failed");
        return 1;
    }

    pid_t p1 = fork();

    if (p1 < 0) {
        perror("Fork 1 Failed");
        return 1;
    }

    if (p1 == 0) {
        // Child process P2 (grep)
        pid_t p2 = fork();

        if (p2 < 0) {
            perror("Fork 2 Failed");
            return 1;
        }

        if (p2 == 0) {
            // Child's child process P3 (sort)
            close(fd1[0]);  // Close unused read end of first pipe
            close(fd1[1]);  // Close unused write end of first pipe
            close(fd2[1]);  // Close unused write end of second pipe

            dup2(fd2[0], STDIN_FILENO);  // Redirect stdin to read end of second pipe
            close(fd2[0]);

            execlp("sort", "sort", NULL);
            perror("execlp sort Failed");
            exit(1);
        } else {
            // Child process P2 (grep)
            close(fd1[1]);  // Close unused write end of first pipe
            close(fd2[0]);  // Close unused read end of second pipe

            dup2(fd1[0], STDIN_FILENO);  // Redirect stdin to read end of first pipe
            close(fd1[0]);

            dup2(fd2[1], STDOUT_FILENO);  // Redirect stdout to write end of second pipe
            close(fd2[1]);

            execlp("grep", "grep", argv[1], NULL);
            perror("execlp grep Failed");
            exit(1);
        }
    } else {
        // Parent process P1 (cat)
        close(fd1[0]);  // Close unused read end of first pipe
        close(fd2[0]);  // Close unused read end of second pipe
        close(fd2[1]);  // Close unused write end of second pipe

        dup2(fd1[1], STDOUT_FILENO);  // Redirect stdout to write end of first pipe
        close(fd1[1]);

        execlp("cat", "cat", "scores", NULL);
        perror("execlp cat Failed");
        exit(1);
    }

    return 0;
}