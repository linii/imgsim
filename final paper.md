# Introduction 

## Goal and Motivation 

Over the past decade, the ubiquity of the Internet has led to the organic growth of large online artist communities—sites where both amateur and professional artists can post, share, and request critiques of their work. However, unauthorized copying and distribution of artwork for personal and monetary gain has been rampant inside and outside of such communities, with few established universal mechanisms for the revocation or prevention of plagiarism.

CryptoDraw establishes a cryptographically secure framework for regulating the display of artwork and enforcing copyright within the artist, viewer, and exhibitor community. CryptoDraw aspires to give artists control over their work, foster an online art community grounded in integrity, establish a trans-web art community. 

## Related work 

For the front-end, we looked at existing extensions such as Chrome images and TinEye. However, while these services were amazingly fast, they are most suited for one-time searches, not continuous monitoring of possible infringement.

[ to be expanded ]

For the back-end, we looked mainly at popular, widely-used image similarity algorithms: 
    - the pHash library (http://phash.org) has been in development since 2008, with its last major update in early 2013. 
    - OpenCV (Open Source Computer Vision, http://opencv.org/) is a popular image processing library written in highly optimized code. One of its features is histogram comparison, which comes equipped with four different measures.
    - Fast Multiresolution Image Querying is a paper published in 1995 by researchers at the University of Washington, originally intended for retrieivng paintings based on an amateur approximation. It utilizes a wavelet-based algorithm. 
    - Image Similarity Search with Compact Data Structures is a paper published in 2004 by researchers at Princeton university.

## Approach and architecture

### Front-end focus 

[ to be expanded ]

### Back-end focus

> Narrowing the scope of the problem 

Online copyright infringement is a blanket term for a variety of undesirable acts CryptoDraw attempts to prevent. What kinds or categories of plagiarism, theft, and general unauthorized use are we seeking to detect? What are our working definitions for what constitutes "infringement" or "plagiarism"

> Low effort copyright infringement

These are instances of unauthorized distribution and failure of attribution, regardless of the Creative Commons license preferred by the user. They usually involve simple digital modifications and manipulations; the end product greatly resembles the original in some rigorous way.

Examples of low-effort copyright infringement include changes in: 
    - Size/resolution: downsizing, upsizing, aspect ratios 
    - Histogram changes: contrast, brightness, curve
    - Color: hue, tinting, grayscale
    - Composition: cropping, rotating
    - Filtering: paint effect, mosaic effect, blurring, sharpening
    - Misc: layered text 

Examples: 
    - Someone creates a digital copy of a popular work and sells t-shirts with the design (for example on CafePress) for a profit 
    - Someone copies a lesser-known work and uses it to make their gallery more popular. 

> High effort copyright infringement (aka plagiarism)

These instances of copyright infringement are much harder to detect, and strike at the boundary between originality, style and content. Furthermore, they sometimes happen when artists subconsciously emulate the style of the artists they admire. This results in muddied motivation; rather than the examples from the case above, these instances are usually for personal learning rather than explicit profit. 

Examples of high effort infringement include: 
    - tracing the work 
    - copying unique visual ideas
    - using parts or details of the original as a reference (usually happens with photographs)
    - copying the original composition

Examples of real-life instances: 
    - in 2010, the artist Yuumei found that one of her pieces had been mostly copied (from digital to traditional media) by a high school student, who had received a $10,000 scholarship as an award. The award was rescinded and the student issued a public apology. 

Controlling high-effort copyright infringement is beyond the scope of this project. Sadly, while humans are fully able to detect such cases (as in the Yuumei incident above), current feature extraction and other related computer vision algorithms cannot account for abstract or imaginative works, encode any complex compositions, or capture an artist's "style"--a mysterious thing that sits somewhere between brush stroke type, textures/colors used, representation of perspective, etc. 

Hence we center our back-end search and analysis on the question: what are the image similarity algorithms that are most resistant to small digital modifications? 

# Methodology 

## Front-end: Chrome Extension 

### OpenPGP for authentication

In order to claim or post artwork, whether original or derived, a CryptoDraw user needs to sign in or register with their public key. We use the PGP (Pretty Good Privacy) cryptoscheme because it is widely used, cryptographically secure, and compatible with aggregated identity databases such as Keybase (which supports linking social media or other accounts to a single public key). 

> Implementation 

We used openPGPjs, a javascript library for PGP. The user is asked to specify their passphrase, which is sent to the keypair generation function along with a randomly generated id. The return public and private keys are then stored in Chrome sync and local storage, respectively. 

### User interface and action flow

For the artist: 
- when the artist decides to upload artwork, she signs the image with her private key and adds it to the global hashed linked list, along with specifications for the license she would like to use.
- the image is then assigned its unique hash, and a link is created between the artist's id and the hash, thus establishing authorship. The image’s metadata is also added to the database.

For the viewer, CryptoDraw extracts images and corresponding image metadata from the currently loaded web page to determine the verification status of each image. 
- If the image is in the database but proof of permission has not been established, an X is displayed, with options to report this image as plagiarism, in addition to similar images selected by the image preprocessing library. 
- If proof is established, a checkmark and the specific Creative Commons license is displayed, with additional options for viewing the artist’s other work, their personal or professional website, or asking for permission to display the image.
- If the image is not in the database, i.e. it is a reference or uncopyrighted image, the option to report the image as plagiarism is displayed. 

For the exhibitor, CryptoDraw shows the desired image with the proof of permission symbol (checkmark), along with an option for replication. (If the image does not have proof of permission, there is no such option.)
- The exhibitor sends a verification request to the server, along with the URL of where they would like to exhibit the piece. 
- By default, this request is approved automatically; however, some artists may choose to be more discreet with where their work is displayed.  The server looks up the artist id associated with the image hash and sends a notification to the artist with more specific details of the verification request if this option is checked in Settings. 
- After artist approval, the exhibitor uploads the image at the requested location. CryptoDraw looks up the image metadata in the database, finds that permission has been given, and assigns the original hash to the replicate, thus displaying the profile associated with the hash and satisfying the attribution component of authorized use.

Note that the roles of viewer, exhibitor, and artist are not at all mutually exclusive. In CryptoDraw, the user can move fluidly between these roles. 

> Implementation 

[ to be expanded ]

### Hashed linked list 

Establishment of chronology allows for a much cleaner conflict-resolution process, since the probability that something is plagiarized is much higher when a very similar piece was published previously. To establish chronology, we use a hashed linked list—a combination of a standard linked list with the hash function, a cryptographic primitive. More specifically, taking the hash of the concatenation of the following generates the hash id of each node: 

1)  The hash id of the previous node, and 
2)  The message digest of the work being uploaded (with corresponding digital signature).
3)  Salted with the current time in UT.

Thus the integrity of the previous node helps preserve and verify the integrity of the next. Furthermore, the integrity of the list can be quickly checked by going through each node. 

> Implementation 

We created a MYSql database instance using Amazon AWS RDS. Each entry in the database has the following fields: 
    - hash of image metadata 
    - hash of this linked list node
    - index of next linked list node
    - artist id 
    - date of upload
    - creative commons license 
    - list of derived children 

Thus image metadata is integrated with hashed linked list data. 

## Back-end: Image Similarity Library 
github: linii.com/imgsim.git

Deciding which algorithms to implement and test for the back-end was largely based on the algorithm’s scalability, space efficiency, and (most importantly) accuracy. On the extreme end of the scale, it would be undesirable for the algorithm to require that the images themselves to be stored in the database, as this would be extraodinarily space inefficient; furthermore, any algorithm that took super-linear time most likely could not return a list of possible matches from a database containing thousands or millions of images (as deviantART does) and serve them to the front-end user within a couple seconds. 

Therefore, I chose to implement signature-based algorithms. Algorithms in this class have two components: 
1. The signing function. Given an image, that is, a series of color values, returns a fixed-length signature that encodes pertinent image information.
2. The scoring function. Given two signatures, returns the similarity between them based on some metric. 

Each image in the database is thus represented by a small sequence of bits, which are computed as the image is added--essentially in constant time. Scalability, then, reduces to the performance of the scoring function, and space efficiency reduces to the size of each signature. This allows us to focus on the original purpose of the image preprocessing library: detecting possible matches within the database and testing for accuracy.

> Obtaining the image database 

To simulate the image database that would be prospectively stored at the mySQL instance, I downloaded a set of 3500 images from deviantART. The medium of these works skewed towards digital painting and concept art, although photography, drawing, sketches, and other visual media were also included.

> Image preprocessing and sanitization 

To make the images ready for processing, and to keep the image requirements simple, I pruned out all non-.jpg formatted images, as well as all images not in the RGB colorspace. Finally, I numbered the images sequentially to allow for fast verification of correctness. 

### 2. Writing algorithms and/or wrappers 

#### Fast Multiresolution Image Querying (fimq)
source: http://grail.cs.washington.edu/projects/query/

> Algorithm Summary 

All images are first preprocessed by fixing aspect ratios and resizing (256x256px in the paper). fimq then utilizes the Haar wavelet decomposition to generate coefficients, and thus a signature, for each image. Only the top 60 coefficients are kept, and they represent the image in the six search arrays (two for each color channel, positive or negative). For example, [i, j] in the array contains a list of image names that have a large coefficient at [i, j]. 

Finally, the metric employed by the querying algorithm takes the difference of the average intensity levels of the two images, and adds it to the sum of the differences between the coefficients in search array associated with the coordinates of the query image's coefficients. This is also known as the L_q metric.  

> Implementation

I implemented fimq straight from the paper in Python using the Pillow/PIL imaging library and numpy. For the intermediate database representation, I piped the search arrays, coefficients of each image, the filename->index lookup dictionary, and averages of each image to separate files, which were processed and stored by the fmiq-process program. The process program scored each query image based on the search array arrays, sorted the scores, and returned the top matches. 

This may be the first (public) implementation of the algorithm in a mainstream language. 

#### SIFT (todo)

> Algorithm Summary 

( to be expanded)

> Implementation 

( to be expanded)

#### pHash 

> Summary 
source: phash.org 

pHash is an open source suite of perceptual hashes, which are used to generate the signatures for our images. Whereas most hashes generate completely different output if even a single bit is changed, the output of a perceptual hash reflects the extent of the change. 

The DCT hash is based on the discrete cosine transform, which is commonly used in signal processing. It outputs a fixed length hash 8 bytes long. The radial hash creates feature vectors based on lines through the image. It outputs 40 unsigned integers.

To calculate the distance (or the similarity) between two images, pHash uses hamming distance and cross-correlation for DCT and radial hashing, respectively. 

> Implementation 

The pHash wrapper (in C) was mostly straightforward, save for wrangling of how to print unsigned 8-bit integers. The processing program was somewhat tricky in that I was trying to recreate instances of the Digest struct used in the pHash header file, which has an accompanying C++ file. Eventually, I used a C++ static cast on my instance, which worked despite the source code being in C. 

#### OpenCV (for C++) Histogram Comparison 

> Summary 

The signature for each image is a histogram. Histograms represent the range intensity values in an image through organizing values by bins. OpenCV provides four metrics for comparing histograms: Correlation, chi-square, intersection, and Bhattacharyya distance. For correlation and intersection, higher values are better; for chi-square and Bhattacharyya, 0 indicates perfect match. 

> Implementation 

The wrapper for the openCV interface converted the image from RGB to HSV colorspace, assigned a histogram to it, and normalized the histogram (all using openCV library functions). Because of the overall fast performance of histogram assignment, only one program (and no intermediate form) was required to perform scoring on each of the query sets over all four metrics. 

The main implementation hurdle for openCV was that C++ didn't have native support for traversing the image directory, forcing me to download Boost, which for reasons that are still mysterious to me refused to work with CMake (the Makefile creator for C-languages) for a long time.

# Results 

## Front-end evaluations 

### Threat models 

> Artist impersonation 

> Mass-reputation attacks 

> 

### User studies 

Unfortunately, we were unable to conduct user studies on the front-end user experience. 

## Back-end evaluations 

Since all of the algorithms were signature-based, most scalability and memory concerns were alleviated. Hence I focused on testing for accuracy--their ability to detect matches from the database under simple modifications.

### Accuracy 

> Query sets 

I extracted a set of 20 query images from the source database (of 3500 images), mostly digital paintings. I kept the original set as "confirmation tests" of the algorithms' base accuracy. I then applied various digital manipulations and modifications to the set after duplication: 
- Color changes: hue, tint, grayscale
- Size changes: upscaling, downscaling, aspect ratio distortion
- Compositional changes: rotation, cropping 
- Histogram changes: contrast
- Filter-type changes: paint-effect

Although including even more modifications would have expanded the breadth of the accuracy tests, almost all of the low-effort copyright infringement methods are covered--mainly the size and compositional changes. 

> Procedure 

For each algorithm I wrote two programs: 
1. The algorithm itself (or a wrapper around it), which assigns a signature to each image in the source database, and outputs the signatures along with the file names. 
2. A processing program that "reconstructs" the signatures from the intermediate form output by the first program, stores the signatures in a simple data structure, and returns the top matches of the scoring function on each query image. 

The threshold for determining what is a "top" match is to consider only the top ten images by corresponding similarity score, and discarding the rest. Ten may seem arbitrary, but it is the upper bound for the number of small thumbnail matches that can be displayed next to a web image without significantly disrupting the composition of the page. 

> Analytics tools 

To expedite the checking process, I wrote a short script that takes in the top matches for each query image in a fixed format, and parses its correctness based on a text file of the mapping between each query image filename and its actual filename in the base. It also differentiates between a "perfect" match (the top match is correct), and other matches (if some filename elsewhere in the top 10 is correct), as well as counts of each instance.  

#### Results
                dct | radial | cv1  |  cv2   |  cv3  | cv4  |  fimq 
original        X       X       X       X       X       X       X             
aspectratio     X               X       X       X       X       X
contrast        X       X       X       X*      X*      X       X
grayscale       X       X                                       X?
paint           X       X       X       X       X       X       X   
resizeup        X       X       X       X       X       X       X
resizedown      X       X       X       X       X       X       X
tint            X       X                                       X
hue             X       X                                       X
cropping                        X       X*      X*      X   
rotate                          X       X       X       X 

For all four openCV measures, tint, grayscale, and hue were difficult to detect, implying that histogram comparisons do not do well with color changes. This makes sense, as the intensity values of an image are altered drastically after color changes, whereas composition changes (such as rotating and cropping) do not. On the other hand, fmiq and pHash perform very well on color-related filters, perhaps due to the wavelet decomposition approach as opposed to a simplistic histogram bin approach. However, they are much more sensitive to composition changes. 

It must also be noted that some of the color changes, especially the tint and hue test cases and in some cases aspectratio, involved altering the images beyond the point of what any plagiarizer would likely need to do to make a profit. In this respect, the resizing, compositional and contrast cases are much more likely to be used to alter an image. 

#### Breakdown

> openCV 

Within the four OpenCV metrics, the ranking of accuracy is as follows: 
1) Bhattacharyya 
2) Correlation
3) Chi-square
4) Intersection

This makes sense, as intersection is the simplest metric --taking the min of the two histogram values at each intensity value. Bhattacharyya is a measure of similarity between two probability distributions, and is widely used in feature extraction and image processing. 

> pHash 

The main difference between the performance of the radial and DCT hashes is that DCT is invariant against changes in aspect ratio but radial hashing is not. One explanation for this is that radial hashing extracts a feature vector from the variances of 180 lines drawn through the image, and /then/ compacts it with DCT; the variances would alter significantly with a change in aspect ratio.

### Scalability 

[Unfortunately, no formal timing tests on the algorithms were conducted.] 

To test for scalability, we determined the time it took for each algorithm to query one image on datasets of varying size (Ninitial = 350, step=350).

### Memory evaluations 

Without data structures: 
pHash DCT:          64-bits 
pHash Radial:       8 * 40 = 320 bits
openCV Histogram    256 * 8 >= 2048  
Image querying:     60 * 32 >= 1920 bits for each image + 256^2*6 (constant, for search arrays)

# Discussion 

## Challenges & limitations 

For the front-end, the main barrier to implementing a more complete version of the architecture in the project proposal was the sheer number of technologies that needed to be learned to even perform very basic operations, such as uploading to a database (which basically required knowledge of full stack web programming). [ to be expanded ]

The main challenges to implementing the image-similarity library was getting the dependencies to play together nicely, remembering to sanitize the image database, and determining a good intermediate form for each of the algorithms.

There are many limitations to the project as it is now:
- The only supported image file format is .jpg. Although it is the most used image format on the web, it would be ideal to expand support to .png, .gif, and .tiff. 
- The only supported color space is RGB. Although RGB (including sRGB and adobe RGB) is technically made for the web, certain images may have CMYK, YIQ or other colorspaces. 
- The only supported medium is visual art. It would be interesting to expand the concept of "similarity algorithms" to audio and video. pHash is the only library out of those implemented that supports similarity among other videos and audio files; however, since video is a series of images, one could easily extend current measures to include video as well. Furthermore, DCT and other wavelet transforms are already being used for audio processing. 
- On the image database side, there needs to be diversification in the type and medium of images. Including more works in traditional/mixed media and photographs would solidify and corroborate on our claim that the algorithms' are invariant against certain modifications regardless of medium.
- pHash recently released a new hashing scheme based on the Maar wavelet. Its digest is longer than both DCT and radial hashing, but it is especially good at encoding edge and corner information. 

For more thorough testing, we need a larger set of queries (fimq, for example, used 100).

## Future work 
Some interesting features for the front-end that were mentioned in the initial project proposal but were not implemented were: 

- Integration with Creative Commons schemes. CryptoDraw's user interface design (for the viewer) is flexible enough to adapt to all licenses, from the most accommodating (the Attribution only license) to least (the Attribution, No Commercial, No Derivative license). For example, a special Derivation symbol could be displayed to signal the viewer that they would need permission from the artist before they can upload derivative fanworks.  
- Uploading works in progress as proof of authenticity. As the artist works, they upload snapshots of the work in progress to the artwork's hashed linked list node. The signature of each snapshot is calculated (perceptual hash for pHash, histogram for openCV, etc). The smaller the difference between each successive pair of scores, and the greater the number of scores there are, the less the chance that the work was copied. This is an additional way to verify the authenticity of an image.
- The Web of Trust scheme for openPGP. Otherwise known as a reputation system. Each user has the ability to rate the trustworthiness of any other profile, as well as the overall trustworthiness of the owner of that profile to rate other people’s reputation. This protects against mass Internet hordes aiming to launch a reputation attack against a well-known artist (for creating political art, for instance).
- A rudimentary log-in system and "verified" accounts. The extension is registered with deviantART and tumblr, and if a viewer could quickly check the credentials of an artist profile, that would be a useful feature. Also, currently the public/private keys are not portable across computers because they are generated upon installation of the extension. By associating the keys with user accounts, we increase the portability of CryptoDraw across machines. 

For the back-end, interesting directions for exploration include:
- Possibly sub-linear time for returning top matches. For example, the MVP (Multi-Vantage Point) tree indexing structure used in the pHash Maar wavelet scheme minimizes the number of computations needed to calculate the distance from one n-dimensional point to the next. 
- Integrating algorithms for best results. Histogram and wavelet-based algorithms are each invariant against certain modifications. By including results from each, we increase the probability of finding the top match. However, this is a two-fold increase in scoring time. 
- The paper "Image Similarity Search with Compact Data Structures" by Lv, Charikar and Li (2004) describes an interesting method of computing similarity using feature vectors of image regions and compact metadata. The paper supplies a metric based on EMD (Earth Movers Distance), and an L1 distance function to calculate distances between distributions of feature vectors. Those vectors are then compacted into bit vectors, which save between 3-72 times as much space as traditional data structures. Finally, an EMD-based embedding method for querying of images speeds up the process significantly. If we had more time, we would certainly implement the algorithms from this paper. 
    + Source: http://www.cs.princeton.edu/courses/archive/spr05/cos598E/bib/p208-lv.pdf

## Reflection

Overall, I severely underestimated the time it took to learn the new tools (and languages, in the case of C++) required for the project. For the front-end, I essentially learned full-stack web development, from PHP scripting and database querying to various quirks of Chrome extensions such as background/content scripts and the OAuth2 API. I still think that the overall user interface approach / architecture is correct, but there are enough logistical issues specific to Chrome extensions (and Javascript, in particular) that I am unsure if there exist workarounds for all of them. 

In terms of the backend, selecting signature-based algorithms as the class to explore was not something that I consciously knew to do initially, but many of the most efficient algorithms seemed to have this in common. However, I also read plenty of other papers that used seemingly novel methods, such as semantic texton forests. Because I'm not an expert in computational vision, my range of possible approaches is inherently limited by my conception of the problem. 

# Conclusion 

Although we did not complete a proof-of-concept implementation of CryptoDraw, we have laid the groundwork for joining the two parts of the project together. 


### Effort 

Lining: 
- initial project proposal for Cryptodraw
- initial presentation (4/10)
- class demo presentation (4/28)
- all of the backend library except for the SIFT implementation
- all technical evaluation of the backend
- setting up the front end extension, scripts, databases

Yutaro: 
- SIFT implementation and scalability testing

Alexandra: 
- Frontend: report image button
