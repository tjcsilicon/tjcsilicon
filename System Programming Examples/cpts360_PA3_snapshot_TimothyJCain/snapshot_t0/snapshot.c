#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <libgen.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "snapshot.h"


#define MAX_CORE_SIZE 262144000;
int file_copy(char *source, char *newFile)
{
    int bufferSize = 1024;
    char *buffer = (char*) malloc(bufferSize * sizeof(char));
    int input = open(source, O_RDONLY);
    int output = open(newFile, O_WRONLY | O_CREAT, 0777);
    if(!input)
    {
	return 0;
    }
    if(!output)
    {
	return 0;
    }
    int bytes_read = read(input, buffer, bufferSize);
    while(bytes_read != 0)
    {
        write(output, buffer, bytes_read);
        bytes_read = read(input, buffer, bufferSize);
    }
    free(buffer);
    close(input);
    close(output); 
    return 1;
}

int snapshot(char *ssname, char *progpath, char *readme)
{
    struct stat statbuf;
    if(stat(ssname, &statbuf) == 0)
    {
        fprintf(stderr, "\ndirectory %s already exists \n",ssname);
        return -1;
    }
    mkdir(ssname,S_IRWXU);
    chmod(ssname, 00777);
    chdir(ssname);
    struct rlimit newLim;
    newLim.rlim_cur = MAX_CORE_SIZE;
    newLim.rlim_max = MAX_CORE_SIZE;
    if(setrlimit(RLIMIT_CORE, &newLim) == -1)
    {
        fprintf(stderr, "\ncould not set ulimit -c unlimited\n");
        return -1;
    }
    char *baseName = basename(progpath);
    printf("%s",baseName);
    int pid = getpid();
    char exeName[50];
    sprintf(exeName, "/proc/%d/exe",pid);
    file_copy(exeName,baseName);
    FILE *README = fopen("README", "w+");
    if(README == NULL)
    {
        fprintf(stderr, "\n could not create README");
        return -1;
    }
    fputs(readme,README);
    fclose(README);
    if(fork() == 0)
    {
        abort();
    }
    else
    {
	    int status;
	    wait(&status);
    }
    chdir("..");
    char tar[256];
    sprintf(tar, "tar -czvf %s.tgz %s", ssname,ssname);
    system(tar);
    chdir(ssname);
    remove("README");
    remove("core");
    remove(baseName);
    chdir("..");
    remove(ssname);
    return 0;
}
