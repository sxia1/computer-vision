// Sophia Xia
// contains functions for applying a 3x3 sobel mask on an image
// Reads a given pgm image, and applies the sobel mask for edge detection
// The modified image is then saved to a new pgm image under the given filename
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * calculates whether or not the given coordinates are within the bounds of an image
 * @param an_image reference to the image
 * @param row x coordinate of the coordinate pair
 * @param col y coordinate of the coordinate pair
 * @return bool True if coordinate pair is within image bounds, else False 
 */
bool inBounds(const Image *an_image, int row, int col){
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  return(row >= 0 && col >= 0 && row < rows && col < cols);
}

/**
 * calculates the 3x3 sobel x or y derivative of a given point in an image 
 * @param an_image reference to the image
 * @param x_deriv if True, function calculates the x deriv, if false function calculated the y deriv
 * @param row x coordinate of the coordinate pair
 * @param col y coordinate of the coordinate pair
 * @return int the x_derivative or y_derivative dependent on param x_deriv
 */
int sobel_deriv(const Image *an_image, bool x_deriv, int row, int col){
  std::vector<int> sobel3_gradient = {1,2,1,0,0,0,-1,-2,-1};
  if(x_deriv) sobel3_gradient = {-1,0,1,-2,0,2,-1,0,1};
  int deriv = 0;
  if(inBounds(an_image, row, col)) deriv += an_image->GetPixel(row,col)*sobel3_gradient[8];
  if(inBounds(an_image, row, col -1)) deriv += an_image->GetPixel(row,col-1)*sobel3_gradient[7];
  if(inBounds(an_image, row, col -2)) deriv += an_image->GetPixel(row,col-2)*sobel3_gradient[6];
  if(inBounds(an_image, row-1, col)) deriv += an_image->GetPixel(row-1,col)*sobel3_gradient[5];
  if(inBounds(an_image, row-1, col -1)) deriv += an_image->GetPixel(row-1,col-1)*sobel3_gradient[4];
  if(inBounds(an_image, row-1, col -2)) deriv += an_image->GetPixel(row-1,col-2)*sobel3_gradient[3];
  if(inBounds(an_image, row-2, col)) deriv += an_image->GetPixel(row-2,col)*sobel3_gradient[2];
  if(inBounds(an_image, row-2, col -1)) deriv += an_image->GetPixel(row-2,col-1)*sobel3_gradient[1];
  if(inBounds(an_image, row-2, col -2)) deriv += an_image->GetPixel(row-2,col-2)*sobel3_gradient[0];
  return deriv;
}


/**
 * modifies image by applying sobel 3x3 edge detection mask 
 * @param an_image reference to the image
 */
void EdgeDetection(Image *an_image){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();

  std::vector<int> gradient_magnitude;

  int gradient_magnitude_height = rows + 2;
  int gradient_magnitude_width = cols + 2;

  for(int r = 1; r < rows+1; r++){
    for(int c = 1; c < cols+1; c++){
      int gradient = 0;
      gradient += pow(sobel_deriv(an_image, true, r, c),2);
      gradient += pow(sobel_deriv(an_image, false, r, c),2);
      gradient = pow(gradient,0.5);
      gradient_magnitude.push_back(gradient);
    }
  }
  int ctr = 0;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int gradient = gradient_magnitude.at(ctr);
      ctr++;
      an_image->SetPixel(r, c, gradient);
    }
  }
}

int main(int argc, char **argv){
  
  if (argc!=3) {
    printf("Usage: %s input_gray_image.pgm output_gray_image_filename.pgm\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  EdgeDetection(&an_image);

  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
