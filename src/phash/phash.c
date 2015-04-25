#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pHash.h"
#include <assert.h>

typedef unsigned long long ulong64;

typedef struct hash {
    char * name;
    ulong64 hash1;
    Digest hash2;
    int d1;
    double d2;
} * Hash;

// int comp1 (const void * elem1, const void * elem2) {
//     Hash1 f = *((Hash1 *)elem1);
//     Hash1 s = *((Hash1 *)elem2);
//     if (f->distance > s->distance)
//         return  1;
//     if (f->distance < s->distance)
//         return -1;
//     return 0;
// }

// int comp2 (const void * elem1, const void * elem2) {
//     Hash2 f = *((Hash2 *)elem1);
//     Hash2 s = *((Hash2 *)elem2);
//     if (f->distance > s->distance)
//         return  1;
//     if (f->distance < s->distance)
//         return -1;
//     return 0;
// }

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



int main (int argc, char ** argv) {
    struct dirent *dp;
    DIR *dfd;
    printf("al;sdkfja;sdfjkasdf");

    char * dir = argv[1];
    printf("%s  ", dir);
    // exit(0);
    //
    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "Can't open %s\n", dir);
        return 0;
    }

    Hash hashes = (Hash ) calloc(2, sizeof(struct hash));
    Hash radialhashes = (Hash) calloc(2, sizeof(struct hash));
    int index = 0;
    int size = 2;

    ulong64 basehash;
    Digest radialbasehash;
    const char * name = argv[2];
    printf("name, %s\n", name);
    ph_dct_imagehash(name, basehash);
    ph_image_digest(name, 1.0, 1.0, radialbasehash, 180);

    while ((dp = readdir(dfd)) != NULL) {
        if (index >= size - 1) {
            size *= 2;
            hashes = (Hash ) realloc(hashes, sizeof(struct hash) * size);
            radialhashes = (Hash ) realloc(radialhashes, sizeof(struct hash) * size);
        }
        char str1[2] = ".";
        char str2[3] = "..";
        char path[15] = "test1/";
        if (!strcmp(dp->d_name, str1) || !strcmp(dp->d_name, str2) || dp->d_name[0] == '.')
            continue;

        printf("filename %s\n", dp->d_name);
        char * name = strcat(path, dp->d_name);
        Digest dig;
        assert (ph_image_digest(name, 1.0, 1.0, dig, 180) != -1);
        ulong64 hash;
        assert (ph_dct_imagehash(name, hash) != -1);
        printf("hash %lld\n", hash);
        hashes[index].hash1 = hash;
        hashes[index].name = strdup(name);

        radialhashes[index].hash2 = dig;
        radialhashes[index].name = strdup(name);

        index++;
    }
    printf("final index: %d", index);
    Hash distances = (Hash ) calloc(index, sizeof(struct hash));
    Hash radialdistances = (Hash) calloc(index, sizeof(struct hash));

    double threshold = 0.90;
    for (int i = 0; i < index; i++) {
        distances[i].d1 = ph_hamming_distance(basehash, hashes[i].hash1);
        distances[i].name = hashes[i].name;
        printf("name %d %s %d\n", i, hashes[i].name, distances[i].d1);

        double pcc;
        ph_crosscorr(radialbasehash, radialhashes[i].hash2, pcc, threshold);
        radialdistances[i].d2 = pcc;
        radialdistances[i].name = radialhashes[i].name;
        printf("name r %d %s %f\n", i, radialhashes[i].name, radialdistances[i].d2);
    }

    // qsort (distances, index, sizeof(*distances), comp1);
    // qsort (radialdistances, index, sizeof(*radialdistances), comp2);
    quick_sort(distances, index, 1);
    quick_sort(radialdistances, index, 0);

    for (int i = 0; i < 10; i++) {
        printf("RANKING %d\n", i);
        printf("distance for %s: %d  \t\n", distances[i].name, distances[i].d1);
        printf("radial distance for %s: %f \t\n", radialdistances[i].name, radialdistances[i].d2);
    }

    free(distances);
    free(hashes);
    free(radialhashes);
    free(radialdistances);

    return 0;

}
