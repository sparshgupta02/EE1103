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
#include <math.h>
#include <string.h>
#include <stdlib.h>


//defining required values 
#define gamma 17.6*10e6
#define H (pow(10,-10))
#define r (1.0)




long double f(long double ALPHA, long double THETA);	   //function to find d(theta)/dt for a particular theta
long double g(long double ALPHA1, long double THETA1);   //function to find d(phi)/dt for a particular theta; for this case, d(phi)/dt depends on theta

int main(int argc, char* argv[])
    {
    FILE* fptr_euler = fopen("euler.txt","w");	//files to write x,y,z obtained from the euler, heun and runge kutta methods
    FILE* fptr_heun = fopen("heun.txt","w");
    FILE* fptr_RK = fopen("RK.txt","w");
    FILE* fptr_euler_theta = fopen("euler_theta.txt","w");//files to write theta obtained from the euler, heun and runge kutta methods
    FILE* fptr_heun_theta = fopen("heun_theta.txt","w");
    FILE* fptr_RK_theta = fopen("RK_theta.txt","w");
    long double theta_start = atof(argv[1]);
    long double theta_stop = atof(argv[2]);
    long double alpha = atof(argv[3]);
    long double delta_t = atof(argv[4]);
    long double theta = theta_start;
    long double phi = 0.0;
    long double temp,x,y,z;
    int count_euler_loop = 0;
    while (theta < theta_stop)		//loop to find the values of x, y, z, theta from euler's method and write it onto the file
        {
        temp = theta;		
        theta+=f(alpha,theta)*delta_t;
        phi+=g(alpha,temp)*delta_t;
        x = r*sin(theta)*cos(phi);		//converting spherical polar coordinates to cartesian coordinates
        y = r*sin(theta)*sin(phi);
        z = r*cos(theta);
        fprintf(fptr_euler,"%Lf\t%Lf\t%Lf\n",x,y,z);		//writing x, y, z onto the file
        fprintf(fptr_euler_theta,"%Lf\n",theta);			//writing theta onto the file
        count_euler_loop+=1;
        }
    fclose(fptr_euler);
    theta = theta_start;
    phi = 0.0;
    long double theta0;
    long double new_f;
    long double new_g;
    int count_heun_loop = 0;
    while (theta<theta_stop)		//loop to find the values of x, y, z, theta from heun's method and write it onto the file
        {
        temp = theta;
        theta0 = theta + f(alpha,theta)*delta_t;
        new_f = (f(alpha,theta) + f(alpha,theta0))/2;
        theta+=new_f*delta_t;
        new_g = (g(alpha,temp) + g(alpha,theta))/2;
        phi+=new_g*delta_t;
        x = r*sin(theta)*cos(phi);		//converting spherical polar coordinates into cartesian coordinates
        y = r*sin(theta)*sin(phi);
        z = r*cos(theta) - 0.1;
        fprintf(fptr_heun,"%Lf\t%Lf\t%Lf\n",x,y,z);		//writing x, y, z onto the file
        fprintf(fptr_heun_theta,"%Lf\n",theta);			//writing theta onto the file
        count_heun_loop+=1;
        }
    fclose(fptr_heun);
    theta = theta_start;
    phi = 0.0;
    long double k1_theta,k2_theta,k3_theta,k4_theta,k1_phi,k2_phi,k3_phi,k4_phi;		//variables needed for runge kutta method
    int count_RK_loop = 0;
    while (theta < theta_stop)			//loop to find the values of x, y, z, theta from runge kutta method and write it onto the file
        {
        temp = theta;
        k1_theta = f(alpha,theta);
        k2_theta = f(alpha,theta + (1.0/2.0)*k1_theta*delta_t);
        k3_theta = f(alpha,theta + (1.0/2.0)*k2_theta*delta_t);
        k4_theta = f(alpha,theta + k3_theta*delta_t);
        theta+=(1.0/6.0)*delta_t*(k1_theta + 2*k2_theta + 2*k3_theta + k4_theta);
        k1_phi = g(alpha,temp);
        k2_phi = g(alpha,temp + (1.0/2.0)*k1_phi*delta_t);
        k3_phi = g(alpha,temp + (1.0/2.0)*k2_phi*delta_t);
        k4_phi = g(alpha,temp + k3_phi*delta_t);
        phi+=(1.0/6.0)*delta_t*(k1_phi + 2*k2_phi + 2*k3_phi + k4_phi);
        x = r*sin(theta)*cos(phi);		//converting spherical polar coordinates into cartesian coordinates
        y = r*sin(theta)*sin(phi);
        z = r*cos(theta) +0.1;
        fprintf(fptr_RK,"%Lf\t%Lf\t%Lf\n",x,y,z);		//writing x, y, z onto the file
        fprintf(fptr_RK_theta,"%Lf\n",theta);		//writing theta onto the file
        count_RK_loop+=1;
        }
    fclose(fptr_RK);
    int min_count;		//min_count needed as number of records may be different for different methods
    if ((count_euler_loop < count_heun_loop) && (count_euler_loop < count_RK_loop))
        min_count = count_euler_loop;
    if ((count_heun_loop < count_euler_loop) && (count_heun_loop < count_RK_loop))
        min_count = count_heun_loop;
    if ((count_RK_loop < count_heun_loop) && (count_RK_loop < count_euler_loop))
        min_count = count_RK_loop;
    long double sum_perfect_x = 0.0;		//perfect_<variable_name> represents the variable obtained from RK45
    long double sum_perfect_y = 0.0;
    long double sum_perfect_z = 0.0;
    long double sum_perfect_theta = 0.0;
    long double mean_perfect_x, mean_perfect_y, mean_perfect_z,mean_perfect_theta;
    FILE* f_euler = fopen("euler.txt","r");
    FILE* f_heun = fopen("heun.txt","r");                                   //reading from all files
    FILE* f_RK = fopen("RK.txt","r");
    FILE* f_euler_theta = fopen("euler_theta.txt","r");
    FILE* f_heun_theta = fopen("heun_theta.txt","r");
    FILE* f_RK_theta = fopen("RK_theta.txt","r");
    for (int i=0;i<min_count;i++)
        {
        fscanf(f_RK,"%Lf %Lf %Lf",&x,&y,&z);
        fscanf(f_RK_theta,"%Lf",&theta);
        sum_perfect_x+=x;
        sum_perfect_y+=y;
        sum_perfect_z+=z;
        sum_perfect_theta+=theta;
        }
    mean_perfect_x = sum_perfect_x/min_count;
    mean_perfect_y = sum_perfect_y/min_count;
    mean_perfect_z = sum_perfect_z/min_count;
    mean_perfect_theta = sum_perfect_theta/min_count;
    long double R2_x_euler, R2_y_euler, R2_z_euler,x_actual,x_predicted_euler,y_actual,y_predicted_euler,z_actual,z_predicted_euler,theta_predicted_euler,theta_actual;
    long double R2_x_heun, R2_y_heun, R2_z_heun,x_predicted_heun,y_predicted_heun,z_predicted_heun,theta_predicted_heun;
    long double R2_theta_euler,R2_theta_heun;
    long double SSR_x_euler = 0.0;
    long double SSR_y_euler = 0.0;
    long double SSR_z_euler = 0.0;
    long double SSR_theta_euler = 0.0;
    long double SSR_theta_heun = 0.0;
    long double SST_theta = 0.0;
    long double SST_x = 0.0;
    long double SST_y = 0.0;
    long double SST_z = 0.0;
    long double SSR_x_heun = 0.0;
    long double SSR_y_heun = 0.0;
    long double SSR_z_heun = 0.0;
    
    fseek(f_RK_theta, 0, SEEK_SET);
    fseek(f_RK, 0, SEEK_SET);
    for (int i=0;i<min_count;i++)
        {
        fscanf(f_RK,"%Lf %Lf %Lf",&x_actual,&y_actual,&z_actual);
        fscanf(f_RK_theta,"%Lf",&theta_actual);
        fscanf(f_euler,"%Lf %Lf %Lf",&x_predicted_euler,&y_predicted_euler,&z_predicted_euler);
        fscanf(f_heun,"%Lf %Lf %Lf",&x_predicted_heun,&y_predicted_heun,&z_predicted_heun);
        fscanf(f_euler_theta,"%Lf",&theta_predicted_euler);
        fscanf(f_heun_theta,"%Lf",&theta_predicted_heun);
        SSR_x_euler+=pow(x_actual - x_predicted_euler,2);                    //finding R^2 values 
        SSR_y_euler+=pow(y_actual - y_predicted_euler,2);
        SSR_z_euler+=pow(z_actual - z_predicted_euler,2);
        SSR_theta_euler+=pow(theta_actual - theta_predicted_euler,2);
        SST_x+=pow(x_actual - mean_perfect_x,2);
        SST_y+=pow(y_actual - mean_perfect_y,2);
        SST_z+=pow(z_actual - mean_perfect_z,2);
        SST_theta+=pow(theta_actual - mean_perfect_theta,2);
        SSR_theta_heun+=pow(theta_actual - theta_predicted_heun,2);
        }
  
    R2_theta_euler = 1-(SSR_theta_euler/SST_theta);
    R2_theta_heun = 1-(SSR_theta_heun/SST_theta);
    fclose(f_RK);
    fclose(f_euler);
    fclose(f_heun);
    fclose(f_euler_theta);
    fclose(f_heun_theta);
    fclose(f_RK_theta);			
    printf("%Lf\t%Lf\t%Lf\t%Lf\n",alpha,delta_t,R2_theta_euler,R2_theta_heun);
    return 0;
    }
    
long double f(long double ALPHA, long double THETA)
    {
    return (gamma*ALPHA*H*sin(THETA)/(ALPHA*ALPHA + 1));
    }
long double g(long double ALPHA1, long double THETA1)
    {
    return (f(ALPHA1,THETA1)/(ALPHA1*sin(THETA1)));
    }

