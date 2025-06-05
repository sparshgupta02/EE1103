/* Roll number : EE23B117
Date :18/8/2023
Version:1 
Description: To estimated value of sin(x) using the truncated series
    The error between the estimated value and the true value (from the math library function sin())
Input :17 1.57
Outout:0.996,-0.004
    */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
 int factorial(int x);               
 long double mysine(float x,int n);

 int main(int argc,char**argv){
    float y=atof(argv[2]);                   //value whose sine is to be calculated =y
    int i=atoi(argv[1]);                    // number of elements in the series =i
    printf("%.3Lf,",mysine(y,i));            // value of expansion on print 
    printf("%.3Lf,",mysine(y,i)-sinl(y));    // error print 

 }
int factorial(int x){                  //funtion to define factorial 
    int z=1;
    int f=1;
    while(z<x){
        z++;
        f=f*z;
    }
    return f;

}
long double mysine(float x,int n){              //funtion to define trunkated sin exp. 
    long double s=0;
    int i=1;
    while(i<=n){
        s=s+(pow(-1,(i+1)))*(pow(x,2*i-1))/factorial(2*i-1);
        i++;
    }
    return s;

}
