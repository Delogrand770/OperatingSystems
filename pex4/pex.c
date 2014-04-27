#include <stdio.h>
#include <stdlib.h>
#include "byutr.h"

#define MAX_FRAMES 15000

//Assume a 4KB page size
#define OFFSET_BITS 12

struct Node{
	unsigned long value;
	struct Node* next;
};

typedef struct Node* node_ptr;

node_ptr push(node_ptr head, node_ptr item){
	item -> next = head;
	return item;
}

node_ptr moveToTop(node_ptr head, node_ptr item){
	node_ptr temp = head;

	if (head == item){ //Item is already where it needs to be
		return head;
	}

	if (head == NULL){ //Putting first item in list
		return item;
	}

	while(temp -> next != NULL && temp -> next != item){ //Go until item is found
		temp = temp -> next;
	}

	//Move pointers around
	node_ptr before = temp;
	node_ptr after = temp -> next -> next;
	before -> next = after;

	return push(head, item);
}

int main(int argc, char **argv){
	FILE *ifp;
	unsigned long num_accesses=0;
	p2AddrTr trace_record;
	
	// If the number of arguements its wrong then quit
	if(argc != 2){
		fprintf(stderr,"usage: b2a input_byutr_file\n");
		exit(1);
	}
	
	// If the file cannot be opened, then quit
	if((ifp = fopen(argv[1],"rb")) == NULL){
		fprintf(stderr,"cannot open %s for reading\n", argv[1]);
		exit(1);
	}
	
	//fread(&trace_record, sizeof(p2AddrTr), 1, ifp);
	num_accesses = 0;

	//Single linked list
	node_ptr head = NULL;

	//array to hold faults
	static int faults[1000];
	int i;
	while(!feof(ifp)){	
		//read next trace record		
		fread(&trace_record, sizeof(p2AddrTr), 1, ifp);
		//to get the page number, we shift the offset bits off of the address
		unsigned long page_num=trace_record.addr >> OFFSET_BITS;//THIS IS THE PAGE NUMBER THAT WAS REQUESTED!!!!!
		num_accesses++;

		//you may want to use this to debug
		if((num_accesses % 100000) == 0){
			fprintf(stderr,"%lu samples read\r", num_accesses);
		}

		//process each page reference
		node_ptr temp = head;
		int depth = 0;

		//search for page_num in the array to get depth
		while(temp != NULL && temp -> value != page_num){
			temp = temp -> next;
			depth++;
		}

		if (temp == NULL){
			depth = -1;

			node_ptr new = malloc(sizeof(struct Node));
			new -> value = page_num;
			new -> next = NULL;

			head = push(head, new);

			for (i = 0; i < 1000; i++){
				faults[i]++;
			}
		} else {		
			head = moveToTop(head, temp);
			for (i = 0; i < depth && i < 1000; i++){
				faults[i]++;
			}
		}
	}

	//find the number of page faults for each number of allocated frames
	printf("Total Accesses:,%lu", num_accesses);
	printf("\nFrames,Misses,Miss Rate\n");
	for(i = 0; i < 1000; i++){
		printf("%d,%d,%f \n", i+1, faults[i], ((double) faults[i]) / ((double) num_accesses));
	}

	fclose(ifp);
	return(0);
}

