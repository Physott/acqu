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





TA2RootTree::TA2RootTree(const char* Name, TA2Analysis* Analysis) : TA2BasePhysics(Name, Analysis),
                                                                    file(0),
                                                                    tree(0),
                                                                    treeCount(0)
{
    strcpy(outputFolder,"~");
    strcpy(fileName,"TTreeOutput");
    
    

    AddCmdList(RootTreeConfigKeys);
}


TA2RootTree::~TA2RootTree()
{
	if(tree)
    {
		for(int i=0; i<treeCount; i++)
			delete tree[i];
		delete[] tree;
	}
    if(file)
    {
		for(int i=0; i<treeCount; i++)
			delete file[i];
		delete[] file;
	}
}


void    TA2RootTree::LoadVariable()
{
    TA2BasePhysics::LoadVariable();
}

void    TA2RootTree::SetConfig(Char_t* line, Int_t key)
{
    switch(key)
    {
    case ERT_OUTPUT_FOLDER:
        strcpy(outputFolder,line);
        return;
    case ERT_FILE_NAME:
        strcpy(fileName,line);
        return;
    case ERT_PARAMETERS:
        if(treeCount>=(TA2ROOTTREE_MAX_MULTIPLICITY-1))
        {
			printf("ERROR: Max TTree outputs is %d\n", TA2ROOTTREE_MAX_MULTIPLICITY);
			return;
		}
		sscanf(line, "%d %d %d", &photons[treeCount], &protons[treeCount], &piPlus[treeCount]);
		treeCount++;
        return;
    default:
        TA2BasePhysics::SetConfig(line, key);
    }
}

void    TA2RootTree::PostInit()
{
    TA2BasePhysics::PostInit();
    
    printf("Init %d trees\n", treeCount);
    for(int i=0; i<treeCount; i++)
		printf("\tInit tree with %d Photons, %d Proton and %d charged Pis\n", photons[i], protons[i], piPlus[i]);

    file	= new TFile*[treeCount];
    tree	= new TTree*[treeCount];
    
    Char_t	strFile[64];
    Char_t	strTree[64];
    
    for(int i=0; i<treeCount; i++)
    {
		sprintf(strTree, "%dH_%dP_%dPp", photons[i], protons[i], piPlus[i]);
		sprintf(strFile, "%s/%s_%s.root", outputFolder, fileName, strTree);
		file[i]	= new TFile(strFile,"RECREATE");
		sprintf(strFile, "tree_%s", strTree);
		tree[i]	= new TTree(strFile, strTree);
		
		for(int k=0; k<photons[i]; k++)
		{
			sprintf(strFile,"Photon%d.", k);
			tree[i]->Branch(strFile,"TLorentzVector",Photon[k].GetP4A(),8000,1);
		}
		for(int k=0; k<protons[i]; k++)
		{
			sprintf(strFile,"Proton%d.", k);
			tree[i]->Branch(strFile,"TLorentzVector",Proton[k].GetP4A(),8000,1);
		}
		for(int k=0; k<piPlus[i]; k++)
		{
			sprintf(strFile,"PiPlus%d.", k);
			tree[i]->Branch(strFile,"TLorentzVector",PiPlus[k].GetP4A(),8000,1);
		}
	}

}

void    TA2RootTree::Reconstruct()
{
    TA2BasePhysics::Reconstruct();
    
    Int_t	multiplicity=-1;
    
    for(int i=0; i<treeCount; i++)
    {
		if(nPhoton == photons[i] && nProton == protons[i] && nPiPlus == piPlus[i])
		{
			multiplicity=i;
			break;
		}
	}
	
	if(multiplicity == -1)
		return;
		
	file[multiplicity]->cd();
	tree[multiplicity]->Fill();
	tree[multiplicity]->FlushBaskets();
}

void    TA2RootTree::Finish()
{
	printf("Write trees to files\n");
	
	for(int i=0; i<treeCount; i++)
    {
		file[i]->cd();
		tree[i]->Write();
	}
	
	if(tree)
    {
		for(int i=0; i<treeCount; i++)
			delete tree[i];
		delete[] tree;
	}
    if(file)
    {
		for(int i=0; i<treeCount; i++)
			delete file[i];
		delete[] file;
	}
	
	TA2DataManager::Finish();
}

void    TA2RootTree::ParseMisc(char* line)
{
	TA2BasePhysics::ParseMisc(line);
}
