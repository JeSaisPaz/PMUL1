#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// On declare la variable globale de la pin pour la temperature du LM35
int pin_temp = A0;
// On declare la variable globale de la pin pour la temperature de check du potard
int pin_check = A1;

// On set le LCD en un afficheur 2 lignes, 16 cases
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fonction pour obtenir la valeur en mv sortant du LM35
float valeurmv(int pin) {
    // On lit la valeur en mv de la pin A0
  int sensorValue = analogRead(pin);
    // On assigne voltage a la valeur sur la pin A0
  float voltage = sensorValue;
    // On renvoie le voltage
  return voltage;
  
}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en celcius
void celcius(int pin) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperature en C");
    // Affiche le symbole temperature
  lcd.print(char(223));
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On divise la valeur en mv par 10 pour obtenir la valeur en celcius (Voir datasheet) et on la met dans une variable temporaire
  float temp = (valeurmv(pin))/10;
    // On affiche la valeur
  lcd.print(temp);
}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en fahrenheit
void fahrenheit(int pin) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperature en F");
    // Affiche le symbole temperature
  lcd.print(char(223)); 
     // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en farenheit (On utilise la fonction celcius) et on l'assigne a temp 
  float temp = celcius(pin)*(9/5)+32;
    // On affiche la valeur
  lcd.print(temp);  
}

  // Prends une valeur en mv a partir d'une pin et renvoie la temperature en kelvin
void kelvin(int pin) {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperature en K");
    // Affiche le symbole temperature
  lcd.print(char(223));
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en kelvin (On utilise la fonction celcius) et on l'assigne a temp 
  float temp = celcius(pin)+273.15;
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

}

void loop() {
    // variable pour la selection des menus
  int menu=1;

    // Switch pour les differents menus possibles et appel de leurs fonctions respetives
  switch(menu) {
      // Partie du menu pour afficher la temperature du LM35 sous plusieures unites
    case 1: celcius(pin_temp); break;
    case 2: fahrenheit(pin_temp); break;
    case 3: kelvin(pin_temp); break;
      // Partie du menu pour afficher la temperature de comparaison du potard sous plusieures unites
    case 4: celcius(pin_check); break;
    case 5: fahrenheit(pin_check); break;
    case 6: kelvin(pin_check); break;   
    default: break;
  }

  
}