#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pHash.h"
#include <assert.h>
#include <inttypes.h>

typedef unsigned long long ulong64;

typedef struct hash {
    char * name;
    ulong64 hash1;
    Digest hash2;
    int d1;
    double d2;
} * Hash;

// typedef struct ph_digest {
//     char *id;                   //hash id
//     uint8_t *coeffs;            //the head of the digest integer coefficient array
//     int size;                   //the size of the coeff array
// } Digest;

void quick_sort (Hash a, int n, int flag) {
    int i, j;
    Hash p, t;
    if (n < 2)
        return;
    p = &a[n / 2];

    if (flag){
        for (i = 0, j = n - 1;; i++, j--) {
            while (a[i].d1 < p->d1)
                i++;
            while (p->d1 < a[j].d1)
                j--;
            if (i >= j)
                break;
            t = &a[i];
            a[i].d1 = a[j].d1;
            a[i].name = a[j].name;

            a[j].d1 = t->d1;
            a[j].name = t->name;
        }
        quick_sort(a, i, flag);
        quick_sort(a + i, n - i, flag);
    }
    else {
        for (i = 0, j = n - 1;; i++, j--) {
            while (a[i].d2 < p->d2)
                i++;
            while (p->d2 < a[j].d2)
                j--;
            if (i >= j)
                break;
            t = &a[i];
            a[i].d2 = a[j].d2;
            a[i].name = a[j].name;

            a[j].d2 = t->d2;
            a[j].name = t->name;
        }
        quick_sort(a, i, flag);
        quick_sort(a + i, n - i, flag);
    }

}


int main(int argc, char ** argv) {
    struct dirent *dp;
    DIR * q;
    char * queries = argv[1];

    int flag = atoi(argv[3]);

    Hash hashes = (Hash ) calloc(2, sizeof(struct hash));
    Hash radialhashes = (Hash) calloc(2, sizeof(struct hash));
    int size = 2;

    // read hashes in from file
    FILE * f;
    char * file = argv[2];
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
        printf("read line %s", line);
        if (index >= size - 1) {
            size *= 2;
            hashes = (Hash ) realloc(hashes, sizeof(struct hash) * size);
            radialhashes = (Hash ) realloc(radialhashes, sizeof(struct hash) * size);
        }
        char * filename = (char *) calloc(1, strlen(line));
        for (int i = 2, j = 0; i < strlen(line) - 1; i++, j++)
            filename[j] = line[i];
        printf("about to insert %s at index %d\n", filename, index);

        int numRead;
        if(flag) {
            hashes[index].name = filename;
            ulong64 hash;
            numRead = fscanf(f, "%lld\n", &hash);
            assert (numRead == 1);

            printf("read in %lld\n", hash);
            hashes[index].hash1 = hash;
        }
        else {
            radialhashes[index].name = filename;

            Digest dig = (Digest) calloc(1, sizeof(struct ph_digest));

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
            // char c = getchar();
            // assert(c == '\n');

            dig.id = id;
            dig.coeffs = coeffs;
            dig.size = size;
            radialhashes[index].hash2 = dig;
        }
        index++;
    }

    if ((q = opendir(queries)) == NULL) {
        fprintf(stderr, "Can't open %s\n", argv[1]);
        return 0;
    }

    // read in queries and calculates closest distances
    while ((dp = readdir(q)) != NULL) {
        char str1[2] = ".";
        char str2[3] = "..";

        if (!strcmp(dp->d_name, str1) || !strcmp(dp->d_name, str2) || dp->d_name[0] == '.')
            continue;

        char * path = (char *) calloc(30, sizeof(char));
        for (int i = 0; i < strlen(argv[1]); i++)
            path[i] = argv[1][i];

        strcat(path, "/");
        char * name =  strcat(path, dp->d_name);
        printf("PROCESSING , %s\n", dp->d_name);

        Hash distances = (Hash ) calloc(index, sizeof(struct hash));
        Hash radialdistances = (Hash) calloc(index, sizeof(struct hash));

        Digest radialbasehash;
        ulong64 basehash;

        if (flag) {
            ph_dct_imagehash(name, basehash);
        }
        else {
            ph_image_digest(name, 1.0, 1.0, radialbasehash, 180);
        }

        double threshold = 0.90;
        for (int i = 0; i < index; i++) {
            if (flag) {
                distances[i].d1 = ph_hamming_distance(basehash, hashes[i].hash1);
                distances[i].name = hashes[i].name;
            }
            else {
                double pcc;
                ph_crosscorr(radialbasehash, radialhashes[i].hash2, pcc, threshold);
                radialdistances[i].d2 = pcc;
                radialdistances[i].name = radialhashes[i].name;
            }
        }

        if(flag)
            quick_sort(distances, index, 1);
        else
            quick_sort(radialdistances, index, 0);

        for (int i = 0; i < index && i < 10; i++) {
            printf("RANKING %d\n", i);
            if (flag)
                printf("(%d): %s: %d \t\n", i, distances[i].name, distances[i].d1);
            else
                printf("(%d): %s: %f \t\n", i, radialdistances[i].name, radialdistances[i].d2);
        }
        free(distances);
        free(radialdistances);

        free(path);
    }
    free(hashes);
    free(radialhashes);

    return 0;
}
