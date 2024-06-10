# Labo 7 HPC - Miguel Jalube - 09.06.2024

## Objectif
L’objectif de ce laboratoire est de se familiariser avec l'outil de profiling `coz` et de comprendre comment il fonctionne. Pour cela on va utiliser un programme de recherche d'une séquence de chiffres dans un fichier texte. On va profiler le programme avec `coz` et analyser les résultats obtenus.
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

## Parallelisation

1. Division du Fichier en Chunks

Le fichier est divisé en plusieurs segments (chunks) que les différents threads vont traiter en parallèle. Cette division est faite en calculant la taille totale du fichier et en la divisant par le nombre de threads disponibles.

```c

fseek(file, 0, SEEK_END);
long filesize = ftell(file);
long chunk_size = filesize / omp_get_max_threads();
```

2. Définition d'une Fonction pour Traiter un Chunk

Une nouvelle fonction find_sequence_in_chunk a été définie pour rechercher la séquence dans une partie spécifique (chunk) du fichier. Cette fonction prend en argument un pointeur vers le fichier, la séquence à rechercher, et les positions de début (start) et de fin (end) du chunk.

```c

int find_sequence_in_chunk(FILE *file, const char *sequence, long start, long end) {
    size_t sequence_length = strlen(sequence);
    size_t match_count = 0;
    int ch;

    fseek(file, start, SEEK_SET);
    while (ftell(file) < end && (ch = fgetc(file)) != EOF) {
        COZ_PROGRESS;
        if (ch == sequence[match_count]) {
            match_count++;
            if (match_count == sequence_length) {
                return 1;
            }
        } else {
            match_count = (ch == sequence[0]) ? 1 : 0;
        }
    }

    return 0;
}
```

3. Utilisation d'OpenMP pour le Traitement en Parallèle

La recherche de la séquence dans le fichier est parallélisée en utilisant OpenMP. Chaque thread est assigné à un chunk du fichier, et la recherche dans chaque chunk se fait indépendamment.

```c

#pragma omp parallel shared(found)
{
    int tid = omp_get_thread_num();
    long start = tid * chunk_size;
    long end = (tid == omp_get_max_threads() - 1) ? filesize : start + chunk_size;

    if (found == 0) {
        FILE *file_chunk = fopen(filename, "r");
        if (file_chunk != NULL) {
            int result = find_sequence_in_chunk(file_chunk, sequence, start, end);
            fclose(file_chunk);
            if (result) {
                #pragma omp critical
                {
                    found = 1;
                }
            }
        }
    }
    COZ_PROGRESS;
}
```

4. Protection des Accès Concurrentiels

L'utilisation de la directive #pragma omp critical garantit que la variable found est mise à jour de manière atomique, évitant ainsi les conditions de course. Cela assure qu'une fois qu'un thread trouve la séquence, les autres threads cessent leur recherche le plus tôt possible.

```c

if (result) {
    #pragma omp critical
    {
        found = 1;
    }
}
```

5. Synchronisation et Fermeture des Fichiers

Chaque thread ouvre et ferme indépendamment son propre pointeur de fichier pour lire son chunk respectif. Le fichier principal est fermé après la fin de la recherche.

## Compilation
La première chose à faire dans le projet est de compiler le programme find_sequence. Pour cela il faut modifier le CMakeLists.txt pour ajouter les flags de compilation puis le programme multithreadé. Voici les modifications apportées au fichier CMakeLists.txt :

```cmake
cmake_minimum_required(VERSION 3.10)
project(FindSequence)

set(CMAKE_C_STANDARD 99)

# Ajouter les options de compilation pour le débogage
add_compile_options(-g -gdwarf-3)

# Ajouter la prise en charge d'OpenMP
find_package(OpenMP REQUIRED)

add_executable(find_sequence src/main.c)
add_executable(find_sequence_p src/mainomp.c)

# Lier la bibliothèque OpenMP à find_sequence_p
target_link_libraries(find_sequence_p OpenMP::OpenMP_C)

# Cible générique pour find_sequence si nécessaire
target_link_libraries(find_sequence m)
```

Pour compiler le programme il faut executer les commandes suivantes :
```bash
$ cmake .
$ make
```

afin de profiler le programme avec `coz` il faut executer la commande suivante :
```bash
## Profiler le programme multithreadé
$ coz run --- ./find_sequence_p inputs/1mtxt 0198

## Profiler le programme non multithreadé
$ coz run --- ./find_sequence inputs/1mtxt 0198
```

## Analyse des résultats

Après avoir posé deux COZ_PROGRESS dans le programme multithreadé, on execute le script `measureXX.sh`, on obtient les résultats suivants :

### fichier de 1M

<img src="images/mainomp1m.png">

on peut voir que si l'on accelere la ligne 14 (qui est la condition de la boucle while), on peut faire gagner au programme ~60% de temps d'exeution.

C'est comme ça que j'interprete les résultats obtenus dans le graphique.

### fichier de 10M

<img src="images/mainomp10m.png">

on peut voir que c'est exactement la même chose que pour le fichier de 1M, ce qui est logique puisque les COZ_PROGRESS sont placés aux mêmes endroits dans le programme.

## Conclusion
Comme on peut le voir dans les graphiques, le programme multithreadé est plus rapide que le programme non multithreadé. Cependant, il y a encore des améliorations à apporter au programme multithreadé pour le rendre encore plus rapide. C'est pourquoi il est important de profiler le programme pour identifier les parties du programme qui peuvent être améliorées.
