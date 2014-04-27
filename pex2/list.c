#include <stdlib.h>
#include "list.h"
#include <stdio.h>

int min(int a, int b){
	if(a<b){
		return a;
	}else{
		return b;
	}
}

void insert(list* lst,char* data){//inserts at back of list.  only uses the pointer.  memory must remain!!!
	//remove a duplicate
	remove_node(lst,contains(lst,data));
	//create the new node
	node* new_node=malloc(sizeof(node));
	new_node->data=data;
	new_node->next=NULL;
	new_node->prev=lst->tail;
	//add new node to list
	if(is_empty(lst)){
		lst->head=new_node;
		lst->tail=new_node;
	}else{	
	//include it as the new tail
		lst->tail=new_node;
	//point the old tail to it
		//new_node->prev->next=new_node;
		node* old_tail=new_node->prev;
		old_tail->next=new_node;
	}
	lst->size++;
}

void print_list(list* lst, int n){//prints last n strings in the list
	int i=1;
	node* nod=lst->tail;

	while(nod->prev && i<n){
		//printf("%d  %s\n",i,nod->data);
		i++;
		nod=nod->prev;
	}
	while(nod){
		printf("%d  %s\n",i,nod->data);
		nod=nod->next;
		i--;
	}
}

int contains(list* lst, char* data){//returns the index of list node containing matching string, or -1 if not found (first node is 1)
	int rtn=1;
	node* nod=lst->head;
	while(nod){
		if(strcmp(data,nod->data)==0){
			return rtn;
		}
		nod=nod->next;
		rtn++;
	}
	return 0;
}

void remove_node(list* lst, int n){//removes the nth node from the list (first node is 1)
	//find the node
	if (n>lst->size || n<1) {
		return;
	}
	node* node_to_remove=lst->head;
	int i;
	for (i=1;i<n;i++){
		node_to_remove=node_to_remove->next;
	}
	//remove it from list
		node* node_before=node_to_remove->prev;
		node* node_after=node_to_remove->next;
		//head case
		if(lst->head==node_to_remove){
			lst->head=node_after;
			node_after->prev=NULL;
		//tail case
		}else if(lst->tail==node_to_remove){
			lst->tail=node_before;
			node_before->next=NULL;
		//middle case
		} else {
			node_before->next=node_after;
			node_after->prev=node_before;
		}
	//free the memory
	//printf("freeing string\n");
	free(node_to_remove->data);
	//printf("freeing node\n");
	free(node_to_remove);
	//update size
	lst->size--;
}

bool is_empty(list* lst){//returns true if list is empty, false otherwise
	return lst->size==0;
}

list* new_list(){//malloc and initialize a new list;
	list* rtn=malloc(sizeof(list));
	rtn->head=NULL;
	rtn->tail=NULL;
	rtn->size=0;
}

char* get(list* lst, int n){
	if(n>lst->size || n<1){
		return NULL;
	}
	int i=1;
	node* nod=lst->tail;

	while(i<n){
		i++;
		nod=nod->prev;
	}
	return nod->data;
}

