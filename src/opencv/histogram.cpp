// Wrapper program for openCV histogram comparison
// Called in the following format
// ./hist [ query directory ] [ img directory ] [ number of top results ]
//
// Lining Wang
// 4/28/2015
//
// Source: http://docs.opencv.org/doc/tutorials/imgproc/histograms/histogram_comparison/histogram_comparison.html


#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <typeinfo>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

int main( int argc, char** argv ) {
    int h_bins = 50;
    int s_bins = 60;
    int histSize[] = { h_bins, s_bins };

    float h_ranges[] = { 0, 180 };
    float s_ranges[] = { 0, 256 };

    const float* ranges[] = { h_ranges, s_ranges };

    int channels[] = { 0, 1 };

    string slash ("/");
    string queries (argv[1]);
    string dir = queries + slash;
    fs::path imageDir(dir);
    fs::directory_iterator it(imageDir), eod;

    std::vector<MatND> array(0);
    std::map<int, string> dict;

    MatND crazy;
    BOOST_FOREACH(fs::path const &p, std::make_pair(it, eod))
    {
        if(!is_regular_file(p))
            continue;

        Mat hsv_test, src_test;

        string another = p.filename().string();
        string complete = dir + another;
        // cout << another << endl;

        src_test = imread(complete, 1);

        if (!src_test.data)
            continue;

        cvtColor(src_test, hsv_test, COLOR_BGR2HSV);
        MatND hist_test;
        calcHist( &hsv_test, 1, channels, Mat(), hist_test, 2, histSize, ranges, true, false );
        normalize( hist_test, hist_test, 0, 1, NORM_MINMAX, -1, Mat() );


        array.push_back(hist_test);
        size_t pos = array.size() - 1;
        dict[pos] = another;
    }

    // go through query file
    string base (argv[2]);
    int n = atoi(argv[3]);
    string dirname = base + slash;
    fs::path queryDir(dirname);
    fs::directory_iterator iterator(dirname), eodin;

    string measurement = base;
    cout << "TESTING: " << measurement << endl;

    map <double, string> dictcorr; map <double, string> dictchi; map <double, string> dictinter;
    map <double, string> dictbhat;

    BOOST_FOREACH(fs::path const &p, std::make_pair(iterator, eodin))
    {
        if(is_regular_file(p))
        {
            vector <double> corr(0);
            vector <double> chi(0);
            vector <double> inter(0);
            vector <double> bhat(0);

            // Calculate histogram of query file
            Mat base, hsv;
            string filename = p.filename().string();

            string complete = dirname + "/" + filename;
            base = imread(complete, 1);

            if (!base.data)
                continue;

            cvtColor(base, hsv, COLOR_BGR2HSV);

            MatND hist;
            calcHist( &hsv, 1, channels, Mat(), hist, 2, histSize, ranges, true, false );
            normalize( hist, hist, 0, 1, NORM_MINMAX, -1, Mat() );

            // make comparisons
            for (int i = 0; i < array.size(); i++) {
                double result1 = compareHist(hist, array[i], 0);
                double result2 = compareHist(hist, array[i], 1);
                double result3 = compareHist(hist, array[i], 2);
                double result4 = compareHist(hist, array[i], 3);

                corr.push_back(result1);
                chi.push_back(result2);
                inter.push_back(result3);
                bhat.push_back(result4);

                dictcorr[result1] = dictchi[result2] = dictinter[result3] = dictbhat[result4] = dict[i];
            }

            sort(corr.begin(), corr.end(), std::greater<double>());
            sort(inter.begin(), inter.end(), std::greater<double>());
            sort(chi.begin(), chi.end());
            sort(bhat.begin(), bhat.end());



            cout << "METHOD: (OpenCV) Correlation Metric" << endl;
            cout << "QUERY: " << filename << " ";
            for (int i = 0; i < n; i++) {
                cout << dictcorr[corr[i]] << " " << corr[i]<< endl;
            }
            cout << endl;

            cout << "METHOD: (OpenCV) Chi-Square Metric" << endl;
            cout << "QUERY: " << filename << " ";
            for (int i = 0; i < n; i++) {
                cout << dictchi[chi[i]] << " " << chi[i] << endl;
            }
            cout << endl;

            cout << "METHOD: (OpenCV) Intersection Metric" << endl;
            cout << "QUERY: " << filename << " ";
            for (int i = 0; i < n; i++) {
                cout << dictinter[inter[i]] << " " << inter[i] << endl;
            }
            cout << endl;

            cout << "METHOD: (OpenCV) Bhattacharyya Metric" << endl;
            cout << "QUERY: " << filename << " ";
            for (int i = 0; i < n; i++) {
                cout << dictbhat[bhat[i]] << " " << bhat[i] << endl;
            }
            cout << endl;
        }
    }
}
