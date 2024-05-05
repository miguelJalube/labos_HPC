#include "k-means.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmmintrin.h>
#include <emmintrin.h>

//#define DISTANCE 1
#define ADDITION 1
//#define DIVISION 1
#define COPY 1

void print128_num(__m128 var)
{
    int val[4];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %i %i %i %i\n", val[0], val[1], val[2], val[3]);
}

void print128_numf(__m128 var)
{
    float val[4];
    memcpy(val, &var, sizeof(val));
    printf("Numerical: %f %f %f %f\n", val[0], val[1], val[2], val[3]);
}


// This function will calculate the euclidean distance between two pixels.
// Instead of using coordinates, we use the RGB value for evaluating distance.
float distance(uint8_t* p1, uint8_t* p2){
    #ifndef DISTANCE
    float r_diff = p1[0] - p2[0];
    float g_diff = p1[1] - p2[1];
    float b_diff = p1[2] - p2[2];
    return sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
    #endif

    #ifdef DISTANCE
    float val[4];
    __m128 v1, v2, v3;
    
    v1 = _mm_set_ps(p1[0], p1[1], p1[2], 0);
    v2 = _mm_set_ps(p2[0], p2[1], p2[2], 0);
    v3 = _mm_sub_ps(v1, v2);
    v3 = _mm_mul_ps(v3, v3);
    _mm_store_ps(val, v3);

    return sqrt(val[0] + val[1] + val[2] + val[3]);
    #endif
}

// Function to initialize cluster centers using the k-means++ algorithm
void kmeans_pp(struct img_1D_t *image, int num_clusters, uint8_t *centers){

    int n = image->width * image->height;
    int t = image->components * sizeof(uint8_t);

    // Select a random pixel as the first cluster center
    int first_center = (rand() % (n)) * image->components; 

    // Set the RGB values of the first center
    centers[R_OFFSET] = image->data[first_center + R_OFFSET];
    centers[G_OFFSET] = image->data[first_center + G_OFFSET];
    centers[B_OFFSET] = image->data[first_center + B_OFFSET];

    float *distances = (float *)malloc(n * sizeof(float));

    // Calculate distances from each pixel to the first center
    uint8_t* dest = malloc(t);
    memcpy(dest, centers, t);

    for(int i = 0; i < n; ++i){

        int x = i * image->components;
        uint8_t* src = malloc(t);
        memcpy(src, image->data + x, t);
        
        distances[i] = distance(src, dest) * distance(src, dest);

        free(src); 
    }
    free(dest);

    // Loop to find remaining cluster centers
    for (int i = 1; i < num_clusters; ++i){
        int x = i * image->components;
        float total_weight = 0.0;

        // Calculate total weight (sum of distances)
        for (int j = 0; j < n; ++j){
            total_weight += distances[i];
        }

        float r = ((float)rand() / (float)RAND_MAX) * total_weight;
        int index = 0;

        // Choose the next center based on weighted probability
        while (r > distances[index]) {
            r -= distances[index];
            index++;
        }

        // Set the RGB values of the selected center
        centers[x + R_OFFSET] = image->data[index * image->components + R_OFFSET];
        centers[x + G_OFFSET] = image->data[index * image->components + G_OFFSET];
        centers[x + B_OFFSET] = image->data[index * image->components + B_OFFSET];

        // Update distances based on the new center
        uint8_t *new_center = malloc(t);
        memcpy(dest, centers + x, t);

        for (int j = 0; j < n; j++) {
            uint8_t* src = malloc(t);
            memcpy(src, image->data + j * image->components, t);
            
            float dist = distance(src, new_center) * distance(src, new_center);

            if (dist < distances[j]) {
                distances[j] = dist;
            }

            free(src);
        }

        free(new_center);
    }

    free(distances);
}

// This function performs k-means clustering on an image.
// It takes as input the image, its dimensions (width and height), and the number of clusters to find.
void kmeans(struct img_1D_t *image, int num_clusters){

    float *centers = calloc(num_clusters * image->components, sizeof(float));
    printf("components : %d\n", image->components);

    // Initialize the cluster centers using the k-means++ algorithm.
    kmeans_pp(image, num_clusters, centers);

    int n = image->width * image->height;

    int *assignments = (int *)malloc(n * sizeof(int));

    // Assign each pixel in the image to its nearest cluster.
    for (int i = 0; i < n; ++i){
        float min_dist = INFINITY;
        int best_cluster = -1;

        uint8_t* src = malloc(image->components * sizeof(uint8_t));
        memcpy(src, image->data + i * image->components, image->components * sizeof(uint8_t));

        for (int c = 0; c < num_clusters; c++){
            uint8_t* dest = malloc(image->components * sizeof(uint8_t));
            memcpy(dest, centers + c * image->components, image->components * sizeof(uint8_t));

            float dist = distance(src, dest);

            if (dist < min_dist) {
                min_dist = dist;
                best_cluster = c;
            }

            assignments[i] = best_cluster;
        }
    }

    ClusterData *cluster_data = (ClusterData *)calloc(num_clusters, sizeof(ClusterData));

    // A OPTIMISER
    // Compute the sum of the pixel values for each cluster.
    #ifndef ADDITION
    for (int i = 0; i < n; ++i){
        int cluster = assignments[i];
        cluster_data[cluster].count++;
        cluster_data[cluster].sum_r += (int)image->data[i * image->components + R_OFFSET];
        cluster_data[cluster].sum_g += (int)image->data[i * image->components + G_OFFSET];
        cluster_data[cluster].sum_b += (int)image->data[i * image->components + B_OFFSET];
    }
    #endif
    #ifdef ADDITION
    __m128i v1, v2, res;
    for (int i = 0; i < n; ++i)
    {
        int cluster = assignments[i];

        v1 = _mm_set_epi32(
            cluster_data[cluster].sum_b,
            cluster_data[cluster].sum_g,
            cluster_data[cluster].sum_r,
            cluster_data[cluster].count);

        v2 = _mm_set_epi32(
            (int)image->data[i * image->components + B_OFFSET],
            (int)image->data[i * image->components + G_OFFSET],
            (int)image->data[i * image->components + R_OFFSET],
            1);

        res = _mm_add_epi32(v1, v2);

        _mm_storeu_si128((__m128i *)&cluster_data[cluster].count, res);
    }


    #endif

    // A OPTIMISER
    // Update cluster centers based on the computed sums
    #ifndef DIVISION
    for (int c = 0; c < num_clusters; ++c){
        //printf("cluster data: \n");
        //printf("1 : %f \n", cluster_data[c].count);
        //printf("2 : %f \n", cluster_data[c].sum_r);
        //printf("3 : %f \n", cluster_data[c].sum_g);
        //printf("4 : %f \n", cluster_data[c].sum_b);
        if (cluster_data[c].count > 0){
            centers[c * image->components + R_OFFSET] = cluster_data[c].sum_r / cluster_data[c].count;
            centers[c * image->components + G_OFFSET] = cluster_data[c].sum_g / cluster_data[c].count;
            centers[c * image->components + B_OFFSET] = cluster_data[c].sum_b / cluster_data[c].count;
        }
    }
    #endif
    #ifdef DIVISION

    /*
        // 128 bits
        typedef struct {
            float count;
            float sum_r;
            float sum_g;
            float sum_b;
        } ClusterData;
    */
    __m128 v1, v2, res;
    for (int c = 0; c < num_clusters; ++c){
        if (cluster_data[c].count > 0){
            v1 = _mm_set_ps(cluster_data[c].count, cluster_data[c].sum_r, cluster_data[c].sum_g, cluster_data[c].sum_b);
            v2 = _mm_set_ps(1, cluster_data[c].count, cluster_data[c].count, cluster_data[c].count);
            res = _mm_div_ps(v1, v2);
            _mm_store_ps(&centers[c*image->components], v1);
        }
    }


    #endif

    free(cluster_data);

    // A OPTIMISER
    // Update image data with the cluster centers
    #ifndef COPY
    for (int i = 0; i < image->width * image->height; ++i){
        int cluster = assignments[i];
        image->data[i * image->components + R_OFFSET] = centers[cluster * image->components + R_OFFSET];
        image->data[i * image->components + G_OFFSET] = centers[cluster * image->components + G_OFFSET];
        image->data[i * image->components + B_OFFSET] = centers[cluster * image->components + B_OFFSET];
    }
    #endif
    #ifdef COPY
    for (int i = 0; i < n; ++i){
        int cluster = assignments[i];
        int x = i * image->components;
        image->data[x + R_OFFSET] = centers[cluster * image->components + R_OFFSET];
        image->data[x + G_OFFSET] = centers[cluster * image->components + G_OFFSET];
        image->data[x + B_OFFSET] = centers[cluster * image->components + B_OFFSET];
    }
    #endif

    free(assignments);
    free(centers);
}