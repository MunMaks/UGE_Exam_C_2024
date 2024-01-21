#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int Cle;    // par exemple des entiers
typedef int Valeur;


typedef struct _maillon{
    Cle cle;
    Valeur valeur;
    struct _maillon *prec, *suiv;
} Maillon, *Liste;


#define TAILLE 21
typedef Liste Table[TAILLE];



int hachage(Cle cle)
{   // Vous avez droit d'utiliser n'importe quelle fonction d'hachage
    return cle % TAILLE;
}



// (a)
Maillon* alloue_maillon(Cle c, Valeur v) {
    Maillon* nouveau_maillon = (Maillon*)malloc(sizeof(Maillon));
    if (nouveau_maillon == NULL) {
        perror("Probleme de memoire, malloc n'a pas reussi\n");
        return NULL;
    }

    nouveau_maillon->cle = c;
    nouveau_maillon->valeur = v;
    nouveau_maillon->prec = nouveau_maillon->suiv = NULL;

    return nouveau_maillon;
}



// (b)
// on suppose que cette fonction existe (d'apres l'enonce)
char* cle2string(Cle c)
{
    return NULL;
}


// on suppose que cette fonction existe (d'apres l'enonce)
char* valeur2string(Valeur v)
{
    return NULL;
}



// affichage d'une liste (cle-valeur), et LISTE VIDE si necessaire
void affiche(Liste l)
{
    if ( !l ) {
        printf("[LISTE VIDE]\n");
        return;
    }
    printf("<-");
    while (l) {
        char* cleStr = cle2string(l->cle);
        char* valeurStr = valeur2string(l->valeur);

        printf(" %s : valeur : %s ", cleStr, valeurStr);

        if (l->suiv) { printf("<->"); }

        // free(cleStr);        // Liberer la memoire si on a alloue
        // free(valeurStr);
        l = l->suiv;
    }
    printf("->\n");
}



// (c)
// on suppose que cette fonction existe (d'apres l'enonce)
int cmp_cle(Cle un, Cle deux)
{   
    // cette une implementation basique
    // si vous voulez vous pouvez modifer
    return (un > deux) ? 1 : 0;
}



Maillon *trouve(Liste l, Cle c)
{

    while (l){
        if (cmp_cle(l->cle, c)){
            return l;
        }
        l = l->suiv;
    }
    return NULL;
}



// (d)
int insere_en_tete(Liste* liste, Maillon *new_maillon)
{
    if (!new_maillon){
        fprintf(stderr, "Maillon a ajouter en tete est NULL\n");
        return 0;
    }

    if ( !(*liste) ){
        *liste = new_maillon;
        return 1;
    }

    (*liste)->prec = new_maillon;
    new_maillon->suiv = *liste;
    new_maillon->prec = NULL;
    *liste = new_maillon;
    return 1;
}



// (e)
Maillon *suppr(Liste *liste, Cle c)
{
    if (!(*liste)){
        fprintf(stderr, "liste est vide, rien a supprimer\n");
        return NULL;
    }
    // trouve() renvoie Maillon * si elle trouve par cle, sinon renvoie NULL
    Maillon *del_maillon = trouve(*liste, c);
    
    // si suivant existe
    if (del_maillon->suiv) { del_maillon->suiv->prec = del_maillon->prec; }

    // si precedente existe
    if (del_maillon->prec) { del_maillon->prec->suiv = del_maillon->suiv; }

    return del_maillon; 
}



// (f)
void liberer_liste(Liste *liste)
{
    while (*liste){
        Maillon *del_maillon = *liste;
        *liste = (*liste)->suiv;
        free(del_maillon);
    }
}



/***********************************************/
/******************* PARTIE 2 ******************/
/***********************************************/



// (a)
void initialise_table(Table T)
{
    for (int i = 0; i < TAILLE; ++i) {
        T[i] = NULL;
    }
}


// (b)
void affiche_table(Table T)
{
    for (int i = 0; i < TAILLE; ++i) {
        printf("Table[%d]: ", i);

        // Affiche la liste chaînée associée à l'index i
        if (T[i] == NULL) {
            printf("[LISTE VIDE]\n");
        } else {
            Liste courant = T[i];
            affiche(courant);
        }
    }
}


// (c)
Maillon *recherche(Table T, Cle c)
{
    for (int i = 0; i < TAILLE; ++i){
        Liste courant = T[i];
        Maillon *maillon = trouve(courant, c);

        // si maillon n'est pas NULL donc on l'a trouve
        if (maillon) { return maillon; }
    }
    return NULL;
}


// (d)
int ajoute(Table *T, Cle c, Valeur v)
{
    // on peut avoir tout simplement "Table T" donc au return on aura
    // insere_en_tete(&T[idx], new_maillon);
    if (!T) {
        fprintf(stderr, "Table n'existe pas\n");
        return 0;
    }
    Maillon *new_maillon = alloue_maillon(c, v);

    if (!new_maillon) { // Pas assez de memoire
        return 0;
    }

    int idx = hachage(c);

    insere_en_tete(&(*T)[idx], new_maillon);

    return 1;
}


// (e)
Maillon *supprime(Table *T, Cle c)
{
    if (!T) { return NULL; } // rien a supprimer

    Maillon *del_maillon = recherche(*T, c);

    if (!del_maillon) {
        fprintf(stderr, "Aucun maillon trouve avec la cle demandee\n");
        return NULL;
    }

    // si suivant existe
    if (del_maillon->suiv) { del_maillon->suiv->prec = del_maillon->prec; }

    // si precedente existe
    if (del_maillon->prec) { del_maillon->prec->suiv = del_maillon->suiv; }

    return del_maillon; 
}


// (f)
void liberer_table(Table *T)
{
    if (!T || !(*T)) {
        fprintf(stderr, "Table n'existe pas\n");
        return;
    }

    for (int i = 0; i < TAILLE; ++i){
        Liste curr = (*T)[i];
        liberer_liste(&curr);
    }
}



// clang -std=c17 -Wall -Wfatal-errors -pedantic exo3.c -o exo3
int main(int argc, char *arvg[])
{   
    return 0;
}
