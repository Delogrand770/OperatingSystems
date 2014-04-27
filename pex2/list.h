#include <stdbool.h>

typedef struct node{

	char* data;
	struct node* prev;
	struct node* next;
} node;

typedef struct {
	node* head;
	node* tail;
	int size;
} list;

void insert(list* lst,char* data);//inserts at back of list.  only uses the pointer.  memory must remain!!!
void print_list(list* lst, int n);//prints first n strings in the list
int contains(list* lst, char* data);//returns the index of list node containing matching string, or -1 if not found
void remove_node(list* lst, int n);//removes the nth node from the list
bool is_empty(list* lst);//returns true if list is empty, false otherwise
list* new_list();//malloc and initialize a new list;
char* get(list* lst, int n);//returns the data at the nth node from the end
