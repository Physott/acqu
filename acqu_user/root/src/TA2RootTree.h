//*************************************************************************
//* Author: Patrik Ott
//*************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2RootTree                                                          //
//                                                                      //
// This class is organising the output to ROOT files                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef __TA2RootTree_h__
#define __TA2RootTree_h__


#include "TFile.h"
#include "TTree.h"

#include "TA2AccessSQL.h"

#define TA2ROOTTREE_MAX_TAGGER	128
#define TA2ROOTTREE_MAX_CB		64

enum {
    ERT_OUTPUT_FOLDER = 30250,
    ERT_FILE_NAME,
};

static const Map_t RootTreeConfigKeys[] = {
    // General keys
    {"RootTree-Output-Folder:"       	, ERT_OUTPUT_FOLDER},
    {"RootTree-File-Name:"           	, ERT_FILE_NAME},
    // Termination
    {NULL        , -1           }
};





class	TA2RootTree	: public TA2AccessSQL
{
private:
	TFile*		file;
	TTree*		tree;

    char        outputFolder[256];
    char        fileName[64];
    
    Int_t				nTagged;
    Double_t*			TaggedEnergy;
    Double_t*			TaggedTime;
    
    Int_t				nCBHits;
    Double_t*			Px;
    Double_t*			Py;
    Double_t*			Pz;
    Double_t*			E;
    Double_t*			Time;
    
public:
	TA2RootTree(const char*, TA2Analysis*);
	~TA2RootTree();
	
	virtual void LoadVariable();            //Creates histograms
    virtual void SetConfig(Char_t*, Int_t); //Parses general information from configuration file
    virtual void ParseMisc(char* line);     //Parses additional information from configuration file
    virtual void Reconstruct();             //Event reconstruction
    virtual void PostInit();                //Initialisation etc.
    virtual void Finish();                //Initialisation etc.

	ClassDef(TA2RootTree, 1)
};


#endif
