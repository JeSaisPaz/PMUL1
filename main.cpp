#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// On set le LCD en un afficheur 2 lignes, 16 cases
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Fonction pour obtenir la valeur en mv sortant du LM35
float valeurmv() {
    // On lit la valeur en mv de la pin A0
  int sensorValue = analogRead(A0);
    // On assigne voltage a la valeur sur la pin A0
  float voltage = sensorValue;
    // On renvoie le voltage
  return voltage;
  
}

// Fonction EN COURS DE PROGRAMMATION qui permet d'obtenir la valeur en mv de comparaison dans l'amplificateur
float comparedmv() {
    // On lit la valeur en mv de la pin A1
  int sensorValue = analogRead(A1);
    // On assigne voltage a la valeur sur la pin A1
  float voltage = sensorValue;
    // On renvoie le voltage
  return voltage;

}

// Fonction d'affichage sur l'ecran LCD en celcius
void celcius() {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperatur en C"); 
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On divise la valeur en mv par 10 pour obtenir la valeur en celcius (Voir datasheet) et on la met dans une variable temporaire
  float temp = (valeurmv())/10;
    // On affiche la valeur
  lcd.print(temp);  
}

void fahrenheit() {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperatur en F"); 
     // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en farenheit (On utilise la fonction celcius) et on l'assigne a temp 
  float temp = celcius()*(9/5)+32;
    // On affiche la valeur
  lcd.print(temp);  
}


void kelvin() {
    // On met le curseur en 0,0 pour commencer l'ecriture
  lcd.setCursor(0, 0); 
    // On affiche l'entete
  lcd.print("Temperatur en K"); 
    // On se met en 4,1 pour afficher la valeur de la temperature au milieu
  lcd.setCursor(4, 1);
    // On converti la valeur en kelvin (On utilise la fonction celcius) et on l'assigne a temp 
  float temp = celcius()+273.15;
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
    case 1: celcius(); break;
    case 2: fahrenheit(); break;
    case 3: kelvin(); break;
    default: break;
  }

  
}