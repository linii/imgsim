Note: because this is a signature-based image similarity scheme using histograms,  we assume constant time for signature generation as each image is added, so we are only concerned with the querying performance. 

Also, we are interested in the performance the four openCV histogram comparison methods: 
    - correlation 
    - chi-square
    - intersection
    - Bhattacharyya distance

###  opencv accuracy data and discussion

Want to determine: 
how accurate was each algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?

#### Correlation
Correlation performed **very well** on the following filters: 
originals       (20/20 perfect)
aspect ratio    (20/20 perfect)
resizedown      (20/20 perfect)
resizeup        (20/20 perfect)
paint           (20/20 perfect)
rotate          (20/20 perfect)
cropping        (18/20 perfect, 1/20 within top 10)

It performed **somewhat well** on the following filters: 
contrast        (12/20 perfect, 3/20 within the top 10)

It **failed** on filters: 
tint            (3/20 within top 10)
grayscale       (0/20)
hue             (0/20)

#### Chi-Square 
Chi-square performed **very well** on filters: 
original        (20/20 perfect)
aspect          (20/20 perfect)
resizedown      (20/20 perfect)
resizeup        (20/20 perfect)
paint           (20/20 perfect)
rotate          (20/20 perfect)

It performed **somewhat well** on the following filters: 
contrast        (6/20 perfect, 4/20 within top 10)
crop            (7/20 perfect, 1/20 within top 10)

It **failed** on filters: 
tint            (1/20 within top 10)
grayscale       (0/20)
hue             (0/20)

#### Intersection 
Intersection performed **very well** on filters: 
original        (19/20 perfect, 1/20 within top 10)
rotate          (19/20 perfect, 1/20 within top 10)
resizeup        (19/20 perfect, 1/20 within top 10)  
paint           (18/20 perfect, 1/20 within top 10)  
resizedown      (16/20 perfect, 3/20 within top 10)
aspect ratio    (15/20 perfect, 4/20 within top 10)

It performed **somewhat well** on the following filters: 
crop            (6/20 perfect, 5/20 within top 10)
contrast        (5/20 perfect, 3/20 within top 10)

It **failed** on filters: 
tint            (1/20 within top 10)
grayscale       (0/20)
hue             (0/20)

#### Bhattacharyya 
Bhattacharyya performed **very well** on filters: 
original        (20/20 perfect)
crop            (20/20 perfect)
resizedown      (20/20 perfect)
resizeup        (19/20 perfect)
paint           (18/20 perfect, 1/20 within top 10)
rotate          (18/20 perfect, 1/20 within top 10)
contrast        (14/20 perfect, 3/20 within top 10)

It performed **somewhat well** on the following filters: 
None

It **failed** on filters: 
tint            (4/20 perfect)
grayscale       (0/20)
hue             (0/20)

#### Analysis: 

For all four measures, tint, grayscale, and hue were difficult to detect, implying that histogram comparisons do not do well with color changes. 

However, on all the other measures, it performed quite well, particularly the Bhattacharyya distance.

The overall ranking of the measures: 
1) Bhattacharyya 
2) Correlation
3) Chi-square
4) Intersection

### opencv scalability testing data **TO BE COLLECTED **

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



