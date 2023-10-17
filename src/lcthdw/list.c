#include "list.h"
#include "dbg.h"

List *List_create(){
	return calloc(1, sizeof(List));
}

void List_destroy(List *list){			//从第一个遍历到最后一个
	LIST_FOREACH(list, first, next, cur){
		if(cur->prev){
			free(cur->prev);
		}
	}

	free(list->last);
	free(list);
}

void List_clear(List *list){			//清除value指向的内存
	LIST_FOREACH(list, first, next, cur){
		free(cur->value);
	}	
}

void List_clear_destroy(List *list){
	List_clear(list);
	List_destroy(list);
}

void List_push(List *list, void *value){
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if(list->last == NULL){
		list->first = node;
		list->last = node;
	}else{
		list->last->next = node;
		node->prev = list->last;
		list->last = node;
	}

	list->count++;

error:
	return;

}

void *List_pop(List *list){
	ListNode *node = list->last;
	return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List *list, void *value){			//在链表的头部插入节点
	ListNode *node = calloc(1, sizeof(ListNode));
	check_mem(node);

	node->value = value;

	if(list->first){
		list->first = node;
		list->last = node;
	}else{
		node->next = list->first;
		list->first->prev = node;
		list->first = node;
	}

	list->count++;

error:
	return;
}

void *List_shift(List *list){
	ListNode *node = list->first;
	return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node){		//分四种情况删除某点
	void *result = NULL;

	check(list->first && list->last, "List is empty");
	check(node, "Node can't be NULL");

	if(node == list->first && node == list->last){
		list->first = NULL;
		list->last = NULL;
	}else if(node == list->first){
		list->first = node->next;
		check(list->first, "The first is null");

		list->first->prev = NULL;
	}else if(node == list->last){
		list->last = node->prev;
		check(list->last, "The last is null");
		list->last->next = NULL;
	}else{
		ListNode *after = node->next;
		ListNode *before = node->prev;

		after->prev = before;
		before->next = after;
	}

	list->count--;
	result = node->value;
	free(node);

error:
	return result;

	
}
