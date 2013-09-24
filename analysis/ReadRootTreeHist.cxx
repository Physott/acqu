
#include "ReadRootTreeHist.h"




ReadRootTreeHist::ReadRootTreeHist(const Char_t* Name)
{
	strcpy(name, Name);
	
	Char_t	helpName[128];
	
	sprintf(helpName, "%s_NTagged", Name);
	if(!(hNTagged		= (TH1I*)gROOT->Get(helpName)))
		hNTagged		= new TH1I(helpName, "NTagged", 32, 0, 32);
	sprintf(helpName, "%s_TaggedTime", Name);
	if(!(hTaggedTime	= (TH1D*)gROOT->Get(helpName)))
		hTaggedTime		= new TH1D(helpName, "TaggedTime", 1000, -250, 250);
	sprintf(helpName, "%s_TaggedEnergy", Name);
	if(!(hTaggedEnergy	= (TH1D*)gROOT->Get(helpName)))
		hTaggedEnergy	= new TH1D(helpName, "TaggedEnergy", 200, 1400, 1600);
	sprintf(helpName, "%s_NCBHits", Name);
	if(!(hNCBHits		= (TH1I*)gROOT->Get(helpName)))
		hNCBHits		= new TH1I(helpName, "NCBHits", 16, 0, 16);
	sprintf(helpName, "%s_CBTime", Name);
	if(!(hCBTime	= (TH1D*)gROOT->Get(helpName)))
		hCBTime		= new TH1D(helpName, "CBTime", 400, -200, 200);
	sprintf(helpName, "%s_CBEnergyAll", Name);
	if(!(hCBEnergyAll	= (TH1D*)gROOT->Get(helpName)))
		hCBEnergyAll		= new TH1D(helpName, "CBEnergyAll", 1600, 0, 1600);
		
	Clear();
}

ReadRootTreeHist::~ReadRootTreeHist()
{
}


void	ReadRootTreeHist::Clear()
{
	hNTagged->Reset("M");
	hTaggedTime->Reset("M");
	hTaggedEnergy->Reset("M");
	hNCBHits->Reset("M");
	hCBTime->Reset("M");
	hCBEnergyAll->Reset("M");
}


void	ReadRootTreeHist::Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy)
{
	hNTagged->Fill(NTagged);
	for(int i=0; i<NTagged; i++)
	{
		hTaggedTime->Fill(TaggedTime[i]);
		hTaggedEnergy->Fill(TaggedEnergy[i]);
	}
}
void	ReadRootTreeHist::Fill(const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll)
{
	hNCBHits->Fill(NCB);
	for(int i=0; i<NCB; i++)
		hCBTime->Fill(CBTime[i]);
	hCBEnergyAll->Fill(CBEnergyAll);
}
void	ReadRootTreeHist::Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy, const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll)
{
	hNTagged->Fill(NTagged);
	for(int i=0; i<NTagged; i++)
	{
		hTaggedTime->Fill(TaggedTime[i]);
		hTaggedEnergy->Fill(TaggedEnergy[i]);
	}
	hNCBHits->Fill(NCB);
	for(int i=0; i<NCB; i++)
	{
		hCBTime->Fill(CBTime[i]);
	}
	hCBEnergyAll->Fill(CBEnergyAll);
}



void	ReadRootTreeHist::Draw(TCanvas* canvas, const int posNTagged, const int posTaggedTime, const int posTaggedEnergy, const int posNCBHits, const int posCBTime, const int posCBEnergyAll)
{
	canvas->cd(posNTagged);	hNTagged->Draw();
	canvas->cd(posTaggedTime);	hTaggedTime->Draw();
	canvas->cd(posTaggedEnergy);	hTaggedEnergy->Draw();
	canvas->cd(posNCBHits);	hNCBHits->Draw();
	canvas->cd(posCBTime);	hCBTime->Draw();
	canvas->cd(posCBEnergyAll);	hCBEnergyAll->Draw();
}
void	ReadRootTreeHist::Save()
{
	hNTagged->Write();
	hTaggedTime->Write();
	hTaggedEnergy->Write();
	hNCBHits->Write();
	hCBTime->Write();
	hCBEnergyAll->Write();
}
