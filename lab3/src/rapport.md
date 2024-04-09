# Labo 3 - Miguel Jalube - 27.03.2024
## Objectif
L'objectif du labo est de trouver des fonctions non optimisées et les analyser avec godbolt et les différents niveaux d'optimisation. Aussi petit à petit proposer une version plus optimisée de la fonction tout en analysant systematiquement le code compilé.
## Custom sqrt
Cette fonction calcule la racine carré d'un entier et retourne -1 si l'entier d'a pas de racine. Elle est toute simple et pas optimisée.

### Non optimisé
Voici le lien godbolt sans optimisations :
https://godbolt.org/z/ehsP4jdEz

https://godbolt.org/z/9aPK4q66e

On voit que le compilateur ne fait pas d'optimisation concernant `number/2` ce qui paraît logique car on a vu que si on ne lui demande pas d'optimiser il ne fera rien.

Il est possible qu'il décide de stocker cette valeur dans une variable aux niveaux d'optimisations supérieurs afin d'éviter de refaire le calcul à chaque fois.

### Optimisé par `-fif-conversion`
Lien : https://godbolt.org/z/MxsGhT4fW

Comme attendu, à ce niveau d'optimisation on voit qu'on a nettement moins d'instructions que pour O0. Ceci est du au fait qu'il optimise bel et bien le calcul de `number/2`.

L'option de compilation spécifique pour cette optimisation est `-fif-conversion` qui enlève les branchements conditionnels repetitifs inutiles en stockant la valeur du résultat du branchement dans un registre.

### Optimisation manuelle [à trouver]
https://godbolt.org/z/11x7z91bE 