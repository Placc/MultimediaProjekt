#include "Classifier.h"

using namespace cv;
using namespace std;

SparseVector convertToSparseVector(Mat hog_features){
	SparseVector vec(hog_features.rows * hog_features.cols);
	for(int y = 0; y < hog_features.rows; y++){
		for(int x = 0; x < hog_features.cols; x++){
			vec[y * hog_features.cols + x] = hog_features.at<float>(y,x);
		}
	}
	return vec;
}