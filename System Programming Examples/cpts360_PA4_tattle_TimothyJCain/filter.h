#include <stdio.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <utmp.h>
#include <time.h>
#include <string.h>
#include <regex.h>

int dFlag = 0;
int numGroups = 0;
static utmpEntry **groupList;

int compare(const void *user1, const void *user2)
{
    utmpEntry *const *x = user1;
    utmpEntry *const *y = user2;
    time_t u1; 
    time_t u2;
    u1 = (*x)->entry->ut_tv.tv_sec;
    u2 = (*y)->entry->ut_tv.tv_sec;
    int value = difftime(u1,u2);
    if(value > 0)
    {
        return -1;
    }
    else if(value == 0)
    {
        return 0;
    }
    else if(value < 1)
    {
        return 1;
    }
    return 0;
}

void initGroupList(utmpEntry *root)
{
    utmpEntry *temp = root;
    groupList = (utmpEntry**) calloc(1, numGroups * sizeof(utmpEntry *));
    int i = 0;
    while(temp != NULL)
    {
        groupList[i] = temp;
        temp = temp->next;
        i++;
    }
    qsort(groupList, numGroups, sizeof(groupList[0]), compare);
}

struct tm* getLogOut(struct tm *timeinfo, utmpEntry **temp)
{
    time_t t;
    utmpEntry *logOff;
    logOff = (*temp)->next;
    while(logOff)
    {
        if(strcmp("",&(logOff->entry->ut_user[0])) == 0 && strcmp(&((*temp)->entry->ut_line[0]),&(logOff->entry->ut_line[0])) == 0 && (logOff->entry->ut_type == DEAD_PROCESS || logOff->entry->ut_type == BOOT_TIME))
        {
            t = logOff->entry->ut_tv.tv_sec;
            timeinfo = localtime(&t);
            break;
        }
        logOff = logOff->next;
        timeinfo = NULL;
    }
    return timeinfo;
}

void filterD(utmpEntry **root, char *d)
{
    utmpEntry *temp = *root;
    utmpEntry *tempNext = (temp)->next;
    utmpEntry *delete;
    utmpEntry *prev = temp;
    char *buffer = (char *)malloc(1024 * sizeof(char));
    time_t t;
    struct tm *timeinfo;
    char chsave[2];
    time_t zeroTime = time(NULL);  //get day at beginning of day, and end of day.
    time_t finalTime, in, out;
    struct tm zeroTM = *localtime(&zeroTime);
    struct tm tfTM = *localtime(&zeroTime);
    struct tm logOut = *localtime(&zeroTime);
    zeroTM.tm_hour = 0;
    zeroTM.tm_min = 0;
    tfTM.tm_hour = 24;
    tfTM.tm_min = 59;
    chsave[0] = d[0];
    chsave[1] = d[1];
    zeroTM.tm_mon = atoi(&chsave[0]) -1;
    tfTM.tm_mon = atoi(&chsave[0]) -1;
    chsave[0] = d[3];
    chsave[1] = d[4];
    zeroTM.tm_mday = atoi(&chsave[0]);
    tfTM.tm_mday = atoi(&chsave[0]);
    chsave[0] = d[6];
    chsave[1] = d[7];
    zeroTM.tm_year = atoi(&chsave[0]);
    tfTM.tm_year = atoi(&chsave[0]);
    zeroTime = mktime(&zeroTM);
    finalTime = mktime(&tfTM);

    while(tempNext)
    {
        if(temp->entry->ut_type == DEAD_PROCESS)
        {
            prev = temp;
            temp = tempNext;
            tempNext = (temp)->next;
        }
        else
        {
            t = (temp)->entry->ut_tv.tv_sec;
            timeinfo = localtime(&t);       //populate buffer with time value
            strftime(buffer, 1024, "%D", timeinfo);
            getLogOut(&logOut,&temp);
            in = mktime(timeinfo);
            out = mktime(&logOut);
            if(difftime(out,zeroTime) < 0 || difftime(finalTime,in) < 0)
            {
                if(strncmp(d,buffer,strlen(d)) != 0) //compare to passed time d
                {
                    delete = temp; //delete = old value
                    temp = tempNext;//temp points to next value
                    if(delete == *root)
                    {
                        *root = temp; //root = temp (next value)
                    }
                    else
                    {
                        prev->next = temp;
                    }
                    tempNext = (temp)->next;
                    free(delete->entry);
                    free(delete); //free old value
                    numGroups--;
                }
                else
                {
                    prev = temp;
                    temp = tempNext;
                    tempNext = (temp)->next;
                }
            }
            else
            {
                prev = temp;
                temp = tempNext;
                tempNext = (temp)->next;
            }
        }
    } 
    free(buffer);
}

void filterU(utmpEntry **root, char *u)
{
    utmpEntry *temp = *root;
    utmpEntry *tempNext = (temp)->next;
    utmpEntry *delete;
    utmpEntry *prev = temp;
    char *nameList[1024];
    char *name;
    int i = 0;
    name = strtok(u,",");
    int equal = 0;
    while(name != NULL)
    {
        nameList[i] = name;
        name = strtok(NULL,",");
        i++;
    }
    for(int j = 0; j < i; j++)
    {
        if(getpwnam(nameList[j]) == NULL)
        {
            fprintf(stderr, "Username %s does not exist\n",nameList[j]);
            exit(EXIT_FAILURE);
        }
    }
    while(tempNext)
    {
        if(temp->entry->ut_type == DEAD_PROCESS)
        {
            prev = temp;
            temp = tempNext;
            tempNext = (temp)->next;
        }
        else
        {
            equal = 0;
            for(int j = 0; j < i;j++)
            {
                if(strncmp(&(temp->entry->ut_user[0]),nameList[j],strlen(nameList[j])) == 0) //compare to names
                {
                    prev = temp;
                    temp = tempNext;
                    tempNext = (temp)->next;
                    equal = 1;
                }
            }
            
            if(equal == 0)
            {
                delete = temp; //delete = old value
                temp = tempNext;//temp points to next value
                if(delete == *root)
                {
                    *root = temp; //root = temp (next value)
                }
                else
                {
                    prev->next = temp;
                }
                tempNext = (temp)->next;
                free(delete->entry);
                free(delete); //free old value
                numGroups--;
            }
            
        }
    } 
}

void filterT(utmpEntry **root, char *t)
{
    utmpEntry *temp = *root;
    utmpEntry *tempNext = (temp)->next;
    utmpEntry *delete;
    utmpEntry *prev = temp;
    char *dateBuf = (char *)malloc(1024 * sizeof(char));
    char *timeBuf = (char *) malloc(1024 * sizeof(char));
    char *curDateBuf = (char *) malloc(1024 * sizeof(char));
    time_t my_time;
    struct tm *timeinfo;
    if(dFlag == 0)
    {
        time_t curTime;
        struct tm *curDate;
        time(&curTime);
        curDate = localtime(&curTime);
        strftime(curDateBuf, 1024, "%D", curDate);
    }
    while(tempNext)
    {
        if(temp->entry->ut_type == DEAD_PROCESS)
        {
            prev = temp;
            temp = tempNext;
            tempNext = (temp)->next;
        }
        else
        {
            my_time = (temp)->entry->ut_tv.tv_sec;
            timeinfo = localtime(&my_time);       //populate buffer with my_time value
            strftime(timeBuf, 1024, "%R", timeinfo);
            if(dFlag == 0)
            {
                strftime(dateBuf,1024,"%D",timeinfo);
                if((strncmp(curDateBuf,dateBuf,strlen(curDateBuf))) == 0 && (strncmp(t,timeBuf,strlen(t))) == 0) //compare to passed my_time t
                {
                    prev = temp;
                    temp = tempNext;
                    tempNext = (temp)->next;
                }
                else
                {
                    delete = temp; //delete = old value
                    temp = tempNext;//temp points to next value
                    if(delete == *root)
                    {
                        *root = temp; //root = temp (next value)
                    }
                    else
                    {
                        prev->next = temp;
                    }
                    tempNext = (temp)->next;
                    free(delete->entry);
                    free(delete); //free old value
                    numGroups--;
                }
            }
            else
            {
                if(strncmp(t,timeBuf,strlen(t)) == 0) //compare to passed my_time t
                {
                    prev = temp;
                    temp = tempNext;
                    tempNext = (temp)->next;
                }
                else
                {
                    delete = temp; //delete = old value
                    temp = tempNext;//temp points to next value
                    if(delete == *root)
                    {
                        *root = temp; //root = temp (next value)
                    }
                    else
                    {
                        prev->next = temp;
                    }
                    tempNext = (temp)->next;
                    free(delete->entry);
                    free(delete); //free old value
                    numGroups--;
                }
            }
        }
    } 
    free(dateBuf);
    free(timeBuf);
    free(curDateBuf);
}