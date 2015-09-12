#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/times.h>

// Largest 32 bit integer
#define LOOP_ITERATIONS (0x7fffffff)

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

	cuserid(user_id);
	euid = geteuid();
	uid = getuid();
	egid = getegid();
	gid = getgid();

	printf("Process Username: %s\n", user_id);
	printf("Effective User ID: %d\n", euid);
	printf("User ID: %d\n", uid);
	printf("Effective Group ID: %d\n", egid);
	printf("Groupd ID: %d\n", gid);
}

void print_termination_info(char* process_name) {
	printf("%s preparing to terminate.\n", process_name);
	print_current_time();
	print_process_info();
	print_process_time();
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
	}
	else if (fork_result > 0) { 
		terminated_pid = wait(&status);
		print_termination_status(terminated_pid, status);
	}
	else {
		perror("fork() failed");
		exit(EXIT_FAILURE);
	}
	return fork_result;
}

int main() {

	pid_t start_result;
	
	start_result = print_start_message();

	// If it's the child process, don't continue
	if (start_result == 0) {
		print_termination_info("First child");	
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
