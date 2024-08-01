#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "parser.h"
#include "list.h"

static const char *httpspart = "https://www.telegram.hr";
static const char *pattern = "href=\"/telesport/";
extern const char *home;
struct list_head lista = LIST_INIT;

static long get_file_size(const char *filename) {
	struct stat file_status;

	if (stat(filename, &file_status) < 0) {
		return -1;
	}

	return file_status.st_size;
}

int add_correct_link(char *matching)
{
	char *start = matching + strlen("href=\"");
#ifdef DEBUG
	printf("Test: ");
	for(int i = 0; i < 5; i++)
		printf("%c", *(start + i));
	printf("\n");
#endif
	char *end = strstr(start, "\"");
#ifdef DEBUG
	printf("End: %c\n", *end);
#endif
	int alloc_size = strlen(httpspart) + end - start + 1;
	char *link = malloc(alloc_size);
	if (!link) {
		printf("Malloc failed ...");
		return -1;
	}

	memset(link, 0, alloc_size);

	strcpy(link, httpspart);
	strncpy(link + strlen(httpspart), start, end - start);

	if (list_contains(&lista, link) == 1) {
		free(link);
		return 0;
	}

	list_add(&lista, link);
#ifdef DEBUG
	printf("Kompletan link: %s \n", link);
#endif
	return 0;
}

static int fill_list(char *content)
{
	char *tmp = content;
	char *matching = NULL;

	while(1) {
		matching = strstr(tmp, pattern);
		if (!matching)
			return 0;

		add_correct_link(matching);
		tmp = matching + strlen("href=\"");
	}

	return 0;
}

struct list_head *parse_home(int fd) {
#ifdef DEBUG
	printf("pattern: %s", pattern);
#endif
	int ret = 0;

	long home_size = get_file_size(home);
	if (home_size == -1) {
		printf("Couldn't get home size ...");
		return NULL;
	}

	char *content = malloc(home_size);
	if (!content) {
		printf("Couldn't allocate memory ...");
		return NULL;
	}
	// 1) Procitaj cijelu datoteku
	ret = read(fd, content, home_size);
	if (ret == -1) {
		printf("Error during file reading. Home size = %ld, ret = %d",
			home_size, ret);
		return NULL;
	}
#ifdef DEBUG
	printf("%s\n", content);
	fflush(stdout);
#endif

	// 2) Trazi linkove uz href koji odgovaraju telesport linkovima sa clanaka
	// 3) Napravi potpune linkove, alociraj memoriju za string i
	// dodaj ih u vezanu listu
	ret = fill_list(content);
	return &lista;
}

