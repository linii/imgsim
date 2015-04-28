"""
Processing program for scoring queries,
given the tools to reconstruct the unique wavelet signature for each image:
search arrays, averages, lookup dictionary and coefficients.

Called in the following format:
python fmiq-process.py [ search array output ] [ coefficient dictionary output]
     [ image averages ] [ lookup dictionary] [ query file 1 ] [ query file 2 ] [ ... ]

Lining Wang
4/25/2015
"""

from fmiq import scoreQuery, process, decImage

from PIL import Image
import numpy as np
import sys
import os

def main():
    assert len(sys.argv) >= 6

    m = 60
    s = 256

    # search arrays in following format: R+, R-, G+, G-, B+, B-
    search = []
    for i in range(6):
        searcharr = [ [ [] for i in range(s)] for i in range(s)]
        search.append(searcharr)

    # reconstruct search arrays
    sf = open(sys.argv[1]) # x
    num = 0
    for line in sf:
        test = line.split()
        if test[0] == "ARRAY":
            num = int(test[1])
            continue
        if len(test) == 2:
            continue
        for item in test[2:]:
            search[num][int(test[0])][int(test[1])].append(int(item))

    coeffs = {}
    first = True
    cf = open(sys.argv[2]) # X
    for c in cf:
        if not c: continue
        f = c.split()
        if not f: continue
        if f[0] == "FILE":
            if not first:
                coeffs[currentFile] = lst
            first = False
            currentFile = f[1]
            lst = []
            continue
        lst.append(c.split())
    coeffs[currentFile] = lst

    avg = open(sys.argv[3]) # X
    avgs = []
    for line in avg:
        line = line.split()
        lst = []
        for item in line:
            lst.append(float(item))
        avgs.append(lst)

    avgs = np.array(avgs)

    lookup = {}
    reverse = {}
    lf = open(sys.argv[4]) # X
    for line in lf:
        c = line.split()
        lookup[int(c[0])] = c[1]
        reverse[c[1]] = int(c[0])

    index = len(avgs)

    querylist = sys.argv[5:]
    for queries in querylist:
        print "TESTING: ", str(queries)
        for root, _, files in os.walk(queries):
            for f in files:
                _, ext = os.path.splitext(f)
                if ext != '.jpg':
                    continue

                fullpath = os.path.join(root, f)

                img = Image.open(fullpath)
                # if img.mode != 'RGB':
                #     os.remove(fullpath)
                #     continue

                imgvals = process(img, 256)
                avg, cs = decImage(imgvals, m, index)

                print "METHOD: Fast Multiresolution Image Querying"
                print "QUERY: ", f, " ",

                scores = scoreQuery(cs, avg, search, index, avgs)
                for s in scores:
                    print lookup[s[1]], " ",


if __name__ == '__main__':
    main()
