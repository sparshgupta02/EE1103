/*
roll no: EE23B137
Name: Krutarth Patel

Input:
N-> the number of points in the dataset
sigma_n -> the standard deviation of the error
Working:
the program fits a gaussian in a noisy lorentzian dataset and gives out the optimal parameters along with the R2 score and the input sigma_n

Output:
sigma_n, A, sigma_g, R2
sigma_n->input standard deviation of the lorentzian
A, sigma_g-> parameter of the gaussian
R2 score
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
		SST += pow((y-mean), 2); //calculating the value of SST
	}
	double R2 = fabs(1-(SSR/SST)); //formula for R2
	printf("%lf %lf %lf %lf\n",sigma_n, A, sigma_g, R2); //printing the requirex values
	
}
