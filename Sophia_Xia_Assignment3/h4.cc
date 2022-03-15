// Sophia Xia
// contains functions used to find and draw (trimmed) hough lines
// reads an image, its hough image, and a threshold
// using that information find the centers of the connected components in the hough inage
// Convert the centers (rho theta coordinates) into cartesian coordinates and draw hough lines on the original image
// If a binary edge image is provided, trimmed hough lines will be drawn on the original image instead
// the modified image is then written to the output filename provided

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

// component struct keeps track of component center and number of votes
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

/**
 * calculates and returns all discrete points on a line between the given coordinates
 * Modified from the given code in Image.cc that does the following:
 *   - Implements the Bresenham's incremental midpoint algorithm;
 *   - (adapted from J.D.Foley, A. van Dam, S.K.Feiner, J.F.Hughes
 *   - "Computer Graphics. Principles and practice", 
 *   - 2nd ed., 1990, section 3.2.2);
 * @param x0 x coordinate of first coordinate pair
 * @param y0 y coordinate of first coordinate pair
 * @param x1 x coordinate of second coordinate pair
 * @param y1 y coordinate of second coordinate pair
 * @return vector<int> contains all points on the line in the form of {x0, y0, x1, y1, ... , xn, yn} 
*/
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

/**
 * calculates and returns the center (hough line rho theta pair) of the connected components in the hough image
 * @param an_image reference to the hough image containing hough lines
 * @param components reference to the image containing the labeled connected components
 * @return vector<int> vector of rho theta coordinate pairs {rho_0, theta_0, rho_1, theta_1 ... rho_n, theta_n} 
*/
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

/**
 * Modifies an image by making it binary based on a threshold
 * @param an_image reference to the image which gets modified
 * @param threshold
 */
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
 * Modifies a binary image by finding and labeling the connected components
 * adapted from my assignment 2 p2.cc code
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

/**
 * calculates cartesian end points of a line within the given bounds from the polar coordinates given
 * @param rows upper bound of the x-value for the line (lower bound is understood to be 0)
 * @param cols upper bound of the y-value for the line (lower bound is understood to be 0)
 * @param rho first element of polar coordinate pair
 * @param theta second element of polar coordinate pair
 */
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

/**
 * calculates and draws hough lines on image with rho theta coordinates
 * @param an_image reference to the image that gets modified
 * @param vector<int> vector of rho theta coordinate pairs {rho_0, theta_0, rho_1, theta_1 ... rho_n, theta_n}  
 */
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

/**
 * calculates and draws trimmed hough lines on image with rho theta coordinates
 * @param an_image reference to the image that gets modified
 * @param an_image reference to the binary edge image
 * @param vector<int> vector of rho theta coordinate pairs {rho_0, theta_0, rho_1, theta_1 ... rho_n, theta_n}  
 * @param gap_tolerance how long a gap in the line segment can be
 * @param min_length minimum length of the lines
 */
void DrawTrimmedHoughLines(Image *an_image, const Image *edge_image, vector<double> houghlines, int gap_tolerance, int min_length){
  if (an_image == nullptr) abort();
  int rows = an_image->num_rows();
  int cols = an_image->num_columns();
  for(int i = 0; i < houghlines.size(); i += 2){
    double rho = houghlines[i];
    double theta = houghlines[i+1];
    vector<int> coords = PolarToCartesian(rows, cols, rho, theta);
    if(coords.size() == 4){
      vector<int> points = GetLinePoints(coords[0], coords[1], coords[2], coords[3]);
      int x0 = -1; int y0 = -1;
      int x1 = -1; int y1 = -1;
      int line_length = 0;
      int gap = 0;
      for(int j = 0; j < points.size(); j += 2){
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
            x1 = points[j]; y1 = points[j+1];
            line_length +=1;
          }
        }
        if(color == 0){
          if(x0 != -1) gap ++;
          if(gap > gap_tolerance){
            if(line_length > min_length){
              DrawLine(x0, y0, x1, y1, 255, an_image);
            }
            x0 = -1; y0 = -1;
            x1 = -1; y1 = -1;
            line_length = 0;
            gap = 0;
          }
        }
      }
    }
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

  // Deep copy hough_image into components
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
  
  AboveThreshold(&components, stoi(threshold));
  
  ConnectedComponents(&components);
  
  vector<double> hough_lines = FindHoughLines(&hough_image, &components);

  // If Binary Edge filename not provided draw regular Hough lines
  if (argc == 5)
    DrawHoughLines(&an_image, hough_lines);
  
  // If Binary Edge filename provided draw trimmed Hough Lines
  if (argc == 6){
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
