Sophia Xia

Which Parts of the Assignment were Completed:

 - h1: COMPLETED
 - h2: COMPLETED
 - h3: COMPLETED
 - h4: COMPLETED
 - Extra Credit: COMPLETED

Instructions to Run:

h1:
$ make h1; ./h1 <input_image.pgm> <output_filename.pgm>
  - output is the input image with Sobel 3x3 mask applied

h2:
$ make h2; ./h2 <edge_image.pgm> <threshold> <output_filename.pgm>
  - the output is the binary version of the edge image provided

h3:
$ make h3; ./h3 <binary_edge_image.pgm> <output_hough.pgm> <output_voting_array.pgm>
  - outputs are the hough image space and a binned version of the hough image space (the voting array)
  - I found that the binning lead to very inaccurate results down the line so I just left the bucket size at 1
    - this means that the two outputs are actually the same

h4:
$ make h4; ./h4 <orig_input_image.pgm> <voting_array.pgm> <threshold> <output_filename.pgm> <optional_binary_edge_image.pgm>
  - output is a original image with Hough lines drawn on it
  - if optional binary edge image is provided, trimmed Hough Lines will be drawn instead

Thresholds used:
Binary Edge Threshold: 110
Vote Threshold:
  - hough_simple_1.pgm: 160
  - hough_simple_2.pgm: 120
  - hough_complex_1.pgm: 100
