//Author Timothy J Cain
//CPTS360 Lab01_compare_files.c
//
// It's not a bad practice to list *why* you include particular
// headers.
#include <stdio.h> // for FILE, NULL, fopen(), and getc()

#include "eprintf.h" // for eprintf_fail()

// Although not strictly required, its a good practice to include the
// header file that corresponds to this source file to detect any
// discrepancies between the function's declaration and its
// definition.


int compareFiles(char *fname0, char *fname1)
{
    FILE *file0;
    file0 = fopen(fname0, "r");
    if(file0 == NULL)
    {
	    //eprintf_fail("Error opening %s\n", fname0);
        fprintf(stderr, "Error opening %s\n", fname0);
        return -1;
    }
    FILE *file1;
    file1 = fopen(fname1, "r");
    if(file1 == NULL)
    {
	    //eprintf_fail("Error opening %s\n", fname1);
        fprintf(stderr, "Error opening %s\n", fname1);
        fclose(file0);
        return -1;
    }
    
    int ch0;
    ch0 = getc(file0);
    int ch1;
    ch1 = getc(file1);
    
    
    do
    {
	if(ch0 == ch1)
	{
	    ch0 = getc(file0);
        ch1 = getc(file1);
	}
	else
	{
	    fclose(file0);
	    fclose(file1);
            return 0;
	}	    
    } while(ch0 != EOF && ch1 != EOF);


    fclose(file0);
    fclose(file1);
    return 1; 
}
