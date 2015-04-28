# imglib: an image similarity algorithm compendium 

A library of fast (ish) image similarity algorithms! 
(and one dataset for evaluating them)

Source of images: 
favorites gallery of prominent artist on dA 
- chosen because she has similar visual taste--would be representative of most styles that we are likely to see in people using this extension 

### Original code & work (guide to directory layout)
- src/fmiq
    - fmiq.py:
    - fmiq-process.py 
- src/opencv 
    - histogram.cpp 
    - CMakeLists.txt (getting this to build the right Makefile was not easy)
- src/phash
    - phash.c 
    - phash-process.c
- src/analytics
    - process.py
- data
    - /opencv, /phash, /fmiq: results of data from runs 
- tests 
    - all folders here created through image manipulation 

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

###### TODO: 
- pHash: 
    - fix sorting function
    - evaluations
- memory concerns
- other filters