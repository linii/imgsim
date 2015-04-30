"""
short program for determining accuracies of
image similarity algorithms
over different image manipulations

invoked like so:
python process.py [ key file*] [ suffix*] [ output file 1*] [ output file 2 ] [ ... ]

    - key file should be in teh following format:
        [ query file name ] : [ actual file name]
    - output files should be in the following format:
        TESTING: [ name of image filter applied ]
        METHOD: [ method 1 name ]
        QUERY: [ query image file name ]: [ #1 match name ], [ #2 match name, etc ]
        METHOD: [ method 2 name ]
        [... ]
    - suffix: can be .jpg, .png, as long as it is consistent across the key
        and output files. ( may be changed later to be more flexible)

Generally, filenames should begin with a number.

Returns printout of the following form:

Method 1:

Correct
[ list of filenames]
Correct # / Total #

Somewhat correct (not first choice):
[ list of filenames]
Somewhat # / Total #

Incorrect
[ list of filenames]
Incorrect # / Total #

Lining Wang
4/27/2015

"""

import sys

def getKey(item):
    return item[1]

def main():
    key =   open(sys.argv[1])
    suffix = sys.argv[2]

    keydict = {}
    numKeys = 0
    for line in key:
        line = line.split(":")
        keydict[line[0] + suffix] = line[1][1:-1]+ suffix
        numKeys += 1

    overall = []
    effects = []
    for l in range(3, len(sys.argv)):
        measures = []
        output = open(sys.argv[l])
        topIndex = -1
        mdict = {}
        mreversedict = {}

        for line in output:
            line = line.split()
            if not line:
                continue
            if line[0] == "TESTING:" and " ".join(line[1:]) not in effects:
                effects.append(" ".join(line[1:]))
            elif line[0] == "METHOD:":
                methodname = " ".join(line[1:])
                if methodname not in mreversedict:
                    topIndex += 1
                    # print "adding, ", methodname, "at index", topIndex
                    measures.append([[] for j in range(3)])
                    mreversedict[methodname] = topIndex
                    mdict[topIndex] = methodname
                currIndex = mreversedict[methodname]
            elif line[0] == "QUERY:":
                filename, p = line[1], line[2:]
                # print "filename", filename
                flag = True
                index = -1
                for i, poss in enumerate(p):
                    # print "comparing"
                    # print poss
                    # print keydict[filename]
                    if poss == keydict[filename]:
                        flag = False
                        index = i
                        break
                if flag:
                    measures[currIndex][2].append(filename)
                else:
                    measures[currIndex][1] = sorted(measures[currIndex][1], key=getKey)
                    if index == 0:
                        measures[currIndex][0].append(filename)
                    else:
                        measures[currIndex][1].append((filename, i+1))

        overall.append(measures)

    # print "names", effects
    for j, f in enumerate(overall):
        print "RESULTS OF TEST: ", effects[j]
        for i, m in enumerate(f):
            print
            print "METHOD: ", mdict[i]
            print "\tCorrect: ", m[0]
            print "\tSum: ", len(m[0]), "/", numKeys, len(m[0])/numKeys
            print
            print "\tSomewhat correct: ", m[1]
            print "\tSum: ", len(m[1]), "/", numKeys, len(m[1])/numKeys
            print
            print "\tNowhere to be found: ", m[2]
            print "\tSum: ", len(m[2]), "/", numKeys, len(m[2])/numKeys
            print "OVERALL CORRECT: ", len(m[0]) + len(m[1]), "/", numKeys, (len(m[0]) + len(m[1])) / numKeys
        print


if __name__ == '__main__':
    main()