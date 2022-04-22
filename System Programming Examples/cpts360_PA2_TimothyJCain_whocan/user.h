#include <grp.h>
#include <pwd.h>
#include <unistd.h>

#define mxnGids 16
#define PATH_MAX 4096

typedef struct user
{
    char *name;
    int uid;
    int gid;
    gid_t suppGroups[mxnGids];
    struct user *next;
} user;

void deconstruct(user *root)
{
    struct user *temp = root->next;

    while(temp)
    {
        root->next = root->next->next;
        free(temp->name);
        free(temp);
        temp = root->next;
    }
    free(root->name);
    free(root);
}