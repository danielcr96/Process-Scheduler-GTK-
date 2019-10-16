#include "parser.h"
#include "taylor.h"
//gcc -o main main.c -lm
//main input.txt
int main(int argc,char *argv[]){

	
	input_parser(argv[1]);
	printf("Num_threads from file=%d\n", thread_num);
    printf("iteraciones=%d\n", iteraciones);
	printf("Modo from file=%d\n", modo);
	taylor(iteraciones);

}
