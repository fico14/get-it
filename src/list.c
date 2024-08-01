#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "list.h"

int list_contains(struct list_head *lista, char *link)
{
	struct list *tmp = lista->next;

	while (tmp) {
		if (strcmp(tmp->link, link) == 0)
			return 1;
		tmp = tmp->next;
	}

	return 0;
}

int list_add(struct list_head *lista, char *link)
{
	if (!link || !lista) {
		return -1;
	}

	struct list *tmp = malloc(sizeof(struct list));
	if (!tmp) {
		printf("Malloc failed ...\n");
		return -1;
	}

	tmp->link = link;
	tmp->next = lista->next;
	lista->next = tmp;
	lista->count++;

	return 0;
}

char *list_get(int num, struct list_head *list)
{
	struct list *tmp = list->next;
	int i = 0;

	while(tmp) {
		if (i++ == num)
			return tmp->link;
		tmp = tmp->next;
	}

	return NULL;
}


int list_destroy(struct list_head *head)
{
	struct list *first = head->next;
	struct list *tmp;

	while (first) {
		 tmp = first;
		 first = first->next;
		 free(tmp->link);
		 free(tmp);
	}

	return 0;
}
