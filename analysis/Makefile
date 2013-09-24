CC=g++
CFLAGS=-c -Wall `root-config --cflags`
LDFLAGS=`root-config --glibs`
SOURCES=main.cxx ReadRootTree.cxx ReadRootTreeHist.cxx 
OBJECTS=$(SOURCES:.cxx=.o)
EXECUTABLE=Analysis

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o Analysis
	
%.o:%.cxx
	$(CC) $(LDFLAGS) $(CFLAGS) -c $< -o $@
	
clean:
	rm -rf Analysis
	rm -rf *.o
