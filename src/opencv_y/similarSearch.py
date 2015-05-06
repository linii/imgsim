import codecs
import sys
import os
from PIL import Image

IMAGE_DIR_NAME = "caltech10"
HIST_DIR = "hist"

# load histogram
def load_histogram():
    hist = {}
    fileP = open("histograms2.txt", "r")
    for line in fileP:
        line = line.rstrip()
        data = line.split("\t")
        file = data[0]
        h = [float(x) for x in data[1:]]
        hist[file] = h
    fileP.close()
    return hist

# compute Noramlized Histogram Intersection
def calculate_histogram_intersection(histogram1, histogram2):
    total = 0
    for i in range(len(histogram1)):
        total += min(histogram1[i], histogram2[i])
    return float(total) / sum(histogram1)

def main():
    histogram = load_histogram()


    while True:
        # input histogram file name
        query_file = raw_input("query? > ")
        
        # exit
        if query_file == "quit":
            break
        
        # transform to path name
        query_file = IMAGE_DIR_NAME + "/" + query_file
        
        # if no such histogram file name -> go back to input
        if not histogram.has_key(query_file):
            print "no histogram"
            continue
        
        # calculate similarity between query and all the others
        result = []
        query_hist = histogram[query_file]
        for target_file in histogram.keys():
            target_hist = histogram[target_file]
            d = calculate_histogram_intersection(query_hist, target_hist)
            result.append((d, target_file))
        
        # sort in order of measurement increasing similarity
        result.sort(reverse=True)
        
        # Output the top 10 images
        # Use PIL to plot 10 images in 2 by 5 as 300*300 pixels / unit
        counter = 0
        canvas = Image.new("RGB", (2000, 800), (255,255,255))  # white canvas
        for score, filename in result[0:10]:
            print "%f\t%s" % (score, filename)
            image = Image.open(filename)
            pos = (400*(counter%5), 400*(counter/5))
            canvas.paste(image, pos)
            counter += 1
        canvas.resize((2000/2, 800/2))
        canvas.show()
        canvas.save("search.jpg", "JPEG")

if __name__ == "__main__":
    main()
