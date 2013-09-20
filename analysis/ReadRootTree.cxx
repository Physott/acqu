#include "ReadRootTree.h"


ReadRootTree::ReadRootTree(const char* _treeFileName, const char* _treeName)	: file(0), tree(0), isOpened(false), canvas(0), period(10000), nPeriod(1), outFile(0)
{
	//printf("ReadRootTree created. Parameters %s %s\n", _treeFileName, _treeName);
	
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	if(!(hNTagged		= (TH1I*)gROOT->Get("NTagged")))
		hNTagged		= new TH1I("NTagged", "NTagged", 32, 0, 32);
	if(!(hTaggedTime	= (TH1D*)gROOT->Get("TaggedTime")))
		hTaggedTime		= new TH1D("TaggedTime", "TaggedTime", 1000, -250, 250);
	if(!(hCountAll		= (TH1I*)gROOT->Get("CountAll")))
		hCountAll		= new TH1I("CountAll", "CountAll", 4, 0, 4);
	if(!(hNCBHits		= (TH1I*)gROOT->Get("NCBHits")))
		hNCBHits		= new TH1I("NCBHits", "NCBHits", 16, 0, 16);
	if(!(hCBTime	= (TH1D*)gROOT->Get("CBTime")))
		hCBTime		= new TH1D("CBTime", "CBTime", 800, 200, 200);
	if(!(hCBEnergyAll	= (TH1D*)gROOT->Get("CBEnergyAll")))
		hCBEnergyAll		= new TH1D("CBEnergyAll", "CBEnergyAll", 1600, 0, 1600);
	
	Clear();
	
	cutCBTime[0]	= -10000;
	cutCBTime[1]	= 10000;
	
	openTree();
}
ReadRootTree::~ReadRootTree()
{
}

void	ReadRootTree::Clear()
{
	hNTagged->Reset("M"); 
	hTaggedTime->Reset("M"); 
	hCountAll->Reset("M"); 
	hNCBHits->Reset("M");
	hCBTime->Reset("M");
	hCBEnergyAll->Reset("M");
	
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
		
	hCountAll->Fill(1);
	tree->GetEntry(index);
	
	hNTagged->Fill(nTagged);
	for(int i=0; i<nTagged; i++)
		hTaggedTime->Fill(TaggedTime[i]);
	hNCBHits->Fill(nCBHits);
	for(int i=0; i<nCBHits; i++)
	{
		hCBTime->Fill(Time[i]);
		if(Time[i]<cutCBTime[0] || Time[i]>cutCBTime[1])
			return false;
	}
	hCountAll->Fill(2);
	
	if(nCBHits == 2)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vecAll	= vec[0] + vec[1];
		hCBEnergyAll->Fill(vecAll.E());
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
		hCBEnergyAll->Fill(vecAll.E());
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
		hCBEnergyAll->Fill(vecAll.E());
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
	//printf("%ld\n",(TCanvas*)gROOT->GetListOfCanvases()->FindObject("ReadRootTreeCanvas"));
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("ReadRootTreeCanvas")))
		canvas	= new TCanvas("ReadRootTreeCanvas", "ReadRootTree", 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 2, 0.001, 0.001);
	
	canvas->cd(1);	hNTagged->Draw();
	canvas->cd(2);	hTaggedTime->Draw();
	canvas->cd(3);	hCountAll->Draw();
	canvas->cd(4);	hNCBHits->Draw();
	canvas->cd(5);	hCBTime->Draw();
	canvas->cd(6);	hCBEnergyAll->Draw();
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
	
	hNTagged->Write();
	hTaggedTime->Write();
	hCountAll->Write();
	hNCBHits->Write();
	hCBTime->Write();
	hCBEnergyAll->Write();
}
void	ReadRootTree::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
