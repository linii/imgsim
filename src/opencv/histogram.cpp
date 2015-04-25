#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include </usr/local/include/boost/filesystem.hpp>
#include </usr/local/include/boost/foreach.hpp>

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

int main( int argc, char** argv ) {

    /// Using 50 bins for hue and 60 for saturation
    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    // hue varies from 0 to 179, saturation from 0 to 255
    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 };

    Mat src_base, hsv_base;
    src_base = imread( argv[1], 1 );
    cvtColor( src_base, hsv_base, COLOR_BGR2HSV );

    MatND hist_base;
    calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
    normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

    fs::path targetDir("/test");
    fs::directory_iterator it(targetDir), eod;

    std::vector<MatND> array(1);
    BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
    {
        if(is_regular_file(p))
        {
            Mat hsv_test, src_test;
            string another = p.filename().string();
            src_test = imread(another, 1);
            cvtColor(src_test, hsv_test, COLOR_BGR2HSV);

            MatND hist_test;
            calcHist( &hsv_test, 1, channels, Mat(), hist_test, 2, histSize, ranges, true, false );
            normalize( hist_test, hist_test, 0, 1, NORM_MINMAX, -1, Mat() );

            array.push_back(hist_test);
        }
    }

    vector< vector<double> > matrix;
    matrix.resize(array.size(), vector<double>(3, 0));
    int j;

    for (int i = 0; i < array.size(); i++) {
        for (j = 0; j < 4; j++){
            matrix[j][i] = compareHist(hist_base, array[i], j);;
        }
    }

    std::sort(matrix[0].begin(), matrix[0].end(), std::greater<double>());
    std::sort(matrix[2].begin(), matrix[2].end(), std::greater<double>());
    std::sort(matrix[1].begin(), matrix[1].end());
    std::sort(matrix[3].begin(), matrix[3].end());

    for (int k = 0; k < 3; k++) {
        for( std::vector<double>::const_iterator i = matrix[k].begin(); i != matrix[k].end(); ++i)
            std::cout << *i << ' ';
    }
}
