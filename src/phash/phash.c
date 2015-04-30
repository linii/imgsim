// Part 1 of utility wrapper for pHash library.
// http://www.phash.org/
//
// Call the executable like this:
// ./phash [ directory ] [ flag ]
//  where directory contains all source images,
//  and flag indicates the type of hashing to be performed.
//      1 for DCT image hashing
//      0 for Radial hashing
//
//  Outputs to stdout the hashes in fixed format, to be read in by
//  phash-process.c
//
//
//  Lining Wang
//  4/29/2015


#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <assert.h>

#include "lib/pHash.h"

typedef unsigned long long ulong64;

int main (int argc, char ** argv) {
    struct dirent *dp;
    DIR *dfd;

    char * dir = argv[1];
    int flag = atoi(argv[2]);

    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "Can't open %s\n", dir);
        return 0;
    }

    const int nb_coeffs = 40; // taken from source code

    while ((dp = readdir(dfd)) != NULL) {
        char * path = (char *) calloc(30, sizeof(char));
        for (int i = 0; i < strlen(argv[1]); i++)
            path[i] = argv[1][i];

        strcat(path, "/");
        char * name =  strcat(path, dp->d_name);

        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..") || dp->d_name[0] == '.')
            continue;

        printf("F %s\n", dp->d_name);
        if (flag) {
            ulong64 hash;
            assert (ph_dct_imagehash(name, hash) != -1);
            printf("%lld\n", hash);
        }
        else {
            Digest dig;
            assert (ph_image_digest(name, 1.0, 1.0, dig, 180) != -1);
            for (int i = 0; i < 40; i++)
                printf("%" PRIu8 " ", dig.coeffs[i]);
            printf("\n");
        }
        free(path);
    }

    return 0;

}
