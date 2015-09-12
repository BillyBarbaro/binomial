#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// Largest 32 bit integer
#define LOOP_ITERATIONS (0x7fffffff)

pid_t printStartMessage() {

	int i;
	pid_t fork_result;

	fork_result = fork();

	if (fork_result == 0) {
		printf("(n (n-2)) binomial coefficient computations of integers n=2, 3, 10, start now!\n");
                for(i = 0; i < LOOP_ITERATIONS; i++) { }
		return EXIT_SUCCESS;
	}
	else if (fork_result > 0) { 
		wait();
		printf("I am the parent\n");
	}
	else {
		perror("fork() failed");
		return EXIT_FAILURE;
	}
	return fork_result;
}

int main() {

	pid_t start_result;
	
	start_result = printStartMessage();

	if (start_result == 0) {	
		return EXIT_SUCCESS;
	}
	else if (start_result > 0) {
		printf("I am the parent\n");
	}
	else {
		return EXIT_FAILURE;
	}
 
	return EXIT_SUCCESS;
}
