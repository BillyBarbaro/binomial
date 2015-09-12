#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// Largest 32 bit integer
#define LOOP_ITERATIONS (0x7fffffff)

void print_process_info() {
	
}

void print_termination_status(pid_t pid, int status) {
	printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
}

pid_t print_start_message() {

	int i;
	int status;
	pid_t fork_result;
	pid_t terminated_pid;

	fork_result = fork();

	if (fork_result == 0) {
		printf("(n (n-2)) binomial coefficient computations of integers n=2, 3, 10, start now!\n");
                for(i = 0; i < LOOP_ITERATIONS; i++) { }
		return EXIT_SUCCESS;
	}
	else if (fork_result > 0) { 
		terminated_pid = wait(&status);
		print_termination_status(terminated_pid, status);
	}
	else {
		perror("fork() failed");
	}
	return fork_result;
}

int main() {

	pid_t start_result;
	
	start_result = print_start_message();

	if (start_result == 0) {	
		return EXIT_SUCCESS;
	}
	else if (start_result < 0) {
		return EXIT_FAILURE;
	}
 
	return EXIT_SUCCESS;
}
