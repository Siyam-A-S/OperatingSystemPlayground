#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

time_t start_time;   // Global variable to store program start time
int count = 0;       // Counter for number of alarms

void alarm_handler(int signum) {
    printf("Hello World!\n");
    count++;
    alarm(1);  // Reschedule alarm every 1 second
}

void sigint_handler(int signum) {
    time_t end_time = time(NULL);
    printf("\nProgram ran for %ld seconds.\n", end_time - start_time);
    printf("Number of alarms: %d\n", count);
    exit(0);  // Exit after handling SIGINT
}

int main() {
    start_time = time(NULL);     // Record start time
    signal(SIGALRM, alarm_handler);  // Register handler for SIGALRM
    signal(SIGINT, sigint_handler);  // Register handler for SIGINT (Ctrl-C)
    alarm(1);  // Schedule first alarm after 1 second
    while (1) {
        pause();  // Wait for signals
    }
    return 0;
}
