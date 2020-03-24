#include <stdio.h>
#include <string.h>


void znajdzPodciag(char ciag[], char ciag2[]) {
	int c[strlen(ciag)+1][strlen(ciag2)+1];
	char b[strlen(ciag)+1][strlen(ciag2)+1]; 							//tworzenie tablic o dlugosci ciagu + 1
	for(int i = 0; i <= strlen(ciag); i++) 
		c[i][0] = 0;
	for(int j = 1; j <= strlen(ciag2); j++) 							//zerowanie pierwszych rzędów tablic
		c[0][j] = 0;
	for(int i = 1; i <= strlen(ciag); i++) 
		for(int j = 1; j <= strlen(ciag2); j++) {
			if(ciag[i-1] == ciag2[j-1]) { 								//jesli litery w ciagach sa sobie rowne zapisujemy "\" w b i dodajemy skos+1
				c[i][j] = c[i-1][j-1] + 1;
				b[i][j] = '\\';
			}
			else if(c[i-1][j] >= c[i][j-1]) {							// jesli dol jest wiekszy lub rowny gorze to wstawiamy "|" i przepisujemy liczbe z lewej
				c[i][j] = c[i-1][j];
				b[i][j] = '|';
			}
			else {
			c[i][j] = c[i][j-1];										// w innym wypadku przepisujemy liczbe z gory i wstawiamy -
			b[i][j] = '-';
			}
		}	
	
	int poz1 = strlen(ciag);											//ustawiam pozycje na ostatni element w tablicy
	int poz2 = strlen(ciag2);
	char wynik[30];
	while(c[poz1][poz2] != 0) {
		if(b[poz1][poz2] == '\\') {										// gdy w b jest "\" drukujemy oraz przeskakujemy po skosie
			wynik[c[poz1][poz2]-1] = ciag[poz1-1]; 
			poz1--;
			poz2--;
		}
		else if(b[poz1][poz2] == '-')
			poz2--;														//gdy w b jest "-" idziemy w lewo
		else
			poz1--;														//gdy w b jest cokolwiek innego idziemy w górę
	}
	printf("%s", wynik);
}


int main() {
	
	char ciag[30] = "abrakadabra";											//przykładowe ciągi, można odkomentować i samemu wprowadzić
	char ciag2[30] = "barakuda";
	//printf("Wprowadz pierwszy ciag");
	//scanf("%s", &ciag);
	//printf("Wprowadz drugi ciag");
	//scanf("%s", &ciag2);
	znajdzPodciag(ciag, ciag2);
	return 0;
}
