#include "Preprocessing.h"

using namespace cv;
using namespace std;
//using namespace boost::filesystem;

namespace Preprocessing {
	
	vector<Rect> getBoundingBoxesByFile(string path){
		vector<Rect> result;
		ifstream file(path);
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

	void loadImagesByDirectory(string dirPath, vector<OriginalImage> &image_vector){
		string str;
		stringstream stream;
		stream << "LS " << dirPath << " > flist";
		system(stream.str().c_str());
		ifstream file("flist");
		while(getline(file, str)){
			stream.str("");
			stream << dirPath << str;
			OriginalImage image(stream.str());
			image_vector.push_back(image);
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
}