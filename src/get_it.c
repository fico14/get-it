#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MAX_NUM_TABS 	5
#define URL_LENGTH 	200

static char filename[MAX_NUM_TABS][20];
static int fd[MAX_NUM_TABS] = {0};
static int pids[MAX_NUM_TABS] = {0};
static int num_procs = 0;
static CURL *curl;

void sighandler(int signum)
{
	printf("\nExiting ...\n");

	curl_easy_cleanup(curl);

	for(int i = 0; i < num_procs; i++) {
		kill(pids[i], SIGKILL);
		close(fd[i]);
		remove(filename[i]);
#ifdef DEBUG
		printf("Deleted: %s\n", filename[i]);
#endif
	}

	exit(0);
}

size_t writefunc(void *ptr, size_t size, size_t nmemb)
{
	int buff_size = size*nmemb;
	return write(fd[num_procs], ptr, buff_size);
}

int main(int argc, char **argv)
{
	CURLcode res;
	char cmd[80] = {0};
	char url[200] = {0};
	int status = 0;
	char c;
	struct sigaction sa;

	sa.sa_handler = sighandler;
	sigaction(SIGINT, &sa, NULL);

	if (argc != 1) {
		printf("Program requires no arguments.\n"
			"It will automatically open links in Firefox\n");
		return 0;
	}

	printf("******************************************************************\n");
	printf("**************** get_it, Telesport Cracker v1.0 ******************\n");
	printf("******************************************************************\n");

	while(1) {
		curl = curl_easy_init();
		if (!curl) {
			printf("Error in library.");
			goto exit;
		}
		printf("\nEnter URL: ");
		scanf("%199s", url);
		while(1) {
                    c = getchar();
                    if (c == ' ' || c == '\n' || c == EOF) 
			    break;
                }
		printf("\n");

		if (strcmp(url, "quit") == 0)
			break;

		if (strcmp(url, "exit") == 0)
			break;

		if (num_procs == MAX_NUM_TABS) {
			printf("No more tabs can be open.\n"
			       "You can exit using 'quit' or 'exit' command\n");
			continue;
		}

		sprintf(filename[num_procs], "response%d.html", num_procs);
		fd[num_procs] = open(filename[num_procs],
					O_CREAT | O_RDWR | O_APPEND,
					0644);
		if (fd[num_procs] == -1) {
			printf("Could not create file, err = %d\n", errno);
			goto exit;
		}

		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		res = curl_easy_perform(curl);

		pid_t pid = fork();
		switch(pid) {
			case -1:   // fork failed
				printf("Internal error ...\n");
#ifdef DEBUG
				printf("fork() failed\n");
#endif
				break;

			case 0:    // child process
				memset(cmd, 0, 80);
				sprintf(cmd, "firefox %s", filename[num_procs]);
				system(cmd);
				exit(0);
				break;

			default:
#ifdef DEBUG
				printf("PID novog procesa: %d\n", pid);
#endif
				pids[num_procs++] = pid;
				break;
		}

		if (num_procs == MAX_NUM_TABS) {
			printf("Max number of open tabs reached.\n"
				"No more tabs can be open.\n");
		}
	}

exit:
	printf("Exiting ...\n");

	/* always cleanup */
	curl_easy_cleanup(curl);

	for(int i = 0; i < num_procs; i++) {
		kill(pids[i], SIGKILL);
		close(fd[i]);
		remove(filename[i]);
#ifdef DEBUG
		printf("%s\n", filename[i]);
#endif
	}

	return 0;
}

