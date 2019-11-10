#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

double taylor(int *currentIteracion,double sumaAcumulada, double *factoAnterior)
{

    double factActual,factAnterior,sumaActual;
	factAnterior=*factoAnterior;
    int i=*currentIteracion;
	int x=1;
    
	factActual=factAnterior*(2 * (double)i - 1)/(2 * (double)i );
	
	//double po = pow(x,(2*(double)i+1));
	//double factwo = po / (2*(double)i+1);
	sumaActual =sumaAcumulada + factActual*(pow(x,(2*(double)i+1)) / (2*(double)i+1));
	*factoAnterior=factActual;
	//printf("Suma taylor: %f\n",2*sum);
    
	i++;
	*currentIteracion=i;
	return sumaActual;
}
