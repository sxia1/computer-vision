// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Modifies an image by making it binary based on a threshold
 * @param threshold
 * @param an_image reference to the image which gets modified
 */
void BinaryThreshold(int threshold, Image *an_image){
  if (an_image == nullptr) abort();
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();

  for(size_t r = 0; r < rows; r++){
    for(size_t c = 0; c < cols; c++){
      if (threshold < an_image->GetPixel(r, c))
        an_image->SetPixel(r,c,255);
      else
        an_image->SetPixel(r,c,0);
    }
  }
}

int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_gray_image.pgm threshold output_binary_image_filename.pgm\n", argv[0]);
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

  BinaryThreshold(stoi(threshold), &an_image);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
