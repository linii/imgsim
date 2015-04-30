#include <stdio.h>
#include <dirent.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "pHash.h"
#include <assert.h>


typedef unsigned long long ulong64;

int main (int argc, char ** argv) {
    struct dirent *dp;
    DIR *dfd;

    char * dir = argv[1];

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

        char str1[2] = ".";
        char str2[3] = "..";

        if (!strcmp(dp->d_name, str1) || !strcmp(dp->d_name, str2) || dp->d_name[0] == '.')
            continue;

        printf("F %s\n", dp->d_name);
        Digest dig;
        assert (ph_image_digest(name, 1.0, 1.0, dig, 180) != -1);
        // printf("%s %d\n", dig.id, dig.size);
        for (int i = 0; i < 40; i++)
            printf("%" PRIu8 " ", dig.coeffs[i]);
        printf("\n");

        // ulong64 hash;
        // assert (ph_dct_imagehash(name, hash) != -1);
        // printf("%lld\n", hash);
        free(path);
    }

    return 0;

}
