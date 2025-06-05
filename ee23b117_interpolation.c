/* Roll number : EE23B117
Date :20/10/2023
Version:2
Description:     Convert the pseudo code to c-code for both Gaussian and Lagrangian interpolation
    Prob 18.26: f(x) = 1/(1+25 x^2)
        Generate the 4th order Lagrange polynomial using 5 equidistant points for x in the interval [-1,1]
        Find the value of f(x0) using a 4th order Newton interpolating polynomial. Assume x0=0.8 to test your code
Input : 10 -1 1 0.8
Output: 0.058811 
    */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//function to generate newton polynomial and taking output as newton  
double newtonian(int N, float x[],float y[], double x0){
	double **fdd = malloc(sizeof(double *)*N);
	for(int i=0;i<N;i++)fdd[i] = malloc(sizeof(double)*N);
	
	for(int i =0;i<N;i++) fdd[i][0] = y[i];
	
	for(int j = 1;j<N;j++){
		for(int i=0;i<N-j;i++){
			fdd[i][j] = (fdd[i+1][j-1] - fdd[i][j-1])/(x[i+j]-x[i]);
		}
	}
	double xterm = 1;
	double newton = fdd[0][0];
	
	for(int i=1;i<N;i++){
		xterm *= (x0-x[i-1]);
		newton += fdd[0][i]*xterm;
		
	}
	return newton;
}

int main(int argc,char* argv[])
{
	 // taking input 
	 int n= atoi(argv[1]);                                     //order of polynomial 
	 float xstart =atof(argv[2]);                               // start point
	 float xend =atof(argv[3]);                                 //end point 
	 float xo=atof(argv[4]);	                            // point of intrapolation 
	 
	 //defining arrays and variables 
	 float x[n+1], y[n+1] ,p;          
	
	 float yp[n+1]; 
	 
	 for (int i=0;i<=n;i++){
	 yp[i]=0;
	 }
	 float x0[n+1];
	 	 
	 for(int i=0;i<=n;i++)
	 {
		  x[i]=(xstart +((xend-xstart)/n)*i);                    //taking step size and defining array for points known 
		  y[i]= 1/(25*pow(x[i],2)+1); //function given to us and using it to create correspoding array in y 
	 }
	
	
	 /* Implementing Lagrange Interpolation */
	 
for (int k=0;k<=n;k++){
         
         x0[k]=(xstart +((xend-xstart)/n)*(k+0.01));     //using lagrange function then adding a lil bit difference to the use of point 
	 
	 for(int i=0;i<=n;i++)                                            //x0 is the array on shifting x values using step size 
	 {
		  p=1;
		  for(int j=0;j<=n;j++)
		  {
			   if(i!=j)
			   {
			    	p = p* (x0[k] - x[j])/(x[i] - x[j]);           //function for lagrangian polynomial 
			    	//printf("%.2f \n", p);
			   }
		  }
		  yp[k] = yp[k] + p * y[i];                //yp is the output arr of lagrangian 
	 }
	 
	// printf("%f\n",yp[k]);
	//printf("lagrange Interpolated value at %.3f is %.3f.", xo, yp);
	 }
	 
double newton = newtonian(n+1, x0,yp, xo);
printf("%f", newton);

}

