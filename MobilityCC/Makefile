TARGET = ./cell 
OBJECT = cell.o mobility.o release.o report.o reserve.o variate.o sim.o initialize.o
CODE = cell.c mobility.c release.c report.c reserve.c variate.c initialize.c 
INCLUDE = cell.h sim.h variate.h 
 
CC=gcc 
CXXFLAGS=-Wall -g -O
CLIBFLAG=

.cc.o: $< $(INCLUDE)
	$(CC) $(CXXFLAGS) -c $<
#
#    Make Source code
#

all: $(TARGET)

$(TARGET):  $(OBJECT) $(INCLUDE)
	$(CC) $(CXXFLAGS) $(OBJECT) $(CLIBFLAG) -lm -o $(TARGET)

#
#    OBJECT Dependencies
#

cell.o: sim.h sim.c cell.h $(CODE) 
initialize.o: cell.h sim.h variate.h initialize.c
mobility.o: cell.h mobility.c 
release.o: cell.h release.c
report.o: cell.h report.c
reserve.o: cell.h reserve.c
variate.o: cell.h variate.c

#
#    Clean the Environment
#

clean:
	rm ./*.o

