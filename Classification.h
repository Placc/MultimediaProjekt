#ifndef _CLASSIFICATION_H_INCLUDED_
#define _CLASSIFICATION_H_INCLUDED_

#include "FeatureExtraction.h"
#include "Image.h"
#include "OriginalImage.h"
#include "Preprocessing.h"
#include "SlidingWindow.h"
#include "PATHS.h"
#include "svm_light\sparse_vector.h"
#include "svm_light\svm_light_util.h"

using namespace std;

namespace Classification{
	void classify(string modelFile, string testDir);
}

#endif