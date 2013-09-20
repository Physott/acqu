CC=g++
CFLAGS=-c -Wall `root-config --cflags`
LDFLAGS=`root-config --glibs`
SOURCES=main.cxx ReadRootTree.cxx AnalysisEtaP.cxx AnalysisEtaP2Gamma.cxx AnalysisEtaP6Gamma.cxx AnalysisEtaP6GammaCanvas.cxx AnalysisTagger.cxx AnalysisEtaP2GammaTaggedCanvas.cxx AnalysisEtaP6GammaTaggedCanvas.cxx Analysis.cxx
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
