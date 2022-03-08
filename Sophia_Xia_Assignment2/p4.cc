// Sophia Xia
// contains function that will draw the orientation of the object onto the image if recognized
// Reads a given pgm image, and calculates the attributes for connected components in the image
// Then the attributes of the objects are compared to those in a database 
// if recognized a dot is drawn at the center of the object and
// an orientation line originating from the center is also drawn on the image
// The modified image is then saved to a new pgm image under the given filename

#include "image.h"
#include "image_objects.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * calculates the attributes for connected components in the image
 * Then the attributes of the objects are compared to those in a database 
 * - currently only roundedness is being used as a comparison metric
 *   - it is not sensitive to shifting, scaling, or rotation unlike the others making it ideal
 * if recognized a dot is drawn at the center of the object and
 * an orientation line originating from the center is also drawn on the image
 * @param an_image reference to the image which gets modified
 * @param filename this is the name of the file that the image will be written to
 */
void ObjectRecognition(Image *an_image, string filename){
	ifstream database;
	database.open(filename);
	int label, cx, cy, area;
	double e_min, roundedness, orientation;
  map<int, struct object_data> objects = GetObjectsData(an_image);	
	while(database >> label >> cx >> cy >> e_min >> area >> roundedness >> orientation){
    for(const auto& obj: objects){
      cout << obj.first << endl;
      if (abs(obj.second.roundedness - roundedness) <= 0.02){ // .02 worked for me through trial and error
        DrawOrientation(an_image, obj.second);
      }
    }
	}
}

int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_file db_file output_file\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string database_file(argv[2]);
  const string output_file(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  ObjectRecognition(&an_image, database_file); 
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
