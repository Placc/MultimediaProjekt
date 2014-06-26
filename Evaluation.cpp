#include "Evaluation.h"

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
	//Detections threshold-abhängig, daher Detections erst nach dem durchgehen festlegen!
	void determinateDetections(OriginalImage image, vector<ClassifiedRect> classifiedRectsInImage, float threshold) {	
		for(vector<ClassifiedRect>::iterator clasRectIt = classifiedRectsInImage.begin(); clasRectIt!=classifiedRectsInImage.end(); ++clasRectIt) {
			if ((*clasRectIt).distanceFromHyperplane>threshold) {
				image.addDetectedBox(*clasRectIt);
			}
			else break;
		}
	}
	//TO-Do!!
	void plotList(vector<double, bool> listOfClassifiedRects) {

	}
	vector<ClassifiedRect> calcClassifiedRectsInAllScalesOfImage(Classifier classifier, OriginalImage &image) {
		vector<ClassifiedRect> listClassifiedRectsOfImage;

		FeatureExtraction::computeHOGPyramid(image);
		FeatureExtraction::computeSlidingWindows(image);
		//"unschön" -> in FeatureExtraction Funktion computeAllSlidingWindows?
		FeatureExtraction::computeSlidingWindows(image);
		for(std::vector<Image>::iterator scaleIt = image.lower_images.begin(); scaleIt != image.lower_images.end(); ++scaleIt){
			FeatureExtraction::computeSlidingWindows(*scaleIt);
		}

		//Alle Slidingwindows vom Original_Image durchgehen, jedesmal von der hog_features - Mat 
		//den SlidingWindow-Bereich anschauen, diesen "klassifizieren"
		//  double value = classifier.classify(image.hog_features(slidingWindow));
		//SlidingWindow anschließend zur Liste hinzu
		vector<SlidingWindow> slidingWindows = image.slidingWindows;
		for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
			Rect window = (*slidWinIt).slidingWindow.rect;
			double distanceFromHyperplane = classifier.classify(image.hog_features(window));
			ClassifiedRect detectionBox(window, distanceFromHyperplane);
			(*slidWinIt).slidingWindow = detectionBox;
			float scaleFactor = (*slidWinIt).scaleFactor;
			Rect windowNormalized(
				window.x * scaleFactor, 
				window.y * scaleFactor, 
				window.width * scaleFactor,
				window.height * scaleFactor
			);
			ClassifiedRect detectionBoxNormalized(windowNormalized, distanceFromHyperplane);
			listClassifiedRectsOfImage.push_back(detectionBoxNormalized);
		}
		//...
		//Das gleiche dann für alle lower_images vom Image
		vector<Image> lowerImages = image.lower_images;
		for(vector<Image>::iterator lowImIt = lowerImages.begin(); lowImIt!=lowerImages.end(); ++lowImIt) {
			Image lowerImage = (*lowImIt);
			for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
				Rect window = (*slidWinIt).slidingWindow.rect;
				double distanceFromHyperplane = classifier.classify(lowerImage.hog_features(window));
				ClassifiedRect detectionBox(window, distanceFromHyperplane);
				(*slidWinIt).slidingWindow = detectionBox;
				float scaleFactor = (*slidWinIt).scaleFactor;
				Rect windowNormalized(
					window.x * scaleFactor, 
					window.y * scaleFactor, 
					window.width * scaleFactor,
					window.height * scaleFactor
				);
				ClassifiedRect detectionBoxNormalized(windowNormalized, distanceFromHyperplane);
				listClassifiedRectsOfImage.push_back(detectionBoxNormalized);
			}
		}	
		return(listClassifiedRectsOfImage);
	}
	void compareWithGroundTruthBoxes(OriginalImage &image, vector<ClassifiedRect> classifiedRectsInImage) {
		vector<Rect> groundTruthBoxes = Preprocessing::getBoundingBoxesByFile(image.path);
		
		for(vector<ClassifiedRect>::iterator crIt = classifiedRectsInImage.begin(); crIt != classifiedRectsInImage.end(); ++crIt) {
			ClassifiedRect detRect = (*crIt);
			for(vector<Rect>::iterator gtBoxIt = groundTruthBoxes.begin(); gtBoxIt != groundTruthBoxes.end(); ++gtBoxIt) {
				Rect rect = (*gtBoxIt);
				bool overlapping = detRect.checkOverlap(rect, 0.5);
				if (overlapping) {
					break;
					detRect.groundTruth=true;
				}
			}
		}
	}
	void showImageWithDetections(OriginalImage &image, float threshold) {
		Mat matToShow = image.image.clone();	
		vector<Rect> groundTruthBoxes = Preprocessing::getBoundingBoxesByFile(image.path);
		//Draw (positive) detection boxes
		for(vector<ClassifiedRect>::iterator detBoxIt = image.detectedBoxes.begin(); detBoxIt != image.detectedBoxes.end(); ++detBoxIt) {
			double distanceFromHyperplane = (*detBoxIt).distanceFromHyperplane; 
			Rect detRect = (*detBoxIt).rect;
			//Farbe, etc. Nicht-Hardcoded
			cv::rectangle(matToShow, detRect, CV_RGB(0,255,0), 1,8,0);

			//print detection score (distanceFromHyperplane)
			char text [20];
			sprintf(text, "%i/%", (distanceFromHyperplane*100));
			drawStringInImage(text, matToShow, detRect.x, detRect.y, CV_RGB(0,255,0));

			//max overlap
			double max = 0;
			for(vector<Rect>::iterator gtBoxIt = groundTruthBoxes.begin(); gtBoxIt != groundTruthBoxes.end(); ++gtBoxIt) {
				Rect rect = (*gtBoxIt);
				double overlap = (*detBoxIt).getOverlap(rect);
				if (overlap > max) max=overlap;
			}
			text [20];
			sprintf(text, "%i/%", (distanceFromHyperplane*100));
			//Verschiebung nach unten (+10) nicht Hardcoded
			drawStringInImage(text, matToShow, detRect.x, detRect.y +10, CV_RGB(0,255,0));

		}
		//Draw (true) bounding boxes
		for(vector<Rect>::iterator gtBoxIt = groundTruthBoxes.begin(); gtBoxIt != groundTruthBoxes.end(); ++gtBoxIt) {
			Rect rect = (*gtBoxIt);
			//Farbe, etc. Nicht-Hardcoded
			cv::rectangle(matToShow, rect, CV_RGB(255,255,0), 1,8,0);
		}

		cv::imshow(image.path, matToShow);
		cv::waitKey();
	}
	bool sortingClassifiedRects(ClassifiedRect cr1, ClassifiedRect cr2) { 
		return(cr1.distanceFromHyperplane < cr2.distanceFromHyperplane); 
	}

	void qualitativeEvaluation() { //3.5
		Classifier classifier = Classifier();
		vector<String> pos_examples;
		vector<String> neg_examples;
		Preprocessing::loadPathsByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		
		for(vector<String>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt){
			OriginalImage image(*posIt);

			//Werden schon normalisiert (ins Original-Bild umgerechnet)
			vector<ClassifiedRect> classifiedRectsInImage = calcClassifiedRectsInAllScalesOfImage(classifier, image);
			compareWithGroundTruthBoxes(image, classifiedRectsInImage);
			sort(classifiedRectsInImage.begin(), classifiedRectsInImage.end(), sortingClassifiedRects);

			//(Wie kommt man an den Threshold?)
			float threshold;
			determinateDetections(image, classifiedRectsInImage, threshold);
			showImageWithDetections(image);
		}

		for(vector<String>::iterator negIt = neg_examples.begin(); negIt != pos_examples.end(); ++negIt){
			OriginalImage image(*negIt);
			vector<ClassifiedRect> classifiedRectsInImage = calcClassifiedRectsInAllScalesOfImage(classifier, image);
			compareWithGroundTruthBoxes(image, classifiedRectsInImage);
			sort(classifiedRectsInImage.begin(), classifiedRectsInImage.end(), sortingClassifiedRects);

			//(Wie kommt man an den Threshold?)
			float threshold;
			determinateDetections(image, classifiedRectsInImage, threshold);
			showImageWithDetections(image);
		}

	}
	void quantitativeEvaluation() { //3.6
		Classifier classifier = Classifier();
		vector<String> pos_examples;
		vector<String> neg_examples;
		Rect roi(Point(16,16), Point(80, 144));
		//Preprocessing::loadPathsByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_POS_NORMALIZED, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		for(vector<String>::iterator posIt = pos_examples.begin(); posIt != pos_examples.end(); ++posIt){
			OriginalImage image(*posIt);
			image.image = image.image(roi);
			//To-Do: hog_features vom Image berechnen
			//		 classify
		}

		for(vector<String>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			OriginalImage image(*negIt);

			//Werden schon normalisiert (ins Original-Bild umgerechnet)
			vector<ClassifiedRect> classifiedRectsInImage = calcClassifiedRectsInAllScalesOfImage(classifier, image);
			compareWithGroundTruthBoxes(image, classifiedRectsInImage);
			sort(classifiedRectsInImage.begin(), classifiedRectsInImage.end(), sortingClassifiedRects);

			//(Wie kommt man an den Threshold?)
			float threshold;
			determinateDetections(image, classifiedRectsInImage, threshold);
		}
	}
}