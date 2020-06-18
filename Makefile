CC = g++
OPT = -O3
#OPT = -g
WARN = -Wall
CFLAGS = $(OPT) $(WARN) $(INC) $(LIB)

# List corresponding compiled object files here (.o files)
BUILD_OBJ = main.o
 
#################################

# default rule

all: pcf8523
	@echo "my work is done here..."


# rule for making pcf8523

pcf8523: $(BUILD_OBJ)
	$(CC) -o $@ $(CFLAGS) $(BUILD_OBJ) -lm
	@echo "-----------DONE WITH pcf8523-----------"


# generic rule for converting any .cpp file to any .o file
 
.cc.o:
	$(CC) $(CFLAGS)  -c $*.cc

.cpp.o:
	$(CC) $(CFLAGS)  -c $*.cpp


# type "make clean" to remove all .o files plus the pcf8523 binary

clean:
	rm -f *.o pcf8523


# type "make clobber" to remove all .o files (leaves pcf8523 binary)

clobber:
	rm -f *.o