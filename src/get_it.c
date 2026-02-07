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
#include <stdint.h>

#include "parser.h"
#include "list.h"

#define MAX_NUM_TABS 	20
#define URL_LENGTH 	200
#define INPUT_LEN 	256

static const char *title =
"******************************************************************\n"
"**************** get_it, Telesport Cracker v2.1 ******************\n"
"******************************************************************\n";

const char *home = "home.html";
static int home_fd;
static const char *telesport = "https://www.telegram.hr/telesport/";
static char filename[MAX_NUM_TABS][20];
static int fd[MAX_NUM_TABS] = {0};
static int pids[MAX_NUM_TABS] = {0};
static int num_procs = 0;
static CURL *curl;

void sighandler(int signal)
{
	(void) signal;

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

static void print_menu(struct list_head *list)
{
	struct list *tmp = list->next;
	int i = 1;

	printf("%s", title);
	printf("Available articles: \n");

	while(tmp) {
		printf("%d) ", i++);
		print_article_name(tmp->link);
		tmp = tmp->next;
	}
	printf("\nFor exit type anything different from available article"
			" numbers\n");
}

static size_t write_home(void *ptr, size_t sz, size_t nmemb)
{
	int buff_size = sz*nmemb;
	return write(home_fd, ptr, buff_size);
}

static size_t writefunc(void *ptr, size_t size, size_t nmemb)
{
	int buff_size = size*nmemb;
	return write(fd[num_procs], ptr, buff_size);
}

int main(int argc, char **argv)
{
	CURLcode res;
	char cmd[80] = {0};
	char *url = NULL;
	uint32_t article_num = 0;
	char input[INPUT_LEN] = {0};
	int ret = 0;
	struct list_head *list = NULL;
	struct sigaction sa;

	(void) argc;
	(void) argv;

	sa.sa_handler = sighandler;
	sigaction(SIGINT, &sa, NULL);

	home_fd = open(home, O_CREAT | O_RDWR | O_APPEND, 0644);
	if (home_fd == -1) {
		printf("Could not create file, err = %d\n", errno);
		goto exit;
	}

	curl = curl_easy_init();
	if (!curl) {
		printf("Error in library.");
		close(home_fd);
		remove(home);
		goto exit;
	}

	curl_easy_setopt(curl, CURLOPT_URL, telesport);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_home);
	res = curl_easy_perform(curl);
	if (res) {
		printf("Error in library, err = %d ...\n", res);
		close(home_fd);
		remove(home);
		curl_easy_cleanup(curl);
		exit(-1);
	}

	/* Move to the start of the file after it is written so we can read
	 * it from the start */
	lseek(home_fd,0,SEEK_SET);

	list = parse_home(home_fd);
	if (!list) {
		printf("Unknown error while parsing ...\n");
		close(home_fd);
		remove(home);
		goto exit;
	}

	close(home_fd);
	remove(home);

	print_menu(list);

	while(1) {
		printf("Enter article number: ");

		/*If  a  newline  is  read,  it is stored into the buffer.
		A terminating null byte ('\0') is stored after the last
		character in the buffer.
		*/
		if (fgets(input, INPUT_LEN - 2, stdin) == NULL) {
			printf("Error while reading input, err = %d!\n", errno);
			goto exit;
		}

		ret = sscanf(input, "%u", &article_num);
		if (ret != 1) {
			printf("Invalid input!\n");
			memset(input, 0, INPUT_LEN);
			continue;
		}

		if (article_num < 1 || article_num > list->count) {
			printf("Article %u doesn't exist!\n", article_num);
			memset(input, 0, INPUT_LEN);
			continue;
		}

		if (num_procs == MAX_NUM_TABS) {
			printf("No more tabs can be open.\n");
			goto exit;
		}

		url = list_get(article_num - 1, list);

		sprintf(filename[num_procs], "%d.html", num_procs);
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

		if (res) {
			printf("Invalid URL ...\n");
			continue;
		}

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
				/* sprintf(cmd, "firefox %s > /dev/null 2>&1", */
				/* 		filename[num_procs]); */
				sprintf(cmd, "xdg-open %s > /dev/null 2>&1",
						filename[num_procs]);
				system(cmd);
				exit(0);
				break;

			default:
#ifdef DEBUG
				printf("child PID: %d\n", pid);
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

	curl_easy_cleanup(curl);
	for(int i = 0; i < num_procs; i++) {
		kill(pids[i], SIGKILL);
		close(fd[i]);
		remove(filename[i]);
#ifdef DEBUG
		printf("%s\n", filename[i]);
#endif
	}

	if (list)
		list_destroy(list);

	return 0;
}

