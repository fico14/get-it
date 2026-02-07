#ifndef _LIST_H_
#define _LIST_H_

#include <stdint.h>

struct list_head {
	uint32_t count;
	struct list *next;
};

#define LIST_INIT {.count = 0, .next = NULL};

struct list {
	char *link;
	struct list *next;
};

int list_add(struct list_head *lista, char *link);
int list_destroy(struct list_head *head);
char *list_get(int num, struct list_head *list);
int list_contains(struct list_head *lista, char *link);

#endif
