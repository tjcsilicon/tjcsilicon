#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <grp.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <linux/limits.h>
#include "whocan.h"
#include <unistd.h>

int canReadDir = 0;

void canUser(user* user, char* action, struct stat fileStats, struct stat parentFileStats)
{
    if(strcmp(user->name, "root") == 0)
    {
        return;
    }
    if(canReadDir == 1)
    {
        canEveryone = 0;
        user->name[0] = '\0';
        return;
    }
    else if((fileStats.st_mode & S_ISVTX) && strcmp(action, "delete") == 0)
    {
        if(strcmp(user->name, "root") == 0 || fileStats.st_uid == user->uid)
        {
            return;
        }
        else
        {
            canEveryone = 0;
            user->name[0] = '\0';
            return;
        }
    }
    else if(S_ISREG(fileStats.st_mode))
    {
        if(canReadDir == 1)
        {
            canEveryone = 0;
            user->name[0] = '\0';
            return;
        }
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IRUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "execute") == 0)
        {
            if(fileStats.st_mode & S_IXUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IRUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(S_ISDIR(fileStats.st_mode))
    {
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IRUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "search") == 0)
        {
            if(fileStats.st_mode & S_IXUSR)
            {
                
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IRUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "cd") == 0)
        {
            if(fileStats.st_mode & S_IXUSR)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
    }
}

void canGroup(user* user, char* action, struct stat fileStats, struct stat parentFileStats)
{
    if(strcmp(user->name, "root") == 0)
    {
        return;
    }
    if(canReadDir == 1)
    {
        canEveryone = 0;
        user->name[0] = '\0';
        return;
    }
    else if((fileStats.st_mode & S_ISVTX) && strcmp(action, "delete") == 0)
    {
        if(strcmp(user->name, "root") == 0 || fileStats.st_uid == user->uid)
        {
            return;
        }
        else
        {
            canEveryone = 0;
            user->name[0] = '\0';
            return;
        }
    }
    else if(S_ISREG(fileStats.st_mode))
    {
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IRGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "execute") == 0)
        {
            if(fileStats.st_mode & S_IXGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IRGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(S_ISDIR(fileStats.st_mode))
    {
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IRGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "search") == 0)
        {
            if(fileStats.st_mode & S_IXGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IRGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "cd") == 0)
        {
            if(fileStats.st_mode & S_IXGRP)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
        
    } 

}
void canOther(user* user, char* action, struct stat fileStats, struct stat parentFileStats)
{
    if(strcmp(user->name, "root") == 0)
    {
        return;
    }
    if(canReadDir == 1)
    {
        canEveryone = 0;
        user->name[0] = '\0';
        return;
    }
    else if((fileStats.st_mode & S_ISVTX) && strcmp(action, "delete") == 0) 
    {
        if(strcmp(user->name, "root") == 0 || fileStats.st_uid == user->uid)
        {
            return;
        }
        else
        {
            canEveryone = 0;
            user->name[0] = '\0';
            return;
        }
    }
    else if(S_ISREG(fileStats.st_mode))
    {
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IROTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "execute") == 0)
        {
            if(fileStats.st_mode & S_IXOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IROTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
    }
    else if(S_ISDIR(fileStats.st_mode))
    {
        if(strcmp(action, "read") == 0)
        {
            if(fileStats.st_mode & S_IROTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "write") == 0)
        {
            if(fileStats.st_mode & S_IWOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "search") == 0)
        {
            if(fileStats.st_mode & S_IXOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "delete") == 0)
        {
            if(parentFileStats.st_mode & S_IWOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "ls") == 0)
        {
            if(fileStats.st_mode & S_IROTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else if(strcmp(action, "cd") == 0)
        {
            if(fileStats.st_mode & S_IXOTH)
            {
                return;
            }
            else 
            {
                canEveryone = 0;
                user->name[0] = '\0';
                return;
            }
        }
        else
        {
            fprintf(stderr, "error, not a valid command\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
            exit(EXIT_FAILURE);
        }
    }

}
void doAction(char* action, char* file)
{
    char* realPath = (char*)calloc(PATH_MAX, sizeof(char*));
    realpath(file, realPath);
    struct stat fileStats;
    struct stat parentStats;
    if(lstat(realPath, &fileStats) < 0)
    {
        fprintf(stderr, "error, could not find file \"%s\"\n", file);
        exit(EXIT_FAILURE);
    }
    char *p;
    char chsave;
    for(int i = 0; i<numGroups; i++)
    {
        if(groupList[i]->uid == fileStats.st_uid || strcmp(groupList[i]->name, "root") == 0)
        {
            p = &realPath[1];
            while(p != NULL)
            {
                chsave = *p;
                *p = '\0';
                lstat(realPath, &parentStats);
                *p = chsave;
                p = strchr(p+1, '/');
                if(!(S_IXUSR & parentStats.st_mode))
                {
                    canReadDir = 1;
                    break;
                }
            }
            canUser(groupList[i], action, fileStats, parentStats);
            canReadDir = 0;
        }
        else if(groupList[i]->gid == fileStats.st_gid || strcmp(groupList[i]->name, "root") == 0)
        {
            p = &realPath[1];
            while(p != NULL)
            {
                chsave = *p;
                *p = '\0';
                lstat(realPath, &parentStats);
                *p = chsave;
                p = strchr(p+1, '/');
                if(!(S_IXGRP & parentStats.st_mode))
                {
                    canReadDir = 1;
                    break;
                }
            }
            canGroup(groupList[i], action, fileStats, parentStats);
            canReadDir = 0;
        }
        else
        {
            p = &realPath[1];
            while(p != NULL)
            {
                chsave = *p;
                *p = '\0';
                lstat(realPath, &parentStats);
                *p = chsave;
                p = strchr(p+1, '/');
                if(!(S_IXOTH & parentStats.st_mode))
                {
                    canReadDir = 1;
                    break;
                }
            }
            canOther(groupList[i], action, fileStats, parentStats);
            canReadDir = 0;
        }
    }
    free(realPath);
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        fprintf(stderr, "usage: ./whocan {command} {file}\n{command}s are:\n\tcd\n\tdelete\n\texecute\n\tls\n\tread\n\tsearch\n\twrite\n");
        exit(EXIT_FAILURE);
    }
    struct user *root = (user*) calloc(1, sizeof(user));
    initUserList(root);
    doAction(argv[1], argv[2]);
    printUserList();
    deconstruct(root);
    free(groupList);
    exit(EXIT_SUCCESS);
}
