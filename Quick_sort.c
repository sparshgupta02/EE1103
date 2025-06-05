/* Roll number : EE23B117
Date :7/9/2023
Version:2
Description: Read from a data file (argv[1]) of integers into a linked list and print out the integers using quick sort of the list 
        
Input : 1 3 2 5 4 [data is stored with each number on a new line ]
Output: 1 2 3 4 5  
    */



#include <stdio.h>
#include <stdlib.h>

// Creating singly linked list
struct Node {
	int data;
	struct Node* next;
};

// inserts a new node at the end of a singly linked list
void insert(struct Node** head, int value)
{
	// using malloc to allocate memory to a dynamic node which has size equal to the struct node 
	struct Node* node
		= (struct Node*)malloc(sizeof(struct Node));
	
		node->data = value;
		node->next = NULL;
		if (*head == NULL) {
			*head = node;
		}
		else {
			struct Node* temp = *head;

			// finding last node
			while (temp->next != NULL) {
				temp = temp->next;         // moves the temp file ahead with each iteration 
			}

			// adding node at last position
			temp->next = node;
		
	}
}

// output the elements of the list 
void display(struct Node* head)
{
	
	struct Node* temp = head;
	
	while (temp != NULL) {
		printf("%d ", temp->data);
		temp = temp->next;
	} 
	printf("\n");
}

// makes a temporary pointer which runs till it reaches the last node to find the last node 
struct Node* last_node(struct Node* head)
{
	struct Node* temp = head;
	while (temp != NULL && temp->next != NULL) {
		temp = temp->next;
	}
	return temp;
}

// function for node partition (uses last element as pivot) 
struct Node* partition(struct Node* first, struct Node* last)
{
	
	// Get first node of given linked list
	struct Node* pivot = first;
	struct Node* front = first;
	int temp = 0;
	while (front != NULL && front != last) {            // front is used to traverse the nodes from first to last 
		if (front->data < last->data) {        // if the front val is less the than the last value then swap is performed 
			pivot = first;
			                              // the swap puts the lower value at the start of the list and leaves others 
			temp = first->data;               
			first->data = front->data;
			front->data = temp;
			
			first = first->next;
		}

		
		front = front->next;
	}

	// Change last node value to current node
	temp = first->data;
	first->data = last->data;
	last->data = temp;
	return pivot;
}

// Performing quick sort in the given linked list
void quick_sort(struct Node* first, struct Node* last)
{
	if (first == last) {
		return;
	}
	struct Node* pivot = partition(first, last);

	if (pivot != NULL && pivot->next != NULL) {
		quick_sort(pivot->next, last);
	}

	if (pivot != NULL && first != pivot) {
		quick_sort(first, pivot);
	}
}


int main(int argc, char **argv ){           // reading file using fscanf 
  FILE *fp = fopen(argv[1], "r");
        
	struct Node* head = NULL;
        int c;
	 while (fscanf(fp, "%d",&c ) == 1){
	 insert(&head,c);
	 }
	 
	// calling the function from top
	quick_sort(head, last_node(head));
	display(head);
	return 0;
}

