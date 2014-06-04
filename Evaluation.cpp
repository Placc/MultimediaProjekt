#include "Evaluation.h"

//TO-DO: weight!!!

void drawStringInImage(string label, Mat image, int x, int y, Scalar color) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.5;
	int thickness = 1;
	int baseline = 0;
	Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	Point pt(x, y);
	putText(image, label, pt, fontface, scale, color, thickness, 8);
}
float precision(float tp, float fp) {
	return(tp/(tp+fp));
}
float recall(float tp, float fn) {
	return(tp/(tp+fn));
}

namespace Evaluation{
	void showImageWithDetections(OriginalImage &image) {
		Mat matToShow = image.image.clone();	
		vector<Rect> boundingBoxes = Preprocessing::getBoundingBoxesByFile(image.path);
		//Draw (positive) detection boxes
		for(vector<WeightedRect>::iterator detBoxIt = image.detectedBoxes.begin(); detBoxIt != image.detectedBoxes.end(); ++detBoxIt) {
			float weight = (*detBoxIt).weight; 
			Rect detRect = (*detBoxIt).rect;
			//Farbe, etc. Nicht-Hardcoded
			cv::rectangle(matToShow, detRect, CV_RGB(0,255,0), 1,8,0);

			//print detection score (weight)
			char text [20];
			sprintf(text, "%i/%", (weight*100));
			drawStringInImage(text, matToShow, detRect.tl.x, detRect.tl.y, CV_RGB(0,255,0));

			//max overlap
			float max = 0;
			for(vector<Rect>::iterator bouBoxIt = boundingBoxes.begin(); bouBoxIt != boundingBoxes.end(); ++bouBoxIt) {
				Rect rect = (*bouBoxIt);
				double overlap = (*detBoxIt).getOverlap(rect);
				if (overlap > max) max=overlap;
			}
			char text [20];
			sprintf(text, "%i/%", (weight*100));
			//!Verschiebung nach unten (+10) nicht Hardcoded
			drawStringInImage(text, matToShow, detRect.tl.x, detRect.tl.y +10, CV_RGB(0,255,0));

		}
		//Draw (true) bounding boxes
		for(vector<Rect>::iterator bouBoxIt = boundingBoxes.begin(); bouBoxIt != boundingBoxes.end(); ++bouBoxIt) {
			Rect rect = (*bouBoxIt);
			//Farbe, etc. Nicht-Hardcoded
			cv::rectangle(matToShow, rect, CV_RGB(255,255,0), 1,8,0);
		}

		cv::imshow(image.path, matToShow);
		cv::waitKey();
	}
	void blub() {
		vector<OriginalImage> pos_examples;
		vector<OriginalImage> neg_examples;
		Rect roi(Point(16,16), Point(80, 144));
		Preprocessing::loadImagesByDirectory(TEST_POS_NORMALIZED, pos_examples);
		Preprocessing::loadImagesByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		for(vector<OriginalImage>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt){
			(*posIt).image = (*posIt).image(roi);
			FeatureExtraction::computeHOGPyramid(*posIt);
			FeatureExtraction::computeSlidingWindows(*posIt);
			for(std::vector<Image>::iterator scaleIt = (*posIt).lower_images.begin(); scaleIt != (*posIt).lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}
		}
		for(vector<OriginalImage>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			
			//TODO: Randomly select some hypothesis of all, s.t. not everything has to be computed...

			FeatureExtraction::computeHOGPyramid(*negIt);
			FeatureExtraction::computeSlidingWindows(*negIt);
			for(std::vector<Image>::iterator scaleIt = (*negIt).lower_images.begin(); scaleIt != (*negIt).lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}
		}

	}
}