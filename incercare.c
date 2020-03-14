#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char a[] = "touch a.in cevatextaici";
    char *p,*b,*nume,*text;
    p = strtok(a," ");
    nume = strtok(NULL," ");
    text = strtok(NULL," ");
    b = NULL;
    printf("%s\n%s\n%s\n",p,nume,text);
    return 0;
}