#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


/**
* Aproxima arcsen(x) mediante Serie de Taylor
*  @arg x Variable dependiente de la función
*  @arg n Cantidad de términos de la serie
*/
double taylor(int n, double sumaAcumulada)
{
    double fact,sum;
    fact=1.0;
	sum=sumaAcumulada;
    int i;
	int x=1;
    for(i=n;i<=n;i++)
    {
	fact *= (2 * (double)i - 1)/(2 * (double)i );
	double po = pow(x,(2*(double)i+1));
	double factwo = po / (2*(double)i+1);
	sum += fact * (pow(x,(2*(double)i+1)) / (2*(double)i+1));
    }

    return sum;


	//printf("aproximación pi = %lf\n",2*arcsin(1.0,n*50));
}
/*
void taylor (){
	int n;
    printf("Cantidad de unidades de trabajo: ");
    scanf("%d",&n);
    printf("2 arcsin(1) = %lf\n",2*arcsin(1.0,n*50));
}*/
