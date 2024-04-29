#!/bin/bash

# Commande pour nettoyer le build
cmake --build build --target clean

# Commande pour construire le projet
cmake --build build

# Commande pour exécuter le programme avec les arguments spécifiés
build/segmentation ../img/sample_640_2.png 2 ../img/test2.png
