// ee23b130 Dhruv Prasad, ee23b147
// October 2023
// v1.0
// Inputs:  Takes M, T and a as argv inputs
// Outputs: avg(T) avg(a) stdev(T) stdev(a)
// Please keep M well below 100 and T/a well above 10 please please please

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#define PERCENTAGE_OF_GLOBAL_MAX_AS_THRESHOLD 70.0f

//COMMENT Defines struct for each peak
struct peak
{
	double s_amp;
	double a_width;
	double t_loc;
};

//COMMENT Declares randn function
double randn();

int main(int argc, char *argv[])
{	
	//COMMENT Variables...
	double factor=PERCENTAGE_OF_GLOBAL_MAX_AS_THRESHOLD/100;
	int M = atoi(argv[1]);
	double T = atof(argv[2]);
	double a = atof(argv[3]);
	double max=0;
	double half;
	double t1=0;
	double t2=0;
	bool x=false;
	bool y=false;
	bool z=false;
	double ycheck;
	double tcheck;
	double y1check;
	double t1check;
	double match;
	//COMMENT Initializing structs with noise (5% of a and T)
	struct peak* p = (struct peak*)malloc(M*(sizeof(struct peak)));  
	for(int m=1; m<=M; m++)
	{
		p[m-1].s_amp = 1;
		p[m-1].a_width = a + ((randn()*0.05*a)/6);
		p[m-1].t_loc = m*T + ((randn()*0.05*T)/6);
	}
	//COMMENT Creating file with coords of M lorentzians or gaussians with added noise (10% of peak)
	int q = 1;
	FILE* ptr = fopen("temp", "w");
	char gaussian[] = "gaussian";
	char Gaussian[] = "Gaussian";
	if(argc==5)
	{
		if(strcmp(argv[4],gaussian)==0 || strcmp(argv[4],Gaussian)==0)
		{
			for(double t=0; t<(M+1)*T; t=t+(a*0.1))
			{	
				double s1 = exp(((-pow((t-p[q-1].t_loc),2))*log(2)/pow(p[q-1].a_width,2)));
				double s2 = exp(((-pow((t-p[q].t_loc),2))*log(2)/pow(p[q].a_width,2)));
				if(q<=M)
				{
					if (s2 > s1)
					{
						fprintf (ptr, "%lf %lf\n", t, (s2 + ((randn()*0.1)/6)));
						q++;
					}
					else
					{
						fprintf (ptr, "%lf %lf\n", t, (s1 + ((randn()*0.1)/6)));
					}
				}
			}
		}
		else
		{
			for(double t=0; t<(M+1)*T; t=t+(a*0.1))
			{	
				double s1 = (pow(p[q-1].a_width,2)/(pow((t-p[q-1].t_loc),2) + pow(p[q-1].a_width,2)));
				double s2 = (pow(p[q].a_width,2)/(pow((t-p[q].t_loc),2) + pow(p[q].a_width,2)));
				if(q<=M)
				{
					if (s2 > s1)
					{
						fprintf (ptr, "%lf %lf\n", t, (s2 + ((randn()*0.1)/6)));
						q++;
					}
					else
					{
						fprintf (ptr, "%lf %lf\n", t, (s1 + ((randn()*0.1)/6)));
					}
				}
			}
		}
	}
	else
	{
		for(double t=0; t<(M+1)*T; t=t+(a*0.1))
		{	
			double s1 = (pow(p[q-1].a_width,2)/(pow((t-p[q-1].t_loc),2) + pow(p[q-1].a_width,2)));
			double s2 = (pow(p[q].a_width,2)/(pow((t-p[q].t_loc),2) + pow(p[q].a_width,2)));
			if(q<=M)
			{
				if (s2 > s1)
				{
					fprintf (ptr, "%lf %lf\n", t, (s2 + ((randn()*0.1)/6)));
					q++;
				}
				else
				{
					fprintf (ptr, "%lf %lf\n", t, (s1 + ((randn()*0.1)/6)));
				}
			}
		}
	}
	fclose(ptr);
	//COMMENT Finds global max of entire plot
	FILE* ptr1 = fopen("temp", "r");
	while (fscanf(ptr1, "%*f %lf", &ycheck) == 1)
	{
		if (ycheck>max)
			max = ycheck;
	}
	fclose(ptr1);
	half = max*0.5;
	//printf("%lf\n",max);
	//COMMENT This entire loop is logic to find peaks by seeing where the graph crosses half of global maximum
	//        and then storing their amplitudes... I'm not doing the <n> + 5sigma method because I think this is
	//        just better...
	int j=1;
	FILE* ptr2 = fopen("temp", "r");
	while (fscanf(ptr2, "%lf %lf", &tcheck, &ycheck) == 2)
	{
		if(y == 0)
		{
			if (ycheck>half)
			{
				if (x == 0)
				{
					t1 = tcheck;
					x = 1;
				}
			}
			else
			{
				if (x == 1)
				{
					t2 = tcheck;
					x = 0;
					y = 1;
				}			
			}
		}
		else
		{	
			//COMMENT This case is for when we find a peak
			//COMMENT Check for false peak by checking peak value great enough (70% of global maximum)
			FILE* ptr3 = fopen("temp", "r");
			fscanf(ptr3, "%lf %lf", &t1check, &y1check);
			match = 0.1*a*floor(((10*t1/a)+(10*t2/a))*0.5);
			while (t1check < match)
			{
				fscanf(ptr3, "%lf %lf", &t1check, &y1check);
			}
			fclose(ptr3);
			//COMMENT Storing info about the peak in it's struct
			if (y1check > factor*max)
			{
				p[j-1].s_amp = y1check;
				p[j-1].a_width = 0.5*(t2-t1);
				p[j-1].t_loc = 0.5*(t1+t2);
				t1=0;
				t2=0;
				//printf("%lf %lf %lf\n",p[j-1].a_width ,p[j-1].t_loc ,p[j-1].s_amp);
				y = 0;
				j++;
			}
			else
			{
				t1=0;
				t2=0;
				y = 0;
			}
		}
	}
	fclose(ptr2);
	//COMMENT I repeat the exercise but using the amplitudes to find hmfw
	j=1;
	y=0;
	x=0;
	t1=0;
	t2=0;
	ptr2 = fopen("temp", "r");
	while (fscanf(ptr2, "%lf %lf", &tcheck, &ycheck) == 2)
	{
		if(y == 0)
		{
			if (ycheck>p[j-1].s_amp*0.5)
			{
				if (x == 0)
				{
					t1 = tcheck;
					x = 1;
				}
			}
			else
			{
				if (x == 1)
				{
					t2 = tcheck;
					x = 0;
					y = 1;
				}			
			}
		}
		else
		{	
			//COMMENT This case is for when we find a peak
			//COMMENT Check for false peak by checking peak value great enough (70% of current maximum)
			FILE* ptr3 = fopen("temp", "r");
			fscanf(ptr3, "%lf %lf", &t1check, &y1check);
			match = 0.01*floor(((100*t1)+(100*t2))*0.5);
			while (t1check < match)
			{
				fscanf(ptr3, "%lf %lf", &t1check, &y1check);
			}
			fclose(ptr3);
			//COMMENT Storing info about the peak in it's struct
			if (y1check > factor*max)
			{
				p[j-1].a_width = 0.5*(t2-t1);
				p[j-1].t_loc = 0.5*(t1+t2);
				t1=0;
				t2=0;
				//printf("%lf %lf %lf\n",p[j-1].a_width ,p[j-1].t_loc ,p[j-1].s_amp);
				y = 0;
				j++;
			}
			else
			{
				t1=0;
				t2=0;
				y = 0;
			}
		}
	}
	fclose(ptr2);
	//COMMENT Calculating the mean and stddev of a and T
	double meanT=0;
	double stddevT=0;
	double meana=0;
	double stddeva=0;
	meanT = (p[M-1].t_loc-p[0].t_loc)/(double)(M-1);
	for(int m=1; m<=M-1; m++)
	{
		stddevT=stddevT+pow(p[m].t_loc-p[m-1].t_loc,2);
	}
	for(int m=1; m<=M; m++)
	{
		meana=meana+p[m-1].a_width;
		double z = p[m-1].a_width;
		stddeva=stddeva+pow(z,2);
	}
	stddevT=stddevT/(double)(M-1);
	stddevT=pow(stddevT-pow(meanT,2),0.5);
	meana=meana/M;
	stddeva=pow((stddeva/M)-pow(meana,2),0.5);
	printf("%lf %lf %lf %lf\n",meanT ,meana ,stddevT ,stddeva);
	return 0;
}

//COMMENT Function to make gaussian noise
double randn()
{
	double U1 = (double)rand()/RAND_MAX;
	double U2 = (double)rand()/RAND_MAX;
	double Z0 = pow((-2*log(U1)),0.5)*cos(2*M_PI*U2);
	if (Z0>6)
		Z0 = 6;
	else if (Z0<-6)
		Z0 = -6;
	return (Z0);
}
