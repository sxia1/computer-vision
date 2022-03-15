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

// Implements the Bresenham's incremental midpoint algorithm;
// (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
// "Computer Graphics. Principles and practice", 
// 2nd ed., 1990, section 3.2.2);  
vector<int> GetLinePoints(int x0, int y0, int x1, int y1) {  
  #ifdef SWAP
  #undef SWAP
  #endif
  #define SWAP(a,b) {a^=b; b^=a; a^=b;}
  vector<int> points;
  const int DIR_X = 0;
  const int DIR_Y = 1;
  // Increments: East, North-East, South, South-East, North.
  int incrE, incrNE, incrS, incrSE, incrN;     
  int d;         /* the D */
  int x,y;       /* running coordinates */
  int mpCase;    /* midpoint algorithm's case */
  int done;      /* set to 1 when done */

  int xmin = x0;
  int xmax = x1;
  int ymin = y0;
  int ymax = y1;

  int dx = xmax - xmin;
  int dy = ymax - ymin;
  int dir;

  if (dx * dx > dy * dy) {  // Horizontal scan.
    dir=DIR_X;
    if (xmax < xmin) {
      SWAP(xmin, xmax);
      SWAP(ymin , ymax);
    } 
    dx = xmax - xmin;
    dy = ymax - ymin;

    if (dy >= 0) {
      mpCase = 1;
      d = 2 * dy - dx;      
    } else {
      mpCase = 2;
      d = 2 * dy + dx;      
    }

    incrNE = 2 * (dy - dx);
    incrE = 2 * dy;
    incrSE = 2 * (dy + dx);
  } else {// vertical scan.
    dir = DIR_Y;
    if (ymax < ymin) {
      SWAP(xmin, xmax);
      SWAP(ymin, ymax);
    }
    dx = xmax - xmin;
    dy = ymax-ymin;    

    if (dx >=0 ) {
      mpCase = 1;
      d = 2 * dx - dy;      
    } else {
      mpCase = 2;
      d = 2 * dx + dy;      
    }

    incrNE = 2 * (dx - dy);
    incrE = 2 * dx;
    incrSE = 2 * (dx + dy);
  }

  /// Start the scan.
  x = xmin;
  y = ymin;
  done = 0;

  while (!done) {
    points.push_back(x);
    points.push_back(y);
    // Move to the next point.
    switch(dir) {
      case DIR_X: 
        if (x < xmax) {
          switch(mpCase) {
            case 1:
              if (d <= 0) {
                d += incrE;  
                x++;
              } else {
                d += incrNE; 
                x++; 
                y++;
              }
              break;

            case 2:
              if (d <= 0) {
                d += incrSE; 
                x++; 
                y--;
              } else {
                d += incrE;  
                x++;
              }
              break;
          } 
        } else {
          done=1;
        }     
        break;

      case DIR_Y: 
        if (y < ymax) {
          switch(mpCase) {
            case 1:
              if (d <= 0) {
                d += incrE;  
                y++;
              } else {
                d += incrNE; 
                y++; 
                x++;
              }
              break;

            case 2:
              if (d <= 0) {
                d += incrSE; 
                y++; 
                x--;
              } else {
                d += incrE;  
                y++;
              }
              break;
          } // mpCase
        } // y < ymin 
        else {
          done=1;
        }
        break;    
    }
  }
  return points;
}


vector<double> FindHoughLines(const Image *hough_image, const Image *components){
  if (hough_image == nullptr || components == nullptr) abort();
  int rows = hough_image->num_rows();
  int cols = hough_image->num_columns();
  int bucket_size = 1;

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
    hough_lines.push_back((centers[label].r*bucket_size)*1);
    hough_lines.push_back((centers[label].c*bucket_size)*M_PI/180);
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

vector<int> PolarToCartesian(int rows, int cols, double rho, double theta){
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
  return coords;
}

void DrawHoughLines(Image *an_image, vector<double> houghlines){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  for(int i = 0; i < houghlines.size(); i += 2){
    double rho = houghlines[i];
    double theta = houghlines[i+1];
    vector<int> coords = PolarToCartesian(rows, cols, rho, theta);
    if(coords.size() == 4) DrawLine(coords[0], coords[1], coords[2], coords[3], 255, an_image);
  }
}

void DrawTrimmedHoughLines(Image *an_image, Image *edge_image, vector<double> houghlines, int gap_tolerance, int min_length){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  //vector<int> segments;
  for(int i = 0; i < houghlines.size(); i += 2){
    double rho = houghlines[i];
    double theta = houghlines[i+1];
    vector<int> coords = PolarToCartesian(rows, cols, rho, theta);
    if(coords.size() == 4){
      vector<int> points = GetLinePoints(coords[0], coords[1], coords[2], coords[3]);
      int x0 = -1;
      int y0 = -1;
      int x1 = -1;
      int y1 = -1;
      int line_length = 0;
      int gap = 0;
      for(int j = 0; j < points.size(); j += 2){
        //cout << x0 << " " << y0 << " " << x1 << " " << y1 << " " << endl;
        int color = edge_image->GetPixel(points[j],points[j+1]);
        if(color == 255){
          if(x0 == -1){
            x0 = points[j]; y0 = points[j+1];
            x1 = points[j]; y1 = points[j+1];
            line_length = 1;
          }
          else{
            if(gap != 0 && gap < gap_tolerance){
              line_length += gap;
              gap = 0;
            }
            x1 = points[j];
            y1 = points[j+1];
            line_length +=1;
          }
        }
        if(color == 0){
          if(x0 != -1) gap ++;
          if(gap > gap_tolerance){
            if(line_length > min_length){
              //segments.push_back(x0);
              //segments.push_back(y0);
              //segments.push_back(x1);
              //segments.push_back(y1);
              DrawLine(x0, y0, x1, y1, 255, an_image);
            }
            x0 = -1;
            y0 = -1;
            x1 = -1;
            y1 = -1;
            line_length = 0;
            gap = 0;
          }
        }
      }
    }
    //if(coords.size() == 4) DrawLine(coords[0], coords[1], coords[2], coords[3], 255, an_image);
  }
}

int main(int argc, char **argv){

  if (argc!=5 && argc!=6) {
    printf("Usage: %s input_gray_image.pgm voting_array threshold output_gray_image_filename.pgm binary_edges.pgm(optional for line trimming)\n", argv[0]);
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
    cout <<"Can't open file " << voting_array_file << endl;
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
  if (argc == 5) DrawHoughLines(&an_image, hough_lines);
  if(argc == 6){
    const string binary_edges(argv[5]);
    Image edge_image;
    if (!ReadImage(binary_edges, &edge_image)) {
      cout <<"Can't open file " << binary_edges << endl;
      return 0;
    }
    DrawTrimmedHoughLines(&an_image, &edge_image, hough_lines, 10, 50);
  }


  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
