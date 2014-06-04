#ifndef _FEATUREEXTRACTION_H_INCLUDED_
#define _FEATUREEXTRACTION_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"
#include "Image.h"
#include "OriginalImage.h"
#include "vl\hog.h"
#include "hog_util.h"
#include "SlidingWindow.h"

#define NUMBER_OF_ORIENTATIONS 9
#define CELL_SIZE 8
#define STEPS_PER_OCTAVE 5
#define SLIDING_WINDOW_HEIGHT 16
#define SLIDING_WINDOW_WIDTH 8
#define SLIDING_WINDOW_X_STEP 1 
#define SLIDING_WINDOW_Y_STEP 1 

namespace FeatureExtraction{
	
	void extractHOGFeatures(Image &image);
	void computeHOGPyramid(OriginalImage &image);
	float* computeScaleFactors();
	void computeSlidingWindows(Image &image);
	Mat getHogOfSlidingWindow(Mat hog_features, int swHogPos, int hogCellsPerRow);
}

#endif