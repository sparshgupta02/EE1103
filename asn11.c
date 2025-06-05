#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define PERCENTAGE_OF_GLOBAL_MAX_AS_THRESHOLD 70.0f

double sigma_n;
int N;

double randn();
double func(double x);

int main(int argc, char* argv[])
{


char * commandsForGnuplot[] = {"set title \"FIT GRAPH\"", "plot 'data.temp'"}


    N = atoi(argv[1]);
    sigma_n = atof(argv[2]);

    FILE * fptr = fopen("datafits.txt", "w");
    double data[N+2];
   
    for (int i=0;i<N+2; i++)
    {
        data[i] = randn();
    }
   

    double step = (double)4/(N+1);
   
    for (int j = 0; j<N+2; j++)
    {
        data[j] += func(-2 + step*j);
    }

    for (int i=0; i<N+2; i++)
    {
        fprintf(fptr, "%lf %lf\n", -2 + step*i, data[i]);
    }

    fclose(fptr);
   
   
    return 0;
}

double randn()
{
double U1 = (double)rand()/RAND_MAX;
double U2 = (double)rand()/RAND_MAX;
double Z0 = pow((-2*log(U1)),0.5)*cos(2*M_PI*U2);
if (Z0>3*sigma_n)
Z0 = 3*sigma_n;
else if (Z0<-3*sigma_n)
Z0 = -sigma_n;
return (0.01*sigma_n*Z0);
}

double func(double x)
{
    return 1/(1+25*pow(x, 2));
}
