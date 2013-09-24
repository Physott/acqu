#include "ReadRootTree.h"


ReadRootTree::ReadRootTree(const char* _treeFileName, const char* _treeName)	: file(0), tree(0), isOpened(false), canvasGeneral(0), period(10000), nPeriod(1), outFile(0)
{
	//printf("ReadRootTree created. Parameters %s %s\n", _treeFileName, _treeName);
	
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	cutCBTime				= new Cut("CutCBTime", 400, -200, 200, "Cut CBTime");
	canvasGeneral			= new ReadRootTreeCanvas("ReadRootTree", "ReadRootTree General");
	canvasCut				= new ReadRootTreeCanvas("CutCBTime", "CutCBTime ReadRootTree");
	
	
	Clear();
	
	openTree();
}
ReadRootTree::~ReadRootTree()
{
	if(cutCBTime)
		delete	cutCBTime;
	if(canvasGeneral)
		delete	canvasGeneral;
	if(canvasCut)
		delete	canvasCut;
}

/*void	ReadRootTree::Clear()
{
	canvasGeneral->Clear();	
}*/
bool	ReadRootTree::openTree()
{
	if(isOpened)
		return true;
		
	file	= new TFile(treeFileName);
	if(!file)
	{
		printf("Could not open file %s\n", treeFileName);
		return false;
	}
	tree	= (TTree*)file->Get(treeName);
	if(!tree)
	{
		printf("Could not open tree %s in file %s\n", treeName, treeFileName);
		return false;
	}
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	tree->SetBranchAddress("Time", &Time);
	
	printf("Open file %s and load tree %s successfully.    %ld Events at all\n", treeFileName, treeName, (long int)tree->GetEntries());

	isOpened = true;
	return true;
}

void	ReadRootTree::Fill(ReadRootTreeCanvas* can)
{
	can->Fill(nTagged, TaggedTime, TaggedEnergy, nCBHits, Time, CBEnergyAll);
}
bool	ReadRootTree::AnalyseEvent(const int index)
{	
	//printf("AnalyseEvent %d\n", index);
	if((index-(nPeriod*period))==0)
	{
		printf("%d Events done\n", nPeriod*period);
		nPeriod++;
	}
		
	tree->GetEntry(index);
	//printf("AnalyseEvent %d\n", index);
	
	if(nCBHits == 2)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vecAll	= vec[0] + vec[1];
	}
	else if(nCBHits == 6)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2] ,E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3] ,E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4] ,E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5] ,E[5]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
	}
	else if(nCBHits == 10)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2] ,E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3] ,E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4] ,E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5] ,E[5]);
		vec[6].SetPxPyPzE(Px[6], Py[6], Pz[6] ,E[6]);
		vec[7].SetPxPyPzE(Px[7], Py[7], Pz[7] ,E[7]);
		vec[8].SetPxPyPzE(Px[8], Py[8], Pz[8] ,E[8]);
		vec[9].SetPxPyPzE(Px[9], Py[9], Pz[9] ,E[9]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5] + vec[6] + vec[7] + vec[8] + vec[9];
	}
	
	CBEnergyAll	= vecAll.E();
	
	Fill(canvasGeneral);
	
	if(cutCBTime->Analyse(nCBHits, Time))
	{
		Fill(canvasCut);
		return true;
	}
	
	return false;
}
void	ReadRootTree::Analyse(const int min, const int max)
{
	int start=min;
	if(start < 0)
		start	= 0;
	if(start >= tree->GetEntries())
		start	= tree->GetEntries()-1;
	int stop=max;
	if(stop < 0)
		stop	= tree->GetEntries();
	if(stop > tree->GetEntries())
		stop	= tree->GetEntries();
		
	for(int i=start; i<stop; i++)
	{
		//printf("bevor\n");
		AnalyseEvent(i);
		//printf("after\n");
	}
		
}
	

bool	ReadRootTree::OpenOutputFile(const Char_t* outputFileName)
{
	if(outFile)
	{
		if(outFile->IsOpen())
			return true;
		delete outFile;
	}
	outFile	= new TFile(outputFileName, "RECREATE");
	if(!outFile)
	{
		printf("OpenOutputFile 2a\n");
		printf("Could not open file %s\n", outputFileName);
		return false;
	}
	printf("OpenOutputFile 3\n");
	return true;
}
void	ReadRootTree::Save()
{
	outFile->cd();
	
	outFile->mkdir("ReadRootTree");
	outFile->cd("ReadRootTree");
	canvasGeneral->Save();
	
	outFile->cd();
	outFile->mkdir("CutCBTime");
	outFile->cd("CutCBTime");
	cutCBTime->Save();
	canvasCut->Save();
}
void	ReadRootTree::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
