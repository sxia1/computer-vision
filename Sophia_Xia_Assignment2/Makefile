##########################################
##
## Makefile for Assignment No. 2
## LINUX Compilation
##
## Sophia Xia
## I just added image_objects.o to ALL_OBJ3 and ALL_OBJ4 
##########################################

# FLAGS
C++FLAG = -g -std=c++11

MATH_LIBS = -lm

EXEC_DIR=.

.cc.o:
	g++ $(C++FLAG) $(INCLUDES) -c $< -o $@

# Including
INCLUDES= -I.

#-->All libraries (without LEDA)
LIBS_ALL =  -L/usr/lib -L/usr/local/lib 

# P1
ALL_OBJ1 = image.o p1.o 
PROGRAM_1 = p1
$(PROGRAM_1): $(ALL_OBJ1)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ1) $(INCLUDES) $(LIBS_ALL)

# P2
ALL_OBJ2 = image.o p2.o
PROGRAM_2 = p2
$(PROGRAM_2): $(ALL_OBJ2)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ2) $(INCLUDES) $(LIBS_ALL)

# P3
ALL_OBJ3 = image.o p3.o image_objects.o
PROGRAM_3 = p3
$(PROGRAM_3): $(ALL_OBJ3)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ3) $(INCLUDES) $(LIBS_ALL)

# P4
ALL_OBJ4 = image.o p4.o image_objects.o
PROGRAM_4 = p4
$(PROGRAM_4): $(ALL_OBJ4)
	g++ $(C++FLAG) -o $(EXEC_DIR)/$@ $(ALL_OBJ4) $(INCLUDES) $(LIBS_ALL)

# Compiling all
all:
	make $(PROGRAM_1)
	make $(PROGRAM_2)
	make $(PROGRAM_3)
	make $(PROGRAM_4)

# Clean files
clean:
	(rm -f *.o; rm -f $(PROGRAM_1); rm -f $(PROGRAM_2); rm -f $(PROGRAM_3); rm -f $(PROGRAM_4))
