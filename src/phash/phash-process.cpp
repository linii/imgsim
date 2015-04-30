// Call this executable like this:
// ./process [ data file ] [ flag ] [ query dir 1 ] [ query dir 2 ] [ ... ]
//
// Where:
//  - data file: output of hash algorithm in the form
//      F [ filename ]
//      [ hash ]
//      F [ filename ]
//      [ ... ]
//  - flag: 1 if DCT hash output file, 0 if radial hash output file
//  - test dir 1: links to query directories

#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "pHash.h"
#include <assert.h>
#include <inttypes.h>

typedef unsigned long long ulong64;

typedef struct hash {
    char * name;
    ulong64 hash1;
    struct ph_digest * hash2;
    int d1;
    double d2;
} * Hash;

// typedef struct ph_digest {
//     char *id;                   //hash id
//     uint8_t *coeffs;            //the head of the digest integer coefficient array
//     int size;                   //the size of the coeff array
// } Digest;


int compareDCT (const void * arg1, const void * arg2) {
    int l = ((Hash)arg1)->d1;
    int r = ((Hash)arg2)->d1;
    if (l < r)
        return -1;
    else if (l > r)
        return 1;
    return 0;
}

int compareRadial (const void * arg1, const void * arg2) {
    int l = ((Hash)arg1)->d2;
    int r = ((Hash)arg2)->d2;
    if (l < r)
        return -1;
    else if (l > r)
        return 1;
    return 0;
}


int main(int argc, char ** argv) {
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
        // printf("about to insert %s at index %d\n", filename, index);

        int numRead;
        if(flag) {
            hashes[index].name = filename;
            ulong64 hash;
            numRead = fscanf(f, "%lld\n", &hash);
            assert (numRead == 1);

            // printf("read in %lld\n", hash);
            hashes[index].hash1 = hash;
        }
        else {
            radialhashes[index].name = filename;
            struct ph_digest * dig = static_cast<struct ph_digest * >(calloc(1, sizeof(struct ph_digest)));

            char * id;
            uint8_t * coeffs;
            int size;

            numRead = fscanf(f, "%s %d\n", id, &size);
            assert (numRead == 2);

            uint8_t * coe = (uint8_t *) calloc(nb_coeffs, sizeof(uint8_t));

            int numRead = getline(&line, &len, f);
            for (int i = 0; i < nb_coeffs; i++) {
                numRead = sscanf(line, "%" SCNu8, coe+i);
                assert(numRead == 1);
            }
            char c = getchar();
            assert(c == '\n');

            for (int i = 0; i < nb_coeffs; i++)
                 printf("%" PRIu8 " ", coe[i]);

            dig->id = id;
            dig->coeffs = coeffs;
            dig->size = size;
            radialhashes[index].hash2 = dig;
        }
        index++;
    }

    for (int i = 3; i < argc; i++) {
        char * queries = argv[i];
        printf("TESTING: %s\n", queries);

        if ((q = opendir(queries)) == NULL) {
            fprintf(stderr, "Can't open %s\n", argv[i]);
            return 0;
        }
        // read in queries and calculates closest distances
        while ((dp = readdir(q)) != NULL) {
            char str1[2] = ".";
            char str2[3] = "..";

            if (!strcmp(dp->d_name, str1) || !strcmp(dp->d_name, str2) || dp->d_name[0] == '.')
                continue;

            char * path = (char *) calloc(30, sizeof(char));
            for (int i = 0; i < strlen(queries); i++)
                path[i] = queries[i];

            strcat(path, "/");
            char * name =  strcat(path, dp->d_name);
            // printf("PROCESSING , %s\n", dp->d_name);

            Hash distances = (Hash ) calloc(index, sizeof(struct hash));
            Hash radialdistances = (Hash) calloc(index, sizeof(struct hash));
            struct ph_digest * radialbasehash = 0;
            ulong64 basehash;

            if (flag) {
                ph_dct_imagehash(name, basehash);
            }
            else {
                ph_image_digest(name, 1.0, 1.0, *radialbasehash, 180);
            }

            double threshold = 0.90;
            for (int i = 0; i < index; i++) {
                if (flag) {
                    distances[i].d1 = ph_hamming_distance(basehash, hashes[i].hash1);
                    distances[i].name = hashes[i].name;
                }
                else {
                    double pcc;
                    ph_crosscorr(*radialbasehash, *radialhashes[i].hash2, pcc, threshold);
                    radialdistances[i].d2 = pcc;
                    radialdistances[i].name = radialhashes[i].name;
                }
            }

            if(flag) {
                qsort(distances, index, sizeof(struct hash), compareDCT);
                printf("METHOD: DCT Image Hash\n");
            }
            else {
                // quick_sort(radialdistances, index, 0);
                qsort(radialdistances, index, sizeof(struct hash), compareRadial);
                printf("METHOD: RADISH (Radial Image Hash) \n");
            }

            printf("QUERY: %s", basename(dp->d_name));
            for (int i = 0; i < index && i < 10; i++) {
                if (flag) {
                    printf(" %s\t", distances[i].name);
                }
                else {
                    printf(" %s", radialdistances[i].name);
                }
            }
            if (i != argc - 1)
                printf("\n");
            free(distances);
            free(radialdistances);
            free(path);
        }
        free(hashes);
        free(radialhashes);
    }

    return 0;
}
