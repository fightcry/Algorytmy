#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rekord
{
	int id;
	char* slowo;
	struct rekord* nast;
	struct rekord* poprz;
};

struct lista
{
	struct rekord* pierwszy;
	struct rekord* ostatni;
};

void wstaw(char s[], struct lista* l)
{
	struct rekord* x = malloc(sizeof(struct rekord));
	x->nast = l->pierwszy;
	if(x->nast != NULL)
		 x->nast->poprz = x;
	x->poprz = NULL;
	l->pierwszy = x; 
	char* y = malloc(strlen(s)*sizeof(char)+1);
	strcpy(y, s);
	x->slowo = y;
	if(l->ostatni == NULL)
		l->ostatni = x;
}

void drukuj(struct lista* l)
{
	if(l->pierwszy != NULL)
	{	
		struct rekord* wskaznik = l->pierwszy;
		while(wskaznik != NULL)
		{
			for(int i = 0; i < strlen(wskaznik->slowo); i++)
				printf("%c", wskaznik->slowo[i]);	
			printf("\n");
			wskaznik = wskaznik->nast;
		}
	}
}

struct rekord* szukaj(char s[], struct lista* l)
{
	struct rekord* wskaznik = l->pierwszy;
	while(wskaznik != NULL)
	{
		if(!strcmp(wskaznik->slowo, s))
			return wskaznik;
		wskaznik = wskaznik->nast;
	}
	
	return NULL;	
}

void usun(char s[], struct lista* l)
{
	struct rekord* wskaznik = szukaj(s, l);
	if(wskaznik != NULL)
	{
		if(wskaznik->poprz != NULL)
			wskaznik->poprz->nast = wskaznik->nast;
		else
			l->pierwszy = wskaznik->nast;
		if(wskaznik->nast != NULL)
			wskaznik->nast->poprz = wskaznik->poprz;
		free(wskaznik);
	}		
}

void kasuj(struct lista* l)
{
	if(l->pierwszy != NULL)
	{
		struct rekord* wskaznik = l->pierwszy;
		struct rekord* wskaznik2 = wskaznik->nast;
		free(wskaznik);
		while(wskaznik2 != NULL)
		{
			wskaznik = wskaznik2->nast;
			free(wskaznik2);
			wskaznik2 = wskaznik;
		}
		l->pierwszy = NULL;
		l->ostatni = NULL;
	}
}

struct lista* bezpowtorzen(struct lista* l)
{
	struct lista* l2 = malloc(sizeof(struct lista));
	l2->pierwszy = NULL;
	l2->ostatni = NULL;
	
	struct rekord* wskaznik = l->ostatni; 
	while(wskaznik != NULL)
	{
		wstaw(wskaznik->slowo, l2);
		wskaznik = wskaznik->poprz;
	}
	
	wskaznik = l2->pierwszy; 
	struct rekord* wskaznik2 = l2->pierwszy;
	while(wskaznik != NULL)
	{
		while(wskaznik2!=NULL)
		{
			if(wskaznik2 != wskaznik)
			{
				if(!strcmp(wskaznik->slowo, wskaznik2->slowo))
				{
					if(wskaznik2->poprz != NULL)
						wskaznik2->poprz->nast = wskaznik2->nast;
					else
						l2->pierwszy = wskaznik2->nast;
					if(wskaznik2->nast != NULL)
						wskaznik2->nast->poprz = wskaznik2->poprz;
					free(wskaznik2);
				}
			}
			wskaznik2 = wskaznik2->nast;
		}
		wskaznik2 = l2->pierwszy;
		wskaznik = wskaznik->nast;		
	}	
	return l2;
}

struct lista* scal(struct lista* l, struct lista* l2)
{
	struct lista* l3 = malloc(sizeof(struct lista));
	l3->pierwszy = NULL;
	l3->ostatni = NULL;
	
	struct rekord* wskaznik = l->ostatni; 
	while(wskaznik != NULL)
	{
		wstaw(wskaznik->slowo, l3);
		wskaznik = wskaznik->poprz;
	}
	
	wskaznik = l2->ostatni; 
	while(wskaznik != NULL)
	{
		wstaw(wskaznik->slowo, l3);
		wskaznik = wskaznik->poprz;
	}
	
	wskaznik = l3->pierwszy; 
	struct rekord* wskaznik2 = l3->pierwszy;
	while(wskaznik != NULL)
	{
		while(wskaznik2!=NULL)
		{
			if(wskaznik2 != wskaznik)
			{
				if(!strcmp(wskaznik->slowo, wskaznik2->slowo))
				{
					if(wskaznik2->poprz != NULL)
						wskaznik2->poprz->nast = wskaznik2->nast;
					else
						l3->pierwszy = wskaznik2->nast;
					if(wskaznik2->nast != NULL)
						wskaznik2->nast->poprz = wskaznik2->poprz;
					free(wskaznik2);
				}
			}
			wskaznik2 = wskaznik2->nast;
		}
		wskaznik2 = l3->pierwszy;
		wskaznik = wskaznik->nast;		
	}	
	return l3;
}

int main()
{
	struct lista l;
	l.pierwszy = NULL;
	l.ostatni = NULL;
	
	char tab[] = "andrzej";
	wstaw(tab, &l);
	char tab2[] = "blazej";
	wstaw(tab2, &l);
	char tab3[] = "piotrek";
	wstaw(tab3, &l);
	wstaw(tab3, &l);
	wstaw(tab2, &l);
	
	struct lista l2;
	l2.pierwszy = NULL;
	l2.ostatni = NULL;
	
	wstaw(tab3, &l2);
	char tab4[] = "szczepan";
	wstaw(tab4, &l2);
	char tab5[] = "tomasz";
	wstaw(tab5, &l2);
	
	printf("Wstawianie \n\n");
	drukuj(&l);
	printf("\nWstawianie 2 \n\n");
	drukuj(&l2);
	printf("\nScal \n\n");
	drukuj(scal(&l, &l2));
	printf("\nBez powtorzen\n\n");
	drukuj(bezpowtorzen(&l));
	printf("\nUsuwanie\n\n");
	usun("andrzej", &l);
	drukuj(&l);
	printf("\nKasowanie\n");
	kasuj(&l);
	drukuj(&l);
	printf("-------------------\n");
	
	return 0;
}
