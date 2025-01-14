#include "list.h"
/* Add a node to the end of the linked list. Assume head_ptr is non-null. */
void append_node (node** head_ptr, int new_data) {
    node* new_node = (node*)malloc(sizeof(node));
    new_node->val=new_data;

	/* If the list is empty, set the new node to be the head and return */
	if (*head_ptr == NULL) {
        *head_ptr=new_node;
		return;
	}

	node* curr = *head_ptr;
	while (curr->next!= NULL) {
		curr = curr->next;
	}
	/* Insert node at the end of the list */
    curr->next=new_node;
}

/* Reverse a linked list in place (in other words, without creating a new list).
   Assume that head_ptr is non-null. */
void reverse_list (node** head_ptr) {
    if(*head_ptr==NULL){
        return;
    }


    node* prev = NULL;
    node* curr = *head_ptr;
    node* next = curr->next;
	while (curr != NULL) {
        curr->next=prev;
        prev=curr;
        curr=next;
        if(next!=NULL){
            next=next->next;
        }
	}
	/* Set the new head to be what originally was the last node in the list */
	*head_ptr = prev; /* INSERT CODE HERE */
}



