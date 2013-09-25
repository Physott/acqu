CC=g++
CFLAGS=-c -Wall `root-config --cflags`
LDFLAGS=`root-config --glibs`
SOURCES=main.cxx ReadRootTree.cxx ReadRootTreeHist.cxx Cut1Value.cxx AnalysisEtaP.cxx AnalysisEtaP2Gamma.cxx AnalysisEtaP6Gamma.cxx AnalysisEtaP6GammaCanvas.cxx CutNValues.cxx CutWindows.cxx AnalysisTagger.cxx
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
