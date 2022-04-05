Sophia Xia

Which Parts of the Assignment were Completed:

 - s1: COMPLETED
 - s2: COMPLETED
 - s3: COMPLETED
 - s4: COMPLETED

Instructions to Run:

s1:
$ make s1; ./s1 input_original_image.pgm input_threshold_value output_parameters_file.txt

s2:
$ make s2; ./s2 input_parameters_file.txt image1.pgm image2.pgm image3.pgm output_directions_file.txt

s3:
$ make s3; ./s3 input_directions.txt image1.pgm image2.pgm image3.pgm step threshold output_needle_image_filename.pgm

s4:
$ make s4; ./s4 input_directions.txt image1.pgm image2.pgm image3.pgm threshold output_albedo_image_filename.pgm

Thresholds used: 100

Formulas used:

To calculate the light source directions from the spheres in s2:
  
  x_component = brightest_pixel_x - sphere_center_x
  y_component = brightest_pixel_y - sphere_center_y
  
  z_component = sqrt(sphere_radius^2 - x_component^2 - y_component^2)
  
  direction_magnitude = sqrt(x_component^2 + y_component^2 + z_component^2)

  scaled_x_component = x_component * brightest_pixel_value / direction_magnitude
  scaled_y_component = y_component * brightest_pixel_value / direction_magnitude
  scaled_z_component = z_component * brightest_pixel_value / direction_magnitude

To calculate the surface normals of the object in s3:

          (s1x s1y s1z)-1    I1
      N = (s2x s2y s2z)   *  I2
          (s3x s3y s3z)      I3
      
      normal = inverse_transpose_light_direction_matrix * image_brightness_vector

      unit_normal = normal/normal_magnitude

To calculate the albedo of the object in s4:

      albedo = normal_magnitude = sqrt(normal_x^2 + normal_y^2 + normal_z^2)
