#include <stdio.h>
#include <stdlib.h>

#define T 3   // stopien B-drzewa

typedef struct{
  short n;        //ilosc kluczy (-1 oznacza wezel usuniety)       
  short leaf;     // czy lisc
  int k[2*T-1];   // klucze (numerowane od pozyji 0)
  int c[2*T];     // "wska�niki" do synow (pozycje w pliku: 0,1,2 ...)
  int pozycja;

} Wezel;


int rozmiarw = sizeof(Wezel); // rozmiar wezla w bajtach
FILE *drzewo;  // plik drzewa (zawierajacy wezly)

void zapisz(int i,Wezel *w){
  // zapisuje *w jako i-ty zapis w pliku drzewa
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fwrite(w,rozmiarw,1,drzewo);
  //  printf("z%d ",i);
}

void odczytaj(int i,Wezel *w){
  // odczytuje i-ty zapis w pliku drzewa i wpisuje do *w
  fseek(drzewo,(long)i*rozmiarw,SEEK_SET);
  fread(w,rozmiarw,1,drzewo);
  //  printf("o%d ",i);
}

void drukujB(int r, int p){
  // drukuje B-drzewo o korzeniu r (pozycja w pliku)
  // wydruk przesuniety o p w prawo
  Wezel w;
  int i,j;
  odczytaj(r,&w);
  if (w.leaf){
    for (i=0;i<p;i++) printf(" ");
    for (i=0;i<w.n;i++) printf("%d ",w.k[i+1]);  //jesli jest lisciem to drukujemy
  } 
  else {
    
    for (i=0;i<p;i++) printf(" ");  //jesli nie to drukujemy wszystkie klucze oraz schodzimy wgłąb drzewa
    for(int i = w.n; i > 0; i--) 
      printf("%d ", w.k[i]);
    printf("\n");  
    
    for(int i = 1; i < 2*T; i++)
      if(w.c[i] != 0)
        drukujB(w.c[i] + (w.pozycja*(2*T-1)), p+4);
  }
}

int* szukajElement(int element, int pozycja) {//szuka elementu lub jesli go nie ma podaje miejsce gdzie mozna go wstawic
Wezel *pomocniczy = malloc(sizeof(Wezel));
int *wynik = malloc(sizeof(int)*3); //3 pole wskazuje na to czy znalezienie sie udalo czy nie
int i = 1;
odczytaj(pozycja, pomocniczy);
while((i <= pomocniczy->n) && (element > pomocniczy->k[i])) 
  i++;
if((i <= pomocniczy->n) && (element == pomocniczy->k[i])) { //jesli znalezlismy element
  wynik[0] = pomocniczy->pozycja;
  wynik[1] = i;
  wynik[2] = 1;
  return wynik;
}

if(pomocniczy->leaf) {
  wynik[0] = pomocniczy->pozycja;
  wynik[1] = i;
  wynik[2] = 0;               //jesli doszlismy do konca i elementu nie bylo
  return wynik;
  }
else
{
  odczytaj((pomocniczy->pozycja * (2*T-1)) + pomocniczy->c[i], pomocniczy); //jesli nie jest lisciem to schodzimy glebiej
  szukajElement(element, pomocniczy->pozycja);
}
}

void wstawElementNiePelny(Wezel *x, int element) {
int i = x->n;
if(x->leaf) {                             //jesli jest lisciem to wstawiamy nowy element do danego wezla oraz sortujemy go
  
  while(i >= 1 && element < x->k[i]) { 
    x->k[i+1] = x->k[i];
    i--;
  }
  Wezel *m = malloc(sizeof(Wezel));

  x->k[i+1] = element;
  x->n++;

  zapisz(x->pozycja, x);
}
else {
  while(i >= 1 && element < x->k[i])                  //schodzimy w dol drzewa, jesli wezel jest pelny to go rozdzielamy, a jesli nie to przechodzimy do nowej funkcji
    i--;
  i++;
  Wezel *pomocniczny = malloc(sizeof(Wezel));
  odczytaj(x->c[i], pomocniczny);
  if(pomocniczny->n == (2*T)-1) {
    rozdzielDrzewo(x, i, pomocniczny, element);
    if(element > x->k[i])
      i++;
  }
  wstawElementNiePelny(pomocniczny, element);
}
}

void wstawElement(int element) {                //pierwsza funkcja ktora wywoluje sie przy wstawianiu, jesli jest miejsce to wstawia, jesli nie to tworzy nowy wezel oraz rozdziela
Wezel *r = malloc(sizeof(Wezel));
odczytaj(szukajElement(element, 1)[0], r);
if(r->n == (2*T)-1) {
  Wezel *s = malloc(sizeof(Wezel));
  s->leaf = 0;
  s->n = 1;
  for(int i = 0; i<2*T ; i++)
      s->c[i] = 0;
  s->k[1] = r->k[T];
  s->pozycja = r->pozycja;
  r->leaf = 1;
  rozdzielDrzewo(s, 1, r, element);
}
else {
  wstawElementNiePelny(r, element);}
}

void rozdzielDrzewo(Wezel *x, int i, Wezel *y, int element) {
  Wezel *z = malloc(sizeof(Wezel));
  z->leaf = y->leaf;
  z->n = T-1;                             //tworzymy z i inicjalizujemy
  for(int j = 1; j < T; j++)              //dzielimy y na y i z, z zawiera elementy wieksze
    z->k[j] = y->k[j+T];
  if(!y->leaf)
    for(int j = 1; j < T; j++)
      z->c[j] = y->c[j+T];
  else
  {
    for(int j = 0; j < 2*T; j++) {      //zerujemy synow zeta, jest to potrzebne do wypisywania
      z->c[j] = 0;
    }
  }
  y->n = T-1;

  x->c[i] = i; 
  y->pozycja = (x->pozycja*(2*T-1)) + i;       //jako wpisujemy pozycje do wezla, jest to tylko ulatwienie dla mnie do wypisywania, tak naprawde nie jest to potrzebne
  x->c[i+1] = i+1;
  z->pozycja = (x->pozycja*(2*T-1)) + i+1;
  if(element > y->k[T-1])
    wstawElementNiePelny(z, element);
  else
    wstawElementNiePelny(y, element);
  
  zapisz(y->pozycja, y);
  zapisz(z->pozycja, z);
  zapisz(x->pozycja, x);
}


int main(){
    drzewo = fopen("bdrzewo","w+");
    Wezel *wezel = malloc(sizeof(Wezel));
    wezel->n = 0;
    wezel->leaf = 1;
    wezel->pozycja = 1;
    for(int i = 0; i<2*T ; i++)
      wezel->c[i] = 0;
    zapisz(wezel->pozycja, wezel);
    
    wstawElement(22);
    wstawElement(32);
    wstawElement(11);
    wstawElement(12);
    wstawElement(33);
    
    drukujB(1, 3);
    printf("\n\n\n");
    
    wstawElement(68);
    
    drukujB(1, 3);
    printf("\n\n\n");
  
    wstawElement(43);

    drukujB(1, 3);
    printf("\n\n\n");
    
    getchar();
    
    fclose(drzewo);
    return 0;
}
