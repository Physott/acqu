#include "ReadRootTree.h"


ReadRootTree::ReadRootTree(const char* _treeFileName, const char* _treeName)	: file(0), tree(0), isOpened(false), canvas(0), period(10000), nPeriod(1), outFile(0)
{
	//printf("ReadRootTree created. Parameters %s %s\n", _treeFileName, _treeName);
	
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	hist	= new ReadRootTreeHist("RawData");
	
	cutCBTime	= new Cut1Value("CutCBTime",400,-200,200);
	cutHist		= new ReadRootTreeHist("CutCBTime");
	
	openTree();
}
ReadRootTree::~ReadRootTree()
{
	if(canvas)
		delete	canvas;
	if(cutCanvas)
		delete	canvas;
}


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

			
bool	ReadRootTree::AnalyseEvent(const int index)
{	
	if((index-(nPeriod*period))==0)
	{
		printf("%d Events done\n", nPeriod*period);
		nPeriod++;
	}
		
	tree->GetEntry(index);
	
	if(nCBHits == 2)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vecAll	= vec[0] + vec[1];
		Fill(hist);	
		if(!cutCBTime->Analyse(nCBHits, Time))
			return false;
		Fill(cutHist);	
		return true;
	}
	if(nCBHits == 6)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2] ,E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3] ,E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4] ,E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5] ,E[5]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
		Fill(hist);	
		if(!cutCBTime->Analyse(nCBHits, Time))
			return false;
		Fill(cutHist);	
		return true;
	}
	if(nCBHits == 10)
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
		Fill(hist);	
		if(!cutCBTime->Analyse(nCBHits, Time))
			return false;
		Fill(cutHist);		
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
		AnalyseEvent(i);
		
}
	

void	ReadRootTree::Draw()
{
	//printf("%ld\n",(TCanvas*)gROOT->GetListOfCanvases()->FindObject("ReadRootTreeHist"));
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("RawData")))
		canvas	= new TCanvas("RawData", "RawData", 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 2, 0.001, 0.001);
	
	hist->Draw(canvas);
	
	
	if(!(cutCanvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("CutCBTime")))
		cutCanvas	= new TCanvas("CutCBTime", "CutCBTime", 50, 50, 1600, 800);
	cutCanvas->Clear();
		
	cutCanvas->Divide(3, 3, 0.001, 0.001);
	
	cutHist->Draw(cutCanvas, 4, 5, 6, 7, 8, 9);
	cutCBTime->Draw(cutCanvas, 1, 2, 3);
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
		printf("ERROR:!!!\n\tCould not open file %s\n", outputFileName);
		return false;
	}
	return true;
}
void	ReadRootTree::Save()
{
	outFile->cd();
	outFile->mkdir("RawData");
	outFile->cd("RawData");
	
	hist->Save();
	
	outFile->cd();
	outFile->mkdir("CutCBTime");
	outFile->cd("CutCBTime");
	
	cutCBTime->Save();
	cutHist->Save();
	
	outFile->cd();
}
void	ReadRootTree::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
