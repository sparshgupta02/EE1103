/* Roll number : EE23B117
Date :24/9/2023
Version:1
Description:Problem 4 uses quick weighted union find method with arrays to find requirement of road
Input: file name of text file for number of realms;number of requests;pairwise requests
Output: text file named : ee23b117_quiz2_q4_output.txt which contains report of request wise set of need to build road and time required in the next file 
    */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    clock_t runtime;
    runtime = clock();

    char *fileinput = argv[1];
    FILE *filepointerinput = fopen(fileinput,"r");

    FILE *filepointeroutput = fopen("ee23b117_quiz2_q4_output.txt","w");
    
    int numrealms = 0;
    fscanf(filepointerinput," %d",&numrealms);

    int numreq = 0;
    fscanf(filepointerinput," %d",&numreq);
    
  
   
    int* root = malloc(sizeof(int)*numrealms);
    int* weight= malloc(sizeof(int)*numrealms);
    
    for (int i=0;i<numrealms;i++)
    { root[i] = i;
    	weight[i]=1;
    }
    
    for (int k=0;k<numreq;k++)
    { 
    int firstrealm = 0;
    int secondrealm = 0;
    
    fscanf(filepointerinput,"%d",&firstrealm);
    fscanf(filepointerinput,"%d",&secondrealm);
    
    
    int parent_a=firstrealm ,parent_b=secondrealm;
    while (parent_a!=root[parent_a])
    { 
    	parent_a=root[parent_a];
    }
    while (parent_b!= root[parent_b])
    {  
       parent_b=root[parent_b];
    }	
    
    if(parent_a==parent_b)
    { fprintf(filepointeroutput, "0");
    }
    else 
    {fprintf(filepointeroutput,"1");
    	if (weight[parent_a]<weight[parent_b])
    		{root[parent_a]=parent_b;
    		weight[parent_b]+=weight[parent_a];
    		}
    	else 
    		{root[parent_b]=parent_a;
    		weight[parent_a]+=weight[parent_b];
    		}
    	
    	}
    }
    
    
      runtime = clock() - runtime;
    int runtimeinms = ((((long double)runtime)/CLOCKS_PER_SEC) * 1000);
    fprintf(filepointeroutput, "\n%d ms", runtimeinms);
    
    fclose(filepointerinput);
    fclose(filepointeroutput);

    return 0;
}
