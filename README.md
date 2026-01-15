# Station Meteo

Station météo embarquée basée sur Arduino, permettant de mesurer, afficher et mémoriser la température et l’humidité via un capteur DHT11, avec interface LCD et navigation par boutons.

## 🎯 Objectif du projet

Ce projet consiste à développer une station météo embarquée basée sur Arduino, capable de mesurer et d’afficher en temps réel la température et l’humidité ambiantes à l’aide d’un capteur DHT11.

L’objectif principal est de proposer une interface utilisateur simple et interactive, reposant sur un écran LCD I2C 16x2 et un système de navigation par boutons, permettant :

- la consultation des valeurs instantanées,
- l’affichage des valeurs minimales et maximales enregistrées,
- la sauvegarde persistante des données grâce à la mémoire EEPROM.

Ce projet est conçu comme une base pédagogique et fonctionnelle pour les systèmes embarqués orientés capteurs et interfaces homme-machine.


## ⚙️ Technologie utilisé
- Microcontrôleur : Arduino Uno
- Language : C/C++
- IDE : VS Code + PlatformIO
- Capteurs : DHT11
- Organes de commande : Bouton poussoir
- Affichage : LCD I2C 16x2
- Autres composants : Resistance, câble dupont

## 🔌 Matériel nécessaire

Voir le fichier [hardware/BOM.md](hardware/BOM.md)

## Schéma de câblage
Voir [hardware/wiring.png](hardware/wiring.png).

## 🔧 Installation et configuration
1. Installer VS Code
2. Installer l’extension PlatformIO
3. Cloner le dépôt :
   ```bash
   git clone https://github.com/ms-alien/Station_Meteo_Arduino.git
4. Ouvrir le projet avec PlatformIO
5. Compiler et téléverser sur la carte
6. Les détails sont disponibles dans [docs/setup.md](docs/setup.md).

## Fonctionnement
Au démarrage, la station initialise l’écran LCD, le capteur DHT11, les boutons de navigation et charge depuis l’EEPROM les valeurs minimales et maximales précédemment enregistrées.

L’interface utilisateur repose sur un menu principal affiché sur l’écran LCD, accessible via quatre boutons :

- UP : navigation vers le haut
- DOWN : navigation vers le bas
- OK : validation
- BACK : retour au menu principal

Le menu principal propose trois sections :

- Temperature : affiche la température actuelle en degrés Celsius.
- Humidity : affiche l’humidité relative en pourcentage.
- Min/Max : permet de consulter les valeurs minimales et maximales enregistrées.

Les mesures sont rafraîchies automatiquement toutes les secondes.
À chaque nouvelle lecture valide, le système compare les valeurs actuelles avec les minimums et maximums existants. En cas de changement, les nouvelles valeurs sont sauvegardées automatiquement dans l’EEPROM, assurant leur conservation même après une coupure d’alimentation.

Dans la section Min/Max, l’utilisateur peut basculer entre :

- l’affichage des minimums et maximums de température,
- l’affichage des minimums et maximums d’humidité.

Le programme fonctionne en boucle continue, assurant :

- la gestion des entrées utilisateur,
- la mise à jour des données,
- l’affichage dynamique selon l’état du menu actif.


## 📷 Illustrations

Images et demonstration disponibles dans [assets/](assets/).

## 🚀 Améliorations futures

Ajout d’un capteur plus précis

Communication Bluetooth / Wi-Fi 

# 👤 Auteur

Ali Diarra

Projet développé dans le cadre d’un portfolio en systèmes embarqués.

## 📄 Licence
Ce projet est sous licence MIT.
