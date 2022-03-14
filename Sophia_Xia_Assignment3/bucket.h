#ifndef BUCKETS_H 
#define BUCKETS_H

#include <iostream>
#include <vector>

using namespace std;

struct bucket_pixel{
  bucket_pixel(): votes(0), rho(0), theta(0.0){}
  int votes;
  int rho;
  double theta;
};
struct bucket{
  bucket(): rows(0), cols(0){}
  int rows;
  int cols;
  vector<struct bucket_pixel> bucket_pixels;
};
#endif
