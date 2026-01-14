#include <Arduino.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <EEPROM.h>

// ========== CONFIGURATION ==========
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Boutons
#define BTN_BACK 11 // Retour
#define BTN_DOWN 10 // Bas
#define BTN_UP 9    // Haut
#define BTN_OK 8    // OK/Entrer

// Caractère flèche
byte fleche[8] = {
    B00000,
    B00100,
    B00110,
    B11111,
    B11111,
    B00110,
    B00100,
    B00000};

// ========== VARIABLES GLOBALES ==========
struct DonneesMemoire
{
  float tempMin;
  float tempMax;
  float humMin;
  float humMax;
};

DonneesMemoire memoire;
float tempMin, tempMax, humMin, humMax;

// ÉTATS MENU
enum MenuState
{
  MENU_PRINCIPAL,
  PAGE_TEMPERATURE,
  PAGE_HUMIDITE,
  PAGE_MIN_MAX
};

MenuState etatMenu = MENU_PRINCIPAL;

// Menu principal a 3 options : 0=Temp, 1=Hum, 2=MinMax
const int NOMBRE_OPTIONS_MENU = 3;
int selectionMenu = 0; // Commence à Température
bool afficherTempDansMinMax = true;

// Variables timing
unsigned long dernierRafraichissement = 0;
unsigned long dernierAppuiBouton = 0;
const unsigned long DELAI_RAFRAICHISSEMENT = 1000;
const unsigned long DELAI_BOUTON = 150;

// ========== EEPROM ==========
void sauvegarderEEPROM()
{
  memoire.tempMin = tempMin;
  memoire.tempMax = tempMax;
  memoire.humMin = humMin;
  memoire.humMax = humMax;
  EEPROM.put(0, memoire);
}

void chargerEEPROM()
{
  EEPROM.get(0, memoire);

  if (isnan(memoire.tempMin) || memoire.tempMin > 150.0 ||
      memoire.tempMax < -150.0 || memoire.humMin > 100.0 ||
      memoire.humMax < 0.0)
  {
    tempMin = 100.0;
    tempMax = -100.0;
    humMin = 100.0;
    humMax = 0.0;
  }
  else
  {
    tempMin = memoire.tempMin;
    tempMax = memoire.tempMax;
    humMin = memoire.humMin;
    humMax = memoire.humMax;
  }
}

// ========== MIN/MAX ==========
void mettreAJourMinMax(float temperature, float humidite)
{
  bool changement = false;

  if (temperature < tempMin)
  {
    tempMin = temperature;
    changement = true;
  }
  if (temperature > tempMax)
  {
    tempMax = temperature;
    changement = true;
  }
  if (humidite < humMin)
  {
    humMin = humidite;
    changement = true;
  }
  if (humidite > humMax)
  {
    humMax = humidite;
    changement = true;
  }

  if (changement)
  {
    sauvegarderEEPROM();
  }
}

// ========== AFFICHAGE ==========
void afficherMenuPrincipal()
{
  lcd.clear();

  // Afficher les DEUX options visibles selon la sélection
  switch (selectionMenu)
  {
  case 0: // Température sélectionnée
    // Ligne 1: Temperature (avec flèche)
    lcd.setCursor(0, 0);
    lcd.write(byte(0));
    lcd.print(" Temperature ");

    // Ligne 2: Humidity (sans flèche)
    lcd.setCursor(0, 1);
    lcd.print("  Humidity    ");
    break;

  case 1: // Humidity sélectionnée
    // Ligne 1: Temperature (sans flèche)
    lcd.setCursor(0, 0);
    lcd.print("  Temperature ");

    // Ligne 2: Humidity (avec flèche)
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    lcd.print(" Humidity    ");
    break;

  case 2: // Min/Max sélectionné
    // Ligne 1: Humidity (sans flèche)
    lcd.setCursor(0, 0);
    lcd.print("  Humidity    ");

    // Ligne 2: Min/Max (avec flèche)
    lcd.setCursor(0, 1);
    lcd.write(byte(0));
    lcd.print(" Min/Max     ");
    break;
  }
}

void afficherTemperature()
{
  lcd.clear();
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (!isnan(t) && !isnan(h))
  {
    mettreAJourMinMax(t, h);

    lcd.setCursor(0, 0);
    lcd.print("Temperature");

    lcd.setCursor(0, 1);
    lcd.print(t, 1);
    lcd.print(" C  ");

    /*lcd.setCursor(7, 1);
    lcd.print("[");
    lcd.print(tempMin, 1);
    lcd.print("-");
    lcd.print(tempMax, 1);
    lcd.print("]");*/
  }
}

void afficherHumidite()
{
  lcd.clear();
  float h = dht.readHumidity();

  if (!isnan(h))
  {
    lcd.setCursor(0, 0);
    lcd.print("Humidity");

    lcd.setCursor(0, 1);
    lcd.print(h, 0);
    lcd.print(" %   ");

    /*lcd.setCursor(8, 1);
    lcd.print("[");
    lcd.print(humMin, 0);
    lcd.print("-");
    lcd.print(humMax, 0);
    lcd.print("]");*/
  }
}

void afficherMinMax()
{
  lcd.clear();

  if (afficherTempDansMinMax)
  {
    lcd.setCursor(0, 0);
    lcd.print("Temp Min/Max");

    lcd.setCursor(0, 1);
    if (tempMin < 99.9 && tempMax > -99.9)
    {
      lcd.print(tempMin, 1);
      lcd.print("C - ");
      lcd.print(tempMax, 1);
      lcd.print("C");
    }
    else
    {
      lcd.print("--");
    }

    lcd.setCursor(14, 1);
    lcd.print("T");
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Hum Min/Max");

    lcd.setCursor(0, 1);
    if (humMin < 99.9)
    {
      lcd.print(humMin, 0);
      lcd.print("% - ");
      lcd.print(humMax, 0);
      lcd.print("%");
    }
    else
    {
      lcd.print("--");
    }

    lcd.setCursor(14, 1);
    lcd.print("H");
  }
}

// ========== GESTION BOUTONS ==========
void gererBoutons()
{
  static int prevUp = HIGH, prevDown = HIGH, prevOk = HIGH, prevBack = HIGH;
  int btnUp = digitalRead(BTN_UP);
  int btnDown = digitalRead(BTN_DOWN);
  int btnOk = digitalRead(BTN_OK);
  int btnBack = digitalRead(BTN_BACK);

  unsigned long maintenant = millis();

  // Anti-rebond
  if (maintenant - dernierAppuiBouton < DELAI_BOUTON)
  {
    return;
  }

  // === BOUTON RETOUR ===
  if (btnBack == LOW && prevBack == HIGH)
  {
    dernierAppuiBouton = maintenant;

    if (etatMenu != MENU_PRINCIPAL)
    {
      etatMenu = MENU_PRINCIPAL;
      afficherMenuPrincipal();
    }
  }

  // === NAVIGATION ===
  switch (etatMenu)
  {

  case MENU_PRINCIPAL:
    // HAUT - monter dans le menu
    if (btnUp == LOW && prevUp == HIGH)
    {
      dernierAppuiBouton = maintenant;
      selectionMenu--;
      if (selectionMenu < 0)
        selectionMenu = NOMBRE_OPTIONS_MENU - 1;
      afficherMenuPrincipal();
    }

    // BAS - descendre dans le menu
    if (btnDown == LOW && prevDown == HIGH)
    {
      dernierAppuiBouton = maintenant;
      selectionMenu++;
      if (selectionMenu >= NOMBRE_OPTIONS_MENU)
        selectionMenu = 0;
      afficherMenuPrincipal();
    }

    // OK - sélectionner
    if (btnOk == LOW && prevOk == HIGH)
    {
      dernierAppuiBouton = maintenant;

      switch (selectionMenu)
      {
      case 0:
        etatMenu = PAGE_TEMPERATURE;
        break;
      case 1:
        etatMenu = PAGE_HUMIDITE;
        break;
      case 2:
        etatMenu = PAGE_MIN_MAX;
        afficherTempDansMinMax = true;
        break;
      }
    }
    break;
  case PAGE_TEMPERATURE:
  case PAGE_HUMIDITE:
    // Rien à gérer ici, seulement BACK est actif
    break;
  case PAGE_MIN_MAX:
    // Dans Min/Max, HAUT/BAS pour changer entre Temp et Hum
    if (btnUp == LOW && prevUp == HIGH)
    {
      dernierAppuiBouton = maintenant;
      afficherTempDansMinMax = true;
    }

    if (btnDown == LOW && prevDown == HIGH)
    {
      dernierAppuiBouton = maintenant;
      afficherTempDansMinMax = false;
    }
    break;
  }

  // Sauvegarder états
  prevUp = btnUp;
  prevDown = btnDown;
  prevOk = btnOk;
  prevBack = btnBack;
}

// ========== SETUP ==========
void setup()
{
  Serial.begin(9600);

  // LCD
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, fleche);

  // DHT11
  dht.begin();

  // EEPROM
  chargerEEPROM();

  // Boutons
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_OK, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);

  // Message accueil
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Station Meteo");
  lcd.setCursor(2, 1);
  lcd.print("With DHT 11");
  delay(1500);

  // Menu principal
  afficherMenuPrincipal();
}

// ========== LOOP ==========
void loop()
{
  unsigned long maintenant = millis();

  // 1. Gérer les boutons
  gererBoutons();

  // 2. Rafraîchir l'affichage
  if (maintenant - dernierRafraichissement >= DELAI_RAFRAICHISSEMENT)
  {
    dernierRafraichissement = maintenant;

    switch (etatMenu)
    {
    case MENU_PRINCIPAL:
      // Pas de rafraîchissement constant
      break;

    case PAGE_TEMPERATURE:
      afficherTemperature();
      break;

    case PAGE_HUMIDITE:
      afficherHumidite();
      break;

    case PAGE_MIN_MAX:
      afficherMinMax();
      break;
    }
  }

  delay(10);
}