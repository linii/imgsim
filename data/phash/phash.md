Note: because this is a signature-based image similarity scheme using histograms,  we assume constant time for signature generation as each image is added, so we are only concerned with the querying performance. 

Also, we are interested in the performance the three pHash comparison methods: 
    - radial hash
    - DCT hash
    - Marr/Mexican hat wavelet

### pHash scalability testing data 

Want to determine: 
for each size of dataset (n= 300 ... ), 
how long does each algorithm take to score a single query?

#### Radial hash

1: 

2: 

3: 

4: 

5: 

6: 

7: 

8: 

9: 

10: 

#### DCT Hash

1: 

2: 

3: 

4: 

5: 

6: 

7: 

8: 

9: 

10: 

#### Marr Wavelet

1: 

2: 

3: 

4: 

5: 

6: 

7: 

8: 

9: 

10: 

### pHash accuracy testing data 

Want to determine: 
how accurate was each algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?

#### Radial hash 

BASE: 

ASPECT: 

CONTRAST: 

CROP: 

HUE: 

PAINT: 

DOWNSIZE: 

UPSIZE: 

ROTATE: 

TINT: 

#### DCT Hash

BASE: 

ASPECT: 

CONTRAST: 

CROP: 

HUE: 

PAINT: 

DOWNSIZE: 

UPSIZE: 

ROTATE: 

TINT: 

#### Marr wavelet

BASE: 

ASPECT: 

CONTRAST: 

CROP: 

HUE: 

PAINT: 

DOWNSIZE: 

UPSIZE: 

ROTATE: 

TINT: 


### pHash memory testing data 