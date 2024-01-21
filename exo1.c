#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1024

typedef char s[T];

int mystere(s ss, int *n)
{
    *n = 0;
    while(*ss){
        ss++; (*n)++;
    }
    return (*n) ? 1 : 0;
}


// pour compiler vous pouvez copier: 
// clang -std=c17 -Wall -Wfatal-errors -pedantic exo1.c -o exo1


int main(int argc, char *arvg[])
{
    s UGE = "Universite Gustave Eiffel!";
    s vide_chaine = "";
    int longueur_un = 0;
    int longueur_deux = 0;

    if (mystere(UGE, &longueur_un))
        printf("Longueur: %d\n", longueur_un);
    
    if (mystere(vide_chaine, &longueur_deux))   // car la fonction renvoie 0 car la chaine est vide
        printf("Longueur: %d\n", longueur_deux);
    
    return 0;
}
