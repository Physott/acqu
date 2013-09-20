
#include "AnalysisEtaP6GammaTaggedCanvas.h"


AnalysisEtaP6GammaTaggedCanvas::AnalysisEtaP6GammaTaggedCanvas(const Char_t* Name)
{
	//printf("AnalysisEtaP6GammaTaggedCanvas created. Name: %s\n", Name);
	
	strcpy(name,Name);
	
	Char_t	str[128];
	Char_t	win[3][16];
	
	strcpy(win[0], "Prompt");
	strcpy(win[1], "Rand1");
	strcpy(win[2], "Rand2");
	
	
	for(int i=0; i<3; i++)
	{
		sprintf(str, "%s_InvMassPi0a_%s", name, win[i]);
		if(!(hInvMassPi0a[i]	= (TH1D*)gROOT->Get(str)))
			hInvMassPi0a[i]		= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_InvMassPi0b_%s", name, win[i]);
		if(!(hInvMassPi0b[i]	= (TH1D*)gROOT->Get(str)))
			hInvMassPi0b[i]		= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_InvMassPi0cEta_%s", name, win[i]);
		if(!(hInvMassPi0cEta[i]	= (TH1D*)gROOT->Get(str)))
			hInvMassPi0cEta[i]	= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_InvMassAll_%s", name, win[i]);
		if(!(hInvMassAll[i]		= (TH1D*)gROOT->Get(str)))
			hInvMassAll[i]		= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_InvMassAllSet_%s", name, win[i]);
		if(!(hInvMassAllSet[i]	= (TH1D*)gROOT->Get(str)))
			hInvMassAllSet[i]	= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_MissMass_%s", name, win[i]);
		if(!(hMissingMass[i]	= (TH1D*)gROOT->Get(str)))
			hMissingMass[i]		= new TH1D(str, str, 1600, 0, 1600);
	}
	for(int i=3; i<5; i++)
	{
		hInvMassPi0a[i]		= 0;
		hInvMassPi0b[i]		= 0;
		hInvMassPi0cEta[i]	= 0;
		hInvMassAll[i]		= 0;
		hInvMassAllSet[i]	= 0;
		hMissingMass[i]		= 0;
	}
	
	Clear();
}
AnalysisEtaP6GammaTaggedCanvas::~AnalysisEtaP6GammaTaggedCanvas()
{
	
}


void	AnalysisEtaP6GammaTaggedCanvas::SubstractBackground()
{
	for(int i=3; i<5; i++)
	{
		if(hInvMassPi0a[i])
			delete hInvMassPi0a[i];
		if(hInvMassPi0b[i])
			delete hInvMassPi0b[i];
		if(hInvMassPi0cEta[i])
			delete hInvMassPi0cEta[i];
		if(hInvMassAll[i])
			delete hInvMassAll[i];
		if(hInvMassAllSet[i])
			delete hInvMassAllSet[i];
		if(hMissingMass[i])
			delete hMissingMass[i];
	}
	
	Char_t	str[128];
	
	sprintf(str, "%s_InvMassPi0a_BG", name);
	hInvMassPi0a[3]		= (TH1D*)hInvMassPi0a[1]->Clone(str);
	hInvMassPi0a[3]->SetTitle(str);
	hInvMassPi0a[3]->Add(hInvMassPi0a[2]);
	hInvMassPi0a[3]->Scale(0.5);
	sprintf(str, "%s_InvMassPi0b_BG", name);
	hInvMassPi0b[3]		= (TH1D*)hInvMassPi0b[1]->Clone(str);
	hInvMassPi0b[3]->SetTitle(str);
	hInvMassPi0b[3]->Add(hInvMassPi0b[2]);
	hInvMassPi0b[3]->Scale(0.5);
	sprintf(str, "%s_InvMassPi0cEta_BG", name);
	hInvMassPi0cEta[3]		= (TH1D*)hInvMassPi0cEta[1]->Clone(str);
	hInvMassPi0cEta[3]->SetTitle(str);
	hInvMassPi0cEta[3]->Add(hInvMassPi0cEta[2]);
	hInvMassPi0cEta[3]->Scale(0.5);
	sprintf(str, "%s_InvMassAll_BG", name);
	hInvMassAll[3]		= (TH1D*)hInvMassAll[1]->Clone(str);
	hInvMassAll[3]->SetTitle(str);
	hInvMassAll[3]->Add(hInvMassAll[2]);
	hInvMassAll[3]->Scale(0.5);
	sprintf(str, "%s_InvMassAllSet_BG", name);
	hInvMassAllSet[3]		= (TH1D*)hInvMassAllSet[1]->Clone(str);
	hInvMassAllSet[3]->SetTitle(str);
	hInvMassAllSet[3]->Add(hInvMassAllSet[2]);
	hInvMassAllSet[3]->Scale(0.5);
	sprintf(str, "%s_MissingMass_BG", name);
	hMissingMass[3]	= (TH1D*)hMissingMass[1]->Clone(str);
	hMissingMass[3]->SetTitle(str);
	hMissingMass[3]->Add(hMissingMass[2]);
	hMissingMass[3]->Scale(0.5);
	
	sprintf(str, "%s_InvMassPi0a_Res", name);
	hInvMassPi0a[4]		= (TH1D*)hInvMassPi0a[0]->Clone(str);
	hInvMassPi0a[4]->SetTitle(str);
	hInvMassPi0a[4]->Add(hInvMassPi0a[3], -1);
	sprintf(str, "%s_InvMassPi0b_Res", name);
	hInvMassPi0b[4]		= (TH1D*)hInvMassPi0b[0]->Clone(str);
	hInvMassPi0b[4]->SetTitle(str);
	hInvMassPi0b[4]->Add(hInvMassPi0b[3], -1);
	sprintf(str, "%s_InvMassPi0cEta_Res", name);
	hInvMassPi0cEta[4]		= (TH1D*)hInvMassPi0cEta[0]->Clone(str);
	hInvMassPi0cEta[4]->SetTitle(str);
	hInvMassPi0cEta[4]->Add(hInvMassPi0cEta[3], -1);
	sprintf(str, "%s_InvMassAll_Res", name);
	hInvMassAll[4]		= (TH1D*)hInvMassAll[0]->Clone(str);
	hInvMassAll[4]->SetTitle(str);
	hInvMassAll[4]->Add(hInvMassAll[3], -1);
	sprintf(str, "%s_InvMassAllSet_Res", name);
	hInvMassAllSet[4]		= (TH1D*)hInvMassAllSet[0]->Clone(str);
	hInvMassAllSet[4]->SetTitle(str);
	hInvMassAllSet[4]->Add(hInvMassAllSet[3], -1);
	sprintf(str, "%s_MissingMass_Res", name);
	hMissingMass[4]	= (TH1D*)hMissingMass[0]->Clone(str);
	hMissingMass[4]->SetTitle(str);
	hMissingMass[4]->Add(hMissingMass[3], -1);
}


void	AnalysisEtaP6GammaTaggedCanvas::Clear()	
{
	for(int i=0; i<3; i++)
	{
		hInvMassPi0a[i]->Reset("M"); 
		hInvMassPi0b[i]->Reset("M"); 
		hInvMassPi0cEta[i]->Reset("M"); 
		hInvMassAll[i]->Reset("M");
		hInvMassAllSet[i]->Reset("M");
		hMissingMass[i]->Reset("M");
	}
	for(int i=3; i<5; i++)
	{
		if(hInvMassPi0a[i])
			delete	hInvMassPi0a[i];
		if(hInvMassPi0b[i])
			delete	hInvMassPi0b[i];
		if(hInvMassPi0cEta[i])
			delete	hInvMassPi0cEta[i];
		if(hInvMassAll[i])
			delete	hInvMassAll[i];
		if(hInvMassAllSet[i])
			delete	hInvMassAllSet[i];
		if(hMissingMass[i])
			delete	hMissingMass[i];
	}
}
	
void	AnalysisEtaP6GammaTaggedCanvas::Fill(const Double_t IMPi0a, const Double_t IMPi0b, const Double_t IMPi0cEta, const Double_t IMAll, const Double_t IMSet, const Double_t missMass, const int TaggerWindow, const int Hits)
{
	for(int i=0; i<Hits; i++)
	{		
		hInvMassPi0a[TaggerWindow]->Fill(IMPi0a); 
		hInvMassPi0b[TaggerWindow]->Fill(IMPi0b); 
		hInvMassPi0cEta[TaggerWindow]->Fill(IMPi0cEta); 
		hInvMassAll[TaggerWindow]->Fill(IMAll); 
		hInvMassAllSet[TaggerWindow]->Fill(IMSet);
		hMissingMass[TaggerWindow]->Fill(missMass);
	}
}
	
	
void	AnalysisEtaP6GammaTaggedCanvas::Draw()
{
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name)))
		canvas	= new TCanvas(name, name, 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(6, 5, 0.001, 0.001);
	
	SubstractBackground();
	
	for(int i=0; i<5; i++)
	{
		canvas->cd((6*i)+1);	hInvMassPi0a[i]->Draw();
		canvas->cd((6*i)+2);	hInvMassPi0b[i]->Draw();
		canvas->cd((6*i)+3);	hInvMassPi0cEta[i]->Draw();
		canvas->cd((6*i)+4);	hInvMassAll[i]->Draw();	
		canvas->cd((6*i)+5);	hInvMassAllSet[i]->Draw();
		canvas->cd((6*i)+6);	hMissingMass[i]->Draw();
	}
}

void	AnalysisEtaP6GammaTaggedCanvas::Save()
{
	SubstractBackground();
	
	for(int i=0; i<5; i++)
	{
		hInvMassPi0a[i]->Write();
		hInvMassPi0b[i]->Write();
		hInvMassPi0cEta[i]->Write();
		hInvMassAll[i]->Write();	
		hInvMassAllSet[i]->Write();
	}
}
