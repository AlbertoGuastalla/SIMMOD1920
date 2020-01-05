#include "NES.h"

nodePtr get_new_node () {
	nodePtr n = NULL;
	
	if (AvailableList.Head == NULL) {
		node_number++;
		event_notice new_event;
		n = (nodePtr) calloc (1, sizeof(struct node));
		n -> event = new_event;
		n -> left = NULL;
		n -> right = NULL;
	} else {
		n = AvailableList.Head;
		AvailableList.Head = AvailableList.Head -> right;
		n -> right = NULL;
	}
	
	return n;
}

void enqueue (nodePtr node_event, dll* Input_Queue) {
	if (Input_Queue -> Head == NULL) {
		Input_Queue -> Head = Input_Queue -> Tail = node_event;
	} else {
		Input_Queue -> Tail -> right = node_event;
		node_event -> left = Input_Queue -> Tail;
		Input_Queue -> Tail = node_event;
	}
}

nodePtr dequeue (dll* Input_Queue) {
	nodePtr n = NULL;
	
	if (Input_Queue -> Head != NULL) {
		n = Input_Queue -> Head;
		Input_Queue -> Head = Input_Queue -> Head -> right;
		n -> right = NULL;
		
		if (Input_Queue -> Head != NULL) {
			Input_Queue -> Head -> left = NULL;
		} else {
			Input_Queue -> Tail = NULL;
		}
	}

	return n;
}

nodePtr event_pop () {
	nodePtr n = NULL;
	
	if (FEL.Head != NULL) {
		n = FEL.Head;
		FEL.Head = FEL.Head -> right;
		n -> right = NULL;
		
		if (FEL.Head != NULL) {
			FEL.Head -> left = NULL;
		}
	}
	
	return n;
}

void schedule (nodePtr node_event) {
	if (FEL.Head == NULL) {
		FEL.Head = FEL.Tail = node_event;
	} else {
		nodePtr n = FEL.Head;
		
		while (n != NULL && node_event -> event.occur_time >= n -> event.occur_time) {
			n = n -> right;
		}
		
		if (n == NULL) {
			FEL.Tail -> right = node_event;
			node_event -> left = FEL.Tail;
			FEL.Tail = node_event;
		} else if (n -> left != NULL) {
			node_event -> right = n;
			node_event -> left = n -> left;
			n -> left -> right = node_event;
			n -> left = node_event;
		} else {
			node_event -> right = FEL.Head;
			FEL.Head -> left = node_event;
			FEL.Head = node_event;
		}
	}
}

void return_node(nodePtr node) {
	memset(node -> event.name, 0, 256);
	node -> event.type = 0;
	node -> event.create_time = 0;
	node -> event.occur_time = 0;
	node -> event.arrival_time = 0;
	node -> event.service_time = 0;
	node -> left = NULL;
	
	if (AvailableList.Head == NULL) {
		AvailableList.Head = node;
	} else {
		node -> right = AvailableList.Head;
		AvailableList.Head = node;
	}
}

void destroy_list(nodePtr list) {
	if (list != NULL) {
		while (list -> right != NULL) {
			nodePtr ptr = list;
			list = list -> right; 
			ptr -> right = NULL;
			list -> left = NULL;
			free(ptr);
		}
		
		free(list);
	}
}

void print_list(dll* Input_Queue){
	nodePtr n = Input_Queue -> Head;
	
	while (n != NULL) {
		printf("(");
		printf("name: %s ", n -> event.name);
		printf("type: %d ", n -> event.type);
		printf("occur_time: %f ", n -> event.occur_time);
		printf(")");
		n = n -> right;
	}
}

void print_FEL(){
	nodePtr n = FEL.Head;
	
	while (n != NULL) {
		printf("(");
		printf("name: %s ", n -> event.name);
		printf("type: %d ", n -> event.type);
		printf("occur_time: %f ", n -> event.occur_time);
		printf(")");
		n = n -> right;
	}
}
