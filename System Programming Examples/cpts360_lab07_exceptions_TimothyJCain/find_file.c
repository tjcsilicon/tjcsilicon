#define _GNU_SOURCE // to get asprintf() prototype
#include <stdio.h>  // this needs to be the first #include in that case

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <getopt.h>

#include "except.h"

/*
 * Here's how you declare an exception with the "except" package:
 */
Except_T StatFailed = { "Failed to open file." };
/*
 * ASSIGNMENT
 *
 * Add additional declarations for exceptions TargetFound and
 * MiscellaneousError.
 */
Except_T TargetFound = { "Target file found" };
Except_T MiscellaneousError = { "Something went wrong" };

char *progname = "*** progname not set ***"; /* should be argv[0] */

int verbose = 0; /* set on command line */

static void explore(char *path, char *target);

static void traverseDirectory(char path[], char *target)
{
    DIR *fd;
    fd = opendir(path);
    if(!fd)
        RAISE(MiscellaneousError);
    struct dirent *ptr = readdir(fd);
    ptr = readdir(fd);
    ptr = readdir(fd);
    while(ptr)
    {
        char *subpath;// = (char*) malloc(sizeof(char*));
        asprintf(&subpath, "%s%c%s", path, '/', ptr->d_name);
        TRY
            explore(subpath, target);
        EXCEPT(TargetFound);
            free(subpath);
            closedir(fd);
            RERAISE;
        ELSE
            fprintf(stderr, "explore() has failed\n");
        END_TRY;
        free(subpath);
        ptr = readdir(fd);
    }
    closedir(fd);
}


static void explore(char *path, char *target)
/* look at, in, and below `path` for a file named `target` */
{
    struct stat pathStat;
    if(stat(path, &pathStat) == -1)
        RAISE(StatFailed);
    char *temp = strrchr(path, '/');
    if(!temp)
    {
        temp = path;
    }
    if(strcmp(temp+1, target) == 0)
    {
        if(verbose == 1)
        {
            printf("%s\n", path);
        }
        RAISE(TargetFound);
    }
    if(S_ISDIR(pathStat.st_mode))
    {
        traverseDirectory(path, target);
    }
}

void findFile(char *top, char *target)
{
    TRY
        explore(top, target);
    EXCEPT(StatFailed)
        ;
    EXCEPT(TargetFound)
        exit(EXIT_SUCCESS);
    END_TRY;
}


void usage(void)
{
    printf("usage: %s [-h] [-v] target [directory]*\n", progname);
}


int main(int argc, char *argv[])
{
    int i, ch;
    char *target;
    extern int optind;

    progname = argv[0];
    while ((ch = getopt(argc, argv, "hv")) != -1) {
        switch (ch) {

        case 'v':
            verbose = 1;
            break;

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case '?':
            usage();
            exit(EXIT_FAILURE);
        }
    }
    if (optind >= argc) {
        usage();
        exit(EXIT_FAILURE);
    }
    target = argv[optind++];
    if (optind == argc) {
        /* directory name(s) not provided */
        findFile(".", target);
    } else {
        /* directory name(s) provided */
        for (i = optind; i < argc; i++)
            findFile(argv[i], target);
    }
    /*
     * If we find the target, we'll exit immediately (and
     * successfully), so if we get to this point...
     */
    exit(EXIT_FAILURE);
}