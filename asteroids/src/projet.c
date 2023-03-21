#include <libTableauNoir.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//structures//
typedef struct {
  double x;
  double y;
  double vx;
  double vy;
  double diametre;
  double angle;
  double rota;
  int existe;
  Image aster;
  int frag;
  int ini;
  int fois;
} aste_t;

typedef struct {
  double x;
  double y;
  double vx;
  double vy;
  double angle;
} vaisseau_t;

typedef struct{
  double a;
  double b;
  double c;
  double d;
  double e;
  double f;
} forme_t;

typedef struct {
  double x;
  double y;
  double vx;
  double vy;
  double angle;
  int diametre;
  int existe;
} proj_t;
  
typedef struct {
  vaisseau_t vaisseau;
  aste_t aste[100];
  proj_t proj;
  int nb_aste;
  Image i[10];
  int collision;
  int perdu;
  int u;
  int d;
  int c;
  int fin;
  double dt;
} donnees_t;

//fonctions//
void start();
void perdu(donnees_t);
donnees_t creer_score(donnees_t);
void score_i();
vaisseau_t creer_vaisseau();
aste_t creer_aste();
aste_t creer_frag(donnees_t*,int);
proj_t creer_proj(donnees_t);
donnees_t initialiser_donnees();
int fini(donnees_t);
void effacer();
void monTriangle(double, double,double , double, double, double);
forme_t forme(donnees_t);
void afficher(donnees_t);
donnees_t aste(donnees_t *);
donnees_t vaisseau(donnees_t *);
donnees_t collision_pa(donnees_t *);
donnees_t collision_va(donnees_t *);
donnees_t commandes(donnees_t*);
void maj(donnees_t *);


int main()
{ creerTableau();
  start();
  fixerModeBufferisation(1);
  choisirCouleurFond(0,0,50);
  fixerTaille(800,600);
  donnees_t etat = initialiser_donnees();
  etat = initialiser_donnees();
  while(!fini(etat)){
    if(etat.perdu == 0){
      score_i();
      maj(&etat);
      afficher(etat);
      tamponner();
      effacer();
    }
  }
  return EXIT_SUCCESS;
}

void start(){
  Image start = chargerImage("start.jpeg");
  afficherImage(start,0-tn_largeur(start)/2,tn_hauteur(start)/2);
  attendreClicGauche();
  libererImage(start);
 }

void fond(){
  Image fond = chargerImage("fond.jpg");
  afficherImage(fond,0-tn_largeur(fond)/2,-75+tn_hauteur(fond)/2);
  libererImage(fond);
}

void perdu(donnees_t etat){
  Image perdu = chargerImage("perdu.jpeg");
  Image score = chargerImage("score.jpeg");
  choisirCouleurFond(0,0,0);
  afficherImage(perdu,0-tn_largeur(perdu)/2,-75+tn_hauteur(perdu)/2);
  afficherImage(score,-100,250);
  afficherImage(etat.i[etat.c],0,250);
  afficherImage(etat.i[etat.d],25,250);
  afficherImage(etat.i[etat.u],50,250);
}
  
donnees_t creer_score(donnees_t etat){
  etat.i[0] = chargerImage("0.jpg");
  etat.i[1] = chargerImage("1.jpg");
  etat.i[2] = chargerImage("2.jpg");
  etat.i[3] = chargerImage("3.jpg");
  etat.i[4] = chargerImage("4.jpg");
  etat.i[5] = chargerImage("5.jpg");
  etat.i[6] = chargerImage("6.jpg");
  etat.i[7] = chargerImage("7.jpg");
  etat.i[8] = chargerImage("8.jpg");
  etat.i[9] = chargerImage("9.jpg");
  return etat;
}

void score_i(){
  Image score_i = chargerImage("0.jpg");
  for(int i = 1 ; i < 5 ; i++){
    afficherImage(score_i,-390+25*i,290);
  }
}

vaisseau_t creer_vaisseau(){
  vaisseau_t v = { .x = 0,
		   .y = 0,
		   .vx = 0,
		   .vy = 0,
		   .angle = 0,		 
  };
  return v;
}

aste_t creer_aste(){
  aste_t a = { .x = rand() % 800,
	       .y = -300 +600*rand()/RAND_MAX,
	       .angle = rand() %360,
	       .diametre = 80,
	       .existe =1,
	       .ini =1,
    };
    return a;
}

aste_t creer_frag(donnees_t* etat,int i){
  aste_t b = {
	       .x = etat->aste[i].x,
	       .y = etat->aste[i].y,
	       .angle = rand() %360,
	       .diametre = etat->aste[i].diametre*2/3,
	       .existe =1,
	       .frag = 1,
	       .fois = etat->aste[i].fois +1,
  };
   return b;
}

proj_t creer_proj(donnees_t etat)
{ proj_t proj = {.existe = 1,
		 .x=etat.vaisseau.x + 15*sin(etat.vaisseau.angle*M_PI/180),
		 .y=etat.vaisseau.y - 15*cos(etat.vaisseau.angle*M_PI/180),
		 .vx=sin(etat.vaisseau.angle*M_PI/180)*500,
		 .vy=-cos(etat.vaisseau.angle*M_PI/180)*500,
		 .diametre = 5,
	        };
  return proj;
}

//initialise les données//
donnees_t initialiser_donnees()
{
  donnees_t etat ;
  etat.fin = 0;
  etat.vaisseau = creer_vaisseau();
  for (int i=0; i<(etat.nb_aste=8); i++) {
    etat.aste[i] = creer_aste();
    etat.aste[i].fois =0;
  }
  etat.proj.existe =0;
  etat.collision =0;
  etat.perdu =0;
  etat.u =0;
  etat.d =0;
  etat.c =0;
  return etat;
}

int fini(donnees_t etat){
  EtatSourisClavier esc = lireEtatSourisClavier();
  if(esc.sourisBoutonGauche==1||fermetureDemandee()) {
    etat.fin = 1;  
  }
  return etat.fin;
}

void effacer(){
  effacerTableau();
}

void monTriangle(double xa,double xb,double xc,double ya,double yb,double yc){
  tracerSegment(xa,ya,xc,yc);
  tracerSegment(xb,yb,xc,yc);
  tracerSegment(xa,ya,xb,yb);
}

//forme du vaisseau//
forme_t forme(donnees_t etat){
  forme_t forme = {
    .a = etat.vaisseau.x + 7.5*sin((etat.vaisseau.angle-120)*M_PI/180),
    .b = etat.vaisseau.x + 7.5*sin((etat.vaisseau.angle+120)*M_PI/180),
    .c = etat.vaisseau.x + 15*sin(etat.vaisseau.angle*M_PI/180),
    .d = etat.vaisseau.y - 7.5*cos((etat.vaisseau.angle-120)*M_PI/180),
    .e = etat.vaisseau.y - 7.5*cos((etat.vaisseau.angle+120)*M_PI/180),
    .f = etat.vaisseau.y - 15*cos(etat.vaisseau.angle*M_PI/180),
  };
  return forme;
}

//affiche les données//
void afficher(donnees_t etat){
  if(etat.perdu==0){
    for(int i =0; i < etat.nb_aste ; i++){
       etat.aste[i].aster = chargerImage("aste.png");    
       Image ini  = rotozoomImage(etat.aste[i].aster,
				  etat.aste[i].rota,1,1);
       if(etat.aste[i].existe==1){
	 if(etat.aste[i].ini == 1){
	   afficherImage(ini,
			 etat.aste[i].x-tn_largeur(etat.aste[i].aster)/2,
			 etat.aste[i].y+tn_hauteur(etat.aste[i].aster)/2);
	 }
	 if(etat.aste[i].frag ==1){
	   Image frag = rotozoomImage(etat.aste[i].aster,
				      etat.aste[i].rota,
				      pow(0.66,etat.aste[i].fois),
				      pow(0.66,etat.aste[i].fois));
	   afficherImage(frag,
			 (etat.aste[i].x-tn_largeur(etat.aste[i].aster)/2)
			 +14*etat.aste[i].fois,
			 (etat.aste[i].y+tn_hauteur(etat.aste[i].aster)/2)
			 -14*etat.aste[i].fois);
	 }
       }
       libererImage(etat.aste[i].aster);
    }
    
    choisirTypeStylo(10,255,0,0);
    monTriangle(forme(etat).a,forme(etat).b,forme(etat).c,
		forme(etat).d,forme(etat).e,forme(etat).f);
    
    if(etat.proj.existe ==1){
      choisirTypeStylo(5,255,255,255);
      tracerPoint(etat.proj.x,etat.proj.y);
    }
  }
}

//déplacement astéroides//
donnees_t aste(donnees_t * etat){
 for(int i = 0; i < etat->nb_aste ; i++){
    etat->aste[i].vx = sin(etat->aste[i].angle*M_PI/180)
      *3500/etat->aste[i].diametre;
    etat->aste[i].vy =-cos(etat->aste[i].angle*M_PI/180)
      *3500/etat->aste[i].diametre;
      
    etat->aste[i].x += etat->dt * etat->aste[i].vx ;
    etat->aste[i].y += etat->dt * etat->aste[i].vy ;
    
    if(etat->aste[i].y < -300)
      etat->aste[i].y = 300;    
    if(etat->aste[i].y> 300)
      etat->aste[i].y = -300;   
    if(etat->aste[i].x < -400)
      etat->aste[i].x =400;    
    if(etat->aste[i].x > 400)
       etat->aste[i].x = -400;    
    if(etat->aste[i].diametre < 20)
      etat->aste[i].diametre = 20;

    etat->aste[i].rota += 0.01;
 } 
    
    if(etat->nb_aste < 10){
      etat->aste[etat->nb_aste] = creer_aste();
      etat->nb_aste ++;
      etat->aste[etat->nb_aste] = creer_aste();
      etat->nb_aste ++;  
 }
 return *etat;
}

//déplacement vaisseau//
donnees_t vaisseau(donnees_t * etat){
   etat->vaisseau.vx -= etat->vaisseau.vx*0.95*etat->dt;
   etat->vaisseau.vy -= etat->vaisseau.vy*0.95*etat->dt;
   etat->vaisseau.x += etat->vaisseau.vx*etat->dt;
   etat->vaisseau.y += etat->vaisseau.vy*etat->dt;
   if(etat->vaisseau.x < -400){
    etat->vaisseau.x = 400;
  }
   if(etat->vaisseau.x > 400){
     etat->vaisseau.x = -400;
   }
   if(etat->vaisseau.y < -300){
    etat->vaisseau.y = 300;
   }
   if(etat->vaisseau.y > 300){
     etat->vaisseau.y = -300;
   }
   return *etat;
}

//collision projectile/astéroide//
  donnees_t collision_pa(donnees_t *etat){
    for(int i =0; i< etat->nb_aste;i++){
    double distance = sqrt(pow(etat->aste[i].x-etat->proj.x,2)+
			   pow(etat->aste[i].y-etat->proj.y,2));
    
    if(distance <(etat->aste[i].diametre+etat->proj.diametre)/2 &&
       etat->proj.existe ==1){	            
      choisirTypeStylo(5,0,0,0);
      effacerPoint(etat->proj.x,etat->proj.y);
      etat->proj.existe = 0;
      etat->collision = 1;
        etat->u++;
      
      if(etat->aste[i].diametre > 40){
	etat->aste[etat->nb_aste] = creer_frag(etat,i);
	etat->nb_aste++;
	etat->aste[etat->nb_aste] = creer_frag(etat,i);
	etat->nb_aste++;
	for(int k =i; k< etat->nb_aste-1; k++){
	  etat->aste[k] = etat->aste[k+1];
	}
	 etat->aste[etat->nb_aste-1].existe=0;
	 etat->nb_aste--;
      }
      else{
	for(int k =i; k< etat->nb_aste-1; k++){
	  etat->aste[k] = etat->aste[k+1];
	}
	etat->aste[etat->nb_aste-1].existe=0;
	etat->nb_aste--;
      }     
    }
  }
  return *etat;
}

//collision vaisseau/astéroide//
donnees_t collision_va(donnees_t * etat){
   for(int i =0; i< etat->nb_aste;i++){
     double d1 =  sqrt(pow(etat->aste[i].x-forme(*etat).a,2)+
		       pow(etat->aste[i].y-forme(*etat).d,2));
     double d2 =  sqrt(pow(etat->aste[i].x-forme(*etat).b,2)+
		       pow(etat->aste[i].y-forme(*etat).e,2));
     double d3 =  sqrt(pow(etat->aste[i].x-forme(*etat).c,2)+
		       pow(etat->aste[i].y-forme(*etat).f,2));
     
     if(d1 < etat->aste[i].diametre/2||
	d2 < etat->aste[i].diametre/2||
	d3 < etat->aste[i].diametre/2){
       etat->perdu = 1;
     }
   }
   return *etat;
}

//score//
donnees_t score(donnees_t * etat){
  if(etat->perdu == 0){
    if(etat->collision == 1 && etat->u < 10){
      afficherImage(etat->i[etat->u],-290,290);
    }
    
    if(etat->collision == 1 && etat->u == 10 && etat->d < 9 ){
      etat->d ++;
      etat->u = 0;
    }
    afficherImage(etat->i[etat->d],-315,290);  
    
    if(etat->collision == 1 && etat->d == 9 && etat->u == 10){
      etat->c ++;
      etat->d = 0;
      etat->u = 0;
    }
    afficherImage(etat->i[etat->c],-340,290);
  }
  return *etat;
}

donnees_t commandes(donnees_t* etat){
  EtatSourisClavier esc = lireEtatSourisClavier();
  
    if(esc.touchesClavier[80]==1){
    etat->vaisseau.angle += 250*etat->dt;
  }
  if(esc.touchesClavier[79]==1){
    etat->vaisseau.angle -= 250*etat->dt;
  }
  if(esc.touchesClavier[82]==1){
    etat->vaisseau.vx += 100*sin(etat->vaisseau.angle*M_PI/180) * etat->dt;
    etat->vaisseau.vy -= 100*cos(etat->vaisseau.angle*M_PI/180) * etat->dt;
  }
  if(esc.touchesClavier[44]==1 && etat->proj.existe==0){       
    etat->proj = creer_proj(*etat);
    }
  return *etat;
    }

//met à jour les données du programme//
void maj(donnees_t * etat){
  etat->dt = delta_temps();
  *etat = creer_score(*etat);
   
//commandes//    
  *etat = commandes(etat);
  
//projectiles//
  etat->proj.x += etat->dt*etat->proj.vx;
  etat->proj.y += etat->dt*etat->proj.vy;
  if(etat->proj.x <-400 || etat->proj.y <-300||
     etat->proj.x > 400 || etat->proj.y > 300){
    etat->proj.existe =0;
  }
  
//astéroides//
  *etat = aste(etat);
  
//vaisseau//
  *etat = vaisseau(etat);
  
//collision projectile/astéroide et score//
  *etat = collision_pa(etat);
  *etat = score(etat);
  
//collision vaisseau/astéroide (défaite)//
 *etat = collision_va(etat);
 if(etat->perdu == 1){
   perdu(*etat);
   Son haha = chargerAudio("haha.mp3");
   jouerSon(haha,0);
 }
  
 libererImage(etat->i[0]);libererImage(etat->i[1]);
 libererImage(etat->i[2]);libererImage(etat->i[3]);
 libererImage(etat->i[4]);libererImage(etat->i[5]);
 libererImage(etat->i[6]);libererImage(etat->i[7]);
 libererImage(etat->i[8]);libererImage(etat->i[9]);
}
