#include <stdio.h>
#include <math.h>
int taylor(int num)
{
    int k,i,p;
    double x,correct_value,estimated_value,expression,accuracy;
    //scanf("%lf %d",&x,&k);
	x=1;
	k=num;
	k=k*50;    
	printf("iteraciones:%d\n",k);

    if(x<-1 || x>1)
    {
        printf("arcsin(x) doesnt exist for the given x\n");
    }
    else {
        correct_value=3.141592653589793238462643383279502884;
        expression = x;
        estimated_value=expression;
        for(i=1;i<k;i++)
        {
          p=(2*(i));
          expression= expression* (x*x)*(p-1)*(p-1)/((p)*(p+1));
          estimated_value =(estimated_value + expression);
        }
		estimated_value =estimated_value*2;
        accuracy = (estimated_value/correct_value)*100;
        printf("Teoric Value= %.9lf \nApproached Value= %.15lf \nAccuracy= %.15lf \n",correct_value,estimated_value,accuracy);
    }
    return 0;
}
