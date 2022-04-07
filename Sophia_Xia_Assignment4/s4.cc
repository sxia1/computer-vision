// Sophia Xia
// contains functions necessary to calculate surface normal magnitudes of an object
// reads the light source directions from a file and uses that information to
// calculate the normal magnitude and draw the Albedo image from three images
// the image is then written to the specified file
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
 * Reads light source directions from a file into a vector
 * @param filename should contain 3 newline separated light source directions
 *        containing space separated x, y, z components
 * @return vector<double> a vector of length 9 containing the directions
 */
vector<double> ReadDirectionFile(string filename){
  vector<double> directions;
  ifstream database;
  database.open(filename);
  double x, y, z;
  while(database >> x >> y >> z){
    directions.push_back(x);
    directions.push_back(y);
    directions.push_back(z);
  }
  database.close();
  return directions;
}

/**
 * Calculates the inverse of a 3 by 3 matrix
 * @param matrix a vector of length 9
 * @return vector<double> vector of length 9 containing the inverse
 */
vector<double> inverse_matrix(vector<double> matrix){
  vector<double> inverse = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  for(int i = 0; i < 9; i++){
    int row = i/3;
    int col = i%3;
    vector<double> minor;
    for(int j = 0; j < 3; j ++){
      for(int k = 0; k < 3; k ++){
        if(j!=row && k!=col) minor.push_back(matrix[j*3+k]);
      }
    }
    double minor_determinant = minor[0]*minor[3] - minor[1]*minor[2];
    inverse[col*3+row] = minor_determinant;
  }
  double determinant = matrix[0]*inverse[0] - matrix[3]*inverse[1] + matrix[6]*inverse[2];
  determinant = matrix[0]*inverse[0] - matrix[1]*inverse[3] + matrix[2]*inverse[6];
  for(int i = 0; i < 9; i++){
    inverse[i] = inverse[i]/determinant;
    if(i%2 == 1) inverse[i] = -inverse[i];
  }
  return inverse;
}

/**
 * calculates the normal magnitude of a pixel
 * @param value_a the pixel value at the pixel from the first image
 * @param value_b the pixel value at the pixel from the second image
 * @param value_c the pixel value at the pixel from the third image
 * @param directions a vector of length 9 that contains the inverse light source direction matrix
 * @return double the magnitude of the normal vector
 */
double NormalMagnitude(int value_a, int value_b, int value_c, vector<double> directions){
  vector<double> normal;
  for(int i = 0; i < 9; i+=3){
    double value = directions[i]*value_a + directions[i+1]*value_b +directions[i+2]*value_c;
    normal.push_back(value);
  }
  double magnitude = pow(pow(normal[0],2)+pow(normal[1],2)+pow(normal[2],2),0.5);
  return magnitude;
}


/**
 * calculates and draws the Albedo on the image
 * @param directions a vector of size 9 that contains 3 light source directions
 * @param one the first image that gets drawn on
 * @param two the second image
 * @param three the third image
 * @param threshold
 * @return *Image reference to the modified first image
 */
Image *Albedo(vector<double> directions, Image *one, const Image *two, const Image *three, int threshold){
  if (one == nullptr || two == nullptr || three == nullptr) abort();
  int rows = one->num_rows();
  int cols = one->num_columns();
  directions = inverse_matrix(directions);
  double max_magnitude = 0.0;
  vector<double> magnitudes;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int one_value = one->GetPixel(r,c);
      int two_value = two->GetPixel(r,c);
      int three_value = three->GetPixel(r,c);
      if(one_value > threshold && two_value > threshold && three_value > threshold){
        double magnitude = NormalMagnitude(one_value,two_value,three_value, directions);
        max_magnitude = max(max_magnitude, magnitude);
        magnitudes.push_back(magnitude);
      }
      else magnitudes.push_back(0);
    }
  }
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      double scaled = 255*magnitudes[r*(cols)+c]/max_magnitude;
      one->SetPixel(r,c,floor(scaled));
    }
  }
  return one;
}

int main(int argc, char **argv){
  if (argc!=7) {
    printf("Usage: %s input_directions.txt image1.pgm image2.pgm image3.pgm threshold output_albedo_image_filename.pgm\n", argv[0]);
    return 0;
  }
  const string input_directions(argv[1]);
  const string input_image_one(argv[2]);
  const string input_image_two(argv[3]);
  const string input_image_three(argv[4]);
  const string threshold(argv[5]);
  const string output_image(argv[6]);

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

  vector<double> directions = ReadDirectionFile(input_directions);
  Image *albedo_image = Albedo(directions, &image_one, &image_two, &image_three, stoi(threshold));

  if (!WriteImage(output_image, *albedo_image)){
    cout << "Can't write to file " << output_image << endl;
    return 0;
  }
}
