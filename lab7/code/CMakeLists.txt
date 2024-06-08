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
