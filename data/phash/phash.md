Note: because this is a signature-based image similarity scheme using histograms,  we assume constant time for signature generation as each image is added, so we are only concerned with the querying performance. 

Also, we are interested in the performance the three pHash comparison methods: 
    - radial hash
    - DCT hash
    - Marr/Mexican hat wavelet

### pHash accuracy testing data 

Want to determine: 
how accurate was each algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?

#### Radial hash 

Radial hash worked very well on:
original        (19/19 perfect)
contrast        (19/19 perfect)
grayscale       (19/19 perfect)
paint           (19/19 perfect)
resizeup        (19/19 perfect)
resizedown      (19/19 perfect)
tint            (19/19 perfect)
hue             (14/19 perfect, 3/19 in top 10)


It failed on: 
aspectratio     (2/19 perfect, 2/19 in top 10)
crop            (2/19 perfect)
rotate          (0/20)


Note: The coefficients of the 12.jpg image were all 0, on some reason, so it defaulted on that file.

#### DCT Hash

DCT hash worked very well on:
original        (20/20 perfect)
aspectratio     (20/20 perfect)
contrast        (20/20 perfect)
grayscale       (20/20 perfect)
paint           (20/20 perfect)
resizeup        (20/20 perfect)
resizedown      (20/20 perfect)
tint            (20/20 perfect)
hue             (17/20 perfect, 2/20 in top 10)

It failed on: 
cropping        (0/20)
rotate          (0/20)

#### Marr wavelet

Marr wavelet hash worked very well on:

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



### pHash memory testing data 