typedef struct File{
    char *name;
    struct File *next;
} File;

typedef struct file_group
{
    int size;
    int groupSize;
    struct File *file;
    struct file_group *next;

}file_group;

void insertdir(char* name, struct file_group *head);
void insertfile(char* name, struct file_group *head);
void insert(char *name, struct file_group *head);
void printDups(struct file_group *head);
