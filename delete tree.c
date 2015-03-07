/*
D5. (4 puncte) Program care primeste ca argument in linia de comanda un
 director si sterge (recursiv) toata arborescenta cu originea in el.
*/
 
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<stdio.h>

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/stat.h>

#if STAT_MACROS_BROKEN
# undef S_ISDIR
#endif

#if !defined S_ISDIR && defined S_IFDIR
# define S_ISDIR(Mode) (((Mode) & S_IFMT) == S_IFDIR)
#endif

/* If PATH is an existing directory or symbolic link to a directory,
   return nonzero, else 0.  */

/* http://src.gnu-darwin.org/src/gnu/usr.bin/grep/isdir.c.html */

int isdir (const char *path)
{
  struct stat stats;

  return stat (path, &stats) == 0 && S_ISDIR (stats.st_mode);
}

/*
1. In Unix totul este un fiser. Incerc sa deschid fiserul ca un director,
   cu functia "opendir".
   http://pubs.opengroup.org/onlinepubs/009695399/functions/opendir.html
   "pd" este pointerul catre director intors de aceasta functie, in caz
   de succes.
2. Presupunand ca am avut succes mai sus, incercam sa citim continutul
   directorului. Pentru asta avem nevoie de o "intrare in director", pde.
   http://www.gnu.org/software/libc/manual/html_node/Directory-Entries.html
3. In interiorul directorului deschis, daca intalnim un fisier care este
   un alt director ( if-ul ), ne asiguram ca nu este el insusi ( "." ) sau
   parintele lui ( ".." ) iar daca trece de aceste teste, apelam recursiv
   functia. Scopul apelului recursiv este "coborarea" in arborescenta.
   
   Verificarea se face cu o functie preluata dintr-un cod open source.
   3.0.
        In structura "stats" punem detaliile fisierului dat prin "path".
        Codul de return a functiei trebuie sa fi 0 pentru a ne asigura ca
        nu a avut loc o eroare.
        S_ISDIR ne va da o valoare ne nula daca fisierul este director.
        Vedem acest lucru uitand-ne in detaliile lui, stats.st_mode.
        Conjunctia lor trebuie sa fie ne nula.
         
   http://linux.die.net/man/2/stat
   
   In caz ca fisierul intalnit nu este un director, il stergem.
4. In main, ma asigur ca am rulat programul cu un argument.
*/

int listdir(char *nf)
{
    DIR *pd;
    struct dirent *pde;
    char cale[256], specificator[256],init[256];
    pd=opendir(nf); 
    if(pd==NULL)
    {
        return -1;
    }
    strcpy(cale, nf);
    strcat(cale,"/");
    while((pde=readdir(pd))!=NULL)
    {
        strcpy(specificator, cale);
        strcat(specificator, pde->d_name);

        if(isdir(specificator))
        {   
            if(strcmp (pde->d_name,".") != 0 && strcmp (pde->d_name,"..") != 0)
            {
                listdir(specificator);
                remove(specificator);
            }
        }
        else
        {
            remove(specificator);
        }
    }
    closedir(pd);
    return 0;
}

int main(int argc,char *argv[])
{
	
    if (argc==2)
    {
        if(listdir(argv[1])==-1)
		{
			perror(argv[1]);
			return 1;
		}
		remove(argv[1]);
    }
    else
    {
        fprintf(stderr,"Utilizare: %s director\n",argv[0]);
        return 1;
    }
    return 0;
}
