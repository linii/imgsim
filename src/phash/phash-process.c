// Part 2 of the utility wrapper for the pHash library
// All source code from: http://www.phash.org/
//
// Call this executable like this:
// ./process [ data file ] [ flag ] [ timing flag ] [ query dir1 ] [ query dir2 ] [ ... ]
//
// Where:
//  - data file: output of hash algorithm in the form
//      F [ filename ]
//      [ hash ]
//      F [ filename ]
//      [ ... ]
//  - flag: 1 if DCT hash output file, 0 if radial hash output file
//  - timing flag:
//  - test dir [#]: links to directories containing files you want to compare
//      to the images in the data file
//
// Lining Wang
// 4/29/2015

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libgen.h>
#include <dirent.h>
#include <assert.h>
#include <inttypes.h>
#include <time.h>

#include "lib/pHash.h"

typedef unsigned long long ulong64;     // matches pHash specification

typedef struct hash {                   // Really sad data structure for containing info :(
    char * name;
    ulong64 hash1;
    struct ph_digest * hash2;
    int d1;
    double d2;
} * Hash;


// 0 is perfect match
int compareDCT (const void * arg1, const void * arg2) {
    int l = ((Hash)arg1)->d1;
    int r = ((Hash)arg2)->d1;
    if (l < r)
        return -1;
    else if (l > r)
        return 1;
    return 0;
}

// 1 is perfect match
int compareRadial (const void * arg1, const void * arg2) {
    double l = ((Hash)arg1)->d2;
    double r = ((Hash)arg2)->d2;
    if (l < r)
        return 1;
    else if (l > r)
        return -1;
    return 0;
}


int main(int argc, char ** argv) {
    int datasetSize = 350;      // specific to my dataset
    int numDataSets = 10;

    struct dirent *dp;
    DIR * q;

    if (argc < 3) {
        fprintf(stderr, "Wrong number of arguments");
        return 0;
    }

    int flag = atoi(argv[2]);
    Hash hashes = (Hash ) calloc(2, sizeof(struct hash));
    Hash radialhashes = (Hash) calloc(2, sizeof(struct hash));
    int size = 2;

    // read hashes in from file
    FILE * f;
    char * file = argv[1];
    if ((f = fopen(file, "r")) == NULL) {
        fprintf(stderr, "Can't open %s\n", argv[2]);
        return 0;
    }

    char * line = NULL;
    int read;
    size_t len = sizeof(char) * 30;
    int index = 0;
    const int nb_coeffs = 40; // taken from source code

    while ((read = getline(&line, &len, f)) != -1) {
        if (index >= size - 1) {
            size *= 2;
            hashes = (Hash ) realloc(hashes, sizeof(struct hash) * size);
            radialhashes = (Hash ) realloc(radialhashes, sizeof(struct hash) * size);
        }

        char * filename = (char *) calloc(1, strlen(line));
        for (int i = 2, j = 0; i < strlen(line) - 1; i++, j++)
            filename[j] = line[i];

        int numRead;
        if(flag) {
            hashes[index].name = filename;
            ulong64 hash;
            numRead = fscanf(f, "%lld\n", &hash);
            assert (numRead == 1);
            hashes[index].hash1 = hash;
        }
        else {
            struct ph_digest * dig = static_cast<struct ph_digest * >(calloc(1, sizeof(struct ph_digest)));
            uint8_t * coe = (uint8_t *) calloc(nb_coeffs, sizeof(uint8_t));
            for (int i = 0; i < 40; i++)
                numRead = fscanf(f, "%hhu", coe+i);

            dig->coeffs = coe;
            dig->size = nb_coeffs;
            radialhashes[index].name = filename;
            radialhashes[index].hash2 = dig;
        }
        index++;
    }
    // printf("FINISHED PROCESSING FILES\n");
    bool timer = atoi(argv[3]);
    unsigned long times[numDataSets];

    for (int i = 4; i < argc; i++) {
        char * queries = argv[i];
        printf("\nTESTING: %s\n", queries);

        if ((q = opendir(queries)) == NULL) {
            fprintf(stderr, "Can't open %s\n", argv[i]);
            return 0;
        }
        int numFiles = 0;

        // read in queries and calculates closest distances
        while ((dp = readdir(q)) != NULL) {
            numFiles++;
            char str1[2] = ".";
            char str2[3] = "..";

            if (!strcmp(dp->d_name, str1) || !strcmp(dp->d_name, str2) || dp->d_name[0] == '.')
                continue;

            char * path = (char *) calloc(30, sizeof(char));
            for (int i = 0; i < strlen(queries); i++)
                path[i] = queries[i];

            strcat(path, "/");
            char * name =  strcat(path, dp->d_name);

            Hash distances, radialdistances;
            ulong64 basehash;
            struct ph_digest radialbasehash;

            if (flag) {
                distances = (Hash ) calloc(index, sizeof(struct hash));
                ph_dct_imagehash(name, basehash);
            }
            else {
                radialdistances = (Hash) calloc(index, sizeof(struct hash));
                ph_image_digest(name, 1.0, 1.0, radialbasehash);
            }

            double threshold = 0.8;
            if (!timer) {
                for (int i = 0; i < index; i++) {
                    if (flag) {
                        distances[i].d1 = ph_hamming_distance(basehash, hashes[i].hash1);
                        distances[i].name = hashes[i].name;
                    }
                    else {
                        double pcc;
                        int result = ph_crosscorr(radialbasehash, *radialhashes[i].hash2, pcc, threshold);
                        radialdistances[i].d2 = pcc;
                        radialdistances[i].name = radialhashes[i].name;
                    }
                }
                if(flag)
                    qsort(distances, index, sizeof(struct hash), compareDCT);
                else
                    qsort(radialdistances, index, sizeof(struct hash), compareRadial);

                printf("QUERY: %s", basename(dp->d_name));
                for (int i = 0; i < index && i < 10; i++) {
                    if (flag)
                        printf(" %s\t", distances[i].name);
                    else
                        printf(" %s\t", radialdistances[i].name);
                }
            }
            else { // experimental timing section.. doesn't work yet
                for (int i = 0; i < numDataSets; i++) {
                    double pcc;
                    double msec;
                    clock_t start_t, end_t, total_t;
                    if (flag) {
                        unsigned long start = clock();
                        for (int j = 0; j < i * datasetSize && j < index; j++) {
                            ph_hamming_distance(basehash, hashes[j].hash1);
                        }
                        unsigned long diff = clock() - start;
                        msec = diff * 1000 / CLOCKS_PER_SEC;
                    }
                    else {
                        unsigned long start = clock();
                        for (int j = 0; j < i * datasetSize && j < index; j++) {
                            ph_crosscorr(radialbasehash, *radialhashes[j].hash2, pcc, threshold);
                        }
                        unsigned long diff = clock() - start;
                        msec = (diff * 1000) / CLOCKS_PER_SEC;
                    }
                    times[i] += msec / 1000;
                }
            }
            printf("\n");
            free(distances);
            free(radialdistances);
            free(path);
        }

        if (flag)
            printf("METHOD: DCT Hash");
        else
            printf("METHOD: Radial Hash");

        for (int i = 0; i < numDataSets; i++) {
            printf("Set of size %d took: %lu\n", (i + 1) * datasetSize, times[i] / numFiles);
        }
    }
    free(hashes);
    free(radialhashes);

    return 0;
}
