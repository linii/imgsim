Note: because this is a signature-based image similarity scheme, 
we assume constant time for signature generation as each image is added,
so we are only concerned with the querying algorithm.

### fimq scalability testing data 

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


### fimq accuracy testing data 

Want to determine: 
how accurate was the algorithm in determining the appropriate match 
for a set of 20 query images, 
which have been modified in various ways
among a database of ~ 3500 images?


KEY: 
1: 1020
2: 2426
3: 2779
4: 2794
5: 3098
6: 3135
7: 3458
8: 3485
9: ???
10: 489
11: 6
12: 1208
13: ???
14: 130
15: 1399
16: 1629
17: 1734
18: 2015
19: 2095
20: 2399

BASE: 
Correct: 2, 3, 4, 5, 7, 8, 11, 12, 14, 15, 16, 17, 18, 20 
6 (#2)
1 (#2)
19 (#4)
10 (#7),

Failed: 13, 9

ASPECT: 
Correct: 3, 4, 5, 6, 7, 8, 11, 14, 15, 17, 18, 20
12 (#2)
19 (#4)
10 (#10)
16 (#10)

Failed: 1, 9, 13

CONTRAST: 100%
Correct: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20

CROP: 0%
Failed, 1, 3, 4, 5, 6, 7, 8, 9 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20

HUE: 

PAINT: 

DOWNSIZE: 
Correct: 2, 3, 4, 5, 7, 8, 11, 12, 14, 15, 16, 17, 18, 20
1 (#2)
6 (#2)
19 (#4)
10 (#7)
Failed: 9, 13

UPSIZE: 

ROTATE: 

TINT: 


### fmiq memory testing data
