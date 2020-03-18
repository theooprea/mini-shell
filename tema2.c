#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file2.h"
Folder * new_folder(char *name)
{
    Folder *new_folder = malloc(sizeof(Folder));
    new_folder->next = NULL;
    new_folder->prev = NULL;
    new_folder->current = malloc(sizeof(Directory));
    new_folder->current->name = malloc((strlen(name)+1) * sizeof(char));
    strcpy(new_folder->current->name,name);
    new_folder->current->fisiernext = NULL;
    new_folder->current->parentDir = NULL;
    new_folder->current->subDirector = NULL;
    return new_folder;
}
Fisier * new_file(char *name)
{
    Fisier *new_file = malloc(sizeof(Fisier));
    new_file->next = NULL;
    new_file->prev = NULL;
    new_file->data = malloc(sizeof(File));
    new_file->data->data = "\0";
    new_file->data->name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(new_file->data->name,name);
    new_file->data->size = strlen(name);
    new_file->data->dir = NULL;
    return new_file;
}
void create_fs(Folder **root)
{
    Folder *fol = new_folder("/");
    *root = fol;
}
void delete_fs(Folder **root)
{
    free((*root)->current->name);
    free((*root)->current);
    free((*root));
    *root = NULL;
}
void print_filelist(Fisier *head)
{
    Fisier *i = head;
    while(i != NULL)
    {
        printf("%s ",i->data->name);
        i = i->next;
    }
}
void print_folderlist(Folder *head)
{
    Folder *i = head;
    while(i != NULL)
    {
        printf("%s ",i->current->name);
        i = i->next;
    }
}
void ls(Folder *you)
{
    print_filelist(you->current->fisiernext);
    print_folderlist(you->current->subDirector);
    printf("\n");
}
void touch(Fisier **head, char *argument, Folder *you)
{
    Fisier *fisiernou = new_file(argument);
    fisiernou->data->dir = you->current;
    if((*head) == NULL || strcmp(fisiernou->data->name,(*head)->data->name) < 0)
    {
        fisiernou->next = *head;
        *head = fisiernou;
    }
    else
    {
        Fisier *i = *head;
        while(i->next && strcmp(fisiernou->data->name,i->next->data->name) > 0)
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
    foldernou->current->parentDir = you->current;
    if((*head) == NULL || strcmp(foldernou->current->name,(*head)->current->name) < 0)
    {
        foldernou->next = *head;
        *head = foldernou;
    }
    else
    {
        Folder *i = *head;
        while(i->next && strcmp(foldernou->current->name,i->next->current->name) > 0)
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
void cd(Folder **you, char *argument, Folder *root)
{
    if(strcmp(argument,"..") == 0)
    {
        if((*you)->current->parentDir == NULL)
        {
            return;
        }
        else
        {
            Directory *aux = (*you)->current->parentDir;
            if(strcmp(aux->name,"/") == 0)
            {
                (*you) = root;
            }
            else
            {
                Directory *aux2 = aux->parentDir;
                Folder *head = aux2->subDirector;
                while(strcmp(head->current->name,aux->name) != 0)
                {
                    head = head->next;
                }
                (*you) = head;
            }
        }
    }
    else
    {
        Folder *p = (*you)->current->subDirector;
        while(p != NULL && strcmp(p->current->name,argument) != 0)
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
void pwd(Directory *parent)
{
    if(parent->parentDir != NULL)
    {
        pwd(parent->parentDir);
        printf("/%s",parent->name);
    }
}
void rm(Fisier **head, char *argument)
{
    Fisier *i = *head;
    while(i != NULL && strcmp(i->data->name,argument) != 0)
    {
        i = i->next;
    }
    if(i == NULL)
    {
        printf("Cannot remove %s: No such file!",argument);
    }
    else
    {
        //free names
        if(i->next == NULL && i->prev == NULL)
        {
            *head = NULL;
            free(i->data->name);
            free(i->data);
            free(i);
        }
        else if(i->next == NULL)
        {
            i->prev->next = NULL;
            free(i->data->name);
            free(i->data);
            free(i);
        }
        else if(i->prev == NULL)
        {
            *head = i->next;
            i->next->prev = NULL;
            free(i->data->name);
            free(i->data);
            free(i);
        }
        else
        {
            Fisier *q = i->prev;
            q->next = i->next;
            i->next->prev = q;
            free(i->data->name);
            free(i->data);
            free(i);
        }
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
            printf("%s\n",you->current->name);
        }
        else if(strcmp(input,"delete fs") == 0)
        {
            delete_fs(&root);
        }
        else if(strcmp(comanda,"touch") == 0)
        {
            touch(&(you->current->fisiernext),argument,you);
        }
        else if(strcmp(comanda,"mkdir") == 0)
        {
            mkdir(&(you->current->subDirector),argument,you);
        }
        else if(strcmp(comanda,"cd") == 0)
        {
            cd(&you,argument,root);
        }
        else if(strcmp(input,"ls") == 0)
        {
            ls(you);
        }
        else if(strcmp(input,"pwd") == 0)
        {
            if(strcmp(you->current->name,"/") != 0)
            {
                pwd(you->current->parentDir);
                printf("/%s\n",you->current->name);
            }
            else
            {
                printf("/");
            }
        }
        else if(strcmp(comanda,"rm") == 0)
        {
            rm(&(you->current->fisiernext),argument);
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