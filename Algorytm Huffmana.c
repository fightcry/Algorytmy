#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> 

typedef struct Wezel *Wskwezla;
struct Wezel
{
    Wskwezla lewy, prawy;
    int wartosc;
    char znak;
    int kod;
};


Wskwezla TworzKodHuffmana(int czestotliwosc_znakow[]) {
    int ilosc_znakow = 0, pozycja_w_tablicy = 0, pozycja_w_tablicy2 = 0, minimalna_wartosc = 0;

    for(int i = 0; i < 256; i++) {                              // patrzymy ile roznych znakow wystepuje w tekscie
        if(czestotliwosc_znakow[i] != 0) 
            ilosc_znakow++;
    }

    Wskwezla tablica_wezlow[ilosc_znakow];
    
    for(int i = 0; i < 256; i++) {                              
        if(czestotliwosc_znakow[i] != 0) {
        Wskwezla x = malloc(sizeof(struct Wezel));               // tworzymy nowy wezel
        x->lewy = NULL;                                 
        x->prawy = NULL;
        x->wartosc = czestotliwosc_znakow[i];
        x->znak = i;                                    // umieszczamy go w liście
        tablica_wezlow[pozycja_w_tablicy] = x;
        pozycja_w_tablicy++;
        }
            
    }

    for(int i = 0; i < ilosc_znakow; i++) {
        pozycja_w_tablicy = __INT_MAX__;
        pozycja_w_tablicy2 = __INT_MAX__;
        minimalna_wartosc = __INT_MAX__;
        
        for(int j = 0; j < ilosc_znakow; j++) {                     //szukamy minimum
            if((tablica_wezlow[j] != NULL) && (tablica_wezlow[j]->wartosc < minimalna_wartosc)) {
                minimalna_wartosc = tablica_wezlow[j]->wartosc;
                pozycja_w_tablicy = j;   
            }
        }
        printf("%d ",tablica_wezlow[pozycja_w_tablicy]->wartosc);
        minimalna_wartosc = __INT_MAX__;

       for(int j = 0; j < ilosc_znakow; j++) {                     //szukamy minimum 2
            if((tablica_wezlow[j] != NULL) && (tablica_wezlow[j]->wartosc <= minimalna_wartosc) && (pozycja_w_tablicy != j)) {
                minimalna_wartosc = tablica_wezlow[j]->wartosc;
                pozycja_w_tablicy2 = j;
            }
        }
        if(pozycja_w_tablicy2 < 50)
            printf("%d \n",tablica_wezlow[pozycja_w_tablicy2]->wartosc);
        if(pozycja_w_tablicy2 == __INT_MAX__)                   //sprawdzamy czy znalezlismy jakikolwiek obiekt, jesli nie to oznacza ze wszystkie oprocz jednego sa nullami i mozemy skonczyc dzialanie algorytmu
            break;        
    
        Wskwezla z = malloc(sizeof(struct Wezel));                           //tworzymy wezel posredniczacy i umieszczamy go w tablicy zamiast wezla minimum, wezel minimum 2 w tablicy zerujemy
        z->lewy = tablica_wezlow[pozycja_w_tablicy];
        z->prawy = tablica_wezlow[pozycja_w_tablicy2];
        z->wartosc = tablica_wezlow[pozycja_w_tablicy]->wartosc + tablica_wezlow[pozycja_w_tablicy2]->wartosc;
        tablica_wezlow[pozycja_w_tablicy] = z;
        tablica_wezlow[pozycja_w_tablicy2] = NULL;
    }

    return tablica_wezlow[pozycja_w_tablicy];                   //zwracamy ostatni dodany wezel
}

void DrukujBinarnie(int n)                                      //kod przechowuję za pomocą inta więc żeby go wypisać stworzyłem specjalną funkcję
{ 
    if (n > 1) 
        DrukujBinarnie(n/2); 
    printf("%d", n % 2); 
} 

int DrukujKodHuffmana(Wskwezla korzen, int kod) {
    int ilosc_znakow = 0;
    if((korzen->lewy == NULL) && (korzen->prawy == NULL)) {                 //gdy jest liściem to drukuj
        DrukujBinarnie(kod);
        korzen->kod = kod;
        printf("\n");
        return 1;
    }
    else {
        kod = kod*2;                                                       //gdy liść jest po lewej to dopisujemy zero do obecnego kodu, 
        ilosc_znakow = ilosc_znakow + DrukujKodHuffmana(korzen->lewy, kod);                               //jeśli jest po prawej to dopisujemy zero i dodajemy jedynkę na koniec
        kod = kod+1;
        ilosc_znakow = ilosc_znakow + DrukujKodHuffmana(korzen->prawy, kod);   
    }
    return ilosc_znakow;
}

int LiczDlugoscKodu(Wskwezla korzen) {
    int dlugosc_kodu = 0;
    if((korzen->lewy == NULL) && (korzen->prawy == NULL)) {                 //gdy jest liściem to drukuj
        printf("xd %d", korzen->wartosc * ((int)log2(korzen->kod)+1));
        if(korzen->kod == 0)
            return korzen->wartosc;
        else
            return (korzen->wartosc * ((int)log2(korzen->kod)+1));
    }
    else {
        dlugosc_kodu = dlugosc_kodu + LiczDlugoscKodu(korzen->lewy);
        dlugosc_kodu = dlugosc_kodu + LiczDlugoscKodu(korzen->prawy);
    }
    return dlugosc_kodu;
}


int main() {
    FILE *plik;
    char nazwa_pliku[] = "przyklad.txt", znak;
    int czestotliwosc_znakow[256] = {0}, dlugosc_tekstu = 0;
    
    plik = fopen(nazwa_pliku, "rb");
    while(fread(&znak, 1, sizeof(char), plik)) {
        printf("%c",znak);
        if(znak > 32 && znak < 127) {
            czestotliwosc_znakow[znak]++;
            dlugosc_tekstu++;
        }
    }
    fclose(plik);
    
    Wskwezla korzen = TworzKodHuffmana(czestotliwosc_znakow);
    printf("\n\n");
    int ilosc_znakow = DrukujKodHuffmana(korzen, 0);
    int dlugosc_kodu = LiczDlugoscKodu(korzen);
    int dlugosc_bez_kodu = (int)log2(ilosc_znakow);
    printf("\n%d : %d czyli kompresja do %f procent", dlugosc_kodu, dlugosc_tekstu*dlugosc_bez_kodu, ((float)dlugosc_kodu/(float)(dlugosc_tekstu*dlugosc_bez_kodu)) * 100);
    
    getchar();
    return 0;
}