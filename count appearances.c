/*F14. (3 puncte) Scrieti un program care numara aparitiile unui string 
intr-un fisier dat. Stringul si specificatorul fisierului sunt dati ca 
argumente in linia de comanda.
ananas*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void numara(FILE *p, char *S)
{
	int nr=0,n=1,ch=-1; //ch=pozitia actuala in fisier
	char *Ln,*l;
	char c;
	int t=(int)strlen(S);
	//c=fgetc(p);
	fseek(p,0,SEEK_END);
	if (!ftell(p))
		fprintf(stderr,"Fisierul este gol!\n");
	else
	{
	fseek(p,0,SEEK_SET);
	c=fgetc(p);
	while (!feof(p)) //exista EOF in interiorul executabilului =>feof
	{
		ch++;
		if (c==S[0])
		{
			n=1;
			c=fgetc(p);
			while (c==S[n] && n!=t)
			{
				n++;
				c=fgetc(p);
			}
			
			if (n==t)
			{
				nr++;
				fseek(p,ch+1,SEEK_SET);
			}
			else
			{
				fseek(p,ch+1,SEEK_SET);
				c=fgetc(p);
			}
		}
		else
			c=fgetc(p);	
	}
	printf("Stringul \"%s\" apare de %d ori\n",S,nr);
	} //else
	fclose(p);
}

int main(int argc, char *argv[])
{
	FILE *ptr_file;
	char *S;
	if (argc!=3)
	{
		//fprintf(stderr,"Eroare! Utilizare: %s cale_fisier string\n",argv[0]);
		perror(argv[1]);
	}
	else
	{
		ptr_file=fopen(argv[1],"rb");
		if (!ptr_file){
		    perror(argv[1]);
		    return 1;
		}
		else
			numara(ptr_file,argv[2]);
	}
return 0;
}
