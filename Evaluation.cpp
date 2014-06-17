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
	void determinateDetections(float threshold) {
		
	}
	//!!wohin mit den WeightedRects? Irgendwo speichern??
	vector<WeightedRect> calcWeightedRectsInAllScalesOfImage(Classifier classifier, OriginalImage &image) {
		FeatureExtraction::computeHOGPyramid(image);
		FeatureExtraction::computeSlidingWindows(image);
		for(std::vector<Image>::iterator scaleIt = (image).lower_images.begin(); scaleIt != (image).lower_images.end(); ++scaleIt){
			FeatureExtraction::computeSlidingWindows(*scaleIt);
		}		
		//"unschön" -> in FeatureExtraction Funktion computeAllSlidingWindows?
		FeatureExtraction::computeSlidingWindows(image);
		for(std::vector<Image>::iterator scaleIt = image.lower_images.begin(); scaleIt != image.lower_images.end(); ++scaleIt){
			FeatureExtraction::computeSlidingWindows(*scaleIt);
		}

		//Alle Slidingwindows vom Original_Image durchgehen, jedesmal von der hog_features - Mat 
		//den SlidingWindow-Bereich anschauen, diesen "klassifizieren"
		//  double value = classifier.classify(image.hog_features(slidingWindow));
		//Wenn value > 0, dann Original_Image.addDetectedBox(slidingWindow Region)  
		//[wird in dieser Funktion nur dann hinzugefügt, wenn nicht schon eine andere dort]
		vector<SlidingWindow> slidingWindows = image.slidingWindows;
		for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
			Rect window = (*slidWinIt).slidingWindow;
			double distanceFromHyperplane = classifier.classify(image.hog_features(window));
			WeightedRect detectedBox(window, distanceFromHyperplane);
			!!!!weiterverwenden!!!
		}
		//...
		//Das gleiche dann für alle lower_images vom Image
		vector<Image> lowerImages = image.lower_images;
		for(vector<Image>::iterator lowImIt = lowerImages.begin(); lowImIt!=lowerImages.end(); ++lowImIt) {
			Image lowerImage = (*lowImIt);
			for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
				Rect window = (*slidWinIt).slidingWindow;
				double value = classifier.classify(lowerImage.hog_features(window));
				WeightedRect detectedBox(window, value);
				!!!!weiterverwenden!!!
			}
		}	
	}
	void showImageWithDetections(OriginalImage &image, float threshold) {
		Mat matToShow = image.image.clone();	
		//umbenennen
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
			//Verschiebung nach unten (+10) nicht Hardcoded
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
	void qualitativeEvaluation() { //3.5
		Classifier classifier = Classifier();
		vector<String> pos_examples;
		vector<String> neg_examples;
		Preprocessing::loadPathsByDirectory(TEST_POS_ORIGINAL, pos_examples);
		Preprocessing::loadPathsByDirectory(TEST_NEG_ORIGINAL, neg_examples);
		
		for(vector<String>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			OriginalImage image(*negIt);
			calcWeightedRectsInAllScalesOfImage(classifier, image);

			//Anschließend hinzugefügte detectedBoxes durchgehen
			//schauen, welche true und welche false ist
			//in einen Vektor<double value, boolean trueOrFalsePositive> schreiben.
			//Nachdem man dann alle durch ist: diesen Vektor sortieren

			//showImageWithDetections(image);
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
			FeatureExtraction::computeHOGPyramid(image);
			//showImageWithDetections(image);
		}

		for(vector<String>::iterator negIt = neg_examples.begin(); negIt != neg_examples.end(); ++negIt){
			OriginalImage image(*negIt);
			FeatureExtraction::computeHOGPyramid(image);
			FeatureExtraction::computeSlidingWindows(image);
			for(std::vector<Image>::iterator scaleIt = (image).lower_images.begin(); scaleIt != (image).lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}
			
			//"unschön" -> in FeatureExtraction Funktion computeAllSlidingWindows?
			FeatureExtraction::computeSlidingWindows(image);
			for(std::vector<Image>::iterator scaleIt = image.lower_images.begin(); scaleIt != image.lower_images.end(); ++scaleIt){
				FeatureExtraction::computeSlidingWindows(*scaleIt);
			}


			//Alle Slidingwindows vom Original_Image durchgehen, jedesmal von der hog_features - Mat 
			//den SlidingWindow-Bereich anschauen, diesen "klassifizieren"
			//  double value = classifier.classify(image.hog_features(slidingWindow));
			//Wenn value > 0, dann Original_Image.addDetectedBox(slidingWindow Region)  
			//[wird in dieser Funktion nur dann hinzugefügt, wenn nicht schon eine andere dort]
			vector<SlidingWindow> slidingWindows = image.slidingWindows;
			for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
				Rect window = (*slidWinIt).slidingWindow;
				double distanceFromHyperplane = classifier.classify(image.hog_features(window));
				WeightedRect detectedBox(window, distanceFromHyperplane);
					//if (distanceFromHyperplane>0) { image.addDetectedBox(detectedBox); }
			}
			//...
			//Das gleiche dann für alle lower_images vom Image
			vector<Image> lowerImages = image.lower_images;
			for(vector<Image>::iterator lowImIt = lowerImages.begin(); lowImIt!=lowerImages.end(); ++lowImIt) {
				Image lowerImage = (*lowImIt);
				for(vector<SlidingWindow>::iterator slidWinIt = slidingWindows.begin(); slidWinIt!=slidingWindows.end(); ++slidWinIt) {
					Rect window = (*slidWinIt).slidingWindow;
					double value = classifier.classify(lowerImage.hog_features(window));
					if (value>0) {
						//*=scaleFactor (oder /= ???), da window nur die Position im lower_Image ist!
						window.x*=lowerImage.scale_factor; window.y*=lowerImage.scale_factor; 
						window.width*=lowerImage.scale_factor; window.height*=lowerImage.scale_factor;
						
						WeightedRect detectedBox(window, value);
						image.addDetectedBox(detectedBox);
					}
				}
			}


			//Anschließend hinzugefügte detectedBoxes durchgehen
			//schauen, welche true und welche false ist
			//in einen Vektor<double value, boolean trueOrFalsePositive> schreiben.
			//Nachdem man dann alle durch ist: diesen Vektor sortieren

			//showImageWithDetections(image);
		}
	}
}