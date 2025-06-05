/* Roll number : EE23B117
Date :3/11/2023
Version:1
Description: Switching of a magnetic particle is commonly used in data storage. The switching dynamics are typically described by the Landau-Lifshitz-Gilbert Equation (LLGE). For a spherical particle, the switching can be simplified using polar coordinates. Use equations (8) and (18),  from Mallinson's paper, to solve for the trajectory theta(t) and phi(t) for  M on the unit sphere.  

    Start with small angle theta_start with respect to z, and apply a field along -z
    For the same timestep delta_t, use Euler, Heun and RK45 to estimate theta(t) and phi(t) 
        You can use the RK45 code available here after suitably modifying it for your problem
        Assume H >> Hk so you can ignore Hk, and alpha < 0.2.
        Stop your simulation when theta > theta_stop
    Assuming RK45 is the most accurate, calculate the R^2 error for Euler and Heun methods
Input : theta_start theta_stop alpha delta_t
Output: alpha delta_t R^2_Euler R^2 Heun
    */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define gamma 1.76e11
#define H 1e-10
#define timestep_rk 0.01

float alpha;//defining it as a global variable 

float d_phi(float phi);//function for differential equation of phi
float d_thetha(float thetha);//function for differential equation of thetha

void rk45(float f(float),double *x, float h); //functions for various methods 
void euler(float f(float),double *x, float h);
void heun(float f(float),double *x, float h);


void main(int argc,char **argv){
    float start=atof(argv[1]);
    float end=atof(argv[2]);
    alpha=atof(argv[3]);
    double delta_t=atof(argv[4]);//timestep
    
    double phi_rk=0;//variables for rk method
    double thetha_rk=start;
    double phi_euler=0;//variables for euler method
    double thetha_euler=start;
    double phi_heun=0;//variables for heun method
    double thetha_heun=start;
    
    float ssr_euler;//variable to get sum of squares of regression 
    float mean_euler=0;
    float ssum_euler=0;//sum of squares of each data point 
    
    float ssr_heun;//variable to get sum of squares of regression
    float mean_heun=0;
    float ssum_heun=0;//sum of squares of each data point 
    float count=1;
    
    
    FILE *fptr1=fopen("rk45.txt","w");
    FILE *fptr2=fopen("euler.txt","w");
    FILE *fptr3=fopen("heun.txt","w");
    FILE *fptr4=fopen("rk45.txt","r");
    
    float time=0;
    fprintf(fptr1,"%f %f %f\n",phi_rk,thetha_rk,time);//printing the intial conditions to the file
    fprintf(fptr2,"%f %f\n",phi_euler,thetha_euler);
    fprintf(fptr3,"%f %f\n",phi_heun,thetha_heun);
    while(thetha_rk<=end){
        rk45(d_phi,&phi_rk,timestep_rk);//calling the first method
        rk45(d_thetha,&thetha_rk,timestep_rk);
        time+=timestep_rk; 
        fprintf(fptr1,"%f %f %f\n",phi_rk,thetha_rk,time);
               
        }
    fclose(fptr1);
    time=delta_t;
    while(thetha_euler<=end){
        euler(d_phi,&phi_euler,delta_t);//calling euler method
        euler(d_thetha,&thetha_euler,delta_t);//adding the thetha and time to the file 
        
        heun(d_phi,&phi_heun,delta_t);//calling the heun method
        heun(d_thetha,&thetha_heun,delta_t);
        //printing to a file
        
        fprintf(fptr2,"%f %f\n",phi_euler,thetha_euler);
        fprintf(fptr3,"%f %f\n",phi_heun,thetha_heun);
        float time_value;
        float thetha_value;
        float phi_value;
        while (fscanf(fptr4, "%f %f %f",&phi_value,&thetha_value,&time_value) == 3){ // loop until end-of-file
            	if(abs(time-time_value)/time<0.001){
            	ssr_euler+=pow((thetha_value-thetha_euler),2);//getiing the sum of squares 
        	ssr_heun+=pow((thetha_value-thetha_heun),2);
            	}
        }
      
        mean_heun+=thetha_heun;//getting the sum of values first and mean will be calulated later
        mean_euler+=thetha_euler;
        count+=1;
        
        ssum_heun+=pow(thetha_heun,2);//getting the sum of squares of each data point 
    	ssum_euler+=pow(thetha_euler,2);
        
    	}
    	
    	float euler_sum=mean_euler;
    	float heun_sum=mean_heun;
	mean_heun=mean_heun/count;
	mean_euler=mean_euler/count;
	/*
	sst is sum of (value-mean_of_value)**2
	here it is calculated as sum of( value**2 +mean_of_value**2 -2*value*mean_of_value) (A-B)**2 formula
	this is finally evaluated as (value**2 + number of terms*mean_of_value**2  -2*mean*sum_of_values)
	taking the mean as a constant value
	*/
	float sst_euler=ssum_euler+count*pow(mean_euler,2)-2*mean_euler*euler_sum;
    	float sst_heun=ssum_heun+count*pow(mean_heun,2)-2*mean_heun*heun_sum;
    	//formula of r2 is 1-ssr/sst
	float r2_heun=1-ssr_heun/sst_heun;
	float r2_euler=1-ssr_euler/sst_euler;
	
	float switching_time =((alpha*alpha +1)*2*log(cos(start/2)/sin(start/2)))/(gamma*alpha*H);
	printf("%f %f %f %f \n",alpha,delta_t,r2_euler,r2_heun);//final values to be printed 
	fclose(fptr2);
	fclose(fptr3);
	fclose(fptr4);
	}
//finding phi value	
float d_phi(float phi){//differential equation of phi
    phi=((gamma*H)/(alpha*alpha+1));
    return phi;}
//finding theta value     
float d_thetha(float thetha){//differential equation of phi
    thetha=((gamma*alpha*H*sin(thetha))/(alpha*alpha+1));
    return thetha;}
//function for rk45 solver
void rk45(float f(float),double *x, float h){

    float c21 = 0.25;
    float c31 = 0.09375, c32 = 0.28125;
    float c41, c42,c43;
    float c51, c52 = -8.0, c53, c54;
    float c61, c62 = 2, c63, c64;
    float c65 = -0.275;
    float a1, a2 = 0, a3, a4, a5 = -0.2;
    float b1, b2 = 0, b3, b4, b5= -0.18, b6;
    float F1, F2, F3, F4, F5, F6, x4;

    
    c41 = (float) 1932/(float) 2197;
    c42 = (float) -7200/(float) 2197;
    c43 = (float) 7296/(float) 2197;
    c51 = c53 = (float) 439/ (float) 216;
    c54 = (float) -845/(float) 4104;
    c61 = (float) -8/(float) 27;
    c63 = (float) -3544/(float) 2565;
    c64 = (float) 1859/(float) 4104;
    a1 = (float) 25/(float) 216;
    a3 = (float) 1408/(float) 2565;
    a4 = (float) 2197/(float) 4104;
    b1 = (float) 16/(float) 135;
    b3 = (float) 6656/(float) 12825;
    b4 = (float) 28561/(float) 56430;
    b6 = (float) 2/(float) 55;

    
    F1 = h * f(*x);
    F2 = h * f(*x + c21 * F1);
    F3 = h * f(*x + c31 * F1 + c32 * F2);
    F4 = h * f(*x + c41 * F1 + c42 * F2 + c43 * F3);
    F5 = h * f(*x + c51 * F1 + c52 * F2 + c53 * F3 + c54 * F4 );
    F6 = h * f(*x + c61 * F1 + c62 * F2 + c63 * F3 + c64 * F4 + c65 * F5);

    *x += b1 * F1 + b3 * F3 + b4 * F4 + b5 * F5 + b6 * F6;
	}

//code for euler function
void euler(float f(float),double *x, float h){
	*x+=h*f(*x);//simple euler method 
	}
//code for heun function	
void heun(float f(float),double *x, float h){
	double predictor=*x+h*f(*x);
	*x = *x + (h / 2.0) * (f(*x) + f(predictor));//getting the average of the slope using the predictor 
	}
    
        
        
        
        
        
        
        
