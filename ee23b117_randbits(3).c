/* Roll number : EE23B117
Date :25/8/2023
Version:1 
Description: Write a main program which will use flags -n -s -t and -h. 
         
             
    Take N as input on command line with the "-n" flag
    Take the seed as input with the "-s" flag
    Dynamically set the seed to the current time using if the program is called with the "-t" flag
    Compare the Hamming distance between two input bit stream files  when using the -h flag. 
        The two input files need not have the same number of random bits. The max Hamming distance would then be the shorter of the two bit sequences.
        
Input : ./asn3 -n 5 -s 12
Outout:   0	0	0	0	1
    */






#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void add_to_file(int n);            //A function to add the random bits to the file "randbits.txt" 
int nvalue, n;
int
main (int argc, char **argv)           
{
  char *nvalue = NULL;
  int index;  

  opterr = 0;
	int count =0;
	char a,b;

  while ((n = getopt (argc, argv, "n:s:th:")) != -1){
    switch (n) 
      {
      case 'n':              // takes a input from user as integer 
        nvalue = optarg;
        break;
        
      case 's':              // takes a seed from user 
         srand(atoi (optarg));
         break;
         
      case 't' :            // takes dynamic seed based upon time
        srand(time(NULL));
        break;
        
        case 'h':                                          //  gives haming distance for 2 input files of bitstrings 
				printf("\n");
				FILE* fp1 = fopen(argv[optind-1], "r"); 
				FILE* fp2 = fopen(argv[optind], "r"); 
				while(1){
        				a=fgetc(fp1);
					b=fgetc(fp2);
					if(a==EOF && b==EOF)
						break;	
					else if(a!=b)
						count+=1; 
						}	
			printf("the haming distance is %d\n",count);
			break;
						

                   
                   
      
      default:
        abort ();
      }
   
}
        
	if(nvalue != NULL)                 
    add_to_file(atoi(nvalue));          
  return 0;
}
void add_to_file(int n)
{
	FILE* f= fopen("randbits.txt", "w");

for (int i = 0; i<n ; i++ ){
fprintf(f,"%d\t", rand()%2);
 }
 fclose(f);
 
 }
 

