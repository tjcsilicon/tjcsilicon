#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "file_group.c"
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    file_group *head = (file_group *)malloc(sizeof(file_group));
    
    head->size = 0;
    head->file = NULL;
    head->next = NULL;
    if(argc == 1)
    {
        insert(".", head);
    }
    else
    {
        for(int i = 1; i<argc; i++)
        {
            insert(argv[i], head);
        }
    }
    printDups(head);
    deconstruct(head);
}