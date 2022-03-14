// Sample test file for the image class.
// Reads a given pgm image, draws a line, and saves it to
// another pgm image.
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;
using namespace ComputerVisionProjects;

struct component{
  component(): r(0), c(0), votes(0){}
  int r;
  int c;
  int votes;
};

/**
 * gets the labels of connected components in an image
 * @param an_image reference to the image
 * @return set<int> set of all labels of connected components
 */
set<int> GetLabels(const Image *an_image){
  if (an_image == nullptr) abort();
  set<int> labels;
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = an_image->GetPixel(r,c);
      if (color != 0) labels.insert(color);
    }
  }
  return labels; 
}

vector<double> FindHoughLines(const Image *hough_image, const Image *components){
  if (hough_image == nullptr || components == nullptr) abort();
  int rows = hough_image->num_rows();
  int cols = hough_image->num_columns();

  vector<double> hough_lines;
  set<int> labels = GetLabels(components);
  map<int, struct component> centers;
  for(const int& label: labels){
    component center;
    centers[label] = center;
  }
  for(int r = 0; r < rows; r ++){
    for(int c = 0; c < cols; c++){
      int color = components->GetPixel(r,c);
      if(color!=0){
        int votes = hough_image->GetPixel(r,c);
        centers[color].r += r*votes;
        centers[color].c += c*votes;
        centers[color].votes += votes;
      }
    }
  }
  for(const int& label: labels){
    centers[label].r = centers[label].r/centers[label].votes;
    centers[label].c = centers[label].c/centers[label].votes;
    hough_lines.push_back(centers[label].r*1);
    hough_lines.push_back(centers[label].c *M_PI/180);
  }
  return hough_lines;
}

void AboveThreshold(Image *an_image, int threshold){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = an_image->GetPixel(r,c);
      if(color < threshold)an_image->SetPixel(r,c,0);
      else an_image->SetPixel(r,c,255);
    }
  }
}
/**
 * Modifies an image by finding and labeling the connected components
 * @param an_image reference to the image which gets modified
 */
void ConnectedComponents(Image *an_image){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  
  // equiv: key=label, value=equivalent label
  map<int, int> equiv;

  int label = 0;
  // First Pass
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      if (an_image->GetPixel(r,c) == 255){
        int upper = (r-1 >= 0) ? an_image->GetPixel(r-1,c) : 0;
        int left = (c-1 >= 0) ? an_image->GetPixel(r,c-1) : 0;
        int upper_left = (r-1 >= 0 && c-1 >= 0) ? an_image->GetPixel(r-1,c-1) : 0;
        if(upper == 0 && left == 0 && upper_left == 0){
          label ++;
          equiv[label] = label;
          an_image->SetPixel(r,c,equiv[label]);
        }else if(upper != 0 && left != 0){
          equiv[left] = equiv[upper];
          an_image->SetPixel(r,c,equiv[left]);
        }else if(left != 0){
          an_image->SetPixel(r,c,equiv[left]);
        }else if(upper != 0){
          an_image->SetPixel(r,c,equiv[upper]);
        }else if(upper_left != 0){
          an_image->SetPixel(r,c,equiv[upper_left]);
        }
      }
    }
  }
  // components: key=label, value=color
  map<int, int> components;
  for(auto const& x: equiv){
    equiv[x.first] = equiv[x.second]; // necessary just in case any equivalencies slipped through and didn't get resolved
    components[x.second] = 255; // only want values of the equiv map because the equivalencies are resolved there
  }
  // Calculating gray level for each component
  int grays = 1;
  for(auto const& x: components){
    components[x.first] = grays;
    grays += 1;
  }
  // Second Pass to resolve equivalences
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int pixel = an_image->GetPixel(r,c);
      if (pixel > 0){
        an_image->SetPixel(r, c, components[equiv[pixel]]);
      }
    }
  }
}

void DrawHoughLines(Image *an_image, vector<double> houghlines){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  
  for(int i = 0; i < houghlines.size(); i += 2){
    double rho = houghlines[i];
    double theta = houghlines[i+1];
    vector<int> coords;
    int x, y;
    y = 0;
    x = rho/cos(theta);
    if(x >= 0 && x < rows){
      coords.push_back(x);
      coords.push_back(y);
    }
    x = 0;
    y = rho/sin(theta);
    if(y >= 0 && y < cols){
      coords.push_back(x);
      coords.push_back(y);
    }
    y = cols-1;
    x = round((rho - y*sin(theta))/cos(theta));
    if(x >= 0 && x < rows){
      coords.push_back(x);
      coords.push_back(y);
    }
    x = rows-1;
    y = round((rho - x*cos(theta))/sin(theta));
    if(y >= 0 && y < cols){
      coords.push_back(x);
      coords.push_back(y);
    }
    if(coords.size() == 4) DrawLine(coords[0], coords[1], coords[2], coords[3], 255, an_image);
  }
}

int main(int argc, char **argv){
  
  if (argc!=5) {
    printf("Usage: %s input_gray_image.pgm voting_array threshold output_gray_image_filename.pgm\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string voting_array_file(argv[2]);
  const string threshold(argv[3]);
  const string output_file(argv[4]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  Image hough_image;
  if (!ReadImage(voting_array_file, &hough_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  //DEEP COPY HOUGH_IMAGE INTO COMPONENTS
  Image components;
  int rows = hough_image.num_rows();
  int cols = hough_image.num_columns();
  components.AllocateSpaceAndSetSize(rows, cols);
  components.SetNumberGrayLevels(255);
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = hough_image.GetPixel(r,c);
      components.SetPixel(r,c,color);
    }
  }
  //THRESHOLD COMPONENTS
  AboveThreshold(&components, stoi(threshold));
  //GET CONNECTED COMPONENTS
  ConnectedComponents(&components);
  
  vector<double> hough_lines = FindHoughLines(&hough_image, &components);
  DrawHoughLines(&an_image, hough_lines);
  
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
