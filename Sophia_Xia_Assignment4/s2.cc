// Sophia Xia
// contains functions for applying a 3x3 sobel mask on an image
// Reads a given pgm image, and applies the sobel mask for edge detection
// The modified image is then saved to a new pgm image under the given filename
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

vector<int> ReadParameterFile(string filename){
  ifstream database;
  database.open(filename);
  int centerx, centery, radius;
  database >> centerx >> centery >> radius;
  database.close();
  vector<int> parameters = {centerx, centery, radius};
  return parameters;
}

vector<int> BrightestPixel(Image *an_image){
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
  cout << centerx << " " << centery << endl;
  vector<int> coords = {centerx, centery};
  return coords;
}

double PartialDerivativeSphere(int x, int y, int centerx, int centery, int radius, bool partialx){
  double numerator = 0;
  if(partialx) numerator = -(x-centerx);
  else numerator = -(y-centery);
  double denominator = pow((pow(radius,2) - pow((x-centerx),2) - pow((y-centery),2)),0.5);
  double debug = pow(radius,2) - pow((x-centerx),2) - pow((y-centery),2);
  cout << numerator << " " <<  debug << " " << denominator << endl;
  return numerator/denominator;
}

vector<double> BrightestNormal(vector<int> parameters, Image *image){
  vector<int> brightest_pixel = BrightestPixel(image);
  int centerx = parameters[0];
  int centery = parameters[1];
  int radius = parameters[2];
  double p = -PartialDerivativeSphere(brightest_pixel[0], brightest_pixel[1], centerx, centery, radius, true);
  double q = -PartialDerivativeSphere(brightest_pixel[0], brightest_pixel[1], centerx, centery, radius, false);
  cout << p << " " << q << endl;
  double magnitude = pow(pow(p,2)+pow(q,2)+1,0.5);
  vector<double> normal;
  normal.push_back(p/magnitude);
  normal.push_back(q/magnitude);
  normal.push_back(1/magnitude);
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

  vector<int> parameters = ReadParameterFile(input_parameters_file);  
  vector<double> normal_one = BrightestNormal(parameters, &image_one);
  vector<double> normal_two = BrightestNormal(parameters, &image_two);
  vector<double> normal_three = BrightestNormal(parameters, &image_three);

  ofstream database;
  database.open(output_file);
  database << normal_one[0] << " " << normal_one[1] << " " << normal_one[2] << "\n";
  database << normal_two[0] << " " << normal_two[1] << " " << normal_two[2] << "\n";
  database << normal_three[0] << " " << normal_three[1] << " " << normal_three[2] << "\n";
  database.close();
}
