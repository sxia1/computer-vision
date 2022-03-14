// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

bool inBounds(Image *an_image, int row, int col){
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  return(row >= 0 && col >= 0 && row < rows && col < cols);
}

int sobel_deriv(Image *an_image, bool x, int row, int col){
  std::vector<int> sobel3_gradient = {1,2,1,0,0,0,-1,-2,-1};
  if(x) sobel3_gradient = {-1,0,1,-2,0,2,-1,0,1};
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
