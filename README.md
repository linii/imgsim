# imglib: image similarity algorithm compendium 

(also backend to CryptoDraw project for CPSC479) 

A library of fast (ish) image similarity algorithms! 
(and the dataset for evaluating them)

Source of images: favorites gallery of prominent artist on dA 
- chosen because she has similar visual taste--would be representative of most styles that we are likely to see in people using this extension 

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

###### TODO: 
- pHash: 
    - fix sorting function
    - optimize (with profiling)
    - evaluations
- openCV: 
    - get Boost working and compile the thing 
    - debug the program 
    - evaluations 
- fimq: 
    - finish implementing query function
    - optimization by porting to C binary 
    - evaluations
- overarching question: want to store signatures of some sort 
    - but how? 
- Compact Data Structures paper: implement if there's enough time 