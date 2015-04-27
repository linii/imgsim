Note: because this is a signature-based image similarity scheme using histograms,  we assume constant time for signature generation as each image is added, so we are only concerned with the querying performance. 

Also, we are interested in the performance the four openCV histogram comparison methods: 
    - correlation 
    - chi-square
    - intersection
    - Bhattacharyya distance

### opencv scalability testing data 

Want to determine: 
for each size of dataset (n= 300 ... ), 
how long does each algorithm take to score a single query?

#### Correlation

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

#### Chi-square

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

#### Intersection

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

#### Bhattacharyya distance 

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

###  opencv accuracy data

Want to determine: 
how accurate was each algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?

#### Correlation 

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

#### Chi-square

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

#### Intersection

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

#### Bhattacharyya distance 

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
