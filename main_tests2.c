#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

#define NB_PREDATEURS 20
#define NB_PROIES 20


float p_ch_dir=0.1;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15; 



int main(void){

    //PARTIE 1
    printf("\n");
    printf("PARTIE 1\n");

    Animal *liste_predateur = NULL;
    Animal *liste_proie = NULL;
    int energie=10;
    srand(time(NULL));

    for(int i = 0; i<NB_PROIES; i++){
        ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,energie,&liste_proie);
    }

    for(int i = 0; i<NB_PREDATEURS; i++){
        ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,energie,&liste_predateur);
    }

    assert(compte_animal_it(liste_predateur)==NB_PREDATEURS);
    assert(compte_animal_it(liste_proie)==NB_PROIES);

    afficher_ecosys(liste_proie, liste_predateur);

    liste_predateur = liberer_liste_animaux(liste_predateur);
    liste_proie = liberer_liste_animaux(liste_proie);
    
    assert(liste_predateur == NULL);
    assert(liste_proie == NULL);

    printf("\n");
    printf("PARTIE 2\n");

    
    lire_ecosys("lecture.txt", &liste_predateur, &liste_proie);
    afficher_ecosys(liste_proie, liste_predateur);
    ecrire_ecosys("ecriture.txt", liste_predateur, liste_proie);
   

    liste_predateur = liberer_liste_animaux(liste_predateur);
    liste_proie = liberer_liste_animaux(liste_proie);
    
    //verification de liberer
    afficher_ecosys(liste_proie, liste_predateur);


}