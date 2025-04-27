#include <Wire.h>
#include <LiquidCrystal_I2C.h>

  // On declare la variable globale de la pin pour la temperature du LM35
const int pin_temp = A0;
  // On declare la variable globale de la pin pour la temperature de check du potard
const int pin_check = A1;
  // On declare la variable globale de la pin pour le boutton de navigation des differents menus
const int pin_button = A2;
  // On declare la variable globale de la pin pour la LED rouge
const int pin_led_rouge = A3;
  // On declare la variable globale de la pin pour la LED verte
const int pin_led_verte = A4;
  // On declare la variable globale de la pin pour TP1 (Monitoring de la polarisation du transistor)
const int pin_tp1 = A5;
  // On declare la variable globale pour le choix du menu car sinon elle se remet a zero a chaque boucle de loop()
int menu_screen = 1;
  // On declare la variable globale pour l'état actuel du bouton
bool status_boutton = LOW;
  // On declare la variable globale pour l'ancien état du bouton
bool old_status_boutton = LOW;

  // On set le LCD en un afficheur 2 lignes, 16 cases
LiquidCrystal_I2C lcd(0x27, 16, 2);

  // Fonction qui va afficher l'entete en fonction de si ca vient du LM35 ou du potard
void entete(int isLM35, int menu_screen) {
    // En gros, ce switch affiche l'unite en fonction de la position dans le menu (Le plus opti possible que j'ai su faire), ce qui explique les cases doubles
  lcd.print(isLM35 ? "Temperature en " : "Declencheur en ");
  switch(menu_screen) {
      case 1:
      case 4:
        lcd.print("C");
        break;
      case 2:
      case 5:
        lcd.print("F");
        break;
      case 3:
      case 6:
        lcd.print("K");
        break;
    }
}

  // Fonction pour obtenir la valeur en mv sortant du LM35
float valeurmv(int pin) {
    // On lit la valeur en mv de la pin A0
  int sensorValue = analogRead(pin);
    // On assigne voltage a la valeur sur la pin 
  float voltage = sensorValue * (5000.0 / 1023.0); // CORRECTION: Je reconverti la valeur en mv reel car c'etait pas bon
    // On renvoie le voltage
  return voltage; 
}

  // Fonction qui donne une information sur la polarisation du transistor T2
void status_polarisation() {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0,0);

  lcd.print("Status de polarisation du transistor");

  int status_tp1 = digitalRead(pin_tp1);
    // On met le curseur en 0,1 pour afficher le status sur la ligne en dessous
  lcd.setCursor(0,1);

    // Si le tp1 est en LOW ca veut dire que le transistor T2 est polarise
  if(status_tp1 == LOW) {
      // On ecris polarisé
    lcd.print("Polaris");
    lcd.print(char(130));
  }

  else {
      // On ecris non-polarisé
    lcd.print("Non-polaris");
    lcd.print(char(130));
  }
}


  // Fonction factorisée pour afficher la température sur le LCD
void afficher_temperature(int pin, bool isLM35, char unite) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0);
    // On affiche l'entete
  entete(isLM35, menu_screen);
    // Affiche le symbole temperature
  lcd.print(char(223));
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti d'abord en Celsius
  float tempC = valeurmv(pin) / 10.0;
    // Variable temporaire qui va contenir l'unite
  float temp;
    // Conversion selon l'unité
  switch (unite) {
    case 'C':
      temp = tempC;
      break;
    case 'F':
      temp = tempC * 9.0 / 5.0 + 32;
      break;
    case 'K':
      temp = tempC + 273.15;
      break;
    default:
      temp = tempC;
      break;
  }

    // On affiche la valeur
  lcd.print(temp);
}

  // Fonction responsable de la gestion de la LED
void status_led() {
      // On compare la valeur en mv de la temperature et celle du potard
      if (valeurmv(pin_temp) > valeurmv(pin_check)) {
        // Si la valeur de la temperature est plus haute on active la led rouge
      digitalWrite(pin_led_rouge, HIGH);
        // On eteinds la led verte sinon on y comprend plus rien
      digitalWrite(pin_led_verte, LOW);
    }
    else {
        // Si la valeur de la temperature est inferieure on active la led verte
      digitalWrite(pin_led_verte, HIGH);
        // On eteinds la led rouge sinon on y comprend plus rien
      digitalWrite(pin_led_rouge, LOW);  // Correction : éteindre la LED rouge
    }
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
    // Je set la pin du boutton en lecture uniquement (De bonne pratique)
  digitalRead(pin_button);
}

void loop() {
    // On appel la fonction de gestion de la LED
  status_led();

    // On lit la valeur de la pin du bouton
  status_boutton = digitalRead(pin_button);

    // On compare l'ancien et le nouveau status du bouton, si il y a une différence et que le bouton est appuyé, on change de menu
  if (status_boutton != old_status_boutton && status_boutton == HIGH) {
      // On incrémente le menu_screen qui sert dans notre switch
    menu_screen++;
      // Si la valeur dépasse 6 (le max possible), elle retourne à 1, la valeur initiale
    if (menu_screen > 7) {
      menu_screen = 1;
    }
    // Anti-rebond simple
    delay(100);
  }

    // On sauvegarde l'état actuel pour le prochain tour
  old_status_boutton = status_boutton;

    // Switch pour les différents menus possibles et appel de leurs fonctions respectives
  switch (menu_screen) {
      // Partie du menu pour afficher la température du LM35 sous plusieurs unités
    case 1: 
      afficher_temperature(pin_temp, true, 'C'); 
      break;
    case 2: 
      afficher_temperature(pin_temp, true, 'F'); 
      break;
    case 3: 
      afficher_temperature(pin_temp, true, 'K'); 
      break;
      // Partie du menu pour afficher la température de comparaison du potard sous plusieurs unités
    case 4: 
      afficher_temperature(pin_check, false, 'C'); 
      break;
    case 5: 
      afficher_temperature(pin_check, false, 'F'); 
      break;
    case 6: 
      afficher_temperature(pin_check, false, 'K'); 
      break;   
      // Partie du menu pour afficher le status de polarization du transistor
    case 7:
      status_polarisation();
    default: 
      break;
  }
}
