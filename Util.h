#ifndef _UTIL_H_INCLUDED_
#define _UTIL_H_INCLUDED_

#pragma once

#include "WeightedRect.h"
#include "Eval.h"
#include "cv.h"
#include "highgui.h"
#include <Windows.h>

using namespace std;
using namespace cv;

namespace Util {
	bool compareRects(WeightedRect &rect1, WeightedRect &rect2);
	bool compareEvals(Eval &eval1, Eval &eval2);
	bool comparePlotValues(Point2d &p1, Point2d &p2);
};

#endif