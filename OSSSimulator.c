/* ------------------------------------------------------------------------- 
 * This is an ANSI C simulator that reproduces a simplified version 
 * of the real scheduling protocol used in most modern systems. 
 * The simulated algorithm is Round-Robin with a relative quantum of time.
 *
 * Name              : OSSSimulator.c  (Operating System Scheduling Simulator)
 * Authors           : Alberto Guastalla
 * Language          : ANSI C
 * Latest Revision   : 8/01/2020
 * ------------------------------------------------------------------------- 
 */
 
#include "NES_List_Manager.c"
#include "uniform.c"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <process.h>				

#define SEED 918273645
#define MIN_NUM_CYCLES 25					/* for approximate t-student with normal density function */
#define MIN_NUMBER_OBSERVATIONS 30				/* for central limit theorem (quasi-normality of the sum) */

#define SERVICE_DELAY_STREAM 0
#define SERVICE_SWAP_IN_STREAM 1
#define SERVICE_CPU_STREAM 2
#define CPU_COMPLETION_CHOICE_STREAM 3
#define SERVICE_IO1_STREAM 4
#define SERVICE_IO2_STREAM 5
#define SWAP_OUT_CHOICE_STREAM 6

#define TAGGED_CUSTOMER_ID 0
#define STANDARD_NORMAL_PERCENTILE_0050 1.645

boolean firstStepValidation;					/* flag for First Step Validation Model */
boolean taggedCustomerPassedSubSystem;				/* flag to show if the tagged customer has been passed SubSystem (Active System) */ 
boolean regenerationCondition;					/* flag to show if the simulation have reached the regeneration condition */
boolean detailedPrint;						/* flag to show an additional print about regeneration cycles */ 

double Z;	   						/* Average work-station think time */
double S1; 		   					/* Average swap-in time */
double S2; 		   					/* Average CPU time */
double S4; 		   					/* Average IO1 time */
double S5; 		   					/* Average IO2 time */
double MPD; 							/* Multi-Programming Degree */
double CPU_QUANTUM; 						/* Quantum of CPU */

// CPU completion choice
double Q33;
double Q34;
double Q35;
double Q36;

// Swap-out choice
double Q60;
double Q61;

unsigned int CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND;		/* Number of jobs in Swap-In station to characterize the regeneration condition */
unsigned int CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND;	/* Number of jobs in Swap-Out station to characterize the regeneration condition */
unsigned int CUSTOMER_IN_IO1_FOR_REGENERATION_COND;		/* Number of jobs in IO1 station to characterize the regeneration condition */
unsigned int CUSTOMER_IN_IO2_FOR_REGENERATION_COND;		/* Number of jobs in IO2 station to characterize the regeneration condition */

double Start;             					/* Beginnig of Observation Period */
double ObservPeriod;      					/* Length of the Observation Period */
double sim_time;              					/* Clock of the simulator - Simulation time */
unsigned int event_counter;        				/* Number of the events processed by the simulator*/
unsigned int cycles;						/* Number of regeneration cycles */

unsigned int nsys;						/* Number of jobs in the system */
unsigned int n_DS;						/* Number of jobs in delay station */
unsigned int n_RS;						/* Number of jobs in reserve station */
unsigned int n_SIS;						/* Number of jobs in swap-in station */
unsigned int n_CPUS;						/* Number of jobs in Cpu station */
unsigned int n_IO1S;						/* Number of jobs in IO1 station */
unsigned int n_IO2S;						/* Number of jobs in IO2 station */
unsigned int n_SOS;						/* Number of jobs in swap-out station */
unsigned int narr_DS;						/* Number of arrival at delay station */
unsigned int ncom_DS;						/* Number of departure at delay station */
unsigned int narr_RS;						/* Number of arrival at reserve station */
unsigned int ncom_RS;						/* Number of departure at reserve station */
unsigned int narr_SIS;						/* Number of arrival at swap-in station */
unsigned int ncom_SIS;						/* Number of departure at swap-in station */
unsigned int narr_CPUS;						/* Number of arrival at Cpu station */			
unsigned int ncom_CPUS;						/* Number of departure at Cpu station */
unsigned int narr_IO1S;						/* Number of arrival at IO1 station */			
unsigned int ncom_IO1S;						/* Number of departure at IO1 station */
unsigned int narr_IO2S;						/* Number of arrival at IO2 station */			
unsigned int ncom_IO2S;						/* Number of departure at IO2 station */
unsigned int narr_SOS;						/* Number of arrival at swap-out station */			
unsigned int ncom_SOS;						/* Number of departure at swap-out station */

double  	 *totalActiveTimes;				/* Total Active Times foreach job */
double  	 *totalResponseTimes;				/* Total Response Times foreach job */
unsigned int *passageSubSystemTimes;				/* Number of times that a job pass through the SubSystem (Active System) */
unsigned int *passageDelayTimes;				/* Number of times that a job pass through the Delay Station */

double leftExtreme;						/* Left Extreme for confidence interval */
double rightExtreme;						/* Right Extreme for confidence interval */
double delta;							/* Half part of interval */
unsigned int numberOfObservationInCurrentCycle;			/* Number of observations in a regeneration cycle */
unsigned int WORKSTATIONS;					/* Number of terminals connected to the system */ 

double sumObs; 							/* Sum of observations in a regeneration cycle */
double regLength;						/* Length of a regeneration cycle (number of observations) */
double sumSumObs;						/* Sum of all observations in all regeneration cycles */
double sumSumObsAt2; 						/* Square of sum of all observations in all regeneration cycles */
double sumRegLength;						/* Sum of all length of all regeneration cycle (number of observations) */
double sumRegLengthAt2;						/* Square of sum of all length of all regeneration cycle (number of observations) */
double sumObsSumRegLength; 					/* Product of sum of observations and length of a regeneration cycle */
double sampleAverageActiveTime;					/* Sample average Active Time (point estimate) */

double meanValues [5];						/* Mean number of customers foreach station produced by MVA */
FILE *fp;							/* File pointer to read mean values produced by MVA */

int main(int argc, char* argv[]){
	/* Command line parameters (Workstations, validation flag, verbosity flag) */
	unsigned int validationStep = 0;
	boolean varbosity = false, vr = false;
	int i, terminals = 20;
	char buffer [20];
	
	for (i = 1; i < argc; i++) {
		if (isNumber(argv[i])) {
			terminals = atoi(argv[i]);
		} else if (strcmp(argv[i], "-fsv") == 0) {
			validationStep = 1;
		} else if (strcmp(argv[i], "-verbosity") == 0) {
			varbosity = true;
		}
	}
	
	/* get the mean value number of jobs foreach station at steady-state from MVA algorithm as
	   heuristic for define the regeneration condition */	
	   
	int ret = spawnl(P_WAIT, "MVA.exe", "MVA.exe", itoa (terminals, buffer, 10), NULL);	
	
	fp = fopen("mean_values.bin","rb");  
	fread(meanValues, sizeof(double), 5, fp);
	
	simulate (terminals, validationStep, varbosity);
	getchar();
    return 0;   
}

void simulate(int terminals, int validation, boolean verbosity) {
	/* Simulation core */
	initialize(terminals, validation, verbosity);
	while (!DecideToStop() || cycles <= MIN_NUM_CYCLES)
	   engine();
	
	ObservPeriod = sim_time - Start;
	report();
}

void engine(void){
	int event_type;
	double  oldtime;
	nodePtr new_event;
	
	/* Get the first event notice from Future Event List */
	new_event = event_pop();

	/* update clock */
	oldtime = sim_time;
	sim_time = new_event->event.occur_time;
		
	/* Collect statistics */
	if (regenerationCondition) CollectRegStatistics(new_event);
		
	if (RegPoint (new_event)) {
		ResetMeasures ();
		regenerationCondition = true;
	}
	
	/* Identify and process current event */
	event_type = new_event->event.type;		
	switch (event_type){
		case ARRIVAL_DELAY : arrivalDelay(new_event);
		break;
		case ARRIVAL_RESERVE : arrivalReserve(new_event);
		break;
		case ARRIVAL_SWAP_IN : arrivalSwapIn(new_event);
		break;
		case ARRIVAL_CPU : arrivalCpu(new_event);
		break;
		case ARRIVAL_IO1 : arrivalIO1(new_event);
		break;
		case ARRIVAL_IO2 : arrivalIO2(new_event);
		break;
		case ARRIVAL_SWAP_OUT : arrivalSwapOut(new_event);
		break;
		case DEPARTURE_DELAY : departureDelay(new_event);
		break;
		case DEPARTURE_RESERVE : departureReserve(new_event);
		break;
		case DEPARTURE_SWAP_IN : departureSwapIn(new_event);
		break;
		case DEPARTURE_CPU : departureCpu(new_event);
		break;
		case DEPARTURE_IO1 : departureIO1(new_event);
		break;
		case DEPARTURE_IO2 : departureIO2(new_event);
		break;
		case DEPARTURE_SWAP_OUT : departureSwapOut(new_event);
		break;
	}
	event_counter++;
	
	if (event_counter == 30) {
		printOutState();
	}	
}

void initialize(int terminals, int validation, boolean verbosity){
	nodePtr curr_notice;
	int i;
	
    /* model setup */
    if (validation == 1) firstStepValidation = true;
	else firstStepValidation = false;
	
	if (verbosity) detailedPrint = true;
	else detailedPrint = false; 
    
    Z 		        	  							= 5000.0;			
	S1 		    		   							= 210.0;			
	S4 		    		   							= 40.0;				
	S5 		    		   							= 180.0;
	regenerationCondition							= false;
	WORKSTATIONS									= terminals;
	
	if (!firstStepValidation) {
		Q33											= 0.0;
		Q34 										= 0.65;
		Q35 										= 0.25;
		Q36 										= 0.10;
		S2 		    								= 27.0;				
		CPU_QUANTUM 								= 3;				
		MPD 	    								= 10;
		CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND 	= 0;
		CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND 	= (MPD >= WORKSTATIONS) ? ((int)(meanValues[1] + 0.5)) : 1;
		CUSTOMER_IN_IO1_FOR_REGENERATION_COND 		= (MPD >= WORKSTATIONS) ? ((int)(meanValues[3] + 0.5)) : 1;
		CUSTOMER_IN_IO2_FOR_REGENERATION_COND 		= (MPD >= WORKSTATIONS) ? ((int)(meanValues[4] + 0.5)) : 6;
	} else {
		Q33											= 0.9;
		Q34 										= 0.065;
		Q35 										= 0.025;
		Q36 										= 0.01;
		S2 		    								= 2.7;				
		CPU_QUANTUM 								= DBL_MAX;			
		MPD 	    								= INT_MAX;	
		CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND 	= 0;
		CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND 	= (int)(meanValues[1] + 0.5);
		CUSTOMER_IN_IO1_FOR_REGENERATION_COND 		= (int)(meanValues[3] + 0.5);
		CUSTOMER_IN_IO2_FOR_REGENERATION_COND 		= (int)(meanValues[4] + 0.5);
	}
	
	// Swap-out choice
	Q60 = 0.40;
	Q61 = 0.60;
    
    /* Initialize multi stream of random numbers */
	PlantSeeds(SEED);	
	
    /* Basic Statistic Measures  */	
	Start     		      			  = 0.0;
	nsys 	  		      			  = 0;								
	n_DS 	  		      			  = 0;									
	n_RS 	  		      			  = 0;	
	n_SIS 	  		      			  = 0;
	n_CPUS	  		      			  = 0;
	n_IO1S							  = 0;		
	n_IO2S							  = 0;	
	n_SOS							  = 0;
	narr_DS   		      			  = 0;							
	ncom_DS      	      			  = 0;					
	narr_RS	  		      			  = 0;							
	ncom_RS	  	          		      = 0;	
	narr_SIS  		      			  = 0;							
	ncom_SIS 		      			  = 0;
	narr_CPUS 		      			  = 0;							
	ncom_CPUS 		      			  = 0;
	narr_IO1S 		      			  = 0;							
	ncom_IO1S  		      			  = 0;
	narr_IO2S 		      			  = 0;							
	ncom_IO2S 		      			  = 0;
	narr_SOS 		      			  = 0;							
	ncom_SOS 		      			  = 0;
	cycles				  			  = 0;
	leftExtreme 		  			  = 0.0;
	rightExtreme 		  			  = 0.0;
	numberOfObservationInCurrentCycle = 0;
	sumObs							  = 0.0;
	regLength 						  = 0.0;
	sumSumObs 						  = 0.0;
	sumSumObsAt2 					  = 0.0;
	sumRegLength 					  = 0.0;
	sumRegLengthAt2 				  = 0.0;
	sumObsSumRegLength 				  = 0.0;
	sampleAverageActiveTime			  = 0.0;
	taggedCustomerPassedSubSystem	  = true;
	totalActiveTimes      			  = (double*) calloc (WORKSTATIONS, sizeof(double));
	passageSubSystemTimes 			  = (int*) calloc (WORKSTATIONS, sizeof(int));
	totalResponseTimes    			  = (double*) calloc (WORKSTATIONS, sizeof(double));
	passageDelayTimes     			  = (int*) calloc (WORKSTATIONS, sizeof(int)); 	
		
	sim_time  = Start;	
    
    /* Basic  counters  */
	event_counter  = 0;
	node_number    = 0;

	/* Future Event List */
	FEL.Head = NULL;
	FEL.Tail = NULL;
	
	/* Available List */
	AvailableList.Head = NULL;
	AvailableList.Tail = NULL;
	
	/* Reserve List */
	Reserve_Queue.Head = NULL;
	Reserve_Queue.Tail = NULL;
	
	/* SwapIn List */
	SwapIn_Queue.Head = NULL;
	SwapIn_Queue.Tail = NULL;
	
	/* Cpu List */
	Cpu_Queue.Head = NULL;
	Cpu_Queue.Tail = NULL;
	
	/* IO1 List */
	IO1_Queue.Head = NULL;
	IO1_Queue.Tail = NULL;
	
	/* IO2 List */
	IO2_Queue.Head = NULL;
	IO2_Queue.Tail = NULL;
	
	/* schedule first events */
	for(i = 1; i <= WORKSTATIONS; ++i){
	   double serviceDelay = GetNegativeExponentialRandomVariate(Z, SERVICE_DELAY_STREAM);	
		
	   curr_notice = get_new_node();
	   
	   curr_notice->event.create_time = Start;
	   curr_notice->event.arrival_time = Start;
	   curr_notice->event.service_time = serviceDelay;
	   curr_notice->event.remained_service_time = 0.0;
	   curr_notice->event.occur_time = Start;
	   curr_notice->event.type = ARRIVAL_DELAY;
   	   curr_notice->right = NULL;
	   curr_notice->left = NULL;
	   curr_notice->event.identifier = (i - 1);
	   sprintf(curr_notice->event.name, "J%d", i);
	   schedule(curr_notice);
    }
}

void arrivalDelay(nodePtr node_event){
	/* manage arrival event at Delay Station */
	n_DS++;
	narr_DS++;
	
	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = DEPARTURE_DELAY;
    node_event->event.occur_time = sim_time + node_event->event.service_time;
    node_event->event.service_time = 0.0;
	node_event->event.remained_service_time = 0.0;
    schedule(node_event); 
}

void departureDelay(nodePtr node_event) {
	/* manage deeparture event at Delay Station */
	n_DS--;
	ncom_DS++;

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = ARRIVAL_RESERVE;
	node_event->event.service_time = 0.0;
	node_event->event.occur_time = sim_time;
	node_event->event.response_time = sim_time;
    schedule(node_event); 
}

void arrivalReserve(nodePtr node_event) {
	/* manage arrival event at Reserve Station */
	n_RS++;
	narr_RS++;
	
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
	
	if (nsys < MPD) {
		/* System is not full */
		node_event->event.type = DEPARTURE_RESERVE;
		node_event->event.service_time = 0.0;
		node_event->event.remained_service_time = 0.0;
		node_event->event.occur_time = sim_time;
		schedule(node_event); 
		nsys++;
	}
	else {
		/* System is full */
		node_event->event.type = NO_EVENT;
		node_event->event.occur_time = 0.0;
		enqueue(node_event,&Reserve_Queue);
	}
}

void departureReserve(nodePtr node_event) {
	/* manage departure event at Reserve Station */
	double serviceSwapIn = GetNegativeExponentialRandomVariate(S1, SERVICE_SWAP_IN_STREAM);
	
	n_RS--;
	ncom_RS++;

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = ARRIVAL_SWAP_IN;
	node_event->event.service_time = serviceSwapIn;
	node_event->event.remained_service_time = 0.0;
	node_event->event.occur_time = sim_time;
    schedule(node_event); 
}

void arrivalSwapIn(nodePtr node_event){
	/* manage arrival event at swap-in Station */
	n_SIS++;
	narr_SIS++;
	
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
	
	if (n_SIS == 1) {
		/* Process arrival at idle server */
		node_event->event.type = DEPARTURE_SWAP_IN;
		node_event->event.occur_time = sim_time + node_event->event.service_time;
		schedule(node_event); 
	}
	else {
		/* Process arrival at busy server */
		node_event->event.type = NO_EVENT;
		node_event->event.occur_time = 0.0;
		enqueue(node_event,&SwapIn_Queue);
	}
}

void departureSwapIn(nodePtr node_event){
	/* manage departure event at swap-in Station */
	double cpuBurst;
	nodePtr next_job;
	
	if (!firstStepValidation) {
		cpuBurst = GetHyperExponentialRandomVariate(SERVICE_CPU_STREAM);
	} else {
		cpuBurst = GetNegativeExponentialRandomVariate(S2, SERVICE_CPU_STREAM);
	}
	
	n_SIS--;
	ncom_SIS++;

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = ARRIVAL_CPU;
	node_event->event.service_time = cpuBurst;
	node_event->event.remained_service_time = cpuBurst;
	node_event->event.active_time = sim_time;
	node_event->event.occur_time = sim_time;
    schedule(node_event); 
    
    if (n_SIS > 0) {
		/* Process departure from a server with a queue */
		next_job = dequeue(&SwapIn_Queue);
		next_job->event.type = DEPARTURE_SWAP_IN;
		next_job->event.occur_time = sim_time + next_job->event.service_time;
		schedule(next_job);
	}
}

void arrivalCpu(nodePtr node_event){
	/* manage arrival event at Cpu Station */
	n_CPUS++;
	narr_CPUS++;
		
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
		
	if (node_event->event.remained_service_time > CPU_QUANTUM) {
		node_event->event.service_time = CPU_QUANTUM;
		node_event->event.remained_service_time = node_event->event.remained_service_time - CPU_QUANTUM;
	} else {
		node_event->event.service_time = node_event->event.remained_service_time;
		node_event->event.remained_service_time = 0.0;
	}	
		
	if (n_CPUS == 1) {
		/* Process arrival at idle server */
		node_event->event.type = DEPARTURE_CPU;
		node_event->event.occur_time = sim_time + node_event->event.service_time;
		schedule(node_event); 
	}
	else {
		/* Process arrival at busy server */
		node_event->event.type = NO_EVENT;
		node_event->event.occur_time = 0.0;
		enqueue(node_event,&Cpu_Queue);
	}
}

void departureCpu(nodePtr node_event){
	/* manage departure event at Cpu Station */
	double choice, cpuBurst, ioBurst;
	nodePtr next_job;
	
	n_CPUS--;
	ncom_CPUS++;
	
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
	node_event->event.occur_time = sim_time;

	if (!firstStepValidation) {
		if (node_event->event.remained_service_time == 0.0) {
			/* Process has finished computing */
			SelectStream(CPU_COMPLETION_CHOICE_STREAM);
			choice = Uniform(0.0, 1.0);
			
			if (choice <= Q34) {
				/* Go to IO1 station */
				ioBurst = GetNegativeExponentialRandomVariate(S4, SERVICE_IO1_STREAM);
				node_event->event.service_time = ioBurst;
				node_event->event.remained_service_time = 0.0;
				node_event->event.type = ARRIVAL_IO1;
			} else if (choice <= Q34 + Q35) {
				/* Go to IO2 station */
				ioBurst = GetNegativeExponentialRandomVariate(S5, SERVICE_IO2_STREAM);
				node_event->event.service_time = ioBurst;
				node_event->event.remained_service_time = 0.0;
				node_event->event.type = ARRIVAL_IO2;
			} else {
				/* Go to Swap-Out station */
				node_event->event.service_time = 0.0;
				node_event->event.remained_service_time = 0.0;
				node_event->event.type = ARRIVAL_SWAP_OUT;
			}
		} else {
			/* Process hasn't finished computing */
		    node_event->event.type = ARRIVAL_CPU;
		}
	} else {
		SelectStream(CPU_COMPLETION_CHOICE_STREAM);
		choice = Uniform(0.0, 1.0);
		
		if (choice <= Q33) {
			/* Go to Cpu station */
			cpuBurst = GetNegativeExponentialRandomVariate(S2, SERVICE_CPU_STREAM);
			node_event->event.service_time = cpuBurst;
			node_event->event.remained_service_time = cpuBurst;
			node_event->event.type = ARRIVAL_CPU;
		} else if (choice <= Q33 + Q34) {
			/* Go to IO1 station */
			ioBurst = GetNegativeExponentialRandomVariate(S4, SERVICE_IO1_STREAM);
			node_event->event.service_time = ioBurst;
			node_event->event.remained_service_time = ioBurst;
			node_event->event.type = ARRIVAL_IO1;
		} else if (choice <= Q33 + Q34 + Q35) {
			/* Go to IO2 station */
			ioBurst = GetNegativeExponentialRandomVariate(S5, SERVICE_IO2_STREAM);
			node_event->event.service_time = ioBurst;
			node_event->event.remained_service_time = ioBurst;
			node_event->event.type = ARRIVAL_IO2;
		} else {
			/* Go to Swap-Out station */
			node_event->event.service_time = 0.0;
			node_event->event.remained_service_time = 0.0;
			node_event->event.type = ARRIVAL_SWAP_OUT;
		}
	}
	
	schedule(node_event);
    
    if (n_CPUS > 0) {
		/* Process departure from a server with a queue */
		next_job = dequeue(&Cpu_Queue);
		next_job->event.type = DEPARTURE_CPU;
		next_job->event.occur_time = sim_time + next_job->event.service_time;
		schedule(next_job);
	}
}

void arrivalIO1(nodePtr node_event){
	/* manage arrival event at IO1 Station */
	n_IO1S++;
	narr_IO1S++;
	
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
	
	if (n_IO1S == 1) {
		/* Process arrival at idle server */
		node_event->event.type = DEPARTURE_IO1;
		node_event->event.occur_time = sim_time + node_event->event.service_time;
		schedule(node_event); 
	}
	else {
		/* Process arrival at busy server */
		node_event->event.type = NO_EVENT;
		node_event->event.occur_time = 0.0;
		enqueue(node_event,&IO1_Queue);
	}
}

void departureIO1(nodePtr node_event){
	/* manage departure event at IO1 Station */
	double cpuBurst;
	nodePtr next_job;
	
	if (!firstStepValidation) {
		cpuBurst = GetHyperExponentialRandomVariate(SERVICE_CPU_STREAM);
	} else {
		cpuBurst = GetNegativeExponentialRandomVariate(S2, SERVICE_CPU_STREAM);
	}
	
	n_IO1S--;
	ncom_IO1S++;

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = ARRIVAL_CPU;
	node_event->event.service_time = cpuBurst;
	node_event->event.remained_service_time = cpuBurst;
	node_event->event.occur_time = sim_time;
    schedule(node_event); 
    
    if (n_IO1S > 0) {
		/* Process departure from a server with a queue */
		next_job = dequeue(&IO1_Queue);
		next_job->event.type = DEPARTURE_IO1;
		next_job->event.occur_time = sim_time + next_job->event.service_time;
		schedule(next_job);
	}
}

void arrivalIO2(nodePtr node_event){
	/* manage arrival event at IO2 Station */
	n_IO2S++;
	narr_IO2S++;
	
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
	
	if (n_IO2S == 1) {
		/* Process arrival at idle server */
		node_event->event.type = DEPARTURE_IO2;
		node_event->event.occur_time = sim_time + node_event->event.service_time;
		schedule(node_event); 
	}
	else {
		/* Process arrival at busy server */
		node_event->event.type = NO_EVENT;
		node_event->event.occur_time = 0.0;
		enqueue(node_event,&IO2_Queue);
	}
}

void departureIO2(nodePtr node_event){
	/* manage departure event at IO2 Station */
	double cpuBurst;
	nodePtr next_job;
	
	if (!firstStepValidation) {
		cpuBurst = GetHyperExponentialRandomVariate(SERVICE_CPU_STREAM);
	} else {
		cpuBurst = GetNegativeExponentialRandomVariate(S2, SERVICE_CPU_STREAM);
	}
	
	n_IO2S--;
	ncom_IO2S++;

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
    node_event->event.type = ARRIVAL_CPU;
	node_event->event.service_time = cpuBurst;
	node_event->event.remained_service_time = cpuBurst;
	node_event->event.occur_time = sim_time;
    schedule(node_event); 
    
    if (n_IO2S > 0) {
		/* Process departure from a server with a queue */
		next_job = dequeue(&IO2_Queue);
		next_job->event.type = DEPARTURE_IO2;
		next_job->event.occur_time = sim_time + next_job->event.service_time;
		schedule(next_job);
	}
}

void arrivalSwapOut(nodePtr node_event){
	/* manage arrival event at swap-out Station */
	totalActiveTimes[node_event->event.identifier] += sim_time - node_event->event.active_time;
	passageSubSystemTimes[node_event->event.identifier] += 1;
	
	n_SOS++;
	narr_SOS++;
	
	/* Update event notice */
	node_event->event.create_time = sim_time;
	node_event->event.arrival_time = sim_time;
	node_event->event.type = DEPARTURE_SWAP_OUT;
	node_event->event.occur_time = sim_time;
	schedule(node_event); 
}

void departureSwapOut(nodePtr node_event){
	/* manage departure event at swap-out Station */
	SelectStream(SWAP_OUT_CHOICE_STREAM);
	double choice = Uniform(0.0, 1.0);
	nodePtr next_job;
	
	n_SOS--;
	ncom_SOS++;
	
	if (choice <= Q60) {
		/* Go to Delay station */
		double serviceDelay = GetNegativeExponentialRandomVariate(Z, SERVICE_DELAY_STREAM);	
		node_event->event.service_time = serviceDelay;
		node_event->event.remained_service_time = 0.0;
		node_event->event.type = ARRIVAL_DELAY;
		totalResponseTimes[node_event->event.identifier] += sim_time - node_event->event.response_time;
		passageDelayTimes[node_event->event.identifier] += 1;
	} else {
		/* Go to Reserve station */
		node_event->event.type = ARRIVAL_RESERVE;
	}
	
	if (n_RS > 0) {
		/* Process departure from a server with a queue */
		next_job = dequeue(&Reserve_Queue);
		next_job->event.service_time = 0.0;
		next_job->event.remained_service_time = 0.0;
		next_job->event.type = DEPARTURE_RESERVE;
		next_job->event.occur_time = sim_time;
		schedule(next_job);
	} else {
		nsys--;
	}

	/* Update event notice */
	node_event->event.arrival_time = sim_time;
	node_event->event.create_time = sim_time;
	node_event->event.occur_time = sim_time;
    schedule(node_event); 
}

void report(){
	print_results();
	
	destroy_list(FEL.Head);
	destroy_list(Reserve_Queue.Head);
	destroy_list(SwapIn_Queue.Head);
	destroy_list(Cpu_Queue.Head);
	destroy_list(IO1_Queue.Head);
	destroy_list(IO2_Queue.Head);
	destroy_list(AvailableList.Head);
}

double GetNegativeExponentialRandomVariate(double eta, int streamIndex) {
	SelectStream(streamIndex);
	double u = Uniform(0.0, 1.0);
	return -eta * log(1.0 - u);
}

double GetHyperExponentialRandomVariate(int streamIndex) { 
	SelectStream(streamIndex);

  	double A1 = 0.8;
  	double eta1 = 15.0;
  	double eta2 = 75.0;
  	double Y = Uniform(0.0, 1.0);
  	double u = Uniform(0.0, 1.0);
  	double expVar;
  	
	if (Y <= A1) {
		return -eta1 * log(1.0 - u);
	} else {
  		return -eta2 * log(1.0 - u);
	}
}

boolean RegPoint(nodePtr node_event) {
	return (node_event -> event.type == ARRIVAL_CPU
			&& n_RS == 0
			&& n_CPUS == 0
			&& n_SIS == CUSTOMER_IN_SWAP_IN_FOR_REGENERATION_COND
			&& n_SOS == CUSTOMER_IN_SWAP_OUT_FOR_REGENERATION_COND
			&& n_IO1S == CUSTOMER_IN_IO1_FOR_REGENERATION_COND
			&& n_IO2S == CUSTOMER_IN_IO2_FOR_REGENERATION_COND
			&& node_event-> event.identifier == TAGGED_CUSTOMER_ID
			&& taggedCustomerPassedSubSystem);
}

void CollectRegStatistics(nodePtr node_event){
	if (node_event -> event.type == ARRIVAL_SWAP_OUT && node_event-> event.identifier == TAGGED_CUSTOMER_ID) {
		regLength += 1;											
		sumObs += sim_time - node_event->event.active_time;	
		taggedCustomerPassedSubSystem = true;	
		numberOfObservationInCurrentCycle++;
	} 	
}

void ResetMeasures (){ 
	taggedCustomerPassedSubSystem = false;

	if (numberOfObservationInCurrentCycle >= MIN_NUMBER_OBSERVATIONS) {
		/* Not grouping regeneration cycles */
		sumSumObs += sumObs;
		sumRegLength += (double) regLength;
		sumSumObsAt2 += sumObs * sumObs;
		sumRegLengthAt2 += (double) regLength * (double) regLength;
		sumObsSumRegLength += sumObs * (double) regLength;
		cycles++;
		
		/* for approximate t-student with normal density function */
		if (cycles > MIN_NUM_CYCLES) {
			ComputeConfidenceIntervals();
		}
		
		if (detailedPrint) {
			printMeasures();
		}
		
		/* Reset Measures */
		regLength = 0.0;
		sumObs = 0.0;
		numberOfObservationInCurrentCycle = 0;
	}
}

void ComputeConfidenceIntervals (){
	sampleAverageActiveTime = sumSumObs / sumRegLength;
	delta = (sqrt((double)cycles / ((double)cycles - 1.0)) *
		sqrt(sumSumObsAt2 - (2.0 * sampleAverageActiveTime * sumObsSumRegLength) + (sampleAverageActiveTime * sampleAverageActiveTime) * sumRegLengthAt2) / sumRegLength);
	leftExtreme = sampleAverageActiveTime - STANDARD_NORMAL_PERCENTILE_0050 * delta;
	rightExtreme = sampleAverageActiveTime + STANDARD_NORMAL_PERCENTILE_0050 * delta;
}

boolean DecideToStop () {
	return (delta / sampleAverageActiveTime <= 0.05);
}

void print_results(){
	if (detailedPrint) {
		int i;

		for (i = 0; i < WORKSTATIONS; ++i) {
			printf("averageActiveTime of process %i: %f\n", i, totalActiveTimes[i] / passageSubSystemTimes[i]);
		}

		for (i = 0; i < WORKSTATIONS; ++i) {
			printf("averageResponseTime of process %i: %f\n", i, totalResponseTimes[i] / passageDelayTimes[i]);
		}
	}
	
	if (!detailedPrint) {
		printf("\nSample Average Active Time                        = %f", sampleAverageActiveTime);
		printf("\nLeft Extreme                                      = %f", leftExtreme);
		printf("\nRigth Extreme                                     = %f", rightExtreme); 
	}
		
	printf("\n\nEnd of simulation.");
}

void printOutState(){
	printf("State of the System after the handling of each of the first 30 events:");
	printf("\n--------------------------------------------------");
	printf("\nNumber of Jobs in the System                   = %d", nsys);
	printf("\nNumber of Jobs in the Delay Station            = %d", n_DS);
	printf("\nNumber of Jobs in the Reserve Station          = %d", n_RS);
	printf("\nNumber of Jobs in the Swap-In Station          = %d", n_SIS);
	printf("\nNumber of Jobs in the Cpu Station              = %d", n_CPUS);
	printf("\nNumber of Jobs in the IO1 Station              = %d", n_IO1S);
	printf("\nNumber of Jobs in the IO2 Station              = %d", n_IO2S);
	printf("\nNumber of Jobs in the Swap-Out Station         = %d", n_SOS);
	
	printf("\nNumber of arrival in the Delay Station         = %d", narr_DS);
	printf("\nNumber of arrival in the Reserve Station       = %d", narr_RS);
	printf("\nNumber of arrival in the Swap-In Station       = %d", narr_SIS);
	printf("\nNumber of arrival in the Cpu Station           = %d", narr_CPUS);
	printf("\nNumber of arrival in the IO1 Station           = %d", narr_IO1S);
	printf("\nNumber of arrival in the IO2 Station           = %d", narr_IO2S);
	printf("\nNumber of arrival in the Swap-Out Station      = %d", narr_SOS);
	
	printf("\nNumber of completion in the Delay Station      = %d", ncom_DS);
	printf("\nNumber of completion in the Reserve Station    = %d", ncom_RS);
	printf("\nNumber of completion in the Swap-In Station    = %d", ncom_SIS);
	printf("\nNumber of completion in the Cpu Station        = %d", ncom_CPUS);
	printf("\nNumber of completion in the IO1 Station        = %d", ncom_IO1S);
	printf("\nNumber of completion in the IO2 Station        = %d", ncom_IO2S);
	printf("\nNumber of completion in the Swap-Out Station   = %d", ncom_SOS);
	
	printf("\nSimulation current time                        = %f", sim_time);
	printf("\nEvent counter                                  = %d", event_counter);
	printf("\nNode counter                                   = %d", node_number);
	
	printf("\n");
}

void printMeasures() {
	printf("\n\nMeasures performed during n.%d regeneration cycle.", cycles);
	printf("\nNumber of observations in current cycle           = %d", numberOfObservationInCurrentCycle);
	printf("\nSum of observations in current cycle              = %f", sumObs);
	printf("\nLength of the current cycle                       = %f", regLength);
	
	printf("\n\nMeasures performed until n.%d regeneration cycle.", cycles);
	printf("\nSample Average Active Time                        = %f", sampleAverageActiveTime);
	printf("\nLeft Extreme                                      = %f", leftExtreme);
	printf("\nRigth Extreme                                     = %f", rightExtreme); 
}
