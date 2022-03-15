// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include "bucket.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace ComputerVisionProjects;

Image *Accumulator(const Image *an_image, int rho_sample, double theta_sample){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  
  int max_rho = round(pow(pow(rows, 2) + pow(cols, 2), 0.5));
  double max_theta = 2*M_PI;
  int hough_rows = round(max_rho/rho_sample);
  int hough_cols = round(max_theta/theta_sample);

  Image *HoughImage = new Image();
  HoughImage->AllocateSpaceAndSetSize(hough_rows, hough_cols);
  int max_vote = 0;
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      if(an_image->GetPixel(r,c) == 255){
        for(int i = 0; i < hough_cols; i ++){
          double rho = r*cos(i*theta_sample) + c*sin(i*theta_sample);
          int rho_row = round(rho/rho_sample);
          if(rho_row < hough_rows && rho_row >= 0){
            int color = HoughImage->GetPixel(rho_row, i) + 1;
            HoughImage->SetPixel(rho_row, i, color);
            max_vote = max(max_vote, color);
          }
        }
      }
    }
  }
  HoughImage->SetNumberGrayLevels(max_vote);
  return HoughImage;
}

Image *BucketedImage(const Image *hough_image, int bucket_size){
  if (hough_image == nullptr) abort();
  int hough_rows = hough_image->num_rows();
  int hough_cols = hough_image->num_columns();

  Image *Bucketed = new Image();
  Bucketed->AllocateSpaceAndSetSize(hough_rows/bucket_size, hough_cols/bucket_size);
  int max_vote = 0;
  for (int r = 0; r < hough_rows; r += bucket_size){
    for (int c = 0; c < hough_cols; c += bucket_size){
      int total_bucket_votes = 0;
      int bucket_rows = min(r + bucket_size, hough_rows); 
      int bucket_cols = min(c + bucket_size, hough_cols);
      int bucket_row = bucket_rows/bucket_size -1;
      int bucket_col = bucket_cols/bucket_size -1;
      //BUCKET
      for(int br = r; br < bucket_rows; br ++){
        for(int bc = c; bc < bucket_cols; bc ++){
          total_bucket_votes += hough_image->GetPixel(br,bc);
        }
      }
      Bucketed->SetPixel(bucket_row, bucket_col, total_bucket_votes);
      max_vote = max(max_vote, total_bucket_votes);
    }
  }
  Bucketed->SetNumberGrayLevels(max_vote);
  return Bucketed;
}

int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_binary_image.pgm output_hough_image_filename.pgm voting_array\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_image_file(argv[2]);
  const string output_voting_file(argv[3]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }
  
  int bucket_size = 1;
  Image *hough_image = Accumulator(&an_image, 1, M_PI/180);
  Image *bucket_image = BucketedImage(hough_image, bucket_size);

  if (!WriteImage(output_image_file, *hough_image)){
    cout << "Can't write to file " << output_image_file << endl;
    return 0;
  }
  if (!WriteImage(output_voting_file, *bucket_image)){
    cout << "Can't write to file " << output_voting_file << endl;
    return 0;
  }
}
