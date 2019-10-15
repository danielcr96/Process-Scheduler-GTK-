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
int quantum;
int thread_num;
int iteraciones;


int tickets[32];
int workload[32];
int percent_halt[32];


void* input_parser (char *file)
{
    char modo_str[32]="";
    int readline, line = 0;
    int index,i =0;
    char var[32]="";
    int value;


    FILE *fp;
    fp = fopen(file, "r");

    if (fp == NULL)
    {
        printf ("Error opening the file\n\n'");
        exit(EXIT_FAILURE);
    } else {

            readline = fscanf(fp, "MODO=%s\n", modo_str);
	      if (strcmp(modo_str, "Expropiativo") == 0 || strcmp(modo_str, "No_Expropiativo") == 0)
		{ 
		  printf ("Modo=%s\n", modo_str);
		  if (strcmp(modo_str, "Expropiativo") == 0) modo=1;
		  if (strcmp(modo_str, "No_Expropiativo") == 0) modo=0;
		} else
		{
		  printf("Modo \"%s\" Invalido\n", modo_str);
		  printf("Los modos validos son \"Expropiativo\" o \"No_Expropiativo\"\n");
		  exit(1);
		}

	    readline = fscanf(fp, "iteraciones=%d\n", &iteraciones);
	    printf("iteraciones=%d\n", iteraciones);

	    readline = fscanf(fp, "Quantum=%d\n", &quantum);
	    printf("Quantum=%d\n", quantum);

	    readline = fscanf(fp, "Num_threads=%d\n", &thread_num);
	    printf("Num_threads=%d\n", thread_num);


	    readline = fscanf(fp, "Thread %d %s %d\n", &index, var, &value);


            while (readline != EOF)
            {
		line++;
		for (i=0;i<thread_num;i++)
		{
		if(strcmp(var, "Num_tickets") == 0 && index==i+1) 
			{tickets[i]=value; printf("Num_tickets thread[%d]=%d\n", i, tickets[i]);}
		if(strcmp(var, "Workload") == 0 && index==i+1)
			{workload[i]=value; printf("Workload thread[%d]=%d\n", i, workload[i]);}
		if(strcmp(var, "Percent_halt") == 0 && index==i+1)
			{percent_halt[i]=value; printf("Percent_halt thread[%d]=%d\n", i, percent_halt[i]);}
		}


	      readline = fscanf(fp, "Thread %d %s %d\n", &index, var, &value);
            }

    fclose(fp);
    }


}
