#!/bin/bash

rm -rf ../img/sample_640_2_out_*.png

# Commande pour nettoyer le build
cmake --build build --target clean

# Commande pour construire le projet
cmake --build build

# Commande pour exécuter le programme avec les arguments spécifiés
hyperfine --warmup 1 'build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_1d.png 1'
hyperfine --warmup 1 'build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_chained.png 2'
