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

#include "TA2BasePhysics.h"

#define TA2ROOTTREE_MAX_MULTIPLICITY    8

enum {
    ERT_OUTPUT_FOLDER = 30250,
    ERT_FILE_NAME,
    ERT_PARAMETERS,
    ERT_MAX_TAGGED,
    ERT_TA2PARTICLE,
};

static const Map_t RootTreeConfigKeys[] = {
    // General keys
    {"RootTree-Output-Folder:"       	, ERT_OUTPUT_FOLDER},
    {"RootTree-File-Name:"           	, ERT_FILE_NAME},
    {"RootTree-Parameters:"				, ERT_PARAMETERS},
    {"RootTree-Max-Tagged:"           	, ERT_MAX_TAGGED},
    {"RootTree-TA2Particle:"           	, ERT_TA2PARTICLE},
    // Termination
    {NULL        , -1           }
};


class	TA2RootTree	: public TA2BasePhysics
{
private:
	TFile**		file;
	TTree**		tree;

    char        outputFolder[256];
    char        fileName[64];
    Int_t		treeCount;
    Int_t		photons[TA2ROOTTREE_MAX_MULTIPLICITY];
    Int_t		protons[TA2ROOTTREE_MAX_MULTIPLICITY];
    Int_t		piPlus[TA2ROOTTREE_MAX_MULTIPLICITY];
    
    Bool_t		ta2Particle;
    
    Int_t		nTaggedSave;
    Double_t*	TaggedEnergy;
    Double_t*	TaggedTime;
    
    
    void	InitTLorentzVectors(TFile** file, TTree** tree);
    void	InitTA2Particles(TFile** file, TTree** tree);
    
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
