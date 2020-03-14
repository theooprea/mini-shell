#ifndef __FILE_H__
#define __FILE_H__

typedef struct Directory {
    // The name of the directory
    char *name;

    // TODO: The list of files of the current directory
    struct fisier *fisiernext;

    // TODO: The list of directories of the current directory
    struct folder *subDirector;

    // The parent directory of the current directory (NULL for the root directory)
    struct Directory *parentDir;

    // Next directory on the same level of the hierarchy ()
} Directory;

typedef struct File {
    // The name of the file
    char *name;

    // The size of the file
    int size;

    // The content of the file
    char *data;

    // The directory in which the file is located
    Directory *dir;
} File;

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

#endif /* __FILE_H__ */
