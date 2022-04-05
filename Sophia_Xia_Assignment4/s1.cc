// Sophia Xia
// Reads a given pgm image (expecting sphere in image), thresholds it and calculates
// the center coordinates and radius of the sphere
// the data calculated is then written to a textfile
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
 * thresholds an image and calculates the center coordinates and radius of the sphere
 * @param an_image reference to the image that should depict a sphere
 * @param threshold 
 * @param filename the name of the file the data should be written to
 */
void DetectSphere(const Image *an_image, int threshold, string filename){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();

  int centerx = 0;
  int centery = 0;
  int area = 0;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      if (threshold < an_image->GetPixel(r, c)){
        centerx += r;
        centery += c;
        area ++;
      }
    }
  }
  centerx = centerx/area;
  centery = centery/area;
  int horizontal_diameter = 1;
  int vertical_diameter = 1;
  for(int r = rows-1; r >= 0; r--){
    if (threshold < an_image->GetPixel(r, centery)){
      horizontal_diameter += r;
      break;
    }
  }
  for(int r = 0; r < rows; r++){
    if (threshold < an_image->GetPixel(r, centery)){
      horizontal_diameter -= r;
      break;
    }
  }
  for(int c = cols-1; c >= 0; c--){
    if (threshold < an_image->GetPixel(centerx, c)){
      vertical_diameter += c;
      break;
    }
  }
  for(int c = 0; c < cols; c++){
    if (threshold < an_image->GetPixel(centerx, c)){
      vertical_diameter -= c;
      break;
    }
  }
  int radius = (horizontal_diameter + vertical_diameter)/4;
  ofstream database;
  database.open(filename);
  database << centerx << " " << centery << " " << radius;
  database.close();
}

int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_original_image.pgm input_threshold_value output_parameters_file.txt\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string threshold(argv[2]);
  const string output_file(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  DetectSphere(&an_image, stoi(threshold), output_file);
}
