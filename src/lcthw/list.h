#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;	//声明结构体，使编译器在编译以下typedef中的listnode时能找到声明

typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	void *value;
} ListNode;

typedef struct List {
	int count;
	ListNode *first;
	ListNode *last;
} List;

List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_count(A) ((A)->count)
#define List_first(A) ((A)->first != NULL ? (A)->first->value : NULL)
#define List_last(A) ((A)->last != NULL ? (A)->last->value : NULL)

void List_push(List *list, void *value);
void *List_pop(List *list);

void List_unshift(List *list, void *value);
void *List_shift(List *list);

void *List_remove(List *list, ListNode *node);

//list要遍历的列表, start开始的节点, move移动的方向, vertex位于的节点,可设置为向前或向后遍历

#define LIST_FOREACH(L, S, M, V) ListNode *_node = NULL;\
	ListNode *V = NULL;\
	for(V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
