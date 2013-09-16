#include "ReadRootTree.h"


ReadRootTree::ReadRootTree(const char* _treeFileName, const char* _treeName)	: file(0), tree(0), isOpened(false), canvas(0), countAll(0)
{
	//printf("ReadRootTree created. Parameters %s %s\n", _treeFileName, _treeName);
	
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	hNTagged			= new TH1I("NTagged", "NTagged", 16, 0, 16);
	hNTagged->SetTitle("NTagged");
	hTaggedTime			= new TH1D("TaggedTime", "TaggedTime", 1000, -250, 250);
	hCountAll			= new TH1I("CountAll", "CountAll", 3, 0, 3);
	hNCBHits			= new TH1I("NCBHits", "NCBHits", 16, 0, 16);
	CBEnergyAll			= new TH1D("CBEnergyAll", "CBEnergyAll", 1600, 0, 1600);
	
	Clear();
	
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
	CBEnergyAll->Reset("M");
	
	countAll	= 0;
	for(int i=0; i<21; i++)
		countMultiplicity[i]	= 0;
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
	
	printf("Open file %s and load tree %s successfully\n", treeFileName, treeName);

	isOpened = true;
	return true;
}


bool	ReadRootTree::AnalyseEvent(const int index)
{	
	countAll++;
	hCountAll->Fill(1);
	tree->GetEntry(index);
	
	hNTagged->Fill(nTagged);
	for(int i=0; i<nTagged; i++)
		hTaggedTime->Fill(TaggedTime[i]);
	hNCBHits->Fill(nCBHits);
	
	if(nCBHits == 2)
	{
		countMultiplicity[2]++;
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		all	= vec[0] + vec[1];
		CBEnergyAll->Fill(all.E());
		return true;
	}
	if(nCBHits == 6)
	{
		countMultiplicity[6]++;
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0] ,E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1] ,E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2] ,E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3] ,E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4] ,E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5] ,E[5]);
		all	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
		CBEnergyAll->Fill(all.E());
		return true;
	}
	if(nCBHits == 10)
	{
		countMultiplicity[10]++;
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
		all	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5] + vec[6] + vec[7] + vec[8] + vec[9];
		CBEnergyAll->Fill(all.E());
		return true;
	}
	if(nCBHits<21)
	{
		countMultiplicity[nCBHits]++;
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
		
	PrintCounters();
}
	

void	ReadRootTree::PrintCounters()
{
	printf("Counters:\t\t%d events in input tree\n", countAll);
	printf("\tMultiplicity:\t%d events are 6 Hits in CB (%lf %%)\n", countMultiplicity[6], 100.0 * double(countMultiplicity[6])/double(countAll));
}
void	ReadRootTree::Draw()
{
	//printf("%ld\n",(TCanvas*)gROOT->GetListOfCanvases()->FindObject("ReadRootTreeCanvas"));
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("ReadRootTreeCanvas")))
		canvas	= new TCanvas("ReadRootTreeCanvas", "ReadRootTree", 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 2, 0.001, 0.001);
	
	//gStyle->SetOptTitle(1);		//SetTitle for every histogram gStyle->SetOptTitle( 1 ); 
	canvas->cd(1);	hNTagged->Draw();
	canvas->cd(2);	hTaggedTime->Draw();
	canvas->cd(3);	hCountAll->Draw();
	canvas->cd(4);	hNCBHits->Draw();
	canvas->cd(5);	CBEnergyAll->Draw();
}
