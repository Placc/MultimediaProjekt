#include "Training.h"
#include "Classification.h"
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	cout << "+++ MMP - Team 3 +++" << endl;
	cout << "Starting..." << endl;
	ifstream model(MODEL_FILE);
	bool exists = model.good();
	model.close();
	if(exists){
		cout << "Model file found. Starting classification..." << endl;
		Classification::classify(MODEL_FILE, TEST_POS_ORIGINAL);
	} else {
		cout << "No model found. Creating new by training..." << endl;
		Training::trainSVM(MODEL_FILE);
	}
}





