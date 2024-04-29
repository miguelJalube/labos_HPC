#include "k-means.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {

    printf("AAAAAAAAAAAAA\n");
    struct img_1D_t *img;
    int nb_cluster = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage : %s <img_src.png> <nb_clusters> <img_dest.png>\n", argv[0]);
        return EXIT_FAILURE;
    }

    nb_cluster = atoi(argv[2]);
    if (nb_cluster <= 0){
        printf("The number of clusters should be greater than 0\n");
        return 1;
    }


    printf("BBBBBBBBBBBBB\n");
    img = load_image_1D(argv[1]);

    printf("CCCCCCCCCCCCC\n");
    printf("Image loaded!\n");

    kmeans(img, nb_cluster);

    save_image(argv[3], img);

    free_image(img);

    printf("Programm ended successfully\n\n");

    return 0;
}
