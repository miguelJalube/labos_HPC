# Labo 3 - Miguel Jalube - 27.03.2024
## Objectif
L'objectif du labo est de trouver des fonctions non optimisées et les analyser avec godbolt et les différents niveaux d'optimisation. Aussi petit à petit proposer une version plus optimisée de la fonction tout en analysant systematiquement le code compilé.
## Custom sqrt
Cette fonction calcule la racine carré d'un entier et retourne -1 si l'entier d'a pas de racine. Elle est toute simple et pas optimisée.

### O0
Voici le lien godbolt sans optimisations :
https://godbolt.org/z/ehsP4jdEz

On voit que le compilateur ne fait pas d'optimisation concernant `number/2` ce qui paraît logique car on a vu que si on ne lui demande pas d'optimiser il ne fera rien.

Il est possible qu'il décide de stocker cette valeur dans une variable aux niveaux d'optimisations supérieurs afin d'éviter de refaire le calcul à chaque fois.

### O1
Lien : https://godbolt.org/z/MxsGhT4fW

Comme attendu, à ce niveau d'optimisation on voit qu'on a nettement moins d'instructions que pour O0. Ceci est du au fait qu'il optimise bel et bien le calcul de `number/2`.

### O2
Lien : https://godbolt.org/z/s1TE1Pr1G