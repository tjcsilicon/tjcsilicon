#include <stdio.h>
#include <stdlib.h>

int level;
enum {
	  NOT_DONE = -1
};
int *val;

void recur(int k, int nElem, void (*handlePerm)(int elems[], int nElems, void *userArg), void *userArg)
{
   int i;
   val[k] = level; 
   level++;
   if (level == nElem) {
       handlePerm(val, nElem, userArg);
   }
   for (i = 0; i < nElem; i++)
       if (val[i] == NOT_DONE)
	       recur(i, nElem, handlePerm, userArg);
       
   level--;
   val[k] = NOT_DONE;
}

void genPerms(int nElem, void (*handlePerm)(int elems[], int nElems, void *userArg), void *userArg)
{
    val = (int *) malloc(sizeof(int) * nElem); 
    int i;
    level = 0;
    for (i = 0; i < nElem; i++)
        val[i] = NOT_DONE;
    for (i = 0; i < nElem; i++)
        recur(i, nElem, handlePerm, userArg);
 
}


