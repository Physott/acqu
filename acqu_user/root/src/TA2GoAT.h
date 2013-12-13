#ifndef __TA2GoAT_h__
#define __TA2GoAT_h__


#include "TFile.h"
#include "TTree.h"
#include "TA2AccessSQL.h"

#define TA2GoAT_MAX_TAGGER	1024
#define TA2GoAT_MAX_PARTICLE	128
#define TA2GoAT_MAX_HITS	860  

enum {
    EG_OUTPUT_FOLDER = 30250,
    EG_FILE_NAME,
};

static const Map_t RootTreeConfigKeys[] = {
    // General keys
    {"RootTree-Output-Folder:"       	, EG_OUTPUT_FOLDER},
    {"RootTree-File-Name:"           	, EG_FILE_NAME},
    // Termination
    {NULL       	 		, -1           }
};

class	TA2GoAT	: public TA2AccessSQL
{
private:
		TFile*		file;				// outFile
		TTree*		treeRawEvent;		// Raw particle information (filled each event)
		TTree*		treeTagger;			// Tagger information (filled each event)
		TTree* 		treeTrigger;		// Trigger information (filled each event)
		TTree* 		treeDetectorHits;	// Detector system hit patterns (filled each event)
		TTree*		treeScaler; 		// Scaler read information (filled each scaler read)

    	char        outputFolder[256];
    	char        fileName[64];
    
    	//Particles    
    	Int_t		nParticles;		
    	Double_t*	Px;
    	Double_t*	Py;
		Double_t*	Pz;
    	Double_t*	E;
    	Double_t* 	Theta;
    	Double_t*	Phi;
    	Double_t*	time;
    	UChar_t*    clusterSize;
    
    	//Tagger
    	Int_t		nTagged;
    	Int_t*		tagged_ch;
    	Double_t*	tagged_t;
    
    	//Apparatus
    	UChar_t*	Apparatus;
    
    	//Charged detector energies
    	Double_t*	d_E;
    	Double_t*	WC0_E;
    	Double_t*	WC1_E;

		//Wire Chamber vertex reconstruction
    	Double_t* 	WC_Vertex_X;
    	Double_t* 	WC_Vertex_Y;
    	Double_t* 	WC_Vertex_Z;
    
    	//Hits
    	Int_t		nNaI_Hits;
    	Int_t*		NaI_Hits;
    	Int_t		nPID_Hits;
    	Int_t*		PID_Hits;
    	Int_t		nWC_Hits;
    	Int_t*		WC_Hits;
    	Int_t		nBaF2_PbWO4_Hits;
    	Int_t*		BaF2_PbWO4_Hits;
    	Int_t		nVeto_Hits;
    	Int_t*		Veto_Hits;
    
    	//Trigger 
    	Double_t 	ESum;	// or Detector Energies
    	Int_t 		Mult; 	
		UInt_t 		L2Pattern;
    
    	//Scalers
    	Int_t		eventNumber;
    	Int_t		eventID;    

	protected:    	

    	void MultiplicityMC();    //Generates the trigger for the MC
    	void MultiplicityHW();    //Generates the trigger from hardware information
    
	public:
	TA2GoAT(const char*, TA2Analysis*);
	~TA2GoAT();
	
	virtual void LoadVariable();            	//Creates histograms
    	virtual void SetConfig(Char_t*, Int_t); //Parses general information from configuration file
    	virtual void ParseMisc(char* line);     //Parses additional information from configuration file
    	virtual void Reconstruct();             //Event reconstruction
    	virtual void PostInit();                //Initialisation etc.
    	virtual void Finish();                	//Initialisation etc.

	ClassDef(TA2GoAT, 1)
};


#endif
