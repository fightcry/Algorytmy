#include <stdio.h>
#include <string.h>
int zakres = 123;
void countingsort(char **wskaznik, int counter, int element, int rozmiar[])
{
 
	int count[zakres];
	char *pom[counter]; 
	memset(count, 0, sizeof count);
	for(int i = 0; i < counter; i++)
	{
		if(rozmiar[i] > element)
			count[*(wskaznik[i]+element)]++;
		else
			count[0]++;
			
	}

	for(int i = 1; i < zakres; i++)
	{
		count[i] = count[i] + count[i-1];
	}

	for(int i = counter-1; i >= 0; i--)
	{
		
		if(rozmiar[i] > element)
		{	
			pom[count[*(wskaznik[i]+element)]-1] = wskaznik[i];
			count[*(wskaznik[i]+element)]--;
		}	
		else
		{	
			pom[count[0]-1] = wskaznik[i];
			count[0]--;
		}
		
	}
	
	for(int i = 0; i < counter; i++)
	{
		wskaznik[i] = pom[i];
	}
}




void radixsort(char tab[], char **wskaznik, int max, int counter, int rozmiar[])
{
	for(int i = max-1; i >=0; i--)
	{
		countingsort(wskaznik, counter, i, rozmiar);
	}
}

int main()
{
	int counter = 1, counterr = 0, max = 0, temp  = 0;
	FILE *plik;
	plik = fopen ("dane.txt", "r");
	while(!feof(plik))
	{
		char ch = fgetc(plik);
		if(ch == '\n')
		{	
			counter++;
			if(temp>max)
				max = temp;
			temp = 0;
		}	
		if((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z'))	
		{	
			counterr++;
			temp++;
		}	
	}
	char tab[counterr];
	char *wskaznik[counter];
	int rozmiar[counter];
	memset(rozmiar, 0, sizeof rozmiar);
	rewind(plik);
	int j = 0;
	wskaznik[0] = tab;
	for(int i = 0; !feof(plik);)
	{
		char ch = fgetc(plik);
		if((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9'))
		{
			rozmiar[j]++;
			tab[i] = ch;
			i++;
		}
		if(ch >= 'A' && ch <= 'Z')
		{
			rozmiar[j]++;
			tab[i] = ch+32;
			i++;
		}
		
		if(ch==10)
		{
			j++;
			wskaznik[j]=&tab[i];
		}
	}
	fclose(plik);
	int z = 0, y = 0;
	for(int i = 0; i < counterr; i++)
	{
		printf("%c", *(wskaznik[0]+i));
		z++;	
		if(z==rozmiar[y])
		{
			printf("\n");
			z = 0;
			y++;
		}
	}
	radixsort(tab, wskaznik, max, counter, rozmiar);

	for(int i = 0; i < counter; i++)
	{
		for(int j = 0; j < rozmiar[i]; j++)
			printf("%c", *(wskaznik[i]+j));
		printf("\n");
	}
}
