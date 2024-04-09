# Labo 3 - Miguel Jalube - 27.03.2024
## Objectif
L'objectif du labo est de trouver des fonctions non optimisées et les analyser avec godbolt et les différents niveaux d'optimisation. Aussi petit à petit proposer une version plus optimisée de la fonction tout en analysant systematiquement le code compilé.

## Partie 1

## Custom sqrt
Cette fonction calcule la racine carré d'un entier et retourne -1 si l'entier d'a pas de racine. Elle est toute simple et pas optimisée.

### Non optimisé
Lien : https://godbolt.org/z/9aPK4q66e

On voit que le compilateur ne fait pas d'optimisation concernant `number/2` ce qui paraît logique car on a vu que si on ne lui demande pas d'optimiser il ne fera rien. Il est possible qu'il décide de stocker cette valeur dans une variable aux niveaux d'optimisations supérieurs afin d'éviter de refaire le calcul à chaque fois.

### Optimisé avec `-O1`
Lien : https://godbolt.org/z/MxsGhT4fW

Comme attendu, à ce niveau d'optimisation on voit qu'on a nettement moins d'instructions que pour O0. Ceci est du au fait qu'il optimise bel et bien le calcul de `number/2`.

L'option de compilation `-O1` enlève les branchements conditionnels repetitifs inutiles en stockant la valeur du résultat du branchement dans un registre.

### Optimisation manuelle
Lien : https://godbolt.org/z/n84eYKTev

## Factorielle
Cette fonction calcule la factorielle de n.

### Non optimisé
Lien : https://godbolt.org/z/9nW9n6KW8 

### Optimisé avec `-O1`
Lien : https://godbolt.org/z/MKTKdcxec

Élimination des branches inutiles : Dans la version non optimisée du code, il y a des instructions de comparaison (cmpl) suivies de branches conditionnelles (je, jne) pour gérer les cas où l'argument de la fonction est égal à 0 ou 1. Cependant, ces branches ne sont pas nécessaires, car la factorielle de 0 ou 1 est toujours égale à 1. Dans le code optimisé, ces branches ont été éliminées, ce qui réduit la complexité du code.

### Optimisation manuelle
Lien : https://godbolt.org/z/TMYGE5Knd

Factorielle iterative : Cette version itérative de la fonction factorielle est plus efficace car elle évite les appels récursifs et utilise une boucle for simple pour calculer le résultat.

## String reverse
### Non optimisé
Lien : https://godbolt.org/z/fzvE5Kfz1

Cette fonction inverse une chaîne de caractères.

### Optimisée avec `-O1`
Lien : https://godbolt.org/z/4W1a1PcME

Réduction de l'utilisation de la pile : Dans la version non optimisée, le compilateur réserve 32 octets d'espace sur la pile pour les variables locales. Dans la version optimisée, l'utilisation de la pile est minimisée. Seul un registre (%rbx) est sauvegardé sur la pile, et cela n'est nécessaire que parce que le registre est utilisé comme paramètre d'entrée pour la fonction `strlen()`.

### Optimisation manuelle
Lien : https://godbolt.org/z/Kc4xW9orh

Utilisation de pointeurs : Dans cette optimisation, on utilise deux pointeurs, un pointant au début de la chaîne et l'autre à la fin, et on le déplace simultanément vers le centre de la chaîne tout en échangeant les caractères. Cela évite la nécessité d'utiliser une division par 2 pour la longueur de la chaîne, ce qui est coûteux en termes de performances.

## Partie 2

## grayscale 1D

### Non optimisée
Lien : https://godbolt.org/z/e6GTs98oo

La fonction choisie du labo 1 est la fonction grayscale 1D.

### Optimisée avec `-O3`
Lien : https://godbolt.org/z/zq1GYcWej

Avec le flag `-O3` on aperçoit que le compilateur utilise les SIMD pour accelerer le traitement des pixels.

### Optimisée manuellement
Lien : https://godbolt.org/z/3caG6d9o4

Dans cette optimisation, il a été choisi de calculer l'indice qui utilise i * img->width à l'extérieur de la boucle imbriquée afin de ne le calculer qu'une seule fois et le stocker dans une variable.