#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


int modo;
int idcalendarizador; //este es la entrada del sch.h
int quantum;
int process_num;
int tickets_num = 0;


int workload[32];
int arriveTime[32];
int priority[32];
int tickets[32];


void* input_parser (char *file){

	char calendarizador[32]="";;
    char modo_str[32]="";
    int readline, line = 0;
    int index,i =0;
    char var[32]="";
	char var1[32]="";
	char var2[32]="";
	char var3[32]="";
    int work_load, tp, pr, tk;
	
    FILE *fp;
    fp = fopen(file, "r");

    if (fp == NULL)
    {
        printf ("Error opening the file\n\n'");
        exit(EXIT_FAILURE);
    } 
	else {

		readline = fscanf(fp, "SCHEDULER=%s\n", calendarizador);
		if(strcmp(calendarizador,"fcfs")==0){
			idcalendarizador=0;}

		else if(strcmp(calendarizador,"sjf")==0){
			idcalendarizador=1;}
		
		else if(strcmp(calendarizador,"rr")==0){
			idcalendarizador=2;}	
	
		else if(strcmp(calendarizador,"ps")==0){
			idcalendarizador=3;}
	
		else if(strcmp(calendarizador,"ps_rr")==0){
			idcalendarizador=4;}
	
		else if(strcmp(calendarizador,"mqs")==0){
			idcalendarizador=5;}	
	
		else if(strcmp(calendarizador,"mfqs")==0){
			idcalendarizador=6;}

		else if(strcmp(calendarizador,"lottery")==0){
			idcalendarizador=7;}		

		readline = fscanf(fp, "MODO=%s\n", modo_str);
	    if (strcmp(modo_str, "Expropiativo") == 0 || strcmp(modo_str, "No_Expropiativo") == 0){ 
		  	printf ("Modo=%s\n", modo_str);
		  	if (strcmp(modo_str, "Expropiativo") == 0) modo=1;
		  	if (strcmp(modo_str, "No_Expropiativo") == 0) modo=0;
		} 
		else{
		  	printf("Modo \"%s\" Invalido\n", modo_str);
		  	printf("Los modos validos son \"Expropiativo\" o \"No_Expropiativo\"\n");
		  	exit(1);
		}

		readline = fscanf(fp, "Num_procesos=%d\n", &process_num);
	    printf("Num_procesos=%d\n", process_num);

		readline = fscanf(fp, "Quantum=%d\n", &quantum);
	    printf("Quantum=%d\n", quantum);
    
		readline = fscanf(fp, "proceso %d %s %d %s %d %s %d %s %d\n", &index, var, &work_load, var1, &tp, var2, &pr, var3, &tk);


        while (readline != EOF){
			line++;
			for (i=0;i<process_num;i++){
				//printf("Proces Num: %d\n", );
				if(strcmp(var, "carga") == 0 && index==i){
					workload[i]=work_load; 
					printf("Workload thread[%d]=%d\n", i, workload[i]);
				}
				if(strcmp(var1, "tp") == 0 && index==i){
					arriveTime[i]=tp; 
					printf("Tiempo de llegada thread[%d]=%d\n", i, arriveTime[i]);
				}
				if(strcmp(var2, "pr") == 0 && index==i){
					priority[i]=pr; 
					printf("Prioridad thread[%d]=%d\n", i, priority[i]);
				}
				if(strcmp(var3, "tk") == 0 && index==i){
					tickets[i]=tk; 
					printf("Tickets thread[%d]=%d\n", i, tickets[i]);
				}
			}

			readline = fscanf(fp, "proceso %d %s %d %s %d %s %d %s %d\n", &index, var, &work_load, var1, &tp, var2, &pr, var3, &tk);
        }

    	fclose(fp);

		for(i=0; i<32; i++){
			tickets_num = tickets_num + tickets[i];
		}
		printf("tickets_num = %d\n", tickets_num);
    }


}
