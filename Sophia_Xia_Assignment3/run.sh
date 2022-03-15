#!/bin/bash

make h1;
make h2;
make h3;
make h4;
BINARY_THRESHOLD=110
SIMPLE1_VOTE_THRESHOLD=160
SIMPLE2_VOTE_THRESHOLD=120
COMPLEX_VOTE_THRESHOLD=100
H4_INPUT=arr.pgm
# 100 180 110
./h1 hough_simple_1.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.pgm;
./h4 hough_simple_1.pgm $H4_INPUT $SIMPLE1_VOTE_THRESHOLD h4_s1_output.pgm h2_output.pgm;
explorer.exe h4_s1_output.pgm;

./h1 hough_simple_2.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.pgm;
./h4 hough_simple_2.pgm $H4_INPUT $SIMPLE2_VOTE_THRESHOLD h4_s2_output.pgm h2_output.pgm;
explorer.exe h4_s2_output.pgm;

./h1 hough_complex_1.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.pgm;
./h4 hough_complex_1.pgm $H4_INPUT $COMPLEX_VOTE_THRESHOLD h4_c1_output.pgm h2_output.pgm;
explorer.exe h4_c1_output.pgm;
