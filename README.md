# PRESENT-cryptosystem
# Implémentation du lightweight cryptosystème PRESENT


## Description

Ce projet est une implémentation en langage C du lightweight cryptosystème PRESENT. 
PRESENT est un algorithme de chiffrement léger conçu pour être efficace sur les dispositifs à ressources limitées, tels que les systèmes embarqués et les appareils IoT.

Le but de ce projet est de fournir une implémentation fonctionnelle de PRESENT, accompagnée de différentes fonctionnalités de chiffrement et déchiffrement, ainsi que d'exemples d'utilisation.

## Fonctionnalités

- Chiffrement de messages avec PRESENT.
- Déchiffrement de messages chiffrés avec PRESENT.
- Génération de clés pour le chiffrement.

## Structure du projet

Le projet est organisé comme suit :

- `main.c` : Un exemple d'utilisation montrant comment chiffrer et déchiffrer un message avec PRESENT.
- `present.c` : Le code source contenant les fonctions d'implémentation de PRESENT.
- `present.h` : Le fichier d'en-tête contenant les déclarations des fonctions PRESENT.
- `param.h` : Fichier de configuration avec les paramètres pour PRESENT (taille de la clé, nombre de tours, etc.).
- `Makefile` : Fichier de configuration pour la compilation du projet.
- `LICENSE` : Le fichier de licence du projet (MIT License).
- `47270450.pdf` : Etude détaillée du cryptosystème 

## Utilisation

Pour utiliser ce projet, vous pouvez inclure les fichiers `present.c` et `present.h` dans votre propre projet, ou bien exécuter l'exemple fourni dans `main.c`.

Assurez-vous de modifier le fichier `param.h` pour définir les paramètres de chiffrement adaptés à vos besoins.

## Compilation

Pour compiler l'exemple, vous pouvez utiliser le fichier Makefile inclus. Ouvrez un terminal dans le répertoire du projet et exécutez la commande :

