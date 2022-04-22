#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <linux/limits.h>
#include "file_group.h"
#include "compare_files.c"


void insert(char *name, struct file_group *head)
{
    struct stat buf;
    lstat(name, &buf);
    if (S_ISDIR(buf.st_mode))
    {
        insertdir(name, head);
    }
    else if (!S_ISLNK(buf.st_mode) && S_ISREG(buf.st_mode))
    {
        insertfile(name, head);
    }
    
}

void insertdir(char* name, struct file_group *head)
{
    DIR *fd = opendir(name);
    if (fd == NULL)
    {
        fprintf(stderr, "Error opening %s\n", name);
        return;
    }
    char *temp = (char*) malloc(PATH_MAX);
    struct stat filetype;

    struct dirent *ptr = readdir(fd); //TODO: Make filename include reletive path ie. brothers/moe
    ptr = readdir(fd);
    ptr = readdir(fd);
    sprintf(temp, "%s", name);
    strcat(temp, "/");
    while(ptr != NULL)
    {
        strcat(temp, ptr->d_name);
        lstat(temp, &filetype);
        if (S_ISDIR(filetype.st_mode))
        {
            insertdir(temp, head);
        }
        else if (!S_ISLNK(filetype.st_mode) && S_ISREG(filetype.st_mode))
        {
            insertfile(temp, head);
        }
        ptr = readdir(fd);
        
        sprintf(temp, "%s", name);
        strcat(temp, "/");
    }
    free(fd);
    free(temp);
}

void insertfile(char* name, struct file_group *head)
{
    struct File *newFile = (File*) calloc(1, sizeof(File));
    struct file_group *temp = head;
    struct stat buf;
    lstat(name, &buf);
    newFile->name = str(name);
    if(buf.st_size == 0)
    {
        return;
    }
    if(!head->next) //Base case for first file added
    {
        struct file_group *newGroup = (file_group*) calloc(1, sizeof(file_group));
        head->next = newGroup;
        newGroup->size = buf.st_size;
        newGroup->file = newFile;
        newGroup->groupSize = 1;
        newGroup->next = NULL;
        return; 
    }
    while(temp) //inserts additional files
    {   //compares the size to the size of each file_group
        if(buf.st_size == temp->size)
        {
            int res = compareFiles(name, temp->file->name);
            if (res == 1) //checks to see if its a duplicate
            {
                newFile->next = temp->file;
                temp->file = newFile;
                temp->groupSize++;
                return;
            }
            else if (res < 0)
            {
                free(newFile->name);
                free(newFile);
                return;
            }
            else //if it isn't a duplicate, it adds it a new group on the end of the list
            {
                break;
            }
        }
        temp = temp->next;
    }
    struct file_group *newGroup = (file_group*) calloc(1, sizeof(file_group));
    temp = head;
    while(temp->next)
    {
        temp = temp->next;
    }
    temp->next = newGroup;
    newGroup->size = buf.st_size;
    newGroup->file = newFile;
    newGroup->groupSize = 1;
    newGroup->next = NULL;
    return;
}


void printDups(struct file_group *head)
{
    struct file_group *temp = head->next;
    struct File *tempFile;
    while(temp)
    {
        int i = 1;
        tempFile = temp->file;
        if(temp->groupSize > 1)
        {
            while(tempFile)
            {
                printf("%d %d %s\n", temp->groupSize, i, tempFile->name);    
                i++;
                tempFile = tempFile->next;
            }
        }
        temp = temp->next;
    }
}

void deconstruct(struct file_group *head)
{
    struct file_group *temp = head->next;
    struct File *tempFile = NULL;

    while(temp)
    {
        while(temp->file)
        {
            tempFile = temp->file;
            temp->file = temp->file->next;
            free(tempFile->name);
            free(tempFile);
        }
        head->next = head->next->next;
        free(temp);
        temp = head->next;
    }
    free(head->file);
    free(head);
}