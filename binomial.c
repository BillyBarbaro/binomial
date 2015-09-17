#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/times.h>

// Largest 32 bit integer
#define LOOP_ITERATIONS (0x7fffffff)
#define SLEEP_INTERVAL 4

void print_current_time() {
	time_t raw_time;
	struct tm *display_time;

	time(&raw_time);
	display_time = localtime(&raw_time);
	printf("Current system time: %s", asctime(display_time));
}

void print_process_time() {
	struct tms process_time;
	clock_t result;
	double user_time_seconds;
	double system_time_seconds;

	result = times(&process_time);
	if (result == (clock_t) -1) {
		perror("times() failed\n");
		exit(EXIT_FAILURE);
	}

	user_time_seconds = (double)process_time.tms_utime / CLOCKS_PER_SEC;
	system_time_seconds = (double)process_time.tms_stime / CLOCKS_PER_SEC;

	printf("User time: %f seconds\n", user_time_seconds);
	printf("System time: %f seconds\n", system_time_seconds);
}

void print_process_info() {
	char* user_id;
	uid_t euid, uid;
	gid_t egid, gid;

	user_id = (char*) cuserid(NULL);
	euid = geteuid();
	uid = getuid();
	egid = getegid();
	gid = getgid();

	printf("Process Username: %s\n", user_id);
	printf("Effective User ID: %d\n", euid);
	printf("User ID: %d\n", uid);
	printf("Effective Group ID: %d\n", egid);
	printf("Group ID: %d\n", gid);
}

void print_termination_info(char* process_name) {
	printf("%s preparing to terminate.\n", process_name);
	print_current_time();
	print_process_info();
	print_process_time();
}

void print_exec_info(char* exec_name) {
	printf("Preparing to hand control to %s.\n", exec_name);
	print_current_time();
	print_process_info();
	printf("\n");
}

void print_termination_status(pid_t pid, int status) {
	printf("Child process %d exited with status %d\n\n", pid, WEXITSTATUS(status));
}

void fork_failure() {
	perror("fork() failure");
	exit(EXIT_FAILURE);
}

void spend_CPU_time() {
	int i;
	for(i = 0; i < LOOP_ITERATIONS; i++) { }
}

pid_t print_start_message() {

	int i;
	int status;
	pid_t fork_result;

	fork_result = fork();

	if (fork_result == 0) {
		printf("\n(n (n-2)) binomial coefficient computations of integers n=2, 3, 10, start now!\n");
		spend_CPU_time();
	}
	else if (fork_result > 0) { 
		wait(&status);
		print_termination_status(fork_result, status);
	}
	else {
		fork_failure();
	}
	return fork_result;
}

int factorial(int i) {
	int result = 1;
	for(i; i > 1; i--) {
		result = result * i;
	}
	return result;
}

int calculate_binomial(int total, int chosen) {
	int total_factorial, chosen_factorial, difference_factorial;

	total_factorial = factorial(total);
	chosen_factorial = factorial(chosen);
	difference_factorial = factorial(total - chosen);

	// This quantity will always be an integer value
	return total_factorial / (chosen_factorial * difference_factorial);
}

void print_binomials(int start) {
	int i;
	int binomial_coef;

	for(i = start; i < 11; i += 2) {
		binomial_coef = calculate_binomial(i, i-2);
		printf("The process %d calculated %i choose %i is %i.\n", getpid(), i, i-2, binomial_coef);
		sleep(SLEEP_INTERVAL);
	}
	spend_CPU_time();
}

pid_t generate_binomials() {

	int status;
	pid_t first_fork_result;
	pid_t second_fork_result;
	pid_t fork_result;

	first_fork_result = fork();

	// If child, print even binomials
	if (first_fork_result == 0) {
		print_binomials(2);
		fork_result = first_fork_result;
	}
	else if (first_fork_result > 0) {
		// Wait half the sleep interval to spread the 2 processes apart
		sleep(SLEEP_INTERVAL/2);

		second_fork_result = fork();

		// If child, print odd binomials
		if (second_fork_result == 0) {
			print_binomials(3);
			fork_result = second_fork_result;
			printf("\n");
		}
		// If parent, wait on 2 children to terminate
		else if (second_fork_result > 0) {
			wait(&status);
			print_termination_status(first_fork_result, status);
			wait(&status);
			print_termination_status(second_fork_result, status);
		}
		else {
			fork_failure();
		}
		fork_result = second_fork_result;
	}
	else {
		fork_failure();
	}

	return fork_result;
}

void run_ls() {
	int status;
	int fork_result;
	const char* ls_file = "ls";
	char* const ls_args[] = {"ls", "-l", NULL};

	fork_result = fork();

	if (fork_result == 0) {
		print_exec_info("ls");
		execvp(ls_file, ls_args);
	}
	else if (fork_result > 0) {
		wait(&status);
		print_termination_status(fork_result, status);
	}
	else {
		fork_failure();
	}
}

int main() {

	pid_t start_result;
	pid_t binomial_result;

	start_result = print_start_message();

	// If it's the child process, don't continue
	if (start_result == 0) {
		print_termination_info("First child");	
		return EXIT_SUCCESS;
	}

	binomial_result = generate_binomials();
	
	// If it is the child process, don't continue
	if (binomial_result == 0) {
		print_termination_info("Binomial child");
		return EXIT_SUCCESS;
	}

	run_ls();

	spend_CPU_time();

	print_termination_info("\nParent process");
	return EXIT_SUCCESS;
}
