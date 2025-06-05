/* Roll number : EE23B117
Date :13/10/2023
Version:1
Description:     Convert the pseudocode for Elimination, Pivoting and Substitution into C-code
    Read the system of equations from a file (the file will contain N rows and N+1 columns)
    Using Gaussian elimination, solve the system of equations
Input : file name N {here file is txt , and N is number of variables}
Output: values of variables in line seperated manner
    */
    
//refer chapra for logic of code 
#include<stdio.h>
#include<math.h>
#include<stdlib.h>




int main(int argc, char *argv[])
{
    int temp;
    int count = 0;                                            //take input on file
    char *file = argv[1];
    FILE *filep = fopen(file,"r");
    
    int n= atoi(argv[2]);
	  if(filep == NULL){
	    printf("Could not open specified file");
	    return -1;
	  }
	  
	  
 /* while(fscanf(filep,"%d",&temp)==1){
    count++;
  }                                                              //commented code to count number on N

  printf("%d",count);

	  if(count == 0){  //do not create array of size 0
	    fclose(filep);
	  }
  else{
    //second run
    int array[count];
    rewind(filep);
    for(int i=0; i<count; i++){
      fscanf(filep,"%d",&array[count]);
    }
    fclose(filep);

    //continue using array...
  }

  return 0;
}
*/
    
         int SIZE=n;
         float x[SIZE], ratio;
         float ** a = malloc((n+1)*sizeof(float *));
         for(int i=0; i< n+1; i++) {
         a[i] = malloc((n+2)*sizeof(float));
	 }
	 int i,j,k;
	
	 /* 2. Reading Augmented Matrix */
	 for(i=1;i<=n;i++)
	 {
		  for(j=1;j<=n+1;j++)
		  {
			   fscanf(filep,"%f", &a[i][j]);
		  }
	 }
	/* Applying Gauss Elimination */
	 for(i=1;i<=n-1;i++)
	 {
		  if(a[i][i] == 0.0)
		  {
			   printf("Mathematical Error!");
			   exit(0);
		  }
		  for(j=i+1;j<=n;j++)
		  {
			   ratio = a[j][i]/a[i][i];
			   
			   for(k=1;k<=n+1;k++)
			   {
			  		a[j][k] = a[j][k] - ratio*a[i][k];
			   }
		  }
	 }
	
	 x[n] = a[n][n+1]/a[n][n];
	//Back subsitution
	 for(i=n-1;i>=1;i--)
	 {
		  x[i] = a[i][n+1];
		  for(j=i+1;j<=n;j++)
		  {
		  		x[i] = x[i] - a[i][j]*x[j];
		  }
		  x[i] = x[i]/a[i][i];
	 }
	 
	 for(i=1;i<=n;i++)
	 {
	  	printf("%f\n", x[i]);
	 }
	 
	 return(0);
}



