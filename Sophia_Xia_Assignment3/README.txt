Sophia Xia

Which Parts of the Assignment were Completed:

 - p1: COMPLETED
 - p2: COMPLETED
 - p3: COMPLETED
 - p4: COMPLETED

Bugs Encountered:

  - the colors of the labeled connected components don't turn out quite right
    - when run on the image with only 2 objects, one object is white while the other is gray
    - when run on the image with multiple objects, one object should be white but this is not the case
  - I later adjusted the color scale so that white would not be an option
    - this is because I draw my orientation lines in white
      - but I'm still not sure what happened with my original color algorithm

Instructions to Run:

p1:
$ make p1; ./p1 <input_image.pgm> <threshold> <output_filename.pgm>
  - replace threshold with a number between 0 and 255
  - I used 125 as the threshold as that worked well for me
  - output is binarized version of input image

p2:
$ make p2; ./p2 <binary_image.pgm> <output_filename.pgm>
  - output is image with labeled connected components

p3:
$ make p3; ./p3 <labeled_connected_components_image.pgm> <output_database.txt> <output_filename.pgm>
  - the database output is a plain text file with the object attributes on each line
  - output is image with dots drawn at the center of the objects with an orientation line emanating from the center

p4:
$ make p4; ./p4 <labeled_connected_components_image.pgm> <database.txt> <output_filename.pgm>
  - output is image with dots drawn at the center of the objects with an orientation line emanating from the center
    - they are only drawn on the objects that were recognized
