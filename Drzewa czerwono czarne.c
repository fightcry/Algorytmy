//                                          pmp@inf.ug.edu.pl  2014
// drukowanie drzew cz-cz z wartownikiem wspolnym
// dla wszystkich wezlow:
// drukowanie na ekran tekstowy i przez dot z pakietu graphviz

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 1        /* stala oznaczajaca kolor wezla */
#define BLACK 0      /* stala oznaczajaca kolor wezla */
#define SZER_EKR 80  /* szerokosc ekranu */
#define IL_POZ   10   /* ilosc poziomow drzewa, ktore beda wydrukowane   */
                     /* gwiazdka bedzie sygnalizowac istnienie nizszych */
                     /* poziomow                                        */

/* struktury danych do reprezentowania drzewa */
typedef struct wezel *Wskwezla; /* wskaznik na wezel drzewa  */
typedef struct wezel{
	int klucz;
	Wskwezla left, right, p;
	int kolor;
} Twezla ;  /* typ wezla */

/* drzewa z wartownikami: wezel wskazywany przez "nil" jest wartownikiem 
   zastepujacym NULL; dla korzenia pole "p" ma wartosc "nil"; 
   przy usuwaniu trzeba odpowiednio wypelnic pole nil->p 
*/
Wskwezla nil; 

void nilInit(void){
/* funkcja inicjujaca nil; musi byc wywolana przed budowaniem drzewa */
  nil= (Wskwezla) malloc(sizeof(Twezla));
  nil->p = NULL;
  nil->kolor = BLACK;
  nil->left = nil->right = NULL;
  nil->klucz = 0;  
}

void drukuj(Wskwezla w);
/* funkcja drukujaca drzewo binarne na ekranie (tutaj tylko deklaracja) */
/* funkcja drukuje drzewo o korzeniu "w"                                */

void drukujDot(Wskwezla r);
  // generuje w plikach drzewo0.gv,  drzewo1.gv ...
  // opis drzew o korzeniu r do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf" 

/* ------------  implementacja ------------------------------------- */
char wydruk[IL_POZ+1][SZER_EKR];

void drukujost(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       if (w==nil)   return; 
       wydruk[poziom][srodek]='*';
}

void drukujwew(Wskwezla w, int l, int p,int poziom){
       int srodek = (l+p)/2;
       int i,dl;
       char s[19];
       if (w==nil)    return;
       if (w->kolor==BLACK)
         dl=sprintf(s,"%d",w->klucz);
       else
	 //	        dl=sprintf(s,"\e[31m%+d\e[0m",w->klucz);
       dl=sprintf(s,"%+d",w->klucz);
       for (i=0;i<dl;i++)
         wydruk[poziom][srodek-dl/2+i]=s[i];
       if (++poziom<IL_POZ){
         drukujwew(w->left,l,srodek,poziom) ;
         drukujwew(w->right,srodek+1,p,poziom) ;
       }
       else {
         drukujost(w->left,l,srodek,poziom) ;
         drukujost(w->right,srodek+1,p,poziom) ;
       }
}

void drukuj(Wskwezla w){
  int j,i;
  for (i=0;i<=IL_POZ;i++)
    for (j=0;j<SZER_EKR;j++)
      wydruk[i][j] = ' ';
  drukujwew(w,0,SZER_EKR,0);
  for (i=0;i<=IL_POZ;i++){
      for (j=0;j<SZER_EKR;j++)
        putchar(wydruk[i][j]);
      printf("\n");
  }
}

void drukujKrawedz(FILE *plikwy, Wskwezla r, int z, Wskwezla syn, int zs){
// wezel r o numerze z jest juz wydrukowany
// teraz drukujemy jego syna "syn" o numerze zs oraz krawedz miedzy nimi
  if (syn == nil){
    fprintf(plikwy,"%d [shape=circle, style=invisible, label=\"",zs);
    fprintf(plikwy,"%d ",0);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d [style=invis];\n ",z,zs);
  } else{
    if (syn->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",zs);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",zs);
    fprintf(plikwy,"%d ",syn->klucz);
    fprintf(plikwy,"\"]\n");
    fprintf(plikwy,"%d -- %d ;\n",z,zs);
  }  
}

int rekDrukujDot(Wskwezla r, int z, FILE *plikwy){
  // drukuje drzewo o korzeniu r 
  // z  - numer wezla r
  // zwraca najwiekszy numer wezla w poddrzewie, ktorego jest korzeniem
  // zakladamy, ze korzen r jest juz wydrukowany
  int nz;
  if (r == nil) return z;
  else{
    drukujKrawedz(plikwy,r,z,r->left,z+1);
    nz=rekDrukujDot(r->left,z+1,plikwy);
    drukujKrawedz(plikwy,r,z,r->right,nz+1);
    nz=rekDrukujDot(r->right,nz+1,plikwy);
    return nz;
  }
}

void drukujDot(Wskwezla r){
  // generuje w plikach drzewo0.gv, dzrewo1.gv, ... 
  // opisy drzew o korzeniu r pzeznaczone do wydrukowania przez program dot
  // zlecenie "dot -Tpdf -O drzewo1.gv" utworzy plik "drzewo1.gv.pdf" 
  static int gdzie=0;
  char numer[10];
  char nazwap[20]="drzewo";
  FILE *plikwy;
  snprintf(numer,9,"%d",gdzie);
  strcat(nazwap,numer);
  strcat(nazwap,".gv");
  plikwy=fopen(nazwap,"w");
  gdzie++;
  fprintf(plikwy, "graph drzewo{\n");
  fprintf(plikwy, "size = \"2,20\"");
  if (r!=nil){
    if (r->kolor == RED)
      fprintf(plikwy,"%d [shape=circle, color=red, label=\"",0);
    else
      fprintf(plikwy,"%d [shape=circle, color=black, label=\"",0);
    fprintf(plikwy,"%d ",r->klucz);
    fprintf(plikwy,"\"]\n");
    rekDrukujDot(r,0,plikwy);
  }
  fprintf(plikwy, "}\n");
  fclose(plikwy);
  printf("wydrukowane %s\n",nazwap);
}

Wskwezla nowyWezel(int klucz, int kolor){
// tworzy nowy wezel. Przydatne do testowania wydruku
     Wskwezla w = (Wskwezla) malloc(sizeof(Twezla));
     w->p = nil;
     w->klucz=klucz;
     w->kolor = kolor;
     w->left = w->right = nil;
     return w;
}  

/* ------------- miejsce na inne funkcje -------------------- */

Wskwezla naprawWstawianie(Wskwezla obecnyWezel) {						
	while(obecnyWezel->p != nil) {
	if(obecnyWezel->kolor == RED)
	if(obecnyWezel->p->kolor == RED) {								
		if(obecnyWezel->p->p != nil)								// jeœli ojciec to korzeñ to wyjdŸ
			if(obecnyWezel->p == obecnyWezel->p->p->left) {
				if(obecnyWezel->p->p->right->kolor == RED) {		//przypadek 1 - przekolorowanie
					obecnyWezel->p->kolor = BLACK;
					obecnyWezel->p->p->kolor = RED;
					obecnyWezel->p->p->right->kolor = BLACK;
					obecnyWezel = obecnyWezel->p->p;
					continue;
				}
				else
					if(obecnyWezel == obecnyWezel->p->right) {		//przypadek 2
						Wskwezla tempLewy = obecnyWezel->left;      //obrót w lewo
						Wskwezla tempDziadek = obecnyWezel->p->p;
						obecnyWezel->p->p->left = obecnyWezel;
						obecnyWezel->p->right = tempLewy;
						obecnyWezel->left = obecnyWezel->p;
						obecnyWezel->left->p = obecnyWezel;
						obecnyWezel->p = tempDziadek;
						obecnyWezel = obecnyWezel->left;
						if(obecnyWezel->p->p == obecnyWezel->p->p->p->left) {
							if(obecnyWezel->p == obecnyWezel->p->p->left) {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->left = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->left = obecnyWezel->right;
							dziadek->left->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->right = dziadek;
							obecnyWezel->p->p = pradziadek;
							}
							else {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->left = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->right = obecnyWezel->left;
							dziadek->right->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->left = dziadek;
							obecnyWezel->p->p = pradziadek;
							}								
						}
						else if(obecnyWezel->p == obecnyWezel->p->p->left){
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->right = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->left = obecnyWezel->p->right;
							dziadek->left->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->right = dziadek;
							obecnyWezel->p->p = pradziadek;
						}
						else {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->right = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->right = obecnyWezel->left;
							dziadek->right->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->left = dziadek;
							obecnyWezel->p->p = pradziadek;
						}
						while(obecnyWezel->p != nil)				//znalezienie korzenia
							obecnyWezel = obecnyWezel->p;
						obecnyWezel->kolor = BLACK;
						return obecnyWezel;
					}
					else {
						Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
						Wskwezla dziadek = obecnyWezel->p->p;
						pradziadek->left = obecnyWezel->p;
						dziadek->p = obecnyWezel->p;
						dziadek->kolor = RED;
						dziadek->left = obecnyWezel->p->right;
						dziadek->left->p = dziadek;
						obecnyWezel->p->kolor = BLACK;
						obecnyWezel->p->right = dziadek;
						obecnyWezel->p->p = pradziadek;
						
						while(obecnyWezel->p != nil)				//znalezienie korzenia
							obecnyWezel = obecnyWezel->p;
						obecnyWezel->kolor = BLACK;
						return obecnyWezel;
					}
			}
			else {
				if(obecnyWezel->p->p->left->kolor == RED) {			//przypadek 1 - przekolorowanie
				obecnyWezel->p->kolor = BLACK;
				obecnyWezel->p->p->kolor = RED;
				obecnyWezel->p->p->left->kolor = BLACK;
				obecnyWezel = obecnyWezel->p->p;
				continue;
				}
				else
					if(obecnyWezel == obecnyWezel->p->left) {		//przypadek 2
						Wskwezla tempPrawy = obecnyWezel->right;    //obrót w lewo, lustrzanie odbity
						Wskwezla tempDziadek = obecnyWezel->p->p;
						obecnyWezel->p->p->right = obecnyWezel;
						obecnyWezel->p->left = tempPrawy;
						obecnyWezel->right = obecnyWezel->p;
						obecnyWezel->right->p = obecnyWezel;
						obecnyWezel->p = tempDziadek;
						obecnyWezel->right = obecnyWezel;
							
						if(obecnyWezel->p->p == obecnyWezel->p->p->p->left) {
							if(obecnyWezel->p == obecnyWezel->p->p->left) {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->left = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->left = obecnyWezel->right;
							dziadek->left->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->right = dziadek;
							obecnyWezel->p->p = pradziadek;
							}
							else {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->left = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->right = obecnyWezel->left;
							dziadek->right->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->left = dziadek;
							obecnyWezel->p->p = pradziadek;
							}								
						}
						else if(obecnyWezel->p == obecnyWezel->p->p->left){
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->right = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->left = obecnyWezel->p->right;
							dziadek->left->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->right = dziadek;
							obecnyWezel->p->p = pradziadek;
						}
						else {
							Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
							Wskwezla dziadek = obecnyWezel->p->p;
							pradziadek->right = obecnyWezel->p;
							dziadek->p = obecnyWezel->p;
							dziadek->kolor = RED;
							dziadek->right = obecnyWezel->left;
							dziadek->right->p = dziadek;
							obecnyWezel->p->kolor = BLACK;
							obecnyWezel->p->left = dziadek;
							obecnyWezel->p->p = pradziadek;
						}
							
						while(obecnyWezel->p != nil)				//znalezienie korzenia
							obecnyWezel = obecnyWezel->p;
						obecnyWezel->kolor = BLACK;
						return obecnyWezel;						
					}
					else {
						Wskwezla pradziadek = obecnyWezel->p->p->p;	//przypadek 3
						Wskwezla dziadek = obecnyWezel->p->p;
						pradziadek->right = obecnyWezel->p;
						dziadek->p = obecnyWezel->p;
						dziadek->kolor = RED;
						dziadek->right = obecnyWezel->left;
						dziadek->right->p = dziadek;
						obecnyWezel->p->kolor = BLACK;
						obecnyWezel->p->left = dziadek;
						obecnyWezel->p->p = pradziadek;

						while(obecnyWezel->p != nil)				//znalezienie korzenia
							obecnyWezel = obecnyWezel->p;
						obecnyWezel->kolor = BLACK;
						return obecnyWezel;								
					}					
			}
		else {
			obecnyWezel->p->kolor = BLACK;
			return obecnyWezel->p;
		}
	}
	obecnyWezel = obecnyWezel->p;
}
	obecnyWezel->kolor = BLACK;
	return obecnyWezel;							
}			
				
Wskwezla wstawWezel(Wskwezla korzen, int klucz) {
	Wskwezla obecnyWezel = korzen;
	Wskwezla dodanyWezel = nowyWezel(klucz, RED);
	while(1) {
		if(obecnyWezel->klucz < klucz) {
			if(obecnyWezel->right == nil) {
				obecnyWezel->right = dodanyWezel;
				dodanyWezel->p = obecnyWezel;
				break;
			}
			else
				obecnyWezel = obecnyWezel->right;		
		}
		else {
			if(obecnyWezel->left == nil) {
				obecnyWezel->left = dodanyWezel;
				dodanyWezel->p = obecnyWezel;
				break;
			}
			else
				obecnyWezel = obecnyWezel->left;	
		}
	}
	return naprawWstawianie(dodanyWezel);
}

int policzWezel(Wskwezla korzen, int wynik[])
{
	int dlugosc = 1, dlugoscPrawa = 0, dlugoscLewa = 0;
	if(korzen->kolor == RED)
		wynik[2]++;
	if(korzen->left != nil)
		dlugoscLewa = policzWezel(korzen->left, wynik);
	if(korzen->right != nil)
		dlugoscPrawa = policzWezel(korzen->right, wynik);
	if(dlugoscLewa > dlugoscPrawa)
		dlugosc = dlugosc + dlugoscLewa;
	else
		dlugosc = dlugosc + dlugoscPrawa;
	if(dlugosc > wynik[0])
		wynik[0] = dlugosc;	
	return dlugosc;
}

int policzMinWezel(Wskwezla korzen)
{
	int dlugosc = 1;
	while(korzen->left != nil && korzen->right != nil)
	{
	if(korzen->left->kolor == BLACK) 
		korzen = korzen->left;
	else if(korzen->right->kolor == BLACK)
		korzen = korzen->right;
	else
		korzen = korzen->left;
	dlugosc++;
	}
	return dlugosc;
}
/* ----------------- program testujacy -----------------------*/

int main(){
	nilInit();
	int klucz, wyborMenu;
	int wynik[3] = {0, 0, 0};
	Wskwezla korzen = nil, kopia;
	int stanPetli = 1;
	Wskwezla dodanyWezel = nowyWezel(38, BLACK);
	korzen = dodanyWezel;
	korzen = wstawWezel(korzen, 31);
	korzen = wstawWezel(korzen, 22);
	korzen = wstawWezel(korzen, 8);
	korzen = wstawWezel(korzen, 20);
	korzen = wstawWezel(korzen, 5);
	korzen = wstawWezel(korzen, 10);
	korzen = wstawWezel(korzen, 9);
	korzen = wstawWezel(korzen, 21);
	korzen = wstawWezel(korzen, 27);
	korzen = wstawWezel(korzen, 29);
	korzen = wstawWezel(korzen, 25);
	korzen = wstawWezel(korzen, 28);
	
	while(stanPetli == 1) {
		printf("Wpisz odpowiednia cyfre:\n 1.Wstawic element 2. Usunac element 3. Wydrukowac drzewo 4. Wydrukowac dane dla 1000 elementow 5. Wyjsc");
		scanf("%d", &wyborMenu);
		switch(wyborMenu) {
			case 1:
			printf("Podaj liczbe do wstawienia");
			scanf("%d", &klucz);
			if(korzen == nil) {
				Wskwezla dodanyWezel = nowyWezel(klucz, BLACK);
				korzen = dodanyWezel;
			}
			else
				korzen = wstawWezel(korzen, klucz);
			break;
			case 2:
			printf("Podaj liczbe do usuniecia");
			scanf("%d", &klucz);
			//usunWezel(korzen, klucz);
			case 3:
			drukuj(korzen);
			policzWezel(korzen, wynik);
			wynik[1] = policzMinWezel(korzen);
			printf("\n Maksymalna glebokosc = %d, minimalna glebokosc = %d, ilosc czerwonych wezlow = %d \n", wynik[0], wynik[1], wynik[2]);
			wynik[0] = 0;
			wynik[1] = 0;
			wynik[2] = 0;
			break;
			case 4:
			kopia = korzen;
			korzen = nil;
			for(int i = 0; i < 1000; i++)
			{	
				if(korzen == nil) {
					Wskwezla dodanyWezel = nowyWezel(i, BLACK);
					korzen = dodanyWezel;
				}
				else
					korzen = wstawWezel(korzen, i);
			}
			policzWezel(korzen, wynik);
			wynik[1] = policzMinWezel(korzen);
			printf("\n Maksymalna glebokosc = %d, minimalna glebokosc = %d, ilosc czerwonych wezlow = %d \n", wynik[0], wynik[1], wynik[2]);
			wynik[0] = 0;
			wynik[1] = 0;
			wynik[2] = 0;
			korzen = kopia;
			break;			
			case 5:
			stanPetli = 0;
		}
	}
	return 0;
}

     




