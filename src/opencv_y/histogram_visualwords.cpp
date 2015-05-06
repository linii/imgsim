#include <iostream>
#include <fstream>
#include <dirent.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv2/legacy/compat.hpp>

using namespace std;

const char* IMAGE_FILE_DIR = "caltech10";
const int DIMENSIONS = 128;
const int SURF_PARAM = 400;
const int NUM_CLUSTER = 500; //the number of clusters <=> the # of dimensions of Visual Words

/* Extract SURF from image files
    input: filename     image file name
    output: queryKeypoints, storage (= memory space), localDescriptors (= SURF for each key point)
    return: 0 sucess, 1 fail
*/
int getSURF(const char* file_name, CvSeq** queryKeypoints, CvSeq** localDescriptors, CvMemStorage** storage) {
    // Load images in gray scale
    printf("%s\n", "Hello!");
    printf("%s\n", file_name);
    IplImage* img = cvLoadImage(file_name, CV_LOAD_IMAGE_GRAYSCALE);
    if (img == NULL) {
        cerr << "cannot load image: " << file_name << endl;
        return 1;
    }

    *storage = cvCreateMemStorage(0);
    CvSURFParams parameters = cvSURFParams(SURF_PARAM, 1);
    cvExtractSURF(img, 0, queryKeypoints, localDescriptors, *storage, parameters);

    return 0;
}

 /* Transform all images in IMAGE_FILE_DIR into histogram
    Create histogram by voting for the closest visual word for each localDescriptors
    input: features (= visual words)
 */
int calculateHistograms(CvMat* features) {
    // indexing by kd-tree for fast search of nearest visual words
    CvFeatureTree* featureTree = cvCreateKDTree(features);

    // open a file to output each image's histogram
    fstream fileOut;
    fileOut.open("histograms2.txt", ios::out);
    if (!fileOut.is_open()) {
        cerr << "cannot open file: histograms2.txt" << endl;
        return 1;
    }

    // transform each image in IMAGE_FILE_DIR
    DIR* dirp;
    if ((dirp = opendir(IMAGE_FILE_DIR)) == NULL) {
        cerr << "cannot open directory: " << IMAGE_FILE_DIR << endl;
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dirp)) != NULL) {
        char* filename = entry->d_name;
        if (strcmp(filename, ".DS_Store") != 0){
            if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
                continue;
            }

            char filepath[256];
            snprintf(filepath, sizeof filepath, "%s/%s", IMAGE_FILE_DIR, filename);

            // initialize histogram
            int* histogram = new int[features->rows];
            for (int i = 0; i < features->rows; i++) {
                histogram[i] = 0;
            }

            // get SURF
            CvSeq* queryKeypoints = NULL;
            CvSeq* localDescriptors = NULL;
            CvMemStorage* storage = NULL;
            int returnedVal = getSURF(filepath, &queryKeypoints, &localDescriptors, &storage);
            if (returnedVal != 0) {
                cerr << "error in getSURF" << endl;
                return 1;
            }

            // expand feature vectors into matrix for fast search via kd-tree
            CvMat* mat = cvCreateMat(localDescriptors->total, DIMENSIONS, CV_32FC1);
            CvSeqReader reader;
            float* pointer = mat->data.fl;
            cvStartReadSeq(localDescriptors, &reader);
            for (int i = 0; i < localDescriptors->total; i++) {
                float* desc = (float*)reader.ptr;
                CV_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
                memcpy(pointer, desc, DIMENSIONS*sizeof(float));
                pointer += DIMENSIONS;
            }

            // vote for the closest visual word for each localDescriptors
            int k = 1;  // 1-NN
            CvMat* indices = cvCreateMat(queryKeypoints->total, k, CV_32SC1);  // the nearest's visual word's index
            CvMat* dists = cvCreateMat(queryKeypoints->total, k, CV_64FC1);    // and its distance
            cvFindFeatures(featureTree, mat, indices, dists, k, 250);
            for (int i = 0; i < indices->rows; i++) {
                int index = CV_MAT_ELEM(*indices, int, i, 0);
                histogram[index] += 1;
            }

            // write histogram infor in file
            fileOut << filepath << "\t";
            for (int i = 0; i < features->rows; i++) {
                fileOut << float(histogram[i]) / float(localDescriptors->total) << "\t";
            }
            fileOut << endl;

            // cleanup
            delete[] histogram;
            cvReleaseMemStorage(&storage);
            cvReleaseMat(&mat);
            cvReleaseMat(&indices);
            cvReleaseMat(&dists);
            cvClearSeq(queryKeypoints);
            cvClearSeq(localDescriptors);
        }
    }

    fileOut.close();
    cvReleaseFeatureTree(featureTree);

    return 0;
}


/* Extract local features from all the images in IMAGE_FILE_DIR and store them in matrix
    input: featuresMat (= matrix for local features)
    output: data (for featuresMat)
*/
int loadDescriptors(CvMat& featuresMat, vector<float>& data) {
    // open IMAGE_FILE_DIR
    DIR* dirp;
    if ((dirp = opendir(IMAGE_FILE_DIR)) == NULL) {
        cerr << "cannot open directory: " << IMAGE_FILE_DIR << endl;
        return 1;
    }

    // run through image files in IMAFE_FILE_DIR
    struct dirent* entry;
    while ((entry = readdir(dirp)) != NULL) {
        char* filename = entry->d_name;
        if (strcmp(filename, ".DS_Store") != 0){
            if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
                continue;
            }

            // change name from *.jpg to IMAGE_FILE_DIR/*.jpg
            char filepath[256];
            snprintf(filepath, sizeof filepath, "%s/%s", IMAGE_FILE_DIR, filename);

            // extract SURF
            CvSeq* queryKeypoints = NULL;
            CvSeq* localDescriptors = NULL;
            CvMemStorage* storage = NULL;
            int ret = getSURF(filepath, &queryKeypoints, &localDescriptors, &storage);
            if (ret != 0) {
                cerr << "error in getSURF" << endl;
                return 1;
            }

            // filepath\t[vectors]
            cout << filepath << "\t" << localDescriptors->total << endl;

            // add features to data
            for (int i = 0; i < localDescriptors->total; i++) {
                float* d_vector = (float*)cvGetSeqElem(localDescriptors, i);  // vec
                for (int j = 0; j < DIMENSIONS; j++) {
                    data.push_back(d_vector[j]);
                }
            }

            cvClearSeq(queryKeypoints);
            cvClearSeq(localDescriptors);
            cvReleaseMemStorage(&storage);
        }
    }

    // transform data to CvMat
    int rows = data.size() / DIMENSIONS; // row of CvMat
    cvInitMatHeader(&featuresMat, rows, DIMENSIONS, CV_32FC1, &data[0]);

    return 0;
}


int main() {
    int returnedVal;

    // extract local descriptors from each image in IMAGE_FILE_DIR
    cout << "Loading Descriptors ..." << endl;
    CvMat featuresMat;
    vector<float> data;
    returnedVal = loadDescriptors(featuresMat, data);

    // Cluster localDescriptors (feature vectors)
    // Compute means of each cluster
    cout << "Clustering ..." << endl;
    CvMat* cluster_ID = cvCreateMat(featuresMat.rows, 1, CV_32S);
    CvMat* mean_vec = cvCreateMat(NUM_CLUSTER, DIMENSIONS, CV_32FC1);
    cvKMeans2(&featuresMat, NUM_CLUSTER, cluster_ID, cvTermCriteria(CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10, 1.0), 1, 0, 0, mean_vec, 0);
    cvReleaseMat(&cluster_ID);  // won't use cluster_ID anymore

    // transform each image into visual word's histogram
    // mean_vec will become visual word for each image
    cout << "Calculating Histograms ..." << endl;
    calculateHistograms(mean_vec);
    cvReleaseMat(&mean_vec);

    return 0;
}

