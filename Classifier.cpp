#include "Classifier.h"


using namespace cv;
using namespace std;

SparseVector Classifier::convertToSparseVector(Mat hog_features)
{
	cout << "asdf" << endl;
	SparseVector vec(hog_features.rows * hog_features.cols);
	for(int y = 0; y < hog_features.rows; y++)
	{
		for(int x = 0; x < hog_features.cols; x++)
		{
			vec[y * hog_features.cols + x] = hog_features.at<float>(y,x);
		}
	}
	return vec;
}

void Classifier::loadModel( string modelFile )
{
	model = SvmLightUtil::getModel( modelFile );
}

void Classifier::addPositiveExample( cv::Mat hog_features )
{
	SparseVector vector = convertToSparseVector( hog_features );
	pos_examples.push_back( vector );
}

void Classifier::addNegativeExample( cv::Mat hog_features )
{
	SparseVector vector = convertToSparseVector( hog_features );
	neg_examples.push_back( vector );
}


double Classifier::classify( Mat hog_features )
{
	SparseVector vec = convertToSparseVector( hog_features );
	double value = svm.classify( vec, DIMENSIONS, model );

	return value;
}

void Classifier::train( string modelFile )
{
	svm.train( pos_examples, neg_examples, DIMENSIONS, modelFile, 1.0, params );
}

