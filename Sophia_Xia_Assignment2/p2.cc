// Sophia Xia
// contains function that labels all connected components in an image
// Reads a given pgm image, and labels all the connected components in it
// The modified image is then saved to a new pgm image under the given filename

#include "image.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <map>

using namespace std;
using namespace ComputerVisionProjects;

/**
 * Modifies an image by finding and labeling the connected components
 * @param an_image reference to the image which gets modified
 */
void ConnectedComponents(Image *an_image){
  if (an_image == nullptr) abort();
  size_t rows = an_image->num_rows();
  size_t cols = an_image->num_columns();
  
  // equiv: key=label, value=equivalent label
  map<int, int> equiv;

  int label = 0;
  // First Pass
  for(size_t r = 0; r < rows; r++){
    for(size_t c = 0; c < cols; c++){
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
  int grays = 255/(components.size()+1); // +1 means white is never an option, important because orientation lines will be drawn in white later
  for(auto const& x: components){
    components[x.first] = grays;
    grays += grays;
  }
  // Second Pass to resolve equivalences
  for(size_t r = 0; r < rows; r++){
    for(size_t c = 0; c < cols; c++){
      int pixel = an_image->GetPixel(r,c);
      if (pixel > 0){
        an_image->SetPixel(r, c, components[equiv[pixel]]);
      }
    }
  }
}

int main(int argc, char **argv){
  
  if (argc!=3) {
    printf("Usage: %s input_image output_image_name\n", argv[0]);
    return 0;
  }
  const string input_file(argv[1]);
  const string output_file(argv[2]);

  Image an_image;
  if (!ReadImage(input_file, &an_image)) {
    cout <<"Can't open file " << input_file << endl;
    return 0;
  }

  ConnectedComponents(&an_image);
 
  if (!WriteImage(output_file, an_image)){
    cout << "Can't write to file " << output_file << endl;
    return 0;
  }
}
