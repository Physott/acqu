
#include "AnalysisEtaP2GammaTaggedCanvas.h"


AnalysisEtaP2GammaTaggedCanvas::AnalysisEtaP2GammaTaggedCanvas(const Char_t* Name)
{
	//printf("AnalysisEtaP2GammaTaggedCanvas created. Name: %s\n", Name);
	
	strcpy(name,Name);
	
	Char_t	str[128];
	Char_t	win[5][16];
	
	strcpy(win[0], "Prompt");
	strcpy(win[1], "Rand1");
	strcpy(win[2], "Rand2");
	
	
	for(int i=0; i<3; i++)
	{
		sprintf(str, "%s_InvMass_%s", name, win[i]);
		if(!(hInvMass[i]	= (TH1D*)gROOT->Get(str)))
			hInvMass[i]		= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "%s_MissingMass_%s", name, win[i]);
		if(!(hMissingMass[i]	= (TH1D*)gROOT->Get(str)))
			hMissingMass[i]		= new TH1D(str, str, 1600, 0, 1600);
	}
	for(int i=3; i<5; i++)
	{
		hInvMass[i]		= 0;
		hMissingMass[i]	= 0;
	}
	
	Clear();
}
AnalysisEtaP2GammaTaggedCanvas::~AnalysisEtaP2GammaTaggedCanvas()
{
}


void	AnalysisEtaP2GammaTaggedCanvas::SubstractBackground()
{
	for(int i=3; i<5; i++)
	{
		if(hInvMass[i])
			delete hInvMass[i];
		if(hMissingMass[i])
			delete hMissingMass[i];
	}
	
	Char_t	str[128];
	
	sprintf(str, "%s_InvMass_BG", name);
	hInvMass[3]		= (TH1D*)hInvMass[1]->Clone(str);
	hInvMass[3]->SetTitle(str);
	hInvMass[3]->Add(hInvMass[2]);
	hInvMass[3]->Scale(0.5);
	sprintf(str, "%s_MissingMass_BG", name);
	hMissingMass[3]	= (TH1D*)hMissingMass[1]->Clone(str);
	hMissingMass[3]->SetTitle(str);
	hMissingMass[3]->Add(hMissingMass[2]);
	hMissingMass[3]->Scale(0.5);
	
	sprintf(str, "%s_InvMass_Res", name);
	hInvMass[4]		= (TH1D*)hInvMass[0]->Clone(str);
	hInvMass[4]->SetTitle(str);
	hInvMass[4]->Add(hInvMass[3], -1);
	sprintf(str, "%s_MissingMass_Res", name);
	hMissingMass[4]	= (TH1D*)hMissingMass[0]->Clone(str);
	hMissingMass[4]->SetTitle(str);
	hMissingMass[4]->Add(hMissingMass[3], -1);
}


void	AnalysisEtaP2GammaTaggedCanvas::Clear()	
{
	
	for(int i=0; i<3; i++)
	{
		hInvMass[i]->Reset("M");
		hMissingMass[i]->Reset("M");
	}
	for(int i=3; i<5; i++)
	{
		if(hInvMass[i])
			delete hInvMass[i];
		if(hMissingMass[i])
			delete hMissingMass[i];
	}
}

void	AnalysisEtaP2GammaTaggedCanvas::Fill(const Double_t invMass, const Double_t missMass, const int TaggerWindow, const int Hits)
{
	for(int i=0; i<Hits; i++)
	{
		hInvMass[TaggerWindow]->Fill(invMass);
		hMissingMass[TaggerWindow]->Fill(missMass);
	}
}
	
	
void	AnalysisEtaP2GammaTaggedCanvas::Draw()
{
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name)))
		canvas	= new TCanvas(name, name, 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(4, 3, 0.001, 0.001);
	
	SubstractBackground();
	
	canvas->cd(1);	hInvMass[0]->Draw();
	canvas->cd(2);	hInvMass[1]->Draw();
	canvas->cd(3);	hInvMass[2]->Draw();
	canvas->cd(4);	hMissingMass[0]->Draw();
	canvas->cd(5);  hInvMass[3]->Draw();
	canvas->cd(6);  hMissingMass[1]->Draw();
	canvas->cd(7);  hMissingMass[2]->Draw();
	canvas->cd(8);  hMissingMass[3]->Draw();
	canvas->cd(9); 	hInvMass[4]->Draw();
	canvas->cd(12); hMissingMass[4]->Draw();
	
}

void	AnalysisEtaP2GammaTaggedCanvas::Save()
{
	SubstractBackground();
	
	for(int i=0; i<5; i++)
	{
		hInvMass[i]->Write();
		hMissingMass[i]->Write();
	}
}
