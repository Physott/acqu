//*************************************************************************
//* Author: Patrik Ott
//*************************************************************************

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2RootTree                                                          //
//                                                                      //
// This class is organising the output to ROOT files                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2RootTree.h"

ClassImp(TA2RootTree)





TA2RootTree::TA2RootTree(const char* Name, TA2Analysis* Analysis) : TA2BasePhysics(Name, Analysis)
//const char* filename, const char* comment
{
	//file	= new TFile(filename,"RECREATE",comment);
}


TA2RootTree::~TA2RootTree()
{
}
