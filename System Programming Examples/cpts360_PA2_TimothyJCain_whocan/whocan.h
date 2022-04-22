#include <stdio.h>
#include <stdlib.h>
#include "user.h"

int numGroups = 0;
static user **groupList;
int canEveryone = 1;

int compare(const void *user1, const void *user2)
{
    user *const *x = user1;
    user *const *y = user2;
    return strcmp((*x)->name, (*y)->name);
}

void printUserList(void)
{
    if(canEveryone == 1)
    {
        printf("(everyone)\n");
        return;
    }
    for(int i = 0; i<numGroups; i++)
    {
        if(strcmp(groupList[i]->name, "\0") != 0)
        {
            printf("%s\n", groupList[i]->name);
        }
    }
    return;
}


void initGroupList(user *root)
{
    user *temp = root;
    groupList = (user**)malloc(numGroups * sizeof(user*));
    int i = 0;
    while(temp != NULL)
    {
        groupList[i] = temp;
        temp = temp->next;
        i++;
    }
    qsort(groupList, numGroups, sizeof(groupList[0]), compare);
}

void initUserList(user* root)
{
    struct passwd *tempUser;
    int nGroups = mxnGids;
    gid_t *groups = (gid_t *) malloc(mxnGids * sizeof(gid_t));

    tempUser = getpwent();
    root->name = strdup(tempUser->pw_name);
    root->uid = tempUser->pw_uid;
    root->gid = tempUser->pw_gid;
    nGroups = mxnGids;
    nGroups = getgrouplist(root->name, root->gid, groups, &nGroups);
    user *temp = root;
    numGroups++;
    while ((tempUser = getpwent()) != NULL) {
        user *newUser = (user*) calloc(1, sizeof(user));
        temp->next = newUser;
        newUser->name = strdup(tempUser->pw_name);
        newUser->uid = tempUser->pw_uid;
        newUser->gid = tempUser->pw_gid;
        getgrouplist(newUser->name, newUser->gid, groups, &nGroups);
        nGroups = mxnGids; 
        nGroups = getgrouplist(tempUser->pw_name, tempUser->pw_gid, groups, &nGroups);
        assert(nGroups > 0);
        for(int i = 0; i < nGroups; i++)
        {
            newUser->suppGroups[i] = groups[i];
        }
        temp = newUser;
        numGroups++;
    }
    free(groups);
    initGroupList(root);
}

