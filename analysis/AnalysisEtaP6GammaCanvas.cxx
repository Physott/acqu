
#include "AnalysisEtaP6GammaCanvas.h"


AnalysisEtaP6GammaCanvas::AnalysisEtaP6GammaCanvas(const Char_t* Name, const bool Eta2Pi0)	: isEta2Pi0(Eta2Pi0)
{
	//printf("AnalysisEtaP6GammaCanvas created. Name: %s\n", Name);
	
	strcpy(name,Name);
	
	Char_t	str[128];
	sprintf(str, "%s_InvMassPi0a", name);
	if(!(hInvMassPi0a[0]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0a[0]		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0b", name);
	if(!(hInvMassPi0b[0]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0b[0]		= new TH1D(str, str, 1600, 0, 1600);
	if(isEta2Pi0)
		sprintf(str, "%s_InvMassEta", name);
	else
		sprintf(str, "%s_InvMassPi0c", name);
	if(!(hInvMassPi0cEta[0]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0cEta[0]	= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0aDiced", name);
	if(!(hInvMassPi0a[1]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0a[1]		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0bDiced", name);
	if(!(hInvMassPi0b[1]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0b[1]		= new TH1D(str, str, 1600, 0, 1600);
	if(isEta2Pi0)
		sprintf(str, "%s_InvMassEtaDiced", name);
	else
		sprintf(str, "%s_InvMassPi0cDiced", name);
	if(!(hInvMassPi0cEta[1]	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0cEta[1]	= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAll", name);
	if(!(hInvMassAll		= (TH1D*)gROOT->Get(str)))
		hInvMassAll			= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAllSet", name);
	if(!(hInvMassAllSet	= (TH1D*)gROOT->Get(str)))
		hInvMassAllSet	= new TH1D(str, str, 1600, 0, 1600);
		
	Clear();
}
AnalysisEtaP6GammaCanvas::~AnalysisEtaP6GammaCanvas()
{
	
}


void	AnalysisEtaP6GammaCanvas::Fill(const Double_t invMassPi0a, const Double_t invMassPi0b, const Double_t invMassPi0cEta, const Double_t invMassAll, const Double_t invMassAllSet)
{
	hInvMassPi0a[0]->Fill(invMassPi0a); 
	hInvMassPi0b[0]->Fill(invMassPi0b); 
	hInvMassPi0cEta[0]->Fill(invMassPi0cEta); 
		
	static int	dice	= 0;
	if(isEta2Pi0)
	{
		switch(dice)
		{
			case 0:
				dice	= 1;
				hInvMassPi0a[1]->Fill(invMassPi0a); 
				hInvMassPi0b[1]->Fill(invMassPi0b); 
				hInvMassPi0cEta[1]->Fill(invMassPi0cEta); 
				break;
			case 1:
				dice	= 0;
				hInvMassPi0a[1]->Fill(invMassPi0b); 
				hInvMassPi0b[1]->Fill(invMassPi0a); 
				hInvMassPi0cEta[1]->Fill(invMassPi0cEta); 
				break;
		}
	}
	else
	{
		switch(dice)
		{
			case 0:
				dice	= 1;
				hInvMassPi0a[1]->Fill(invMassPi0a); 
				hInvMassPi0b[1]->Fill(invMassPi0b); 
				hInvMassPi0cEta[1]->Fill(invMassPi0cEta); 
				break;
			case 1:
				dice	= 2;
				hInvMassPi0a[1]->Fill(invMassPi0cEta); 
				hInvMassPi0b[1]->Fill(invMassPi0a); 
				hInvMassPi0cEta[1]->Fill(invMassPi0b); 
				break;
			case 2:
				dice	= 0;
				hInvMassPi0a[1]->Fill(invMassPi0b); 
				hInvMassPi0b[1]->Fill(invMassPi0cEta); 
				hInvMassPi0cEta[1]->Fill(invMassPi0a); 
				break;
		}
	}
	
	hInvMassAll->Fill(invMassAll);
	hInvMassAllSet->Fill(invMassAllSet);
}


void	AnalysisEtaP6GammaCanvas::Draw()
{
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name)))
		canvas	= new TCanvas(name, name, 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 3, 0.001, 0.001);
	
	canvas->cd(1);	hInvMassPi0a[0]->Draw();
	canvas->cd(2);	hInvMassPi0b[0]->Draw();
	canvas->cd(3);	hInvMassPi0cEta[0]->Draw();
	canvas->cd(4);	hInvMassPi0a[1]->Draw();
	canvas->cd(5);	hInvMassPi0b[1]->Draw();
	canvas->cd(6);	hInvMassPi0cEta[1]->Draw();
	canvas->cd(7);	hInvMassAll->Draw();
	canvas->cd(8);	hInvMassAllSet->Draw();
}

void	AnalysisEtaP6GammaCanvas::Save()
{
	hInvMassPi0a[0]->Write();
	hInvMassPi0b[0]->Write();
	hInvMassPi0cEta[0]->Write();
	hInvMassPi0a[1]->Write();
	hInvMassPi0b[1]->Write();
	hInvMassPi0cEta[1]->Write();
	hInvMassAll->Write();	
	hInvMassAllSet->Write();
}
