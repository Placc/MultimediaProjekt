#ifndef _TRAINING_H_INCLUDED_
#define _TRAINING_H_INCLUDED_

#include "FeatureExtraction.h"
#include "Image.h"
#include "OriginalImage.h"
#include "Preprocessing.h"
#include "SlidingWindow.h"
#include "PATHS.h"
#include "Classifier.h"
#include "opencv2\imgproc\imgproc.hpp"
#include "cv.h"
#include "highgui.h"

namespace Training {

	void trainSVM(string modelPath);

}

#endif