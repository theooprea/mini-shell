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
void touch(Fisier **head, char *argument, Folder *you)
{
    Fisier *fisiernou = new_file(argument);
    fisiernou->data.dir = you;
    if((*head) == NULL || strcmp(fisiernou->data.name,(*head)->data.name) < 0)
    {
        fisiernou->next = *head;
        *head = fisiernou;
    }
    else
    {
        Fisier *i = *head;
        while(i->next && strcmp(fisiernou->data.name,i->next->data.name) > 0)
        {
            i = i->next;
        }
        if(i->next != NULL)
        {
            fisiernou->next = i->next;
            fisiernou->prev = i;
            i->next->prev = fisiernou;
            i->next = fisiernou;
        }
        else
        {
            i->next = fisiernou;
            fisiernou->prev = i;
        }
    }
}
void mkdir(Folder **head, char *argument, Folder *you)
{
    Folder *foldernou = new_folder(argument);
    foldernou->data.parentDir = you;
    if((*head) == NULL || strcmp(foldernou->data.name,(*head)->data.name) < 0)
    {
        foldernou->next = *head;
        *head = foldernou;
    }
    else
    {
        Folder *i = *head;
        while(i->next && strcmp(foldernou->data.name,i->next->data.name) > 0)
        {
            i = i->next;
        }
        if(i->next != NULL)
        {
            foldernou->next = i->next;
            foldernou->prev = i;
            i->next->prev = foldernou;
            i->next = foldernou;
        }
        else
        {
            i->next = foldernou;
            foldernou->prev = i;
        }
    }
}
void cd(Folder **you, char *argument)
{
    if(strcmp(argument,"..") == 0)
    {
        if((*you)->data.parentDir == NULL)
        {
            return;
        }
        else
        {
            *you = (*you)->data.parentDir;   
        }
    }
    else
    {
        Folder *p = (*you)->data.subDirector;
        while(p != NULL && strcmp(p->data.name,argument) != 0)
        {
            p = p->next;
        }
        if(p == NULL)
        {
            printf("Cannot move to %s: No such directory!",argument);
        }
        else
        {
            (*you) = p;   
        }
    }
}
void pwd(Folder *you)
{
    if(you->data.parentDir != NULL)
    {
        pwd(you->data.parentDir);
        printf("/%s",you->data.name);
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
            touch(&(you->data.fisiernext),argument,you);
        }
        else if(strcmp(comanda,"mkdir") == 0)
        {
            mkdir(&(you->data.subDirector),argument,you);
        }
        else if(strcmp(comanda,"cd") == 0)
        {
            cd(&you,argument);
        }
        else if(strcmp(input,"ls") == 0)
        {
            ls(you);
        }
        else if(strcmp(input,"pwd") == 0)
        {
            pwd(you);
            printf("\n");
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