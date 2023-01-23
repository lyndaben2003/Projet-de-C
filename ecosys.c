#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  Animal * na = creer_animal(x,y,energie);
  *liste_animal= ajouter_en_tete_animal(*liste_animal, na);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
  Animal *precedent= NULL;  //element precedent
  Animal *pdp= *liste; //pointeur de parcours
  while(pdp!=NULL){
    if(pdp==animal){ //s'il trouve l'animal il s'arrete
      if(precedent==NULL){ 
        *liste=pdp->suivant;
        free(pdp);
        pdp=*liste;
        break;
      }else{
        precedent->suivant= pdp->suivant;
        free(pdp);
        pdp=precedent->suivant;
        break;
      }
    }else{
      precedent=pdp;
      pdp=pdp->suivant;
    }
  }
  
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
  Animal *tmp=NULL;
  while(liste){
    tmp=liste;
    liste=liste->suivant;
    free(tmp);
  }
  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie){
  FILE *f= fopen(nom_fichier, "w");
  assert(f);

  //ecriture proies
  fprintf(f, "<proies>\n");
  Animal *tmp=liste_proie;
  while(tmp){
    fprintf(f,"x=%d, y=%d, dir[%d %d], e=%6f\n", tmp->x, tmp->y, tmp->dir[0], tmp->dir[1], tmp->energie);
    tmp = tmp->suivant;
  }
  fprintf(f,"</proies>\n");

  //ecriture predateurs
  fprintf(f,"<predateurs>\n");
  tmp=liste_predateur;

  while(tmp){
    fprintf(f,"x=%d, y=%d, dir[%d %d], e=%6f\n", tmp->x, tmp->y, tmp->dir[0], tmp->dir[1], tmp->energie);
    tmp = tmp->suivant;
  }

  fprintf(f,"</predateurs>\n");

  fclose(f);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie){
  FILE *f= fopen(nom_fichier, "r");
  assert(f); //permet de verifier si le fichier existe

  
  char buffer[256];
  fgets(buffer, 256, f); //on lit "<proies>\n"
  assert(strncmp(buffer, "<proies>",8)==0);

  //proies
  int x_lu,y_lu, dir_lu[2];
  float e_lu;
  fgets(buffer,256, f); //1ere proie
  while((strncmp(buffer, "</proies>",9)!=0)){
    int n = sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f\n", &x_lu, &y_lu, &dir_lu[0], &dir_lu[1], &e_lu);
    assert(n==5); 

    Animal *a_lu = creer_animal(x_lu,y_lu,e_lu);
    a_lu->dir[0] = dir_lu[0];
    a_lu->dir[1] = dir_lu[1];
    *liste_proie = ajouter_en_tete_animal(*liste_proie, a_lu);

    fgets(buffer, 256,f);
  }
  //predateurs
  fgets(buffer, 256, f); //on lit "<proies>\n"
  assert(strncmp(buffer, "<predateurs>",12)==0);

  fgets(buffer,256, f); //1er predateur
  while((strncmp(buffer, "</predateurs>",13)!=0)){
    int n = sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f\n", &x_lu, &y_lu, &dir_lu[0], &dir_lu[1], &e_lu);
    assert(n==5); 

    Animal *a_lu = creer_animal(x_lu,y_lu,e_lu);
    a_lu->dir[0] = dir_lu[0];
    a_lu->dir[1] = dir_lu[1];
    *liste_predateur = ajouter_en_tete_animal(*liste_predateur, a_lu);

    fgets(buffer, 256,f);

  }
  fclose(f);
}



/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    while(la){
      if((float)rand()/RAND_MAX <= p_ch_dir){
        la->dir[0] = (rand()%3)-1;
        la->dir[1] = (rand()%3)-1;

        la->x = (la->x + SIZE_X + la->dir[0])%SIZE_X;
        la->y = (la->y + SIZE_Y + la->dir[1])%SIZE_Y;
      }
      la = la->suivant;
    }

}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
   Animal *a = liste_animal? *liste_animal: NULL;
   while(a){
    if((float)rand()/RAND_MAX <= p_reproduce){
      ajouter_animal(a->x, a->y, a->energie/2, liste_animal);
      a->energie /=2;
    }
    a = a->suivant;
   }
}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
  bouger_animaux(*liste_proie);
  Animal *tmp= *liste_proie;
  while(tmp){
    tmp->energie= tmp->energie - 1.0; 
    if(monde[tmp->x][tmp->y]>0){ //herbe
      tmp->energie= tmp->energie + monde[tmp->x][tmp->y];
      monde[tmp->x][tmp->y]= temps_repousse_herbe;
    }
    if(tmp->energie < 0){ // si il est sans energie -> mort
      Animal *temp = tmp;
      tmp =tmp->suivant;
      enlever_animal(liste_proie, temp);
    }else{ //sinon suivant
      tmp = tmp->suivant;
    }
  }
  reproduce(liste_proie, p_reproduce_proie);
}



/* Part 2. Exercice 6, question 1 */
//verifiation s'il y'a un animal sur un place
Animal *animal_en_XY(Animal *l, int x, int y) { 
  while(l){
    if((l->x == x)&&(l->y == y)){ 
      return l;
    }
    l = l->suivant;
  }
  return NULL;
} 

/* Part 2. Exercice 6, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
  bouger_animaux(*liste_predateur);
  Animal * pdp = *liste_predateur; //pointeur de parcours
  Animal * tmp = NULL; //pointeur temporaire
    while(pdp){
      pdp->energie =  pdp->energie - 1;
      tmp = animal_en_XY((*liste_proie),pdp->x, pdp->y); //il existe animal?
      if(tmp){ //si oui
        pdp->energie = pdp->energie + tmp->energie; 
        enlever_animal(liste_proie, tmp); //la proie est mangée
      }
      if((tmp==NULL)&&(pdp->energie < 0)){ //sinon et predateur n'a pas d'energie 
        Animal *tmp1 = pdp; //temporaire1
        pdp = pdp->suivant;
        enlever_animal(liste_predateur, tmp1); //predateur mort
      }else{ //sinon
        pdp = pdp->suivant; 
      }
    }
    reproduce(liste_predateur, p_reproduce_predateur);
}

/* Part 2. Exercice 7, question 1 */
//initialise l'herbe a 1
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
  for(int i = 0; i<SIZE_X; i++){
    for(int j = 0; j<SIZE_Y; j++){
      monde[i][j] ++;
    }
  }
}

