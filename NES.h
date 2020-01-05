typedef enum boolean {false, true} boolean;

#define NO_EVENT -1
#define ARRIVAL_DELAY 0
#define ARRIVAL_RESERVE 1
#define ARRIVAL_SWAP_IN 2
#define ARRIVAL_CPU 3
#define ARRIVAL_IO1 4
#define ARRIVAL_IO2 5
#define ARRIVAL_SWAP_OUT 6
#define DEPARTURE_DELAY 7
#define DEPARTURE_RESERVE 8
#define DEPARTURE_SWAP_IN 9
#define DEPARTURE_CPU 10
#define DEPARTURE_IO1 11
#define DEPARTURE_IO2 12
#define DEPARTURE_SWAP_OUT 13
#define END 14

#include <stdio.h>  
#include <stdlib.h> 
#include <string.h> 
#include <ctype.h>                         

/* Declarations and pointers dependent on the functions implemented to handle the basic data structures */
/*======================================================================================================*/

/* Definition of the type used to specify the pointer to a node of a queue*/
typedef struct node* nodePtr;
/* Definition of the type used to specify the header of a queue*/
typedef struct DLL{
	nodePtr Head;
	nodePtr Tail;
}dll;
/* the Future Event List and the Input Queue are declared as Doubly Linked Lists
   with a header having fields pointing to the first and to the last elements of the list */
   
dll FEL;
dll Reserve_Queue;
dll SwapIn_Queue;
dll Cpu_Queue;
dll IO1_Queue;
dll IO2_Queue;
dll AvailableList;

/*======================================================================================================*/


int job_number;    /* (progressive) Job identification number */
int node_number;   /* progressive nunmber used to identify the newly generated node */

typedef struct {
	int type;
	int identifier;
	char name[256];
	double create_time;
	double occur_time;
	double arrival_time;
	double service_time;
	double remained_service_time;
	double active_time;
	double response_time;
} event_notice;


struct node{
	event_notice event;
	nodePtr left;	// Pointer to the previous node in the doubly linked list
	nodePtr right;  // Pointer to the next node in the doubly linked list
};

boolean isNumber(const char* s) 
{ 
	int i;
    for (i = 0; i < strlen(s); i++) 
        if (isdigit(s[i]) == false) 
            return false; 
  
    return true; 
} 

void simulate(int, int, boolean);
void initialize(int, int, boolean);
void engine(void);
nodePtr event_pop();
nodePtr get_new_node();
nodePtr dequeue(dll* Input_Queue);
void enqueue(nodePtr node_event, dll* Input_Queue);
void arrivalDelay(nodePtr);
void departureDelay(nodePtr);
void arrivalReserve(nodePtr);
void departureReserve(nodePtr);
void arrivalSwapIn(nodePtr);
void departureSwapIn(nodePtr);
void arrivalCpu(nodePtr);
void departureCpu(nodePtr);
void arrivalIO1(nodePtr);
void departureIO1(nodePtr);
void arrivalIO2(nodePtr);
void departureIO2(nodePtr);
void arrivalSwapOut(nodePtr);
void departureSwapOut(nodePtr);
void report(void);
void print_results(void);
boolean RegPoint(nodePtr);
void CollectRegStatistics(nodePtr);
void ResetMeasures();
void ComputeConfidenceIntervals();
boolean DecideToStop();
void print_list(dll* Input_Queue);
void print_FEL();
void schedule(nodePtr node_event);
void destroy_list(nodePtr list);
void return_node(nodePtr node);
double GetNegativeExponentialRandomVariate(double, int);
double GetHyperExponentialRandomVariate(int);
void printOutState();
void printMeasures();



