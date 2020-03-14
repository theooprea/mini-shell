#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
Folder * new_folder(char *name)
{
    Folder *new_folder = malloc(sizeof(Folder));
    new_folder->next = NULL;
    new_folder->prev = NULL;
    new_folder->data.fisiernext = NULL;
    new_folder->data.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_folder->data.name,name);
    new_folder->data.parentDir = NULL;
    new_folder->data.subDirector = NULL;
    return new_folder;
}
Fisier * new_file(char *name)
{
    Fisier *new_file = malloc(sizeof(Fisier));
    new_file->next = NULL;
    new_file->prev = NULL;
    new_file->data.data = "\0";
    new_file->data.name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_file->data.name,name);
    new_file->data.size = strlen(name);
    new_file->data.dir = NULL;
    return new_file;
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
void print_filelist(Fisier *head)
{
    Fisier *i = head;
    while(i != NULL)
    {
        printf("%s ",i->data.name);
        i = i->next;
    }
}
void print_folderlist(Folder *head)
{
    Folder *i = head;
    while(i != NULL)
    {
        printf("%s ",i->data.name);
        i = i->next;
    }
}
void ls(Folder *you)
{
    print_filelist(you->data.fisiernext);
    print_folderlist(you->data.subDirector);
    printf("\n");
}
void touch(Fisier **head, char *argument)
{
    Fisier *fisiernou = new_file(argument);
    if((*head) == NULL)
    {
        fisiernou->next = *head;
        *head = fisiernou;
    }
    else
    {
        Fisier *i = *head;
        while(i->next)
        {
            i = i->next;
        }
        i->next = fisiernou;
        fisiernou->prev = i;
    }
}
void mkdir(Folder **head, char *argument)
{
    Folder *foldernou = new_folder(argument);
    if((*head) == NULL)
    {
        foldernou->next = *head;
        *head = foldernou;
    }
    else
    {
        Folder *i = *head;
        while(i->next)
        {
            i = i->next;
        }
        i->next = foldernou;
        foldernou->prev = i;
    }
}
int main()
{
    Folder *root,*you;
    char input[100],copy[100];
    char *comanda,*argument;
    fgets(input, 100, stdin);
    input[strlen(input) - 1] = '\0';
    while(strcmp(input,"stop") != 0)
    {
        strcpy(copy,input);
        comanda = strtok(copy," ");
        //printf("%s\n",comanda);
        argument = strtok(NULL," ");
        //printf("%s\n",argument);
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
        else if(strcmp(comanda,"touch") == 0)
        {
            touch(&(you->data.fisiernext),argument);
        }
        else if(strcmp(comanda,"mkdir") == 0)
        {
            mkdir(&(you->data.subDirector),argument);
        }
        else if(strcmp(input,"ls") == 0)
        {
            ls(you);
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