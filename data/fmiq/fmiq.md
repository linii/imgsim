Note: because this is a signature-based image similarity scheme, 
we assume constant time for signature generation as each image is added,
so we are only concerned with the querying algorithm.

### fimq accuracy testing data 

Want to determine: 
how accurate was the algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?

The algorithm performed very well on: 

It performed somewhat well on: 
original        (20/20 perfect)
contrast        (20/20 perfect)
paint           (20/20 perfect)
tint            (20/20 perfect)
hue             (19/20 perfect, 1/20 within top 10)
resizeup        (18/20 perfect, 2/20 within top 10)
resizedown      (14/20 perfect, 4/20 within top 10)
aspect ratio    (13/20 perfect, 5/20 within top 10)

It failed on: 
crop            (0/20)
rotate          (0/20)

Note: grayscale is not applicable; unable to make the colorspace consistent across openCV and the python Pillow (image manipulation) module. However, we expect that the fimq algorithm will do well given that grayscale is a color change.

#### Analysis and discussion 

In contrast to the openCV histogram comparison algorithms, fmiq performs very well on color-related filters, perhaps due to the wavelet decomposition approach as opposed to a simplistic histogram bin approach. 

However, it is not ideal for analyzing images that have undergone compositional changes, such as rotations and cropping.

### fimq scalability testing data (**TODO**) 

Want to determine: 
for each size of dataset (n= 300 ... ), 
how long does the algorithm take to score a single query?

Note: here the queries have not been modified, and exist directly 
in the folder. 

SET
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


### fmiq memory testing data (**TODO**)
