#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
//functie de creare un nou nod de tip folder,
//echivalentul nodului de lista de Directory
Folder *new_folder(char *name)
{
	Folder *new_folder = malloc(sizeof(Folder));

	new_folder->next = NULL;
	new_folder->prev = NULL;
	new_folder->current = malloc(sizeof(Directory));
	new_folder->current->name = malloc((strlen(name)+1) * sizeof(char));
	strcpy(new_folder->current->name, name);
	new_folder->current->fisiernext = NULL;
	new_folder->current->parentDir = NULL;
	new_folder->current->subDirector = NULL;
	return new_folder;
}
//functie de creare un nou nod de tip fisier,
//echivalentul nodului de lista de File
Fisier *new_file(char *name, char *content)
{
	Fisier *new_file = malloc(sizeof(Fisier));

	new_file->next = NULL;
	new_file->prev = NULL;
	new_file->data = malloc(sizeof(File));
	new_file->data->data = malloc((strlen(content) + 1) * sizeof(char));
	strcpy(new_file->data->data, content);
	new_file->data->name = malloc((strlen(name) + 1) * sizeof(char));
	strcpy(new_file->data->name, name);
	new_file->data->size = strlen(content);
	new_file->data->dir = NULL;
	return new_file;
}
//functia care creaza rootul
void create_fs(Folder **root)
{
	Folder *fol = new_folder("/");
	*root = fol;
}
//functie care sterge rootul (nu recursiv)
void delete_fs(Folder **root)
{
	free((*root)->current->name);
	free((*root)->current);
	free((*root));
	*root = NULL;
}
//functie de afisare a elementelor dintr-o lista de fisiere
void print_filelist(Fisier *head)
{
	Fisier *i = head;

	while (i != NULL) {
		printf("%s ", i->data->name);
		i = i->next;
	}
}
//functie de afisare a elementelor dintr-o lista de foldere
void print_folderlist(Folder *head)
{
	Folder *i = head;

	while (i != NULL) {
		printf("%s ", i->current->name);
		i = i->next;
	}
}
//functie de afisare a contentului din folderul
//curent, apeleaza cele 2 functii precedente
void ls(Folder *you)
{
	print_filelist(you->current->fisiernext);
	print_folderlist(you->current->subDirector);
	printf("\n");
}
//functie de creare a si adaugare a unui nod de fisiere in lista de fisiere
//functia prelucreaza inputul astfel incat se separa numele fisierului
//si datele in siruri diferite, apoi cauta locul noului nod in lista respectiva
//si il insereaza in locul respectiv
void touch(Fisier **head, char *argument, Folder *you, char *input)
{
	char *content;

	content = strtok(input, " ");
	content = strtok(NULL, " ");
	content = strtok(NULL, " ");
	Fisier *fisiernou = new_file(argument, content);

	fisiernou->data->dir = you->current;
	if ((*head) == NULL) {
		fisiernou->next = *head;
		*head = fisiernou;
	} else if (strcmp(fisiernou->data->name, (*head)->data->name) < 0) {
		fisiernou->next = *head;
		(*head)->prev = fisiernou;
		*head = fisiernou;
	} else {

		Fisier *i = *head;

		while (i->next &&
			strcmp(fisiernou->data->name,
				i->next->data->name) > 0) {
			i = i->next;
		}
		if (i->next != NULL) {
			fisiernou->next = i->next;
			fisiernou->prev = i;
			i->next->prev = fisiernou;
			i->next = fisiernou;
		} else {
			i->next = fisiernou;
			fisiernou->prev = i;
		}
	}
}
//functie de creare a si adaugare a unui nod de folder in lista de foldere
//functia cauta locul noului nod in lista respectiva
//si il insereaza in locul respectiv si creaza legaturile aferente
void mkdir(Folder **head, char *argument, Folder *you)
{
	Folder *foldernou = new_folder(argument);

	foldernou->current->parentDir = you->current;
	if ((*head) == NULL) {
		foldernou->next = *head;
		*head = foldernou;
	} else if (strcmp(foldernou->current->name,
	 (*head)->current->name) < 0) {
		foldernou->next = *head;
		(*head)->prev = foldernou;
		*head = foldernou;
	} else {
		Folder *i = *head;

		while (i->next && strcmp(foldernou->current->name,
		 i->next->current->name) > 0)
			i = i->next;

		if (i->next != NULL) {
			foldernou->next = i->next;
			foldernou->prev = i;
			i->next->prev = foldernou;
			i->next = foldernou;
		} else {
			i->next = foldernou;
			foldernou->prev = i;
		}
	}
}
//functia care schimba pointerul *you din main cu orice sub director al
//rootului, din folderul unde se afla curent trebuie sa merg pana la direcotrul
//bunic al lui si sa cobor un nivel pt a reveni la o structura de tip folder,
//nu directory iar in cazul in care nu exista bunicul, adica parintele este
//rootul fac astfel incat folderul sa devina root
void cd(Folder **you, char *argument, Folder *root)
{
	if (strcmp(argument, "..") == 0) {
		if ((*you)->current->parentDir == NULL)
			return;

		else {
			Directory *aux = (*you)->current->parentDir;

			if (strcmp(aux->name, "/") == 0) {
				(*you) = root;
			} else {
				Directory *aux2 = aux->parentDir;

				Folder *head = aux2->subDirector;

				while (strcmp
					(head->current->name, aux->name) != 0)
					head = head->next;

				(*you) = head;
			}
		}
	} else {
		Folder *p = (*you)->current->subDirector;

		while (p != NULL && strcmp(p->current->name, argument) != 0)
			p = p->next;

		if (p == NULL)
			printf("Cannot move to '%s': No such directory!\n"
				, argument);

		else
			(*you) = p;

	}
}
//functie care printeaza folderele parinte pana cand urmatorul este NULL
//si se apeleaza recursiv pentru a afisa intregul working directory
void pwd(Directory *parent)
{
	if (parent->parentDir != NULL) {
		pwd(parent->parentDir);
		printf("/%s", parent->name);
	}
}
//functie care sterge fisierele dintr-o lista respectiva cu un nume anume
//parcurge lista si cand gaseste numele cautat rupe legaturile in functie de
//cazurile mai jos verificate si elibereaza elementele alocate dinamic din
//nodul respectiv
void rm(Fisier **head, char *argument)
{
	Fisier *i = *head;

	while (i != NULL && strcmp(i->data->name, argument) != 0)
		i = i->next;

	if (i == NULL)
		printf("Cannot remove '%s': No such file!\n", argument);

	else {
		if (i->next == NULL && i->prev == NULL) {
			*head = NULL;
			free(i->data->data);
			free(i->data->name);
			free(i->data);
			free(i);
		} else if (i->next == NULL) {
			i->prev->next = NULL;
			free(i->data->data);
			free(i->data->name);
			free(i->data);
			free(i);
		} else if (i->prev == NULL) {
			*head = i->next;
			i->next->prev = NULL;
			free(i->data->data);
			free(i->data->name);
			free(i->data);
			free(i);
		} else {
			Fisier *q = i->prev;

			q->next = i->next;
			i->next->prev = q;
			free(i->data->data);
			free(i->data->name);
			free(i->data);
			free(i);
		}
	}
}
//functie auxiliara pentru alte functii care calculeaza numarul de nivele
//intre folderul in care ne aflam si un alt folder mai aproape de root cu
//numele rootname
int taburi_necesare(Folder *you, char *rootname)
{
	if (strcmp(you->current->name, rootname) == 0)
		return 0;

	int number_of_tabs = 1;
	Directory *aux = you->current->parentDir;

	while (strcmp(aux->name, rootname) != 0) {
		number_of_tabs++;
		aux = aux->parentDir;
	}
	return number_of_tabs;
}
//functie care printeaza intr-o maniera asemanatoare tree-ului din linux
//calculeaza nr de space-uri necesare si apoi printeaza lista de fisiere
void print_sub_filelist(Folder *you, char *rootname)
{
	int spaces = (taburi_necesare(you, rootname) + 1) * 4;
	Fisier *i = you->current->fisiernext;

	while (i) {
		int spaces_aux = spaces;

		while (spaces_aux > 0) {
			printf(" ");
			spaces_aux--;
		}
		printf("%s\n", i->data->name);
		i = i->next;
	}
}
//functie recursiva care, folosindu-se de functiile de mai sus, afiseaza
//subdirectoarele si fisierele pornind cu folderul curent intr-un mod recursiv
void tree(Folder *root, char *rootname, int level_of_current_root)
{
	int spaces = taburi_necesare(root, rootname) * 4;

	while (spaces > 0) {
		printf(" ");
		spaces--;
	}
	printf("%s\n", root->current->name);
	print_sub_filelist(root, rootname);
	if (root->current->subDirector != NULL)
		tree(root->current->subDirector,
		rootname, level_of_current_root);

	if (root->next != NULL && level_of_current_root !=
		taburi_necesare(root->next, "/"))
		tree(root->next, rootname, level_of_current_root);

}
//functie care elibereaza (sterge) elementele dintr-o lista de fisiere
void free_filelist(Fisier **head)
{
	Fisier *aux;

	while ((*head) != NULL) {
		aux = *head;
		*head = (*head)->next;
		free(aux->data->data);
		free(aux->data->name);
		free(aux->data);
		free(aux);
	}
}
//functie care sterge un singur folder, eliminand toate fisierele din el prima
//data, apoi reface legaturile dintre folderele (nodurile) vecine urmand ca
//apoi sa elibereze elementele alocate dincamic din nodul respectiv
void remove_single_directory(Folder **you)
{
	Folder *i = *you;

	if (i->next == NULL && i->prev == NULL) {
		i->current->parentDir->subDirector = NULL;
		free_filelist(&(i->current->fisiernext));
		free(i->current->name);
		free(i->current);
		free(i);
	} else if (i->next == NULL) {
		i->prev->next = NULL;
		free_filelist(&(i->current->fisiernext));
		free(i->current->name);
		free(i->current);
		free(i);
	} else if (i->prev == NULL) {
		i->current->parentDir->subDirector = i->next;
		i->next->prev = NULL;
		free_filelist(&(i->current->fisiernext));
		free(i->current->name);
		free(i->current);
		free(i);
	} else {
		Folder *q = i->prev;

		q->next = i->next;
		i->next->prev = q;
		free_filelist(&(i->current->fisiernext));
		free(i->current->name);
		free(i->current);
		free(i);
	}
}
//functie recursiva care parcure toate subdirectoarele din folderul curent si
//elimina prima data fisierele, utilizand free, apoi cand intaneste un folder
//fara liste de fisiere si de directoare il sterge folosind functia remove_
//single_directory
void rmdir(Folder *current)
{
	if (current->next != NULL)
		rmdir(current->next);

	if (current->current->subDirector != NULL)
		rmdir(current->current->subDirector);

	remove_single_directory(&current);
}
//functie care primeste folderul curent ca parametru, cauta folderul de sters
//si apeleaza functiile de mai sus pentru a sterge continutul recursiv
void rmdir_aux(Folder **you, char *argument)
{
	Folder *i = (*you)->current->subDirector;

	while (i != NULL && strcmp(i->current->name, argument) != 0)
		i = i->next;

	if (i == NULL)
		printf("Cannot remove '%s': No such directory!\n", argument);

	else {
		if (i->current->subDirector == NULL)
			remove_single_directory(&i);

		else {
			rmdir(i->current->subDirector);
			remove_single_directory(&i);
		}
	}
}
//functie care cauta intr-o lista de fisiere un fisier cu un anumit content
//si in acelasi timp verifica sa fie impinite conditiile de min_size si
//max_size
void seek_in_list(Folder *you, char *subcontent, int min_size, int max_size)
{
	Fisier *it = you->current->fisiernext;

	while (it != NULL) {
		if (it->data->size >= min_size &&
		 it->data->size <= max_size &&
		  strstr(it->data->data, subcontent) != NULL)
			printf("%s ", it->data->name);

		it = it->next;
	}
}
//functia recursiva find care cauta recursiv in listele de fisiere de la
//folderul din care e apelat pana la o adancime de max_depth si apeleaza la
//fiecare folder functia seek_in_list
void find(Folder *you, char *subcontent, int max_depth,
int min_size, int max_size, int depth_of_initial_folder)
{
	int depth_to_starting_level =
	taburi_necesare(you, "/") - depth_of_initial_folder;

	seek_in_list(you, subcontent, min_size, max_size);
	if (you->current->subDirector != NULL &&
	 depth_to_starting_level < max_depth)
		find(you->current->subDirector, subcontent, max_depth,
		 min_size, max_size, depth_of_initial_folder);

	if (you->next != NULL)
		find(you->next, subcontent, max_depth, min_size,
		 max_size, depth_of_initial_folder);

}
//fnctia auxiliara in cadrul careia apelez functia recursiza find, din input
//separ in siruri auxiliare si respectiv inturi prin functia atoi din siruri
//din inputul pe care il citesc de la tastatura
void find_aux(Folder *you, char *input)
{
	char *argument2, *argument3, *argument4, *subcontent;

	argument2 = strtok(input, " ");
	argument2 = strtok(NULL, " ");
	argument3 = strtok(NULL, " ");
	argument4 = strtok(NULL, " ");
	subcontent = strtok(NULL, " ");
	int max_depth = atoi(argument2);

	int min_size = atoi(argument3);

	int max_size = atoi(argument4);

	int depth_of_initial_folder = taburi_necesare(you, "/");

	seek_in_list(you, subcontent, min_size, max_size);
	if (max_depth > 0)
		find(you->current->subDirector, subcontent, max_depth,
		min_size, max_size, depth_of_initial_folder);

	printf("\n");
}
//in main am facut 2 foldere, unul root din care portneste tot si unul you care
//tine minte unde se afla utilizatorul(folderul curent), apoi citesc inputul
//si il inpart in functie de comanda cautata si apelez functiile
//corespunzatoare comenzilor date, cat timp nu se da comanda stop sau delete_fs
int main(void)
{
	Folder *root, *you;

	char input[100], copy[100];

	char *comanda, *argument;

	fgets(input, 100, stdin);
	input[strlen(input) - 1] = '\0';
	while (strcmp(input, "stop") != 0) {
		strcpy(copy, input);
		comanda = strtok(copy, " ");
		argument = strtok(NULL, " ");
		if (strcmp(input, "create fs") == 0) {
			create_fs(&root);
			you = root;
		} else if (strcmp(input, "delete fs") == 0) {
			delete_fs(&root);
			break;
		} else if (strcmp(comanda, "touch") == 0)
			touch(&(you->current->fisiernext),
			 argument, you, input);

		else if (strcmp(comanda, "mkdir") == 0)
			mkdir(&(you->current->subDirector), argument, you);

		else if (strcmp(comanda, "cd") == 0)
			cd(&you, argument, root);

		else if (strcmp(input, "ls") == 0)
			ls(you);

		else if (strcmp(input, "pwd") == 0) {
			if (strcmp(you->current->name, "/") != 0) {
				pwd(you->current->parentDir);
				printf("/%s\n", you->current->name);
			} else
				printf("/\n");

		} else if (strcmp(input, "tree") == 0) {
			int level_from_root = taburi_necesare(you, "/");

			tree(you, you->current->name, level_from_root);
		} else if (strcmp(comanda, "rm") == 0)
			rm(&(you->current->fisiernext), argument);

		else if (strcmp(comanda, "rmdir") == 0)
			rmdir_aux(&you, argument);

		else if (strcmp(comanda, "find") == 0)
			find_aux(you, input);

		fgets(input, 100, stdin);
		input[strlen(input) - 1] = '\0';
	}
	return 0;
}
