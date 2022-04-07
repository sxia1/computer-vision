// Sophia Xia
// contains functions to calculate light source direction on a sphere
// reads sphere parameters from a file and uses that to calculate the vector
// the brightest pixel on the sphere to its center
// this is done for 3 different sphere images under different lighting conditions
// the vectors are then all written to a textfile
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Reads parameters from a file into a vector
 * @param filename file should consist of 3 space separated numbers
 * @return vector<int> a vector of length three containing centerx, centery, radius
 */
vector<int> ReadParameterFile(string filename){
  ifstream database;
  database.open(filename);
  int centerx, centery, radius;
  database >> centerx >> centery >> radius;
  database.close();
  vector<int> parameters = {centerx, centery, radius};
  return parameters;
}

/**
 * finds the coordinates and value of the brightest pixel in an image
 * @param an_image reference to the image that should depict a sphere
 * @return vector<int> vector of length 3 containing x coordinate, y coordinate, and pixel value
 */
vector<int> BrightestPixel(const Image *an_image){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  int max_brightness = 0;
  for(int r = 0; r < rows; r++)
    for(int c = 0; c < cols; c++)
      max_brightness = max(max_brightness, an_image->GetPixel(r, c));
  int centerx = 0;
  int centery = 0;
  int area = 0;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      if(an_image->GetPixel(r, c) == max_brightness){
        centerx += r;
        centery += c;
        area += 1;
      }
    }
  }
  centerx = centerx/area;
  centery = centery/area;
  vector<int> coords = {centerx, centery, max_brightness};
  return coords;
}

/**
 * finds the vector from the brightest pixel to the center of the sphere
 * @param parameters a vector containing sphere x coordinate, y coordinate, and radius
 * @param image reference to the image that should depict a sphere
 * @return vector<double> contains x y and z components of the vector scaled to pixel brightness
 */
vector<double> BrightestNormal(vector<int> parameters, const Image *image){
  vector<int> brightest_pixel = BrightestPixel(image);
  int centerx = parameters[0];
  int centery = parameters[1];
  int radius = parameters[2];
  int dx = brightest_pixel[0]-centerx;
  int dy = brightest_pixel[1]-centery;
  int max_brightness = brightest_pixel[2];
  double dz = pow(pow(radius, 2)-pow(dx,2)-pow(dy,2),.5);
  double magnitude = pow(pow(dx,2)+pow(dy,2)+pow(dz,2),0.5);
  vector<double> normal;
  normal.push_back(max_brightness*dx/magnitude);
  normal.push_back(max_brightness*dy/magnitude);
  normal.push_back(max_brightness*dz/magnitude);
  return normal;
}

int main(int argc, char **argv){
  
  if (argc!=6) {
    printf("Usage: %s input_parameters_file.txt image1.pgm image2.pgm image3.pgm output_directions_file.txt\n", argv[0]);
    return 0;
  }
  const string input_parameters_file(argv[1]);
  const string input_image_one(argv[2]);
  const string input_image_two(argv[3]);
  const string input_image_three(argv[4]);
  const string output_file(argv[5]);

  Image image_one;
  if (!ReadImage(input_image_one, &image_one)) {
    cout <<"Can't open file " << input_image_one << endl;
    return 0;
  }
  Image image_two;
  if (!ReadImage(input_image_two, &image_two)) {
    cout <<"Can't open file " << input_image_two << endl;
    return 0;
  }
  Image image_three;
  if (!ReadImage(input_image_three, &image_three)) {
    cout <<"Can't open file " << input_image_three << endl;
    return 0;
  }
  // READ SPHERE PARAMETERS FROM FILE
  vector<int> parameters = ReadParameterFile(input_parameters_file);
  // CALCULATE LIGHT SOURCE DIRECTIONS ON SPHERE
  vector<double> normal_one = BrightestNormal(parameters, &image_one);
  vector<double> normal_two = BrightestNormal(parameters, &image_two);
  vector<double> normal_three = BrightestNormal(parameters, &image_three);
  // WRITE LIGHT SOURCE DIRECTIONS TO FILE
  ofstream database;
  database.open(output_file);
  database << normal_one[0] << " " << normal_one[1] << " " << normal_one[2] << "\n";
  database << normal_two[0] << " " << normal_two[1] << " " << normal_two[2] << "\n";
  database << normal_three[0] << " " << normal_three[1] << " " << normal_three[2] << "\n";
  database.close();
}
