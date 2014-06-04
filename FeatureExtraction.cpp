#include "FeatureExtraction.h"

using namespace std;
using namespace cv;

namespace FeatureExtraction {

	void extractHOGFeatures(Image &image){
		float* imageArray = (float*)vl_malloc(image.image.rows * image.image.cols * image.image.channels() * sizeof(float));
		for(int y = 0; y < image.image.rows; y++)
			for(int x = 0; x < image.image.cols; x++)
				for(int c = 0; c < image.image.channels(); c++)
					imageArray[c * image.image.rows * image.image.cols + y * image.image.cols + x ] = (float)(image.image.at<Vec3b>(y,x)[c]);
		VlHog* hog = vl_hog_new(VlHogVariantUoctti, vl_size(NUMBER_OF_ORIENTATIONS), VL_FALSE);
		vl_hog_put_image(hog, imageArray, vl_size(image.image.cols), vl_size(image.image.rows), vl_size(image.image.channels()), vl_size(CELL_SIZE));
		vl_size height =vl_hog_get_height(hog);
		vl_size width = vl_hog_get_width(hog);
		float* hogArray = (float*)vl_malloc(width * height * vl_hog_get_dimension(hog) * sizeof(float));
		vl_hog_extract(hog, hogArray);		
		vl_hog_delete(hog);
		vl_free(imageArray);
		Mat features = convert_hog_array(hogArray, NUMBER_OF_ORIENTATIONS, (int)width, (int)height, (int)width, (int)height); 
		vl_free(hogArray);
		image.hog_features = features;
	}

	float* computeScaleFactors(){
		float unitScaleFactor = powf(2.f, 1.f/static_cast<float>(STEPS_PER_OCTAVE));
		int arraySize = 7 * STEPS_PER_OCTAVE + 1;
		float* scaleFactors = (float*)malloc(arraySize * sizeof(float));
		for(int i = 0; i < arraySize; i++){
			scaleFactors[i] =  powf(unitScaleFactor, static_cast<float>(i));
		}
		return scaleFactors;
	}

	void computeHOGPyramid(OriginalImage &image){
		float* scaleFactors = computeScaleFactors();
		int arraySize = 7 * STEPS_PER_OCTAVE + 1;
		Mat workingCopy;
		for(int i = 0; i < arraySize; i++){
			if(i % STEPS_PER_OCTAVE == 0)
				resize(image.image, workingCopy,  Size(static_cast<int>(image.image.cols / scaleFactors[i]), static_cast<int>(image.image.rows / scaleFactors[i])));
			else
				resize(workingCopy, workingCopy,  Size(static_cast<int>(image.image.cols / scaleFactors[i]), static_cast<int>(image.image.rows / scaleFactors[i])));
			if(workingCopy.cols > SLIDING_WINDOW_WIDTH * CELL_SIZE && workingCopy.rows > SLIDING_WINDOW_HEIGHT * CELL_SIZE){	
				if(scaleFactors[i] > 1.0f){
					Image scaledImage(workingCopy, image.path, scaleFactors[i]);
					extractHOGFeatures(scaledImage);	
					image.lower_images.push_back(scaledImage);
				} else {
					extractHOGFeatures(image);
				}
			}
		}
		free(scaleFactors);
	}

	Mat getHogOfSlidingWindow(Mat hog_features, int swHogPos, int hogCellsPerRow){
		Mat result(SLIDING_WINDOW_WIDTH * SLIDING_WINDOW_HEIGHT, hog_features.cols, CV_32FC1);
		for(int j = 0; j < SLIDING_WINDOW_HEIGHT; j++)
			for(int i = 0; i < SLIDING_WINDOW_WIDTH; i++)
				hog_features.row(swHogPos + j * hogCellsPerRow + i).copyTo(result.row(j * SLIDING_WINDOW_WIDTH + i));
		return result;
	}

	void computeSlidingWindows(Image &image){
		int hogCellsPerRow = image.image.cols / CELL_SIZE; 
		for(int y = 0; y < image.image.rows - SLIDING_WINDOW_HEIGHT * CELL_SIZE; y += SLIDING_WINDOW_X_STEP * CELL_SIZE)
			for(int x = 0; x < image.image.cols - SLIDING_WINDOW_WIDTH * CELL_SIZE; x += SLIDING_WINDOW_Y_STEP * CELL_SIZE){
				int swWidth = (int)(SLIDING_WINDOW_WIDTH * CELL_SIZE * image.scale_factor);
				int swHeight = (int)(SLIDING_WINDOW_HEIGHT * CELL_SIZE * image.scale_factor);
				int swHogPos = (int)(((double)(y) / CELL_SIZE * (double)(hogCellsPerRow)) + (double)(x) / CELL_SIZE);
				Rect swRect((int)(x * image.scale_factor), (int)(y * image.scale_factor), swWidth, swHeight);
				SlidingWindow sw(getHogOfSlidingWindow(image.hog_features, swHogPos, hogCellsPerRow), swRect);
				image.slidingWindows.push_back(sw);
			}
	}
}