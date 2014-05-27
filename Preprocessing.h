#ifndef _PREPROCESSING_H_INCLUDED_
#define _PREPROCESSING_H_INCLUDED_

#include "cv.h"
#include "highgui.h"
#include "OriginalImage.h"
#include <fstream>
//#include <boost\filesystem.hpp>

using namespace std;
using namespace cv;

namespace Preprocessing {
	vector<Rect> getBoundingBoxesByFile(string path);
	void loadImagesByDirectory(string dirPath, vector<OriginalImage> &image_vector);
}

#endif