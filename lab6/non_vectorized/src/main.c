#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../powercap/inc/powercap.h"
#include "../powercap/inc/powercap-sysfs.h"
#include "../powercap/inc/powercap-rapl.h"
#include "../powercap/inc/powercap-rapl-sysfs.h"
#include "../powercap/src/powercap-common.h"

const char* to_string(powercap_rapl_zone zone) {
    switch (zone) {
        case POWERCAP_RAPL_ZONE_PACKAGE :
            return "PACKAGE";
            break;
        case POWERCAP_RAPL_ZONE_CORE :
            return "CORE";
            break;
        case POWERCAP_RAPL_ZONE_UNCORE :
            return "UNCORE";
            break;
        case POWERCAP_RAPL_ZONE_DRAM :
            return "DRAM";
            break;
        case POWERCAP_RAPL_ZONE_PSYS :
            return "PSYS";
            break;    
        default:
            return "UNKNOWN";
            break;
    }
}

int main(int argc, char** argv){
    uint64_t *energy_uj1, *energy_uj2;

    // On récupère le nombre de package disponible dans l’architecture
    uint32_t npackages = powercap_rapl_get_num_instances();
    
    energy_uj1 = (uint64_t *)malloc(npackages * sizeof(uint64_t));
    energy_uj2 = (uint64_t *)malloc(npackages * sizeof(uint64_t));

    // On créer une instance pour chaque package
    powercap_rapl_pkg pkg[npackages];
    // On défini la zone que l’on souhaite analyser, ici psys
    powercap_rapl_zone zone = POWERCAP_RAPL_ZONE_PSYS;
    // On vérifie si les packages supporte la zone qu’on souhaite analysée.
    bool supported[npackages];
    bool has_one_supported = false;
    for (size_t i = 0; i < npackages; i++) {
        supported[i] = powercap_rapl_is_zone_supported(&pkg[i], zone);
        if (supported[i] != 1){
            printf("RAPL is not supported on package %ld\n", i);
        } else {
            has_one_supported = true;
        }
    }
    if (!has_one_supported) {
        printf("No supported package for %s\n", to_string(zone));
        return EXIT_FAILURE;
    }
    // Maintenant on lance la mesure de consommation énergétique
    for(size_t j = 0; j < npackages; ++j){
        if (supported[j]){
            if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj1[j])) {
                printf(" Failed to get energy on package %ld\n", j);
                break;
            }
        }
    }
    // CODE MESURER
    // On remesure la consomation énergétique
    for (size_t j = 0; j < npackages; j++) {
        if (supported[j]){
            if (powercap_rapl_get_energy_uj(&pkg[j], zone, &energy_uj2[j])) {
                printf(" failed to get energy on package %ld\n", j);
                break;
            }
        }
    }
    for (size_t i = 0; i < npackages; i++) {
        powercap_rapl_destroy(&pkg[i]);
    }
    return EXIT_SUCCESS;
}
