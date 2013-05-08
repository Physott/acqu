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




class	TA2RootTree	: public TA2BasePhysics
{
private:
	TFile*		file;
	TTree*		tree;

public:
	TA2RootTree(const char*, TA2Analysis*);
	~TA2RootTree();
	
	virtual void LoadVariable();            //Creates histograms
    virtual void SetConfig(Char_t*, Int_t); //Parses general information from configuration file
    virtual void ParseMisc(char* line);     //Parses additional information from configuration file
    virtual void Reconstruct();             //Event reconstruction
    virtual void PostInit();                //Initialisation etc.

	ClassDef(TA2RootTree, 1)
};


#endif
