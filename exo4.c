#include <stdio.h>
#include <stdlib.h>

// On peut ajouter la condition de
// typedef ... typevalue mais ce n'est pas obligatoire

typedef struct _maillon{
    int value;
    struct _maillon *suiv;  // l'adresse du lien suivant
    struct _maillon **prec; // l'adresse de l'adresse du lien precedente
} Maillon, *Liste;



Liste alloueMaillon(int v)
{
    Liste new_maillon = (Liste)malloc(sizeof(Maillon));
    if (!new_maillon) {
        fprintf(stderr, "Pas de memoire pour nouvelle maillon...\n");
        return NULL;
    }
    new_maillon->value = v;
    new_maillon->suiv = NULL;
    new_maillon->prec = NULL;

    return new_maillon;
}



void move(Liste *cible, Liste *source)
{
    if (!(*source)) { return; } // rien a faire, car source est vude

    Maillon *curr = *source;

    if (!(*source)->suiv) { *source = NULL; } // un seul maillon

    if (!(*cible)) {    // cible est vide
        *cible = curr;

        // source a au moins deux maillons
        if (*source && (*source)->suiv) {
            *source = (*source)->suiv;
            
            curr->suiv = NULL;

            (*source)->prec = NULL;
        }
        return; // on a finit donc on sort
    }

    // desormais cible n'est pas vide

    if ( (*source) && (*source)->suiv){     // si source est NULL donc on fait rien
        (*source) = (*source)->suiv;
        (*source)->prec = NULL;
    }

    curr->suiv = *cible;    // lien entre nouveau maillon et la tete de cible
    
    (*cible)->prec = &curr; // car prec c'est l'adresse de l'adresse de maillon precedent
    
    *cible = curr;          // la nouvelle tete de cible
}



void affiche(Liste un, Liste deux)
{
    printf("liste un: ");
    if (!un) { printf("LISTE EST VIDE\n"); }

    while (un){
        printf((!un->suiv) ? ("%d") : "%d -> ", un->value);
        un = un->suiv;
    }

    printf("\nliste deux: ");
    if (!deux) { printf("LISTE EST VIDE\n"); }

    while (deux){
        printf((!deux->suiv) ? ("%d") : "%d -> ", deux->value);
        deux = deux->suiv;
    }
    printf("\n");
}


void libereMaillons(Liste lst)
{
    if (!lst){ return; }    // rien a liberer

    Maillon * del_node = NULL;
    while (lst){
        del_node = lst;
        lst = lst->suiv;
        free(del_node);
    }
}

// pour compiler vous pouvez copier: 
// clang -std=c17 -Wall -Wfatal-errors -pedantic exo4.c -o exo4

int main(int argc, char *argv[])
{
    int first = 1, second = 2;
    Liste un = alloueMaillon(first);        // cible
    Liste deux = alloueMaillon(second);


    int third = 3, fouth = 4, five = 5;
    Liste trois = alloueMaillon(third);     // source
    Liste quatre = alloueMaillon(fouth);
    Liste cinq = alloueMaillon(five);

    if (! un || !deux || !trois || !quatre || !cinq){
        fprintf(stderr, "Allocation n'est pas reussite \n");
        libereMaillons(un);
        libereMaillons(trois);
        return 1;
    }
    
    un->suiv = deux;
    deux->prec = &un;
    
    trois->suiv = quatre;
    quatre->prec = &trois;

    quatre->suiv = cinq;
    cinq->prec = &quatre;

    printf("\nAVANT: \n");
    affiche(un, trois);
    

    // a modifier si vous voulez
    
    move(&un, &trois);

    move(&un, &trois);

    move(&un, &trois);


    printf("\nAPRES: \n");
    affiche(un, trois);

    libereMaillons(un);
    libereMaillons(trois);

    return 0;
}
