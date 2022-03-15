// Sophia Xia
// contains function that writes attributes of connected components (objects) to a file
// Reads a given pgm image, and calculates the attributes for connected components in the image
// Then the attributes of the objects are written to a file
// a dot is drawn at the center of each object
// an orientation line originating from the center is also drawn on the image
// The modified image is then saved to a new pgm image under the given filename

#include "image.h"
#include "image_objects.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Writes the attributes of all objects in the map to a file
 * each object gets its own line and attributes are separated by spaces
 * @param objects a map where the key is the label of the object
 *  and the value is a struct of all the object attributes
 * @param filename this is the name of the file that will be written to
 */
void WriteStats(Image *an_image, map<int, struct object_data> objects, string filename){
  ofstream database;
  database.open(filename);
  for(const auto& obj: objects){
    database << obj.first << " ";
    database << obj.second.x << " ";
    database << obj.second.y << " ";
    database << obj.second.e_min << " ";
    database << obj.second.area << " ";
    database << obj.second.roundedness << " ";
    database << obj.second.orientation << "\n";
    DrawOrientation(an_image, obj.second);
  }
  database.close();
}

int main(int argc, char **argv){

  if (argc!=4) {
    printf("Usage: %s input_labeled_image output_database output_image\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);
  const string output_image(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  map<int, struct object_data> objects = GetObjectsData(&an_image);	
  WriteStats(&an_image, objects, output_file);

  if (!WriteImage(output_image, an_image)){
    cout << "Can't write to file " << output_image << endl;
    return 0;
  }
}
