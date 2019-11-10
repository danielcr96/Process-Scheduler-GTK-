#include "parser.h"





int currentProc=0;

void fcfs(){
	//currentProc++;
	printf("currentProc ->  %d\n ", currentProc);


}

void sjf(){		//return la posición del valor mas pequeño
	int t;
	int temp[32];

	for(int i = 0 ; i < 32; i++){
		temp[i] = workload[i];
	}

  	for(int i = 0 ; i < ( process_num - 1 ); i++){
    	for(int j = 0 ; j < process_num - i - 1; j++){
      		if(temp[j]> temp[j+1]){
        		t = temp[j];
        		temp[j] = temp[j+1];
        		temp[j+1] = t;
      		}
    	}
  	}

	for(int i = 0 ; i < process_num; i++){
		printf("temp -> %d\n", temp[i]);
		printf("workload -> %d\n", workload[i]);
	}

	for(int i = 0 ; i < process_num; i++){
		if (workload[i]==temp[0]){
			currentProc = i;
			printf("currentProc -> %d\n", i);
		}
	}
}

void rr(){

}

void ps(){
	int t1;
	int temp1[32];

	for(int i = 0 ; i < 32; i++){
		temp1[i] = priority[i];
	}

  	for(int i = 0 ; i < ( process_num - 1 ); i++){
    	for(int j = 0 ; j < process_num - i - 1; j++){
      		if(temp1[j]> temp1[j+1]){
        		t1 = temp1[j];
        		temp1[j] = temp1[j+1];
        		temp1[j+1] = t1;
      		}
    	}
  	}

	for(int i = 1 ; i < process_num; i++){
		printf("temp -> %d\n", temp1[i]);
		printf("priority -> %d\n", priority[i]);
	}

	for(int i = 0 ; i < process_num; i++){
		if (priority[i]==temp1[0]){
			currentProc = i;
			printf("currentProc -> %d\n", i);
		}
	}
}

void ps_rr(){

}

void schedule(int id){
	if(id == 0){ fcfs();}
	else if(id == 1){ sjf();}
	else if(id == 2){ rr();}
	else if(id == 3){ ps();}
	else if(id == 4){ ps_rr();}
}
