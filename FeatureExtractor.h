#include "cv.h"
#include "highgui.h"
#include "vl\hog.h"
#include "Image.h"
#include "hog_util.h"

using namespace std;
using namespace cv;

class FeatureExtractor{
public:
	void extractHOGFeatures(Image &image, int numberOfOrientations, int cellSize);
};