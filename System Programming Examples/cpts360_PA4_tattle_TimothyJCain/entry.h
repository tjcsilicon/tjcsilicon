#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <utmp.h>

typedef struct utmpEntry
{
    struct utmp *entry;
    struct utmpEntry *next;
    int loggedOff;
} utmpEntry;

void deconstruct(utmpEntry *root)
{
    struct utmpEntry *temp = root->next;

    while(temp)
    {
        root->next = root->next->next;
        free(temp->entry);
        free(temp);
        temp = root->next;
    }
    free(root->entry);
    free(root);
}