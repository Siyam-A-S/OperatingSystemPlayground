// OS Project 2: Part 1
//Team member: Siyam Shahriar, Prasun Dhungana

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>

void DadProcess(int *SharedMem, sem_t *mutex);
void ChildProcess(int *SharedMem, sem_t *mutex);
void MomProcess(int *SharedMem, sem_t *mutex);

int main(int argc, char *argv[]) {
    

    if (argc != 3) {
        printf("Usage: %s <Number of Parents> <Number of Students>\n", argv[0]);
        exit(1);
    }

    int num_of_parents = atoi(argv[1]);
    int num_of_students = atoi(argv[2]);

    if (num_of_parents < 1 || num_of_parents > 2 || num_of_students < 1) {
        printf("Invalid input. Must have 1/2 parent(s) and 1 student.\n");
        exit(1);
    }
    
    srand(time(NULL));

    int ShmID;
    int *ShmPTR;
    sem_t *mutex;
    pid_t pid;

     // Shared memory for the BankAccount
    ShmID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        perror("shmget error");
        exit(1);
    }

    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1) {
        perror("shmat error");
        exit(1);
    }

    *ShmPTR = 0; // Initialize BankAccount
    printf("Initial Bank Balance = %d\n", *ShmPTR);

    // Create semaphore
    mutex = sem_open("bankaccount", O_CREAT, 0644, 1);
    if (mutex == SEM_FAILED) {
        perror("Failed to open semaphore");
        exit(1);
    }

    

    // Fork parents (Dear Old Dad and/or Lovable Mom)
    for (int i = 0; i < num_of_parents; i++) {
        pid = fork();
        if (pid < 0) {
            perror("parent fork error");
            exit(1);
        } else if (pid == 0) {
            if (i == 0)
                DadProcess(ShmPTR, mutex);
            else
                MomProcess(ShmPTR, mutex);
            exit(0);
        }
    }

    // Fork children (Poor Students)
    for (int i = 0; i < num_of_students; i++) {
        pid = fork();
        if (pid < 0) {
            perror("child fork error");
            exit(1);
        } else if (pid == 0) {
            ChildProcess(ShmPTR, mutex);
            exit(0);
        }
    }

    // Wait for all child processes to complete
    while (wait(NULL) > 0);
    printf("All processes complete. Cleaning up...\n");

    // Detach and remove shared memory
    shmdt((void *)ShmPTR);
    shmctl(ShmID, IPC_RMID, NULL);

    // Unlink semaphore
    sem_close(mutex);
    sem_unlink("bankaccount");

    return 0;
}
void DadProcess(int *SharedMem, sem_t *mutex) {
    while (1) {
        sleep(rand() % 6); // Sleep 0-5 seconds
        printf("Dear Old Dad: Attempting to Check Balance\n");

        sem_wait(mutex); // Enter critical section
        int localBalance = *SharedMem;

        if (localBalance < 100) {
            int amount = rand() % 101; // (0-100)
            if (amount % 2 == 0) {
                localBalance += amount;
                printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", amount, localBalance);
                *SharedMem = localBalance;
            } else {
                printf("Dear Old Dad: Doesn't have any money to give\n");
            }
        } else {
            printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", localBalance);
        }

        sem_post(mutex); // Exit critical section
    }
}

void ChildProcess(int *SharedMem, sem_t *mutex) {
    while (1) {
        sleep(rand() % 6); // Sleep 0-5 seconds
        printf("Poor Student: Attempting to Check Balance\n");

        sem_wait(mutex); // Enter critical section
        int localBalance = *SharedMem;

        int need = rand() % 51; // (0-50)
        printf("Poor Student needs $%d\n", need);

        if (need <= localBalance) {
            localBalance -= need;
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, localBalance);
            *SharedMem = localBalance;
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", localBalance);
        }

        sem_post(mutex); // Exit critical section
    }
}

void MomProcess(int *SharedMem, sem_t *mutex) {
    while (1) {
        sleep(rand() % 6); // Sleep 0-5 seconds
        printf("Lovable Mom: Attempting to Check Balance\n");

        sem_wait(mutex); // Enter critical section
        int localBalance = *SharedMem;

        if (localBalance < 100) {
            int amount = rand() % 126; // (0-125)
            localBalance += amount;
            printf("Lovable Mom: Deposits $%d / Balance = $%d\n", amount, localBalance);
            *SharedMem = localBalance;
        }

        sem_post(mutex); // Exit critical section
    }
}