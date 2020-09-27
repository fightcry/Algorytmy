#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

typedef struct knot *Ptknot; 
typedef struct knot{
	int key, rank;      //struktura węzła zawierająca  klucz range oraz ojca
	Ptknot p;
} TKnot ; 

typedef struct Edge
{
	int u, v, weight;     //struktura przedstawiająca krawędź z u do v o wadze weight
}Edge;

struct Graph 
{                         //graf zawiera ilosc wierzcholkow oraz krawędzi. Dodatkowo zawiera wskaznik na tablice krawędzi
    int V, E; 
    struct Edge* edge; 
}; 
  
struct Graph* createGraph(int V, int E) //tworzymy graf o zadanej ilosc krawedzi i wierzcholkow. Same krawedzie nalezy potem dodawac ręcznie
{ 
    struct Graph* graph = malloc(sizeof(struct Graph)); 
    graph->V = V; 
    graph->E = E; 
    graph->edge = malloc(E*sizeof(Edge)); 
    return graph; 
} 

Ptknot MakeSet(int k) {
  Ptknot x = (Ptknot) malloc(sizeof(TKnot));
  x->key = k;
  x->p = x;               //tworzymy zbior i go inicjalizujemy
  x->rank = 0;
  return x;
}

Ptknot FindSet(Ptknot x) {
if(x != x->p) {
  Ptknot temp = (Ptknot) malloc(sizeof(TKnot));
  temp = x->p;                                      //znajdujemy korzen zbioru oraz po drodze przepisujemy wezly wyzej (kompresujemy sciezke)
  x->p = temp->p;
  x->p = FindSet(temp->p);
}
return x->p;
}

void Union(Ptknot x, Ptknot y) {
if(x->rank < y->rank)
  x->p = y;
else {                                      //laczymy dwa wezly, wezel o wyzszej randze zostaje korzeniem. zwiekszamy range jednego jesli oba mialy taka same range
  y->p = x;
  if(x->rank == y->rank)
    x->rank++;
}
}

int EdgeComp(const void* a, const void* b)      //funkcja porownujaca dwa rozne krawędzie
{ 
    struct Edge* a1 = (struct Edge*)a; 
    struct Edge* b1 = (struct Edge*)b; 
    return a1->weight > b1->weight; 
} 

void Kruskal(struct Graph* graph) 
{ 
    int V = graph->V;       
    struct Edge result[V];  //Tutaj skladujemy wynikowy graf 
    int e = 0, i = 0; 
    Ptknot a = malloc(sizeof(TKnot));
    Ptknot b = malloc(sizeof(TKnot));
  
    qsort(graph->edge, graph->E, sizeof(graph->edge[0]), EdgeComp); //sortujemy graf

    Ptknot *subsets = malloc(V*sizeof(TKnot)); //miejsce przechowywania podzbiorow
    for (int v = 0; v < V; ++v) 
      subsets[v] = MakeSet(v);   //tworzymy jednoelemntowe zbiory z wiercholkow
    
    while ((e < V - 1) && (i < graph->E)) 
    { 
        struct Edge next = graph->edge[i++]; //bierzemy krawedzie po kolei poniewaz uprzednio je posortowalismy
        for(int i = 0; i < V; i++) {
          if(next.u == subsets[i]->key)
            a = subsets[i];
          if(next.v == subsets[i]->key)   //szukamy w zbiorze podzbiorow odpowiedniego wierzcholka
            b = subsets[i];
        }
        a = FindSet(a); 
        b = FindSet(b); 

        if (a != b) //jesli krawedzie nie powoduja cyklu to przepisujemy oraz zwiekszamy licznik
        { 
            result[e++] = next; 
            Union(FindSet(a), FindSet(b)); 
        } 
    } 
    printf("\n"); 
    for (i = 0; i < e; ++i) 
        printf("%d -- %d == %d\n", result[i].u, result[i].v, result[i].weight); 
    return; 
} 

int main() {
/*Ptknot a = MakeSet(0);
Ptknot b = MakeSet(1);
Ptknot c = MakeSet(2);
Ptknot d = MakeSet(3);
Ptknot e = MakeSet(4);
Ptknot f = MakeSet(5);
Ptknot g = MakeSet(6);
Ptknot h = MakeSet(7);
Ptknot i = MakeSet(8);
Ptknot j = MakeSet(9);

Union(FindSet(a), FindSet(b));
Union(FindSet(c), FindSet(d));
Union(FindSet(b), FindSet(c));
Union(FindSet(f), FindSet(g));
Union(FindSet(d), FindSet(f));
Union(FindSet(a), FindSet(h));*/

/*int V = 4;  
int E = 5; 
struct Graph* graph = createGraph(V, E); 

graph->edge[0].u = 0; 
graph->edge[0].v = 1; 
graph->edge[0].weight = 10; 
  
graph->edge[1].u = 0; 
graph->edge[1].v = 2; 
graph->edge[1].weight = 6; 
  
graph->edge[2].u = 0; 
graph->edge[2].v = 3; 
 graph->edge[2].weight = 5; 
  
graph->edge[3].u = 1; 
graph->edge[3].v = 3; 
graph->edge[3].weight = 15; 
  
graph->edge[4].u = 2; 
graph->edge[4].v = 3; 
graph->edge[4].weight = 4; 
  
Kruskal(graph); */

return 0;
}