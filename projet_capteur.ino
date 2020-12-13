#include "sonde_temperature.h"

#define PORT_MOSFET 6
#define PORT_POT A0
#define TAB_SIZE 10

Sonde* capteur_temperature = new Sonde();
float temperature;
float temperature_de_commande = 0;
float erreur = 0;

float erreur_tab[TAB_SIZE];
 
uint8_t ind_ecr_erreur_tab=0;
float integrale_erreur=0;
float derivee_erreur=0;
 
float Kd=0;//valeur choisie au hasard
float Ki=0; //valeur choisie au hasard
float Kp=255./125.;//valeur choisie au hasard

/******************* valeur_de_commande **********************
 *  *********************************************
 *  ********************************************
 */
float valeur_de_commande()
{
  int valeur_recupere=0;

  valeur_recupere=analogRead(PORT_POT);
  valeur_recupere=(valeur_recupere*5.)/3.35;
  //Serial.println(valeur_recupere);
  return map(valeur_recupere, 0, 1023, 0, 125);
}

/******************* UPDATE_TAB_ERREUR **********************
 *  *********************************************
 *  ********************************************
 */
void update_tab_erreur(float new_erreur)
{
  //calcul intégrale
  integrale_erreur=integrale_erreur-erreur_tab[ind_ecr_erreur_tab];
  integrale_erreur=integrale_erreur+new_erreur;
 
  //calcul dérivée
  uint8_t ind_lec_erreur_tab=ind_ecr_erreur_tab+5;
  if (ind_lec_erreur_tab>=TAB_SIZE )  //gestion modulo
  ind_lec_erreur_tab-=TAB_SIZE;
  derivee_erreur=new_erreur-erreur_tab[ind_lec_erreur_tab];
 
  //maj tableau 
  erreur_tab[ind_ecr_erreur_tab]=new_erreur;
  ind_ecr_erreur_tab++;
  if (ind_ecr_erreur_tab>=TAB_SIZE)
  ind_ecr_erreur_tab=0;  
}

/******************* PID_CORRECTION **********************
 *  *********************************************
 *  ********************************************
 */
void pid_correction(float erreur){  
   
  erreur  = erreur*Kp;
  erreur += Kd*derivee_erreur; //quand l'erreur diminue, ce terme est négatif et permet d'éviter le dépassement
  erreur += (Ki*integrale_erreur)/TAB_SIZE;

  if(erreur>255)
  {
    erreur=255;
  }
  else if(erreur<0)
  {
    erreur=0;
  }
  
  if(temperature<temperature_de_commande)//si le capteur detecte une temperature inferieur à la temperature de commande alors on active le MOSFET
  {

    analogWrite(PORT_MOSFET, erreur);
  }
  else
  {
    analogWrite(PORT_MOSFET, 0);
  }
}
 
/******************* SETUP **********************
 *  *********************************************
 *  ********************************************
 */
 
void setup() {
  Serial.begin(115200);
  capteur_temperature->initialiser_sonde_ds18b20();
  pinMode(PORT_MOSFET, OUTPUT);
  pinMode(PORT_POT, INPUT);

  for (int i=0;i<TAB_SIZE;i++) //initialiser tout le tableau à 0 pour pas avoir de problème lors de l'utilisation.
  erreur_tab[i]=0;

}

/******************* LOOP **********************
 *  *********************************************
 *  ********************************************
 */
void loop() {
 //Lecture de la temperature avec la sonde onewire
 if(capteur_temperature->sonde_connecte() == 1) //si la sonde est bien connecté
 {
    temperature=capteur_temperature->get_temperature(); //lit la temperature et la stock dans la variable temperature
 }
 /*else
 {
  Serial.println("Pas de sonde connectée!!!!");
 }*/

  temperature_de_commande=valeur_de_commande();//recupere la temperature de commande
  erreur=temperature_de_commande-temperature; //calcul l'erreur 
  update_tab_erreur(erreur); //met à jour le tableau d'erreur
  pid_correction(erreur);  //met à jour la régulation
  
  //pour tracer une courbe dans le traceur serie
 Serial.print(temperature);
 Serial.print(" ");
 Serial.print(temperature_de_commande);
 Serial.print(" ");
 Serial.println(temperature_de_commande-temperature);
}
