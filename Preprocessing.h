#ifndef _PREPROCESSING_H_INCLUDED_
#define _PREPROCESSING_H_INCLUDED_

#pragma once

#include "cv.h"
#include "highgui.h"
#include "OriginalImage.h"
#include "PATHS.h"
#include <io.h> 
#include <fstream>
//#include <boost\filesystem.hpp>

using namespace std;
using namespace cv;

namespace Preprocessing {
	vector<Rect> getBoundingBoxesByFile(string path);
	void loadPathsByDirectory(string dirPath, vector<string> &image_vector);
	bool checkINRIADirectory(string dirPath);
}

#endif