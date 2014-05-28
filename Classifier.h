#ifndef _CLASSIFIER_H_INCLUDED_
#define _CLASSIFIER_H_INCLUDED_

#include "svm_light\sparse_vector.h"
#include "svm_light\svm_light_util.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "cv.h"
#include "FeatureExtraction.h"
#include "highgui.h"

#define DIMENSIONS (31*SLIDING_WINDOW_WIDTH*SLIDING_WINDOW_HEIGHT)
#define COST 0

using namespace std;
using namespace cv;

class Classifier {
private:
	SvmLightUtil svm;
	joachims::MODEL* model;
	SvmLightUtil::Parameters params;
	vector<SparseVector> pos_examples;
	vector<SparseVector> neg_examples;
public:
	Classifier(){
		SvmLightUtil::Parameters params;
		params.kernel_type = SVM_LIGHT_LINEAR;
		params.model_type = CLASSIFICATION;
		params.svm_c = COST;
		this->params = params;
		svm = SvmLightUtil();
	}

	void loadModel(string modelFile);
	void convertToSparseVector(Mat hog_features);
	void addPositiveExample(Mat hog_features);
	void addNegativeExample(Mat hog_features);
	void classify(Mat hog_features);
	void train(string modelFile);
};

#endif