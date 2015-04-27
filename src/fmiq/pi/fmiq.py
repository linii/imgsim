"""
Fast Multiresolution Image Querying implementation
Signature generation program

Requirements:
    - all imgs in current folder should be in jpg form
    - and in RGB colorspace

Writes to files with information about wavelet signatures of images

Lining Wang
4/24/2015
"""

from PIL import Image
import os,sys
import numpy as np

def decArray (arr):
    # the entries of A are assumed to be 3-
    # dimensional color components, each in the range [0; 1].
    import math
    oldarr = arr
    h = len(arr)
    stwo = math.sqrt(2)
    hroot = math.sqrt(h)
    for i in range(h):
        arr[i] /= hroot

    while h > 1:
        h /= 2
        Arr = np.zeros(256)
        for i in range(h):
            Arr[i] = (arr[2*i] + arr[2*i + 1]) / stwo
            Arr[h + i] = (arr[2*i] - arr[2*i + 1]) / stwo
        arr = Arr

def getKey (item):
    return item[0]

def decImage (colors, m, index):
    l = 256
    assert len(colors[0]) == 256 and type(colors[0]) == np.ndarray

    avgs = []
    topcoeffs = []
    for k, img in enumerate(colors):
        for i in range(l):
            decArray(img[i])
            img.transpose()
            decArray(img[i])
            img.transpose()

        lst = []
        for i in range(256):
            for j in range(256):
                lst.append([img[i][j], i, j, img[i][j] < 0, k, index])

        topcoeffs += (sorted(lst, key=getKey, reverse=True))[:m]
        avgs.append(img[0][0])

    topcoeffs = (sorted(topcoeffs, key=getKey, reverse=True))[:m]
    return avgs, topcoeffs


def insertCoeffs(coeffs, search):
    for c in coeffs:
        i, j = c[1], c[2]
        search[c[3] + 2 * c[4]][i][j].append(c[5])

    return search

def binF(i, j):
    return min(max(i, j), 5)

def process (img, side):
    """
    Returns YIQ version of the jpeg/png image, which is either in
    RGB or CMYK color space.
    Rescaled
    """
    import colorsys
    img = img.resize((side, side))
    vals = list(img.getdata())
    l = len(vals)

    c1 = np.zeros(shape=(side, side), dtype=float)
    c2 = np.zeros(shape=(side, side), dtype=float)
    c3 = np.zeros(shape=(side, side), dtype=float)

    k = 0
    for i in range(side):
        for j in range(side):
            if k >= l:
                break

            r = float(vals[k][0]) / 255
            g = float(vals[k][1]) / 255
            b = float(vals[k][2]) / 255
            c1[i, j], c2[i, j], c3[i, j] =  colorsys.rgb_to_yiq(r, g, b)
            k += 1

    return c1, c2, c3


def scoreQuery(basecoeffs, baseavg, search, index, avgs):
    scores = [ [0 for j in range(2)] for i in range(index)]

    weights = [ [0 for x in range(3)] for x in range(5)]
    weights[0] = [4.04, 0.78, 0.46, 0.42, 0.41, 0.32]
    weights[1] = [15.14, 0.92, 0.53, 0.26, 0.14, 0.07]
    weights[2] = [22.62, 0.40, 0.63, 0.25, 0.15, 0.38]

    for channel in range(3):
        for i in range(1, index):
            scores[i][0] += weights[channel][0] * (baseavg[channel] - avgs[i][channel])
            scores[i][1] = i

        lst = []
        for c in basecoeffs:
            if not c: continue
            i, j = int(c[1]), int(c[2])
            if  c > 0:
                item = search[channel * 2][i][j]
            else:
                item = search[channel*2 + 1][i][j]
            for i in item:
                scores[i][0] -= weights[channel][binF(i, j)]
                scores[i][1] = i

    scores = sorted(scores, key=getKey)
    return scores[:10] # return top 10 scores


def main ():
    m = 60 # as described in paper, only keeping top 60 coefficients
    index = 0 # keeps track of number of files seen
    s = 256 # height and width of file... must be power of 2

    avgs = [] # averages at arr[0, 0]
    lookup = {} # lookup dictionary mapping indices to filenames
    coeffdict = {} # coeff lookup dictionary mapping filenames to coefficients

    # search arrays in following format: R+, R-, G+, G-, B+, B-
    search = []
    for i in range(6):
        searcharr = [ [ [] for i in range(s)] for i in range(s)]
        search.append(searcharr)

    directory = sys.argv[1]

    # preprocess all images; generate signatures for each
    for root, _, files in os.walk(directory):
        for f in files:
            _, ext = os.path.splitext(f)
            if ext != '.jpg':
                continue
            fullpath = os.path.join(root, f)
            img = Image.open(fullpath)
            if img.mode != 'RGB':
                os.remove(fullpath)
                continue
            print "PROCESSING FILE", f
            imgvals = process(img, s)
            avg, coeffs = decImage(imgvals, m, index)
            avgs.append(avg)
            search = insertCoeffs (coeffs, search)
            coeffdict[f] = coeffs
            lookup[index] = f
            index += 1

    with open("lookup.txt", 'w') as f:
        for i in lookup:
            f.write(str(i) + " " + lookup[i] + "\n")

    with open("coeffs.txt", 'w') as f:
        for fil in coeffdict:
            f.write("FILE " + str(fil) + "\n")
            for c in coeffdict[fil]:
                for comp in c:
                    f.write(str(comp) + " ")
                f.write("\n")
            f.write("\n")

    with open("avgs.txt", 'w') as f:
        for item in avgs:
            for piece in item:
                f.write(str(piece) + " ")
            f.write("\n")

    with open("search.txt", 'w') as f:
        for i, arr in enumerate(search):
            f.write("ARRAY " + str(i) + "\n")
            for j in range(s):
                for k in range(s):
                    if j == 151 and k == 50 and i == 0:
                        print arr[j][k]
                    f.write(str(j) + " " + str(k) + " ")
                    for item in arr[j][k]:
                        f.write(str(item) +  " ")
                    f.write("\n")


# if __name__ == '__main__':
#     main()
