// Sophia Xia
// this file contains the functions necessary to calculate the attributes of a connected component (object) and store them in a struct
// also contains drawing functionality for object orientation lines and dots
// created because both p3 and p4 use these functions

#include "image.h"
#include "image_objects.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * gets the labels of connected components in an image
 * @param an_image reference to the image
 * @return set<int> set of all labels of connected components
 */
set<int> GetLabels(const Image *an_image){
  if (an_image == nullptr) abort();
  set<int> labels;
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = an_image->GetPixel(r,c);
      if (color != 0) labels.insert(color);
    }
  }
  return labels; 
}

/**
 * calculates various attributes of all connected components in an image and stores them 
 * @param an_image reference to the image
 * @return map<int, struct object_data> a map where the connected component label
 *  is the key and the struct containing the attributes is the value
 */
map<int, struct object_data> GetObjectsData(const Image *an_image){
  if (an_image == nullptr) abort();

  set<int> labels = GetLabels(an_image);

  // INSTANTIATE MAP AND POPULATE
  map<int, struct object_data> objects;
  for(const int& label: labels){
    object_data object;
    objects[label] = object;
  }

  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();
  
  // FIRST PASS: GET CENTERS AND AREAS
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = an_image->GetPixel(r,c);
      if (color != 0){
        objects[color].area += 1;
        objects[color].x += r;
        objects[color].y += c;
      }
    }
  }
  for(const int& label: labels){
    objects[label].x = objects[label].x/objects[label].area;
    objects[label].y = objects[label].y/objects[label].area;
  }

  // SECOND PASS: GET A'S B'S AND C'S
  for(int r = 0; r < rows; r++){
    for(int c = 0; c < cols; c++){
      int color = an_image->GetPixel(r,c);
      if (color != 0){
        int cx = objects[color].x;
        int cy = objects[color].y;
        objects[color].a += pow(r-cx, 2);
        objects[color].b += 2*(r-cx)*(c-cy);
        objects[color].c += pow(c-cy, 2);
      }
    }
  }

  for(const int& label: labels){
    int a = objects[label].a;
    int b = objects[label].b;
    int c = objects[label].c;
    double theta1 = atan2(b, a-c)/2.0;
    double theta2 = theta1 + (M_PI/2.0);
    double e_min = a*pow(sin(theta1),2.0) - b*sin(theta1)*cos(theta1) + c*pow(cos(theta1), 2.0);
    double e_max = a*pow(sin(theta2),2.0) - b*sin(theta2)*cos(theta2) + c*pow(cos(theta2), 2.0);
    objects[label].orientation = 180*theta1/M_PI;
    objects[label].e_min = e_min;
    objects[label].e_max = e_max;
    objects[label].roundedness = e_min/e_max;
  }
  return objects; 
}

/**
 * Draws a 3 pixel by 3 pixel dot given a pair of coordinates 
 * @param an_image reference to the image that gets modified
 * @param x the center x value of dot
 * @param y the center y value of dot
 */
void DrawDot(Image *an_image, int x, int y){
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();
  an_image->SetPixel(x,y, 255);
  if(y-1 >= 0) an_image->SetPixel(x,y-1, 255);
  if(x-1 >= 0) an_image->SetPixel(x-1,y, 255);
  if(x+1 < rows) an_image->SetPixel(x+1,y, 255);
  if(y+1 < cols) an_image->SetPixel(x,y+1, 255);
  if(x-1 >= 0 && y-1 >= 0) an_image->SetPixel(x-1,y-1, 255);
  if(x+1 < rows && y-1 >= 0) an_image->SetPixel(x+1,y-1, 255);
  if(x-1 >= 0 && y+1 < cols) an_image->SetPixel(x-1,y+1, 255);
  if(x+1 < rows && y+1 < cols) an_image->SetPixel(x+1,y+1, 255);
}

/**
 * Draws the orientation line of a connected component(object) starting from its center
 * Also draws a dot at the center of the object
 * @param an_image reference to the image that gets modified
 * @param object a struct containing all the object attributes
 */
void DrawOrientation(Image *an_image, struct object_data object){
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();

  int cx = object.x;
  int cy = object.y;
  double theta1 = object.orientation*M_PI/180;
  DrawDot(an_image, cx, cy);
  
  int lx = cx+30*cos(theta1);
  int ly = cy+30*sin(theta1);
  if(lx <= rows && ly <= cols){
    DrawLine(cx, cy, lx, ly, 255, an_image);
  }
}
