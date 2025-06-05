/* Roll number : EE23B117
Date :27/10/2023
Version:1
Description: Start with a baseline noisy series of length N with zero mean and standard deviation sigma_n (you can reuse the code from a previous assignment on the Box-Muller transform that converts uniform random distribution to a normal distribution)
    Add to it a Lorentzian curve of the form f(x) = 1/(1+25 x^2), with x ranging from -2 to 2.
    Fit the data to a Gaussian of the from g(x) = A * exp(-x^2/(2*sigma_g^2))
    Plot the goodness of fit R^2 versus sigma_n. Label both axis of the plot.
Input : 1000 1 
Output:0.93
    */
    
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592654
#define e 2.71828

double box_muller(double std_dev){ //returns a normally distributed variable with standard deviation according to user input
	double u1 = (double)rand()/RAND_MAX;
	double u2 = (double)rand()/RAND_MAX;
	
	double x1 = cos(2*PI*u1)*pow(-2*log(u2),0.5);

	if(x1>3.0) x1 = 3.0;
	if(x1<-3.0)x1 = -3.0;
	return (x1*std_dev);

}

double f(double x){ //source function
	double val = 1.0/(double)(1+25*pow(x,2));
	return val;
}

double fit(double x, double A, double sigma_g){ //the gaussian function which is fit
	return  A * pow(e,(-(x*x)/(2*pow(sigma_g,2))));
}

int main(int argc, char *argv[]){
	int N = atoi(argv[1]);
	float sigma_n = atof(argv[2]);
	
	double noise[N];
	FILE *fp = fopen("data.txt", "w"); //storing the dataset
	for(int i=0;i<N;i++){
		noise[i] = box_muller(sigma_n);
		double x = -2.0 + 4.0*(double)(i)/(N-1);
		fprintf(fp, "%lf ", x);
		fprintf(fp, "%lf\n", f(x) + noise[i]);
	}
	
	FILE *fp1 = popen("gnuplot -persistant", "w"); //opening pipeline for gnuplot 
	
	fprintf(fp1, "f(x) = a*exp(-x**2/(2*b**2))\n");
	fflush(fp1);
	fprintf(fp1, "set fit quiet\n");
	fflush(fp1);
	fprintf(fp1, "fit f(x) 'data.txt' via a,b\n"); //fitting function
	fflush(fp1);
	fprintf(fp1, "set print 'pm.txt'\n");
	fflush(fp1);
	fprintf(fp1, "print a\n");
	fprintf(fp1, "print b\n");
	fflush(fp1);
	
	
	fclose(fp1);
	fclose(fp);
	
	FILE *fp2 = fopen("pm.txt", "r"); //opening file to retrieve optimal parameters
	double A;
	double sigma_g;
	fscanf(fp2, "%lf\n", &A);
	fscanf(fp2, "%lf", &sigma_g);
	
	double SSR = 0; //for R2
	double SST = 0;
	double mean =0;
	fp = fopen("data.txt", "r"); 
	for(int i =0;i<N;i++){ //caculating SSR and mean
		double y, x;
		fscanf(fp, "%lf %lf\n", &x, &y);
		SSR += pow(y-fit(x, A, sigma_g),2);
		mean += y;
	}
	mean /= N;
	fseek(fp, 0,SEEK_SET); //setting the file pointer to zero
	for(int i=0;i<N;i++){
		double y, x;
		fscanf(fp, "%lf %lf\n", &x, &y);
		SSR += pow(y-fit(x, A, sigma_g),2);
		mean += y;
	}
	mean /= N;
	fseek(fp, 0,SEEK_SET); //setting the file pointer to zero
	for(int i=0;i<N;i++){
		double y, x;
		fscanf(fp, "%lf %lf\n", &x, &y);
		SST += pow((y-mean), 2); //calculating the value of SST
	}
	double R2 = (SSR/SST)-1; //formula for R2
	printf("%lf %lf %lf %lf\n",sigma_n, A, sigma_g, R2); //printing the requirex values
	
}
