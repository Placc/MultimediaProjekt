#include "Preprocessing.h"

using namespace cv;
using namespace std;
//using namespace boost::filesystem;

namespace Preprocessing {
	
	vector<Rect> getBoundingBoxesByFile(string p){
		vector<Rect> result;
		stringstream path;
		if(p.find("pos")){
			path << p.substr(0, p.find("pos")) << "annotations" << p.substr(p.find_last_of("\\"), p.find('.') + 1 - p.find_last_of("\\")) << "txt";
		} else 
			path << p;
		ifstream file(path.str());
		string s;
		while(getline(file, s)){
			if(s.find("Bounding box") != string::npos){
				string positions = s.substr(s.find(":"), string::npos);
				int firstX = stoi(positions.substr(positions.find_first_of("(")+1));
				int firstY = stoi(positions.substr(positions.find_first_of(",")+1));
				int lastX = stoi(positions.substr(positions.find_last_of("(")+1));
				int lastY = stoi(positions.substr(positions.find_last_of(",")+1));
				Point p1(firstX, firstY);
				Point p2(lastX, lastY);
				result.push_back(Rect(p1, p2));
			}
		}
		file.close();
		return result;
	}

	void loadPathsByDirectory(string dirPath, vector<String> &image_vector){
		string str;
		stringstream stream;
		stream << "LS " << dirPath << " > flist";
		system(stream.str().c_str());
		ifstream file("flist");
		while(getline(file, str)){
			stream.str("");
			stream << dirPath << str;
			image_vector.push_back(stream.str());
		}
		file.close();
		remove("flist");

		//******************************************************
		//* BOOST::FILESYSTEM - Solution:
		//******************************************************
	
		/*path directory(dirPath);
		directory_iterator end;
		if(exists(directory) && is_directory(directory)){
			for(directory_iterator it(directory); it!=end; ++it){
				if(it->path().extension() == ".png" || it->path().extension() == ".jpg"){
					Mat image = imread(it->path().string(), CV_LOAD_IMAGE_COLOR);
					image_vector.push_back(image);
				}
			}
		}*/
	}

	bool checkINRIADirectory(string dirPath){
		if (_access(dirPath.c_str(), 0) == 0){
			string strings[] = {TRAIN_POS_NORMALIZED, TRAIN_ANNOTATIONS, TRAIN_NEG_ORIGINAL, TRAIN_POS_ORIGINAL,
				TEST_ANNOTATIONS, TEST_NEG_ORIGINAL, TEST_POS_NORMALIZED, TEST_POS_ORIGINAL};
			for(int i = 0; i < 8; i++){
				if (!(_access((dirPath + strings[i]).c_str(), 0) == 0))
					return false;
			}
			return true;
		} 
		return false;
	}
}