#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000


float p_ch_dir=0.01;
float p_reproduce_proie=0.4;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;


int main(void) {
  srand(time(NULL));
  //A completer. Part 2:
  //exercice 4
  
  Animal *a = creer_animal(1, 2, 10.); //variable de test
  a->dir[0] = 1;
  a->dir[1] = 1;
  a->suivant = NULL;
  printf("Deplacement\n");
  for(int i = 0; i<10; i++){
    bouger_animaux(&*a);
    printf("dir : (%d,%d)\n", a->dir[0], a->dir[1]);
    printf("X : %d, Y : %d\n", a->x, a->y);
  }

  printf("\n");

  Animal *ani = creer_animal(0, 0, 10.);
  printf("Reproduce\n");
  for(int j = 0; j<5; j++){
    reproduce(&ani, 1.);
    printf("Nb d'animaux : %d\n", compte_animal_it(ani));
  }

  a = liberer_liste_animaux(a);
  ani = liberer_liste_animaux(ani);

  //exo 5
  printf("\n");

  int energie=10;
  //creer 20 proies
  Animal *liste_proie = NULL;
  for(int i = 0; i<NB_PROIES; i++){
    ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,energie,&liste_proie);
  }

  //creer 20 predateurs
  Animal *liste_predateur = NULL;
  for(int i = 0; i<NB_PROIES; i++){
    ajouter_animal(rand()%SIZE_X,rand()%SIZE_Y,energie,&liste_predateur);
  }

  //ecosys avec les proies et predateurs crees
  printf("Ecosys avec les deux listes :\n");
  afficher_ecosys(liste_proie, liste_predateur);
  printf("\n");


  int nb=0;

  //exo7.1
  int monde[SIZE_X][SIZE_Y];
   for(int i = 0; i<SIZE_X; i++){
    for(int j = 0; j<SIZE_Y; j++){
      monde[i][j] = 0;
    }
  }

  //exo 5.2, 6.3
  printf("Boucle while ici :\n");

  while((nb<200)&&((liste_proie)||(liste_predateur))){
    rafraichir_proies(&liste_proie, monde);
    rafraichir_predateurs(&liste_predateur, &liste_proie);
    rafraichir_monde(monde);
    printf("\nEcosys nb: %d\n", nb);
    afficher_ecosys(liste_proie,liste_predateur);
    printf("\n");
    usleep(T_WAIT);
    nb++;
  }
  
  liberer_liste_animaux(liste_predateur);
  liberer_liste_animaux(liste_proie);

  //verification de liberer
  printf("Verification :\n");
  afficher_ecosys(liste_proie, liste_predateur);

  return 0;
}

