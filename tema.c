#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
typedef struct folder
{
    Directory data;
    struct folder *next;
    struct folder *prev;
}Folder;
typedef struct fisier
{
    File data;
    struct fisier *next;
    struct fisier *prev;
}Fisier;
Folder * new_folder(char *name)
{
    Folder *new_folder = malloc(sizeof(Folder));
    new_folder->next = NULL;
    new_folder->prev = NULL;
    new_folder->data.fisiernext = NULL;
    new_folder->data.name = malloc((strlen(name)+1)*sizeof(char));
    strcpy(new_folder->data.name,name);
    new_folder->data.parentDir = NULL;
    new_folder->data.subDirector = NULL;
}
void create_fs(Folder **root)
{
    Folder *fol = new_folder("/");
    *root = fol;
}
void delete_fs(Folder **root)
{
    if((*root) != NULL)
    {
        free((*root)->data.name);
        free((*root));
        *root = NULL;
    }
    else
    {
        printf("No existing file system\n");   
    }
}
void touch(Folder *you, char input)
{
    char *p,*nume,*text;
}
int main()
{
    Folder *root,*you;
    char input[100],copy[100];
    char *test;
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    while(strcmp(input,"stop") != 0)
    {
        strcpy(copy,input);
        test = strtok(copy," ");
        //printf("%s\n",test);
        if(strcmp(input,"create fs") == 0)
        {
            create_fs(&root);
            you = root;
            printf("%s\n",you->data.name);
        }
        else if(strcmp(input,"delete fs") == 0)
        {
            delete_fs(&root);
        }
        else
        {
            printf("error! command %s not found\n",input);
        }
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0';
    }
    return 0;
}