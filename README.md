# imglib: an image similarity algorithm compendium 

A library of fast(ish) image similarity algorithms! 

**Source of images:** favorites gallery of prominent artist on dA 
- chosen because she has similar visual taste--would be representative of most styles that we are likely to see in people using this extension 

### Original code & work (guide to directory layout)
- **src/fmiq**
    - **fmiq.py:** implements the fast multiresolution querying algorithm (see paper/ directory)
    - **fmiq-process.py:** parses the output of fmiq on the source images and queries with test images 
- **src/opencv** 
    - **histogram.cpp:** wrapper around four opencv measures, as well as their queries
        - how to build: run make 
    - CMakeLists.txt (getting this to build the right Makefile was not easy)
- **src/phash**
    - **phash.c**: wrapper around phash library (located in lib). generates perceptual hashes of everything in source image directory   
        - compatible with radial hashing, DCT hashing
        - how to build: g++ -c phash.c ; g++ -o phash phash.c main.o => generates executable **phash**
    - **phash-process.c**: processes the output of phash.c; scores all test images and assigns relative distances 
        - how to build: g++ -c phash-process.c ; g++ -o process phash-process.c main.o => generates executable **process** 
- **src/analytics**
    - **process.py** all purpose tool for evaluating the accuracy of the algorithms above
- **data/**
    - **data/opencv**
        - all: results of algorithm on 10 different test data sets 
        - opencvresults: output  of process.py on those files 
        - opencv.md: some data consolidation analysis
    - **data/phash**
        - radial/: results of radial hashing algorithm on 10 different test data sets 
        - dct/: results of dct hashing algorithm on 10 different test data sets 
        - radishresults: output of process.py on radial/ 
        - dctresults: results of process.py on dct/
        - phash.md: some data consolidation and analysis
    - **data/fmiq**
        - all: results of algorithm on 10 different test data sets 
        - fmiqresults: output of process.py on those files 
        - fimq.md: some data consolidation and analysis
- **tests/**: directories of test data set after various small digital modifications
    - all folders here created through image manipulation by **imagemagick**command line tool 

### Requirements & dependencies (to be expanded)
- openCV 
- Boost

### Log

###### Thursday (worked 4.5 hours)
- gathered source images 
- narrowed down similarity algorithms
- implemented part of multiresolution querying 
- set up mySQL database for front-end 

###### Friday (worked 8 hours) 
- set up **pHash** (perceptual hash)
    - installed **MacPorts**, **libpng**, **CImg** dependencies
    - ...before downloading source and copying it into the directory 
    - wrote wrapper program **phash.c**
    - testing two hashes: radial and regular 
- set up **openCV** histogram comparison method (a library for C++)
    - set up **cMake**
    - installed **Boost** and related libraries for iterating through directories
    - learned basic C++
    - wrote wrapper program histogram.cpp
- finished implementing multiresolution querying (see fmiq)
    - fixed PIL pip install
    - attempted optimizations using pypy (didn't work)
    - each image takes about 7 seconds to analyze
    - straight from paper; rgb to yiq color space conversion
- homogenized **source images**
    - installed **imagemagick** through macports
    - converted all files to jpg 
    - converted all jpg to rgb color space
    - set up test files, including effects 

###### Saturday (worked 7 hours)
- set up test images
    - 20 for each effect (see tests folder)
    - aspect ratio: fixed aspect ratio (300x300px)
    - contrast: increased histogram contrast
    - crop: cropped images to squares, up to 1/2 of image cropped off
    - grayscale: converted to grayscale
    - hue: modulate histogram 
    - paint: blobby effect 
    - resizedown, resizeup: 50% and 200%, respectively 
    - rotate: rotate 90 degrees
    - tintred: duochrome
    - txt: todo, overlaid text 
- tested for duplicates among source images
- fixing fimq: tried for 30 minutes to install PIl, gave up and installed a virtualenv instead
- completed fimq implementation
- completed fimq processing and porting 

###### Sunday (worked 5 hours)
- debugged fimq porting 
    - this took a ridiculously long time. 
    - started data collection!!! 
- set up wrapper files for phash: still working on radial though 
    - solution to exporting signature data: write to file in a specific format
- now both phash and fimq are ready for data collection

###### Monday (worked 9 hours)
- get Boost working and compile the thing 
- debug the program 
- wrote a program to process output
- analyzed and wrote evaluations for fmiq and opencv
- collected data for fmiq and opencv 
- analyzed data for fmiq and opencv 
- wrote most of presentation

###### Tuesday (worked 4 hours)
- fixed pHash 
- conducted pHash evaluations

###### Wednesday (worked 3.5 hours)
- implemented radial hashing 
- evaluated radial hashing results 
- cleaned up and documented code