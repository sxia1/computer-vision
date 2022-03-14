#!/bin/bash

make h1;
make h2;
make h3;
make h4;
BINARY_THRESHOLD=100
VOTE_THRESHOLD=150
COMPLEX_VOTE_THRESHOLD=110

./h1 hough_simple_1.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.txt;
./h4 hough_simple_1.pgm h3_output.pgm $VOTE_THRESHOLD h4_s1_output.pgm;
explorer.exe h4_s1_output.pgm;

./h1 hough_simple_2.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.txt;
./h4 hough_simple_2.pgm h3_output.pgm $VOTE_THRESHOLD h4_s2_output.pgm;
explorer.exe h4_s2_output.pgm;

./h1 hough_complex_1.pgm h1_output.pgm;
./h2 h1_output.pgm $BINARY_THRESHOLD h2_output.pgm;
./h3 h2_output.pgm h3_output.pgm arr.txt;
./h4 hough_complex_1.pgm h3_output.pgm $COMPLEX_VOTE_THRESHOLD h4_c1_output.pgm;
explorer.exe h4_c1_output.pgm;
