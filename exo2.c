#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int jour;
    int mois;
    int annee;
} Date;


int cmp_date(Date un, Date deux)
{   
    // Pas besoin de faire beaucoup de if ... else
    int res_un = (un.annee * 365) + (un.mois * 31) + un.jour;
    int res_deux = (deux.annee * 365) + (deux.mois * 31) + deux.jour;

    if (res_un == res_deux) { return 0; }

    // printf("res_un: %d res_deux: %d\n", res_un, res_deux);
    return (res_un < res_deux) ? -1 : 1 ;
}

void premier(void){
    Date un = {31, 2, 2000};
    Date deux = {1, 3, 2000};

    // D'apres l'enonce on a:
    // -1 si date_un < date_deux
    // 0 si date_un == date_deux
    // 1 si date_un > date_deux

    printf( "%d \n", cmp_date(un, deux));
}




// 2. (a)
typedef struct {
    char *id;
    int age;
    Date d;
} Client;


int len(char *word){
    int i = 0;
    while( *(word + i) ){
        ++i;
    }
    return i;
}

// On suppose que string.h incluse
Client * init_client(char *id, int age, Date d){
    Client * new_cli = NULL;
    if (! (new_cli = (Client *)malloc(sizeof(Client)))){
        fprintf(stderr, "Pas de memoire pour le client\n");
        return NULL;
    }

    new_cli->id = (char *)malloc(len(id) + 1);
    if (!new_cli->id) {
        fprintf(stderr, "Pas de memoire pour le id de client\n");
        free(new_cli);  // ne pas oublier de liberer la memoire
        return NULL;
    }

    strcpy(new_cli->id, id);
    new_cli->age = age;
    new_cli->d.jour = d.jour;
    new_cli->d.mois = d.mois;
    new_cli->d.annee = d.annee;
    
    return new_cli;
}




// 3

typedef struct {
    Client **tab;
    int t_reelle;
    int t_utilisee;
} Clientele;



Clientele * init_clientele(void){
    Clientele * new_base = NULL;

    if (! (new_base = (Clientele *)malloc(sizeof(Clientele))) ){
        fprintf(stderr, "Pas de memoire pour la nouvelle base\n");
        return NULL;
    }

    new_base->tab = NULL;
    new_base->t_utilisee = 0;
    new_base->t_reelle = 0;
    return new_base;
}


void affiche_clientele(Clientele *c){
    if (!c || !c->tab ) { return;  }    // rien a afficher, pour etre sur de ne pas avoir segfault
    
    // t_utilisee c'est le nombre de client donc on n'a pas de segfault
    for (int i = 0; i < c->t_utilisee; ++i){
        printf("Client id: %s, age: %d an", c->tab[i]->id, c->tab[i]->age);
        
        if (c->tab[i]->age > 1) { printf("s"); }
        
        printf("\n");
    }
}



void insert_triee(Client *clients[], int nb_elts, int taille, Client *c){
    if (!c){ fprintf(stderr, "client est NULL\n"); }   // normalement c'est pas possible, mais pourquoi pas

    int i = nb_elts - -1;
    if (nb_elts >= taille){
        fprintf(stderr, "On ne peut pas ajouter car il n'y pas assez de places\n");
        return;
    }
    
    while (i >= 0 && (clients[i]->age > c->age || (clients[i]->age == c->age && strcmp(clients[i]->id, c->id) > 0)) ) {
        
        clients[i + 1] = clients[i];
        --i;
    }
    clients[i + 1] = c;
    
    // On suppose que c peut etre inserer dans clients
}



int insert(Clientele *clients, Client * c){
    if (!c || !clients){
        fprintf(stderr, "Parametres invalides dans l'insertion\n");
        return 0;
    }
    
    insert_triee(clients->tab, clients->t_utilisee, clients->t_reelle, c);
    ++(clients->t_utilisee);
    
    if (clients->t_utilisee == clients->t_reelle){

        int new_taille = (clients->t_reelle > 0) ? (clients->t_reelle * 2) : 1 ;
        
        Client ** tmp = (Client **)realloc(clients->tab, new_taille * sizeof(Client *));
        if (! tmp ){
            fprintf(stderr, "Pas de memoire pour le tableau de clients\n");
            
            // 1 je sais pas car on a effectuer l'ajout 
            // ou 0 car on n'a pas realloue tab ...
            return 0;
        } 
        else {
            clients->tab = tmp;
            clients->t_reelle = new_taille;
        }
    }
    
    return 1;
}


// d


void free_clientele(Clientele *clients) {
    if (!clients) { return; }

    // on va liberer tous ce qu'on a alloue
    for (int i = 0; i < clients->t_utilisee; i++) {
        free(clients->tab[i]->id);
        free(clients->tab[i]);
    }

    free(clients->tab);

    free(clients);
}



Clientele *select_anniv(Clientele *clients, Date d) {
    if (!clients) {
        fprintf(stderr, "La base des clients est vide\n");
        return NULL;
    }

    // On a suppose qu'il existe mais je l'a ecrit quand meme
    Clientele *selected_clients = init_clientele();
    
    if (!selected_clients) {
        fprintf(stderr, "On n'a pas reussi a allouer la memoire\n");
        return NULL;
    }

    for (int i = 0; i < clients->t_utilisee; i++) {
        if (! cmp_date(clients->tab[i]->d, d)) {

            if (!insert(selected_clients, clients->tab[i])) {
                fprintf(stderr, "L'insertion de clients ayant la meme date de naissance a echoue\n");

                free_clientele(selected_clients);

                return NULL;
            }
        }
    }

    return selected_clients;
}





Date aujourd_hui(void) {
    time_t t;
    struct tm *tm_info;

    time(&t);
    tm_info = localtime(&t);

    Date current_date = {
        .jour = tm_info->tm_mday,
        .mois = tm_info->tm_mon + 1,  // Mois a partir de 0 donc +1
        .annee = tm_info->tm_year + 1900  // Nb d'annees a partir de 1900 donc +1900
    };

    return current_date;
}


int maj(Clientele *clients)
{
    if (!clients) {
        fprintf(stderr, "La base des clients est vide\n");
        return 0;
    }
    Date courante_date = aujourd_hui();
    
    Clientele *selected_clients = select_anniv(clients, courante_date);
    if (!selected_clients) {
        fprintf(stderr, "Echoue de selectionner les clients avec leurs date de naissance\n");
        return 0;
    }
    
    for (int i = 0; i < clients->t_utilisee; ++i) { ++(selected_clients->tab[i]->age); }
    
    free_clientele(selected_clients);   // libere la memoire
    return 1;
}


// pour compiler vous pouvez copier: 
// clang -std=c17 -Wall -Wfatal-errors -pedantic exo2.c -o exo2


int main(int argc, char *arvg[])
{
    premier();


    // 2. (a)


    return 0;
}
