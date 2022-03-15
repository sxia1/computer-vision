#!/bin/bash

make h1;
make h2;
make h3;
make h4;
BINARY_THRESHOLD=100
SIMPLE1_VOTE_THRESHOLD=160
SIMPLE2_VOTE_THRESHOLD=120
COMPLEX_VOTE_THRESHOLD=100

# 100 180 110
./h1 hough_simple_1.pgm s1_h1_output.pgm;
./h2 s1_h1_output.pgm $BINARY_THRESHOLD s1_h2_output.pgm;
./h3 s1_h2_output.pgm s1_h3_output.pgm s1_arr.pgm;
./h4 hough_simple_1.pgm s1_arr.pgm $SIMPLE1_VOTE_THRESHOLD s1_h4_output1.pgm;
./h4 hough_simple_1.pgm s1_arr.pgm $SIMPLE1_VOTE_THRESHOLD s1_h4_output2.pgm s1_h2_output.pgm;
#explorer.exe h4_s1_output.pgm;

./h1 hough_simple_2.pgm s2_h1_output.pgm;
./h2 s2_h1_output.pgm $BINARY_THRESHOLD s2_h2_output.pgm;
./h3 s2_h2_output.pgm s2_h3_output.pgm s2_arr.pgm;
./h4 hough_simple_2.pgm s2_arr.pgm $SIMPLE2_VOTE_THRESHOLD s2_h4_output1.pgm;
./h4 hough_simple_2.pgm s2_arr.pgm $SIMPLE2_VOTE_THRESHOLD s2_h4_output2.pgm s2_h2_output.pgm;
#explorer.exe h4_s2_output.pgm;

./h1 hough_complex_1.pgm c1_h1_output.pgm;
./h2 c1_h1_output.pgm $BINARY_THRESHOLD c1_h2_output.pgm;
./h3 c1_h2_output.pgm c1_h3_output.pgm c1_arr.pgm;
./h4 hough_complex_1.pgm c1_arr.pgm $COMPLEX_VOTE_THRESHOLD c1_h4_output1.pgm;
./h4 hough_complex_1.pgm c1_arr.pgm $COMPLEX_VOTE_THRESHOLD c1_h4_output2.pgm c1_h2_output.pgm;
#explorer.exe h4_c1_output.pgm;
