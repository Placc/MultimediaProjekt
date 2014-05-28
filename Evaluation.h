#ifndef _EVALUATION_H_INCLUDED_
#define _EVALUATION_H_INCLUDED_

#include "opencv2\imgproc\imgproc.hpp"
#include "cv.h"
#include "highgui.h"
#include "OriginalImage.h"

namespace Evaluation {
	void showImageWithDetections(OriginalImage &image);
	void plotQuantitative(vector<OriginalImage> &pos, vector<OriginalImage> &neg); //?
}

#endif