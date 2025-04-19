#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define True 1
#define False 0

  // On declare la variable globale de la pin pour la temperature du LM35
int pin_temp = A0;
  // On declare la variable globale de la pin pour la temperature de check du potard
int pin_check = A1;
  // On declare la variable globale de la pin pour le boutton de navigation des differents menus
int pin_button = A2;
  // On declare la variable globale de la pin pour la LED rouge
int pin_led_rouge = A3;
  // On declare la variable globale de la pin pour la LED verte
int pin_led_verte = A4;
  // On declare la variable globale pour le choix du menu car sinon elle se remet a zero a chaque boucle de loop()
int menu_screen = 1;
  // On declare la variable globale pour l'état actuel du bouton
bool status_boutton = LOW;
  // On declare la variable globale pour l'ancien état du bouton
bool old_status_boutton = LOW;

  // On set le LCD en un afficheur 2 lignes, 16 cases
LiquidCrystal_I2C lcd(0x27, 16, 2);

  // Fonction qui va afficher l'entete en fonction de si ca vient du LM35 ou du potard
void entete(bool isLM35) {
  if (isLM35 == True) {
      // Si ca vient du LM35 on dit temperature
    lcd.print("Temperature en F");
  }
  else {
      // Si ca vient du potard on dit declancheur
    lcd.print("Declancheur en F");
  }

}

  // Fonction pour obtenir la valeur en mv sortant du LM35
float valeurmv(int pin) {
    // On lit la valeur en mv de la pin A0
  int sensorValue = analogRead(pin);
    // On assigne voltage a la valeur sur la pin A0
    // CORRECTION: Je reconverti la valeur en mv reel car c'etait pas bon
  float voltage = sensorValue * (5000.0 / 1023.0); // pour une alim 5V
    // On renvoie le voltage
  return voltage; 

}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en celcius
void celcius(int pin, int isLM35) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  entete(isLM35);
    // Affiche le symbole temperature
  lcd.print(char(223));
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On divise la valeur en mv par 10 pour obtenir la valeur en celcius (Voir datasheet) et on la met dans une variable temporaire
  float temp = (valeurmv(pin)) / 10;
    // On affiche la valeur
  lcd.print(temp);

}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en fahrenheit
void fahrenheit(int pin, int isLM35) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  entete(isLM35);
    // Affiche le symbole temperature
  lcd.print(char(223)); 
     // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en farenheit a partir des mv et on l'assigne a temp 
  float temp = (valeurmv(pin) / 10) * (9.0 / 5.0) + 32;
    // On affiche la valeur
  lcd.print(temp);  

}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en kelvin
void kelvin(int pin, int isLM35) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  entete(isLM35);
    // Affiche le symbole temperature
  lcd.print(char(223));
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en kelvin a partir des mv et on l'assigne a temp 
  float temp = (valeurmv(pin)) / 10 + 273.15;
    // On affiche la valeur
  lcd.print(temp);

}

void setup() {
    // On initialise l'ecran LCD
  lcd.init();
    // On allume la backlight
  lcd.backlight();
    // On clear l'ecran
  lcd.clear();
    // Je set la pin A3 en output pour allumer une LED rouge quand la temperature depasse le seuil
  pinMode(pin_led_rouge, OUTPUT);
    // Je set la pin A4 en output pour allumer une LED verte quand la temperature ne depasse pas le seuil
  pinMode(pin_led_verte, OUTPUT);

}

void loop() {
    // On compare la valeur en mv de la temperature et celle du potard
  if (valeurmv(pin_temp) >valeurmv(pin_check)) {
      //Si la valeur de la temperature est plus haute on active la led rouge
    digitalWrite(A3,HIGH);
  }
  else {
      // Si la valeur de la temperature est inferieure on active la led verte
    digitalWrite(A4,HIGH);
  }

    // On lit la valeur de la pin du bouton
  status_boutton = digitalRead(pin_button);

    // On compare l'ancien et le nouveau status du bouton, si il y a une différence et que le bouton est appuyé, on change de menu
  if (status_boutton != old_status_boutton && status_boutton == HIGH) {
      // On incrémente le menu_screen qui sert dans notre switch
    menu_screen++;
      // Si la valeur dépasse 6 (le max possible), elle retourne à 1, la valeur initiale
    if (menu_screen > 6) {
      menu_screen = 1;
    }
    // Anti-rebond simple
    delay(200);

  }

    // On sauvegarde l'état actuel pour le prochain tour
  old_status_boutton = status_boutton;

    // Switch pour les différents menus possibles et appel de leurs fonctions respectives
  switch (menu_screen) {
      // Partie du menu pour afficher la température du LM35 sous plusieurs unités
    case 1: celcius(pin_temp, True); break;
    case 2: fahrenheit(pin_temp, True); break;
    case 3: kelvin(pin_temp, True); break;
      // Partie du menu pour afficher la température de comparaison du potard sous plusieurs unités
    case 4: celcius(pin_check, False); break;
    case 5: fahrenheit(pin_check, False); break;
    case 6: kelvin(pin_check, False); break;   
    default: break;
  }

}
