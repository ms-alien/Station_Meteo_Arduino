# Setup du projet

Ce document explique comment installer, configurer et lancer le projet.

---

## 1. Prérequis

### Matériel
- Carte : Arduino Uno 
- Capteurs : DHT11
- Affichage : LCD 16x2 I2C
- Boutons poussoirs 
- Résistances 
- Câbles Dupont
- Alimentation USB

Voir le détail dans [hardware/BOM.md](hardware/BOM.md).

### Logiciel
- VS Code
- PlatformIO extension
- Pilote USB de la carte (CH340 si nécessaire)

---

## 2. Installation de l’environnement

1. Installer **Visual Studio Code**
2. Installer l’extension **PlatformIO IDE**
3. Redémarrer VS Code

---

## 3. Cloner le projet

```bash
git clone https://github.com/ms-alien/Station_Meteo_Arduino.git
cd Station_Meteo_Arduino
```
- Ouvrir le dossier avec VS Code.
---
## 4. Configuration PlatformIO

Le projet est déjà configuré via le fichier platformio.ini.

Vérifier :
Le type de carte (board)
La vitesse de communication (monitor_speed)
Aucune modification n’est nécessaire par défaut.

---

## 5. Câblage du matériel

Suivre le schéma de câblage :

Image : hardware/wiring.png

Schéma électronique : hardware/schematic.pdf

Résumé des connexions principales :

DHT11 → Pin D7

LCD I2C → SDA / SCL

Boutons → Pins D8, D9, D10

---

## 6. Compilation et téléversement

Brancher la carte en USB

Cliquer sur Upload dans PlatformIO

## 7. Vérification du fonctionnement

L’écran LCD s’allume
Les valeurs de température et d’humidité s’affichent
Les boutons permettent de naviguer dans le menu

## 8. Dépannage rapide
Problème : écran LCD vide
Vérifier l’adresse I2C (0x27 ou 0x3F)
Vérifier SDA / SCL

Problème : DHT ne répond pas
Vérifier le pin
Attendre au moins 2 secondes entre lectures

Problème : téléversement impossible
Vérifier le port COM
Installer le pilote USB

---

## 9. Notes
- Projet conçu pour être simple, modulaire et évolutif
- Compatible avec des améliorations futures (menu avancé, alertes)
- Dans le code vous remarquerez que la partie affichage min/max dans les pages temperature et humidity sont grisés. Cela est du au fait que l'affichage n'etait pas très cohérente sur le LCD, mais vous pourrez voir les aperçus dans les images et demo puisque à ce moment le code n'avais pas encore été modifié. 