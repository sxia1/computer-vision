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

vector<struct bucket> localMaxima(const Image *hough_image, int rho_sample, double theta_sample, int bucket_size){
  if (hough_image == nullptr) abort();
  int hough_rows = hough_image->num_rows();
  int hough_cols = hough_image->num_columns();

  vector<struct bucket> vote_array;
  for (int r = 0; r < hough_rows; r += bucket_size){
    bucket previous_bucket;
    int previous_votes = 0;
    bool up = false;
    for (int c = 0; c < hough_cols; c += bucket_size){
      bucket current_bucket;
      int bucket_rows = min(r + bucket_size, hough_rows); 
      int bucket_cols = min(c + bucket_size, hough_cols);
      current_bucket.rows = bucket_rows - r;
      current_bucket.cols = bucket_cols - c;
      int curr_votes = 0;
      //BUCKET
      for(int br = r; br < bucket_rows; br ++){
        for(int bc = c; bc < bucket_cols; bc ++){
          int votes = hough_image->GetPixel(br,bc);
          bucket_pixel pixel;
          pixel.votes = votes;
          pixel.rho = br*rho_sample;
          pixel.theta = bc*theta_sample;
          current_bucket.bucket_pixels.push_back(pixel);
          curr_votes = max(votes, curr_votes);
        }
      }
      // 50 threshold as preliminary screen
      if(up && curr_votes < previous_votes && previous_votes > 50){
        vote_array.push_back(previous_bucket);
        up = false;
      }
      if(!up && curr_votes > previous_votes) up = true;
      previous_votes = curr_votes;
      previous_bucket = current_bucket;
    }
  }
  return vote_array;
}

void writeArrayToFile(vector<struct bucket> vote_array, string filename){
  ofstream database;
  database.open(filename);
  for(int i = 0; i < vote_array.size(); i += 2){
    database << vote_array[i].rows << " ";
    database << vote_array[i].cols << " ";
    database << "\n";
    for(int j = 0; j < vote_array[i].bucket_pixels.size(); j++){
      database << vote_array[i].bucket_pixels[j].votes << " ";
      database << vote_array[i].bucket_pixels[j].rho << " ";
      database << vote_array[i].bucket_pixels[j].theta << " ";
      database << "\n";
    }
  }
  database.close();
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

  Image *hough_image = Accumulator(&an_image, 1, M_PI/180);
  vector<struct bucket> voting_array = localMaxima(hough_image, 1, M_PI/180, 25);
  writeArrayToFile(voting_array, output_voting_file);

  if (!WriteImage(output_image_file, *hough_image)){
    cout << "Can't write to file " << output_image_file << endl;
    return 0;
  }
}
