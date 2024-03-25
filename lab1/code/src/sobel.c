#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "image.h"
#include "sobel.h"
#include <likwid-marker.h>

#define GAUSSIAN_KERNEL_SIZE    3
#define SOBEL_KERNEL_SIZE       3
#define SOBEL_BINARY_THRESHOLD  150  // in the range 0 to uint8_max (255)


//#define DEBUG 1
//#define DEBUG_GAUSS 1

const int16_t sobel_v_kernel[SOBEL_KERNEL_SIZE*SOBEL_KERNEL_SIZE] = {
    -1, -2, -1,
     0,  0,  0,
     1,  2,  1,
};

const int16_t sobel_h_kernel[SOBEL_KERNEL_SIZE*SOBEL_KERNEL_SIZE] = {
    -1,  0,  1,
    -2,  0,  2,
    -1,  0,  1,
};

const uint16_t gauss_kernel[GAUSSIAN_KERNEL_SIZE*GAUSSIAN_KERNEL_SIZE] = {
    1, 2, 1,
    2, 4, 2,
    1, 2, 1,
};

struct img_1D_t *edge_detection_1D(const struct img_1D_t *input_img){
    struct img_1D_t *res_img1, *res_img2;
	LIKWID_MARKER_INIT;

    //TODO/

    res_img1 = allocate_image_1D(input_img->width,input_img->height, 1);    
    res_img2 = allocate_image_1D(input_img->width,input_img->height, 1);

	LIKWID_MARKER_START("grayscale");
    rgb_to_grayscale_1D(input_img, res_img1);
	LIKWID_MARKER_STOP("grayscale");
    
	//LIKWID_MARKER_START("gaussian");
    gaussian_filter_1D(res_img1, res_img2, gauss_kernel);
	//LIKWID_MARKER_STOP("gaussian");
    
	//LIKWID_MARKER_START("sobel");
    sobel_filter_1D(res_img2, res_img1, sobel_v_kernel, sobel_h_kernel);
	//LIKWID_MARKER_STOP("sobel");

    LIKWID_MARKER_CLOSE;
    free(res_img2);

    return res_img1;
}

/**
 * Prenez un à un les pixels et stockez-les dans une nouvelle image. Pour
 * la conversion, vous devrez multiplier chaque composante R, G et B par leur facteur respectif et
 * les additionner les uns aux autres pour obtenir une nuance de gris.
 * 
 * 
 *  struct img_1D_t {
        int width;
        int height;
        int components;

        uint8_t *data;
    };
 * 
 */

void rgb_to_grayscale_1D(const struct img_1D_t *img, struct img_1D_t *result){
    int index = 0;
    
    for(int i = 0; i < img->height; ++i){
        for(int j = 0; j < img->width; ++j){
            uint8_t r = img->data[index + R_OFFSET];
            uint8_t g = img->data[index + G_OFFSET];
            uint8_t b = img->data[index + B_OFFSET];
            result->data[j + i * img->width] = r*FACTOR_R + g*FACTOR_G + b*FACTOR_B;
            index += img->components;
        }
    }
}

/**
 * Appliquez le filtre gaussien sur les pixels qui ne sont pas présents dans
 * les bords, pour ceux dans les bords, copiez simplement leur valeur actuelle. Ensuite, multipliez
 * chaque pixel par la matrice et additionnez leurs valeurs, puis finalement divisez la valeur totale
 * par la pondération gaussienne proposée. 
 * 
*/

void gaussian_filter_1D(const struct img_1D_t *img, struct img_1D_t *res_img, const uint16_t *kernel){
    const uint16_t gauss_ponderation = 16;
    
    for(int i = 0; i < img->height; ++i){
        for(int j = 0; j < img->width; ++j){
            int value = 0;
            // On edges
            if(i == 0 || j == 0 || i == img->height - 1 || j == img->width - 1){
                value = img->data[j + i * img->width];
            }
            // No on edges
            else{
                int ctn = 0;
                for (int x = -1; x < 2; ++x){
                    for(int y = -1; y < 2; ++y){
                        value += kernel[ctn] * img->data[(j+y) + (i+x) * img->width];
                        ++ctn;
                    }
                }
                value /= gauss_ponderation;
            }
            res_img->data[j + i * res_img->width] = value;
        }
    }
}

/**
 * Appliquez le filtre de Sobel sur les pixels qui ne sont pas présents dans les
 * bords, pour ces derniers, copiez simplement leur valeur actuelle. Ensuite, sur chaque pixel,
 * appliquez la matrice horizontale (résultat H) et verticale (résultat V) comme pour l’application
 * d’un filtre gaussien, c’est-à-dire la somme de chaque pixel multiplié par la matrice superposée.
 * Pour décider si un pixel est un "bord", il vous suffira de calculer la norme des résultat H et V
 * et de les comparé au seuil. Si cette dernière est supérieure au seuil donné dans le code, vous
 * pourrez définir le pixel comme noir, sinon blanc.
*/
void sobel_filter_1D(const struct img_1D_t *img, struct img_1D_t *res_img, const int16_t *v_kernel, const int16_t *h_kernel){
    for(int i = 0; i < img->height; ++i){
        for(int j = 0; j < img->width; ++j){
                int value = 0;
                // On edges
                if(i == 0 || j == 0 || i == img->height - 1 || j == img->width - 1){
                    value = img->data[j + i * img->width];
                }
                // No on edges
                else{
                    int h_result =  0;
                    int v_result = 0;

                    int ctn = 0;
                    for (int x = -1; x < 2; ++x){
                        for(int y = -1; y < 2; ++y){
                            h_result += h_kernel[ctn] * img->data[(j+y) + (i+x) * img->width];
                            v_result += v_kernel[ctn] * img->data[(j+y) + (i+x) * img->width];
                            ++ctn;
                        }
                    }

                    if(sqrt(h_result * h_result + v_result * v_result) >= SOBEL_BINARY_THRESHOLD){
                        value = 255;
                    }
                }
            res_img->data[j + i * img->width] = value;
        }
    }
}


struct img_chained_t *edge_detection_chained(const struct img_chained_t *input_img){
    struct img_chained_t *res_img1, *res_img2, *res_img3;
    //TODO

    res_img1 = allocate_image_chained(input_img->width,input_img->height, 1);    
    res_img2 = allocate_image_chained(input_img->width,input_img->height, 1);   
    res_img3 = allocate_image_chained(input_img->width,input_img->height, 1);

    #ifdef DEBUG
    printf("Start...\n");
    #endif

    rgb_to_grayscale_chained(input_img, res_img1);
    #ifdef DEBUG
    save_image_chained("../images/inter_grey.png",res_img1);
    printf("rgb_to_grayscale_chained() done\n");    
    #endif

    gaussian_filter_chained(res_img1, res_img2, gauss_kernel);
    #ifdef DEBUG
    save_image_chained("../images/inter_gauss.png",res_img2);
    printf("gaussian_filter_chained() done\n");    
    #endif
    
    sobel_filter_chained(res_img2, res_img3, sobel_v_kernel, sobel_h_kernel);
    #ifdef DEBUG
    save_image_chained("../images/inter_sobel.png",res_img3);
    printf("sobel_filter_chained() done\n");
    #endif

    free(res_img1);
    free(res_img2);

    return res_img3;
}

void rgb_to_grayscale_chained(const struct img_chained_t *img, struct img_chained_t *result){
    struct pixel_t *px      = img->first_pixel;
    struct pixel_t *res_px  = result->first_pixel;
    for(int i = 0; i < img->height*img->width; ++i){
        uint8_t r = px->pixel_val[0];
        uint8_t g = px->pixel_val[1];
        uint8_t b = px->pixel_val[2];
        px = px->next_pixel;

        res_px->pixel_val[0] = (uint8_t)(r*FACTOR_R + g*FACTOR_G + b*FACTOR_B);
        res_px  = res_px->next_pixel;
    }
}

void gaussian_filter_chained(const struct img_chained_t *img, struct img_chained_t *res_img, const uint16_t *kernel) {
    const uint16_t gauss_ponderation = 16;

    struct pixel_t *res_px = res_img->first_pixel;
    struct pixel_t *px = img->first_pixel;
    struct pixel_t *history = px;

    int counter = 0;
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int result = 0;

            // Si on est sur les bords
            if(i == 0 || j == 0 || i == img->height - 1 || j == img->width - 1){
                result = px->pixel_val[0];

            // si non
            }else{
                struct pixel_t* tmp = history;

                // Calcul du kernel avec les voisins
                // Parcours du kernel
                for (int x = 0; x < GAUSSIAN_KERNEL_SIZE*GAUSSIAN_KERNEL_SIZE; ++x){

                    // Si on est en bout de ligne du kernel
                    if(x % GAUSSIAN_KERNEL_SIZE == 0){

                        // on skip n = (width - kernel_width + 1) pixels de l'image pour avoir le voisin de la ligne suivante
                        for(int y = 0; y < img->width - GAUSSIAN_KERNEL_SIZE; ++y){
                            if(tmp->next_pixel != NULL){
                                tmp = tmp->next_pixel;
                            }
                        }
                    }

                    // On cumule le resultat
                    result += kernel[x] * tmp->pixel_val[0];

                    // On avance au pixel suivant
                    if(tmp->next_pixel != NULL){
                        tmp = tmp ->next_pixel;
                    }
                }

                // On divise pas la pondération
                result /= gauss_ponderation;
            }

            // On stocke le resultat
            res_px->pixel_val[0] = result;

            // Afin de maintenir history toujours en haut à gauche de px
            if ( !(
                    i == 0                  || (i == 1 && j == 0) ||
                    i == img->height - 1    || (j == (img->width - 1) && i == (img->height - 2))
            )) {
                if(history->next_pixel != NULL){
                    history = history->next_pixel;
                }
            }

            // On passe au px suivant
            if(px->next_pixel != NULL){
                px = px->next_pixel;
            }
            
            if(res_px->next_pixel != NULL){
                res_px = res_px->next_pixel;
            }
            ++counter;
        }
        #ifdef DEBUG_GAUSS
        printf("\rProgression : i => %4d/%4d", i, img->height);
        fflush(stdout);
        #endif
    }
    #ifdef DEBUG_GAUSS
    printf("\n");
    #endif
}


void sobel_filter_chained(const struct img_chained_t *img, struct img_chained_t *res_img, const int16_t *v_kernel, const int16_t *h_kernel){
    struct pixel_t *res_px = res_img->first_pixel;
    struct pixel_t *px = img->first_pixel;
    struct pixel_t *history = px;

    int counter = 0;
    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            int result = 0;
            int v_result = 0;
            int h_result = 0;

            // Si on est sur les bords
            if(i == 0 || j == 0 || i == img->height - 1 || j == img->width - 1){
                result = px->pixel_val[0];

            // si non
            }else{
                struct pixel_t* tmp = history;

                // Calcul du kernel avec les voisins
                // Parcours du kernel
                for (int x = 0; x < GAUSSIAN_KERNEL_SIZE*GAUSSIAN_KERNEL_SIZE; ++x){

                    // Si on est en bout de ligne du kernel
                    if(x % GAUSSIAN_KERNEL_SIZE == 0){

                        // on skip n = (width - kernel_width + 1) pixels de l'image pour avoir le voisin de la ligne suivante
                        for(int y = 0; y < img->width - GAUSSIAN_KERNEL_SIZE; ++y){
                            if(tmp->next_pixel != NULL){
                                tmp = tmp->next_pixel;
                            }
                        }
                    }

                    // On cumule le resultat
                    v_result += v_kernel[x] * tmp->pixel_val[0];
                    h_result += h_kernel[x] * tmp->pixel_val[0];


                    // On avance au pixel suivant
                    if(tmp->next_pixel != NULL){
                        tmp = tmp ->next_pixel;
                    }
                }

                // On divise pas la pondération
                if(sqrt(h_result * h_result + v_result * v_result) >= SOBEL_BINARY_THRESHOLD){
                    result = 255;
                }
            }

            // On stocke le resultat
            res_px->pixel_val[0] = result;

            // Afin de maintenir history toujours en haut à gauche de px
            if ( !(
                    i == 0                  || (i == 1 && j == 0) ||
                    i == img->height - 1    || (j == (img->width - 1) && i == (img->height - 2))
            )) {
                if(history->next_pixel != NULL){
                    history = history->next_pixel;
                }
            }

            // On passe au px suivant
            if(px->next_pixel != NULL){
                px = px->next_pixel;
            }
            
            if(res_px->next_pixel != NULL){
                res_px = res_px->next_pixel;
            }
            ++counter;
        }
        #ifdef DEBUG_GAUSS
        printf("\rProgression : i => %4d/%4d", i, img->height);
        fflush(stdout);
        #endif
    }
    #ifdef DEBUG_GAUSS
    printf("\n");
    #endif
}
