#!/bin/bash

rm -rf ../img/sample_640_2_*.png

# Commande pour nettoyer le build
cmake --build build --target clean

# Commande pour construire le projet
cmake --build build

# Commande pour exécuter le programme avec les arguments spécifiés
hyperfine --warmup 3 'build/segmentation ../img/sample_640_2.png 5 ../img/sample_640_2_out_5.png'
hyperfine --warmup 3 'build/segmentation ../img/sample_640_2.png 10 ../img/sample_640_2_out_10.png'
hyperfine --warmup 3 'build/segmentation ../img/sample_640_2.png 100 ../img/sample_640_2_out_100.png'
