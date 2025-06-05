/* Roll number : EE23B117
Date :20/10/2023
Version:2
Description:     Convert the pseudo code to c-code for both Gaussian and Lagrangian interpolation
    Prob 18.26: f(x) = 1/(1+25 x^2)
        Generate the 4th order Lagrange polynomial using 5 equidistant points for x in the interval [-1,1]
        Find the value of f(x0) using a 4th order Newton interpolating polynomial. Assume x0=0.8 to test your code
Input : 5 -1 1 0.8
Output: 0.183957 
    */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>


void gaussEliminationLS(int m, int n, double a[m][n], double x[n-1]){
    int i,j,k;
    for(i=0;i<m-1;i++){
        //Partial Pivoting
        for(k=i+1;k<m;k++){
            //If diagonal element(absolute vallue) is smaller than any of the terms below it
            if(fabs(a[i][i])<fabs(a[k][i])){
                //Swap the rows
                for(j=0;j<n;j++){                
                    double temp;
                    temp=a[i][j];
                    a[i][j]=a[k][j];
                    a[k][j]=temp;
                }
            }
        }
        //Begin Gauss Elimination
        for(k=i+1;k<m;k++){
            double  term=a[k][i]/ a[i][i];
            for(j=0;j<n;j++){
                a[k][j]=a[k][j]-term*a[i][j];
            }
        }
         
    }
    //Begin Back-substitution
    for(i=m-1;i>=0;i--){
        x[i]=a[i][n-1];
        for(j=i+1;j<n-1;j++){
            x[i]=x[i]-a[i][j]*x[j];
        }
        x[i]=x[i]/a[i][i];
    }
             
}



void cubicspline(int n, double step, double sig[n+1], double y[n+1], double as[n], double bs[n], double cs[n], double ds[n]){
    for(int i=0;i<n;i++){
        ds[i]=y[i];
        bs[i]=sig[i]/2.0;
        as[i]=(sig[i+1]-sig[i])/(step*6.0);
        cs[i]=(y[i+1]-y[i])/step-step*(2*sig[i]+sig[i+1])/6.0;
    }
}


void splinegen(int n, double step, double a[n-1][n], double y[n+1]){
    for(int i=0;i<n-1;i++){
        a[i][i]=4*(step);
    }
    for(int i=0;i<n-2;i++){
        a[i][i+1]=step;
        a[i+1][i]=step;
    }
    for(int i=1;i<n;i++){
        a[i-1][n-1]=(y[i+1]-y[i])*6/(double) step-(y[i]-y[i-1])*6/(double)step;
    }
} 

void printMatrix(int m, int n, double matrix[m][n]){
    int i,j;
    for(i=0;i<m;i++){
        for(j=0;j<n;j++){
            printf("%lf\t",matrix[i][j]);
        }
        printf("\n");
    } 
}


int main(int argc,char* argv[])
{
	 // taking input 
	 int n= atoi(argv[1]);                                     //order of polynomial 
	 float xstart =atof(argv[2]);                               // start point
	 float xend =atof(argv[3]);                                 //end point 
	 float xo=atof(argv[4]);	                            // point of intrapolation 
	 
	 //defining arrays and variables 
	double x[n+1], y[n+1];   
	float step=(xend-xstart)/n ;      
	double as[n]; //array to store the ai's
        double bs[n]; //array to store the bi's
        double cs[n]; //array to store the ci's
        double ds[n]; //array to store the di's
	double sig[n+1]; //array to store Si's
        double sigTemp[n-1]; //array to store the Si's except S0 and Sn
        sig[0]=0;
        sig[n]=0;
        
	 /*double ** a = malloc((n+1)*sizeof(double *));
         for(int i=0; i< n+1; i++) {
         a[i] = malloc((n+2)*sizeof(double));
	 }
	 */
	 
	 for(int i=0;i<=n;i++)
	 {
		  x[i]=(xstart +((xend-xstart)/n)*i);      //taking step size and defining array for points known 
		  y[i]= 1/(25*pow(x[i],2)+1);             //function given to us and using it to create correspoding array in y 
	 }
	 double a[n-1][n]; //matrix to store the tridiagonal system of equations that will solve for Si's
    splinegen(n,step,a,y); //to initialize tri[n-1][n]

    //Perform Gauss Elimination 
    gaussEliminationLS(n-1,n,a,sigTemp);
    for(int i=1;i<n;i++){
        sig[i]=sigTemp[i-1];
    }
    
    //calculate the values of ai's, bi's, ci's, and di's
    cubicspline(n,step,sig,y,as,bs,cs,ds);
    
    int i = (xo-xstart)*(n-1)/(xend-xstart);
    double interpolatedval = as[i]*pow(xo-(xstart+step*i),3)+bs[i]*pow(xo-(xstart+step*i),2)+cs[i]*(xo-(xstart+step*i))+ds[i];
    printf("%lf",interpolatedval);   
     
}
