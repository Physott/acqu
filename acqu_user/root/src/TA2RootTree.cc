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





TA2RootTree::TA2RootTree(const char* Name, TA2Analysis* Analysis) : TA2AccessSQL(Name, Analysis),
                                                                    file(0),
                                                                    tree(0),
                                                                    nTagged(0),
                                                                    nCBHits(0),
                                                                    TaggedEnergy(0),
                                                                    TaggedTime(0),
                                                                    Px(0),
                                                                    Py(0),
                                                                    Pz(0),
                                                                    E(0),
                                                                    Time(0)
{
    strcpy(outputFolder,"~");
    strcpy(fileName,"TTreeOutput");

    AddCmdList(RootTreeConfigKeys);
}


TA2RootTree::~TA2RootTree()
{
	if(tree)
		delete tree;
    if(file)
		delete file;
}


void    TA2RootTree::LoadVariable()
{
    TA2AccessSQL::LoadVariable();
}

void    TA2RootTree::SetConfig(Char_t* line, Int_t key)
{
    switch(key)
    {
    case ERT_OUTPUT_FOLDER:
        strcpy(outputFolder,line);
        while(outputFolder[strlen(outputFolder)-1]=='\n' || outputFolder[strlen(outputFolder)-1]=='\r')
			outputFolder[strlen(outputFolder)-1]='\0';
        return;
    case ERT_FILE_NAME:
        strcpy(fileName,line);
        while(fileName[strlen(fileName)-1]=='\n' || fileName[strlen(fileName)-1]=='\r')
			fileName[strlen(fileName)-1]='\0';
        return;
    default:
        TA2AccessSQL::SetConfig(line, key);
    }
}

void    TA2RootTree::PostInit()
{
    TA2AccessSQL::PostInit();
    
    TaggedEnergy	= new Double_t[TA2ROOTTREE_MAX_TAGGER];
    TaggedTime		= new Double_t[TA2ROOTTREE_MAX_TAGGER];
    
    Px				= new Double_t[TA2ROOTTREE_MAX_CB];
    Py				= new Double_t[TA2ROOTTREE_MAX_CB];
    Pz				= new Double_t[TA2ROOTTREE_MAX_CB];
    E				= new Double_t[TA2ROOTTREE_MAX_CB];
    Time			= new Double_t[TA2ROOTTREE_MAX_CB];
    
    printf("---------\n");
    printf("Init Tree\n");
    printf("---------\n");
    
    Char_t	str[256];
    sprintf(str, "%s/%s_%d.root", outputFolder, fileName, GetRunNumber());
    file	= new TFile(str,"RECREATE");
    sprintf(str, "%s_%d", fileName, GetRunNumber());
	tree	= new TTree(str, str);
    
	tree->Branch("nTagged",&nTagged,"nTagged/I");
	tree->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	tree->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
	
	tree->Branch("nCBHits",&nCBHits,"nCBHits/I");
	tree->Branch("Px", Px, "Px[nCBHits]/D");
	tree->Branch("Py", Py, "Py[nCBHits]/D");
	tree->Branch("Pz", Pz, "Pz[nCBHits]/D");
	tree->Branch("E", E, "E[nCBHits]/D");	
	tree->Branch("Time", Time, "Time[nCBHits]/D");
}


void    TA2RootTree::Reconstruct()
{
	// Collect Tagger Hits
    nTagged	= fTagger->GetNParticle();  
    //printf("nTagged: %d",nTagged);
	for(int i=0; i<nTagged; i++)
	{
		TaggedEnergy[i]	= fTagger->GetParticles(i).GetE();
		TaggedTime[i]	= fTagger->GetParticles(i).GetTime();
	}
	
	// Collect CB Hits
    nCBHits	= fCB->GetNParticle();      
    //printf("nCBHits: %d",nCBHits);
	for(int i=0; i<nCBHits; i++)
	{
		Px[i]			= fCB->GetParticles(i).GetPx();
		Py[i]			= fCB->GetParticles(i).GetPy();
		Pz[i]			= fCB->GetParticles(i).GetPz();
		E[i]			= fCB->GetParticles(i).GetE();
		Time[i]			= fCB->GetParticles(i).GetTime();
	}
	
	tree->Fill();
}

void    TA2RootTree::Finish()
{
	printf("------------------\n");
	printf("Write Tree to file\n");
	printf("------------------\n");
	
	file->cd();
	tree->Write();
	
	
	if(tree)
		delete tree;
    if(file)
		delete file;
	
	TA2AccessSQL::Finish();
}

void    TA2RootTree::ParseMisc(char* line)
{
	TA2AccessSQL::ParseMisc(line);
}
