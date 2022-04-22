#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <regex.h>
#include "entry.h"
#include "filter.h"

char *progname = "*** error: 'progname' not set ***"; //taken from experiment_tmm_pt2.c by Robert Lewis (WSU TC)

void printList(utmpEntry *root) //prints list
{
    time_t t;
    struct tm *timeinfo;
    struct tm *logOut;
    char *buffer = (char *)malloc(1024 * sizeof(char));
    for(int i = 0; i<numGroups; i++)
    {
        if(groupList[i]->entry->ut_type == USER_PROCESS)
        {
            if(strcmp(&groupList[i]->entry->ut_line[0],"") == 0)
            {
                return;
            }
            printf("\n");
            printf("login  : %s\n",groupList[i]->entry->ut_user);
            printf("tty    : %s\n",groupList[i]->entry->ut_line);
            t = groupList[i]->entry->ut_tv.tv_sec;
            timeinfo = localtime(&t);
            strftime (buffer, 1024, "%D %H:%M", timeinfo);
            printf("log on : %s\n",buffer);
            logOut = getLogOut(logOut,&groupList[i]); //search through subsequent entries for DEAD_PROCESS with same tty as user (log out entry)
            if(logOut)
            {
                strftime (buffer, 1024, "%D %H:%M", logOut);
                printf("log off: %s\n",buffer);
            }
            else
            {
                printf("log off: (still logged in)\n");
            }
            printf("host   : %s\n",groupList[i]->entry->ut_host);
        } 
    }
    free(buffer);
}

void insert(struct utmp *entry, utmpEntry *root) //inserts new entry to list
{
    utmpEntry *temp = root;

    if(!root->entry)
    {
        root->entry = entry;
        root->next = NULL;
        root->loggedOff = 0;
    }
    else
    {
        utmpEntry *newEntry = malloc(sizeof(utmpEntry));
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newEntry;
        newEntry->entry = entry;
        newEntry->next = NULL;
        newEntry->loggedOff = 0;
    }
    numGroups++;
}

void readWtmp(char *utmpName, utmpEntry *root) //reads utmp/wtmp entry
{
    FILE *file;
    if(strcmp(utmpName, "") == 0)
    {
        file = fopen("/var/log/wtmp", "rb");
    }
    else
    {
        file = fopen(utmpName,"rb");
    }
    if(!file)
    {
        fprintf(stderr, "could not open wtmp/utmp file\n");
        return;
    }
    int bytesRead = 0;
    do
    {
        struct utmp *utmpEnt = (struct utmp *) calloc(1, sizeof(struct utmp));
        bytesRead = fread(utmpEnt,sizeof(struct utmp), 1, file);
        insert(utmpEnt,root);
    } while(bytesRead != 0);
    fclose(file);
}

int reCheck(char * exp, char type) //checks user input flags using regular expressions
{
    regex_t regex;
    int value = 0;
    switch(type)
    {
        case 't':
            regcomp(&regex, "^[0-2][0-9]:[0-5][0-9]$", 0);
            value = regexec(&regex, exp, 0, NULL, 0);
            regfree(&regex);
            return value;
            break;
        case 'd':
            regcomp(&regex, "^[0-9][0-9]/[0-9][0-9]/[0-9][0-9]$", 0);
            value = regexec(&regex, exp, 0, NULL, 0);
            regfree(&regex);
            return value;
            break;
    }
    return 0;
}

static void usage(void) //taken and modified from experiment_tmm_pt2.c written by Robert Lewis (WSU TC)
{
    printf("usage: %s [{args}]\n", progname);
    printf("%s",
            "'tattles' on the active users that logged on to a system\n"
            "prints the login, tty, log on and log off/system reboot time, and the host IP address\n"
            " {args} are:\n\n");
    printf("%s\n",
            "  -h                             this help message\n");
    printf("%s\n",
            "  -f {fileName}                  opens specified file (default /var/log/wtmp)\n");
    printf("%s\n",
            "  -u {userName1,userName2,...}   only prints log data from specified username (additional usernames seperated with a ','\n");
    printf("%s\n",
            "  -t {HH:MM}                     only prints log data at that time\n");
    printf("%s\n",
            "  -d {MM/DD/YY}                  only prints log data at that date\n");
    return;
}


int main(int argc, char *argv[])
{
    progname = argv[0];
    if(argc > 9)
    {
        fprintf(stderr, "too many arguments\n\n");
        usage();
        exit(EXIT_FAILURE);
    }
    utmpEntry *root = (utmpEntry *) malloc(sizeof(utmpEntry));
    root->entry = NULL;
    root->next = NULL;
    root->loggedOff = 0;
    int ch, ok;
    static char *f = ""; 
    static char *d = "";
    static char *t = "";
    static char *u = "";
    ok = 1;
    progname = argv[0];
    while ((ch = getopt(argc, argv, "h::f:u:t:d:")) != -1) {//taken from experiment_tmm_pt2.c by Robert Lewis (WSU TC)
        switch (ch) {

        case 'h':
            usage();
            exit(EXIT_SUCCESS);

        case 'f':
            f = optarg;
            break;
        
        case 'u':
            u = optarg;
            break;

        case 't':
            t = optarg;
            break;
        
        case 'd':
            d = optarg;
            dFlag = 1;
            break;

        default:
            ok = 0;
            break;
        }
    }
    if (!ok) {//taken from experiment_tmm_pt2.c by Robert Lewis (WSU TC)
        usage();
        exit(EXIT_FAILURE);
    }
    readWtmp(f, root);
    if(strcmp(d,"") != 0)
    {
        if(reCheck(d,'d') == REG_NOMATCH)
        {
            usage();
            exit(EXIT_SUCCESS);
        }
        filterD(&root,d);
    }
    if(strcmp(u,"") != 0)
    {
        filterU(&root,u);
    }
    if(strcmp(t,"") != 0)
    {
        if(reCheck(t,'t') == REG_NOMATCH)
        {
            usage();
            exit(EXIT_SUCCESS);
        }
        filterT(&root,t);
    }
    initGroupList(root);
    printList(root);
    free(groupList);
    deconstruct(root);
    return 0;
}