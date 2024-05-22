# Labo 4 HPC - Miguel Jalube - 30.04.2024

## Objectif
Dans ce laboratoire, vous serez amené(e)s à analyser et à écrire du code en utilisant les instructions
SIMD. Vous trouverez toutes les informations nécessaires concernant les instructions SIMD pour les
processeurs Intel à l’adresse suivante : https://www.intel.com/content/www/us/en/docs/intrinsics-
guide/index.html

## Benchmarking
Pour le benchmarking du programme, hyperfine a été utilisé. Les comparaisons se font toujours par rapport au benchmark sans optimisations et les autres optimisations sont desactivées lors du test d'une optimisation particulière.

## Architecture utilisée
```bash
miguel@miguel-Yoga-7-14IAL7:~$ lscpu
Architecture :                              x86_64
  Mode(s) opératoire(s) des processeurs :   32-bit, 64-bit
  Address sizes:                            39 bits physical, 48 bits virtual
  Boutisme :                                Little Endian
Processeur(s) :                             16
  Liste de processeur(s) en ligne :         0-15
Identifiant constructeur :                  GenuineIntel
  Nom de modèle :                           12th Gen Intel(R) Core(TM) i7-1260P
    Famille de processeur :                 6
    Modèle :                                154
    Thread(s) par cœur :                    2
    Cœur(s) par socket :                    12
    Socket(s) :                             1
    Révision :                              3
    Vitesse maximale du processeur en MHz : 4700.0000
    Vitesse minimale du processeur en MHz : 400.0000
    BogoMIPS :                              4992.00

```

Voici un aperçu des details d'un des coeurs du processeur de la machine utilisée, sachant que le processeu a 16 coeurs :

```bash
miguel@miguel-Yoga-7-14IAL7:~$ cat /proc/cpuinfo
processor       : 0
vendor_id       : GenuineIntel
cpu family      : 6
model           : 154
model name      : 12th Gen Intel(R) Core(TM) i7-1260P
stepping        : 3
microcode       : 0x430
cpu MHz         : 983.942
cache size      : 18432 KB
physical id     : 0
siblings        : 16
core id         : 0
cpu cores       : 12
apicid          : 0
initial apicid  : 0
fpu             : yes
fpu_exception   : yes
cpuid level     : 32
wp              : yes
flags           : fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc art arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf tsc_known_freq pni pclmulqdq dtes64 monitor ds_cpl vmx smx est tm2 ssse3 sdbg fma cx16 xtpr pdcm sse4_1 sse4_2 x2apic movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm 3dnowprefetch cpuid_fault epb ssbd ibrs ibpb stibp ibrs_enhanced tpr_shadow flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid rdseed adx smap clflushopt clwb intel_pt sha_ni xsaveopt xsavec xgetbv1 xsaves split_lock_detect avx_vnni dtherm ida arat pln pts hwp hwp_notify hwp_act_window hwp_epp hwp_pkg_req hfi vnmi umip pku ospke waitpkg gfni vaes vpclmulqdq rdpid movdiri movdir64b fsrm md_clear serialize arch_lbr ibt flush_l1d arch_capabilities
vmx flags       : vnmi preemption_timer posted_intr invvpid ept_x_only ept_ad ept_1gb flexpriority apicv tsc_offset vtpr mtf vapic ept vpid unrestricted_guest vapic_reg vid ple shadow_vmcs ept_mode_based_exec tsc_scaling usr_wait_pause
bugs            : spectre_v1 spectre_v2 spec_store_bypass swapgs eibrs_pbrsb
bogomips        : 4992.00
clflush size    : 64
cache_alignment : 64
address sizes   : 39 bits physical, 48 bits virtual
power management:
```

Les intructions SIMD pouvant être utilisées sur cette architecture sont celles marquées SSE sur https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html.

## Segmentation

Tout en haut du fichier k-means.c se trouvent des `#define` qui permettent d'activer ou desactiver plusieurs optimisations. Ceci facilite le switch entre la version de base et la version optimisée.

Un script `run_seg.sh` a été ajouté au projet et permet de rapidement clean, compiler puis lancer des benchmark avec 5, 10 et 100 clusters sur l'image `sample_640_2.png`.

### Benchmark du fichier d'origine

```bash
Benchmark 1: build/segmentation ../img/sample_640_2.png 5 ../img/sample_640_2_out_5.png
  Time (mean ± σ):      73.2 ms ±   3.5 ms    [User: 56.8 ms, System: 16.2 ms]
  Range (min … max):    69.0 ms …  89.8 ms    40 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 10 ../img/sample_640_2_out_10.png
  Time (mean ± σ):     120.6 ms ±   5.8 ms    [User: 92.3 ms, System: 28.1 ms]
  Range (min … max):   114.9 ms … 143.9 ms    25 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 100 ../img/sample_640_2_out_100.png
  Time (mean ± σ):     897.0 ms ±   7.2 ms    [User: 661.4 ms, System: 235.2 ms]
  Range (min … max):   889.7 ms … 911.8 ms    10 runs
```

### Optimisations avec SIMD
Le partie du code de la segmentation de l'image qui ont été optimisées sont les suivantes :
- Calcul de la distance
- Accumulation des pixels
- Division des pixels
- Copie des pixels

En résumé il s'agit de chaque boucle de calcul dans la fonction `kmeans()` (Voir le code).

#### Distance

Après avoir simd la fonction distance : 
```bash
Benchmark 1: build/segmentation ../img/sample_640_2.png 5 ../img/sample_640_2_out_5.png
  Time (mean ± σ):      73.8 ms ±   2.2 ms    [User: 59.5 ms, System: 14.2 ms]
  Range (min … max):    70.7 ms …  82.0 ms    40 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 10 ../img/sample_640_2_out_10.png
  Time (mean ± σ):     124.8 ms ±   5.3 ms    [User: 97.2 ms, System: 27.3 ms]
  Range (min … max):   119.2 ms … 144.3 ms    23 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 100 ../img/sample_640_2_out_100.png
  Time (mean ± σ):     955.5 ms ±   4.5 ms    [User: 718.2 ms, System: 236.7 ms]
  Range (min … max):   947.2 ms … 961.1 ms    10 runs
```

On voit que le fait d'inclure le SIMD dans la fonction de distance ne nous fait pas gagner de temps.
Probablement du à l'utilisation de vecteurs 128 bits alors qu'on a 4 valeurs de 8 bits. Cependant les instructions disponibles pour l'architecture utilisée ne permettent pas d'utiliser un vecteur de 32 bits.

#### Addition

```bash
Benchmark 1: build/segmentation ../img/sample_640_2.png 5 ../img/sample_640_2_out_5.png
  Time (mean ± σ):      72.5 ms ±   5.3 ms    [User: 57.2 ms, System: 15.2 ms]
  Range (min … max):    69.0 ms … 103.6 ms    41 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 10 ../img/sample_640_2_out_10.png
  Time (mean ± σ):     117.6 ms ±   1.9 ms    [User: 89.9 ms, System: 27.5 ms]
  Range (min … max):   113.8 ms … 121.0 ms    25 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 100 ../img/sample_640_2_out_100.png
  Time (mean ± σ):     899.0 ms ±  14.9 ms    [User: 673.3 ms, System: 225.3 ms]
  Range (min … max):   888.7 ms … 935.3 ms    10 runs
```
La partie qui fait l'accumulation fait gagner un peu de temps pour la segmentation en en 5 et 10 clusters et perd 2 ms pour la segmentation en 100 clusters.

#### Division
La division n'a pas pu être optimisée car elle génère une segfault. Je n'ai pas reussi à trouver une version optimisée qui fonctionne de cette partie du code, cependant le code optimisé a été desactivé.

L'erreur est due à qu'il a été impossible de modifier le code pour avoir 4 components dans l'image ce qui empeche le store, en effet il est obligatoire de store 4 éléments et non 3.

#### Copie
pour la copie la seule chose qui a été optimisée est un code motion. On gagne quelques ms par rapport au fichier de base :
```bash
Benchmark 1: build/segmentation ../img/sample_640_2.png 5 ../img/sample_640_2_out_5.png
  Time (mean ± σ):      72.8 ms ±   3.6 ms    [User: 55.3 ms, System: 17.3 ms]
  Range (min … max):    69.0 ms …  90.1 ms    42 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 10 ../img/sample_640_2_out_10.png
  Time (mean ± σ):     119.6 ms ±   3.3 ms    [User: 91.4 ms, System: 28.0 ms]
  Range (min … max):   113.7 ms … 128.3 ms    25 runs
 
Benchmark 1: build/segmentation ../img/sample_640_2.png 100 ../img/sample_640_2_out_100.png
  Time (mean ± σ):     894.7 ms ±   5.3 ms    [User: 655.1 ms, System: 239.3 ms]
  Range (min … max):   887.8 ms … 906.5 ms    10 runs
```
## Edge detection
Tout en haut du fichier `sobel.c` se trouvent des `#define` qui permettent d'activer ou desactiver plusieurs optimisations. Ceci facilite le switch entre la version de base et la version optimisée.

Un script `run_det.sh` a été ajouté au projet et permet de rapidement clean, compiler puis lancer des benchmark sur la version tableau 1D ainsi que sur la version liste chainée de l'edge detection sur l'image `sample_640_2.png`.

### Sans optimisations
Voici les résultats de l'edge detection du labo 1 sans optimisations :
```bash
Benchmark 1: build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_1d.png 1
  Time (mean ± σ):      60.7 ms ±   2.6 ms    [User: 57.1 ms, System: 3.5 ms]
  Range (min … max):    59.3 ms …  77.3 ms    47 runs
 
Benchmark 1: build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_chained.png 2
  Time (mean ± σ):      1.566 s ±  0.077 s    [User: 1.542 s, System: 0.024 s]
  Range (min … max):    1.517 s …  1.756 s    10 runs
```
### Optimisations
#### Code motions
Voici les résultats après avoir fait des code motions pour eviter de recalculer certaines valeurs dans des boucles :
```bash
Benchmark 1: build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_1d.png 1
  Time (mean ± σ):      60.7 ms ±   1.5 ms    [User: 57.4 ms, System: 3.1 ms]
  Range (min … max):    59.2 ms …  67.6 ms    48 runs
 
Benchmark 1: build/edge_detection ../img/sample_640_2.png ../img/sample_640_2_out_chained.png 2
  Time (mean ± σ):      1.550 s ±  0.043 s    [User: 1.525 s, System: 0.024 s]
  Range (min … max):    1.517 s …  1.631 s    10 runs
```
on peut voir qu'on optimise légèrement les performances de la version liste chainée.
#### Grayscale SIMD
L'optimisation de la fonction grayscale en SIMD n'a pas été concluante car il est impossible de faire une addition horizontale dans l'architecture du PC utilisé.

## Conclusion
Concernant la segmentation en clusters de l'image, l'optimisation en SIMD a permis de gagner quelques ms d'execution. L'optimisation de la distance en SIMD est la seule partie qui a eu des resultats moins bons que l'original.

Les autres fonctions de sobel n'ont pas été optimisées car il est difficile de voir comment vectoriser les datas. Les indices sont parfois négatifs et cela rend compliqué à charger les données dans des vecteurs SIMD.

Voici une comparaison entre toutes les mesures du rapport pour la segmentation et l'edge detection :

### Segmentation

| Méthode / Cluster | Sans SIMD (ms) | SIMD Distance (ms) | SIMD Addition (ms) | SIMD Copie (ms) |
|--------------------|----------------|--------------------|--------------------|-----------------|
| 5                  | 73.2  | 73.8 | 72.5 | 72.8 |
| 10                 | 120.6 | 124.8 | 117.6 | 119.6 |
| 100                | 897.0 | 955.5 | 899.0 | 894.7 |

### Edge Detection

| Méthode / Type | Sans SIMD (ms) | Avec SIMD (ms) |
|----------------|----------------|----------------|
| 1D             | 60.7 | 60.7 |
| Liste Chainée | 1566 | 1550 |
