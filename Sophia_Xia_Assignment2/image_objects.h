// Sophia Xia
// this file contains the functions necessary to calculate the attributes of a connected component (object) and store them in a struct
// also contains drawing functionality for object orientation lines and dots
// created because both p3 and p4 use these functions

#ifndef IMAGE_OBJECTS_H
#define IMAGE_OBJECTS_H
#include "image.h"
#include <cstdio>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>

using namespace std;
using namespace ComputerVisionProjects;

// contains necessary attributes of object
struct object_data{
  object_data(): area(0), x(0), y(0), a(0), b(0), c(0) {}
  int label;
  int area;
  int x;
  int y;
  int a;
  int b;
  int c;
  double orientation;
  double e_min;
  double e_max;
  double roundedness;
};

/**
 * gets the labels of connected components in an image
 * @param an_image reference to the image
 * @return set<int> set of all labels of connected components
 */
set<int> GetLabels(const Image *an_image);

/**
 * calculates various attributes of all connected components in an image and stores them 
 * @param an_image reference to the image
 * @return map<int, struct object_data> a map where the connected component label
 *  is the key and the struct containing the attributes is the value
 */
map<int, struct object_data> GetObjectsData(const Image *an_image);

/**
 * Draws a 3 pixel by 3 pixel dot given a pair of coordinates 
 * @param an_image reference to the image that gets modified
 * @param x the center x value of dot
 * @param y the center y value of dot
 */
void DrawDot(Image *an_image, int x, int y);

/**
 * Draws the orientation line of a connected component(object) starting from its center
 * Also draws a dot at the center of the object
 * @param an_image reference to the image that gets modified
 * @param object a struct containing all the object attributes
 */
void DrawOrientation(Image *an_image, struct object_data obj);

#endif
