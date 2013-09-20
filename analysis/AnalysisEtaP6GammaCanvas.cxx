
#include "AnalysisEtaP6GammaCanvas.h"


AnalysisEtaP6GammaCanvas::AnalysisEtaP6GammaCanvas(const Char_t* Name)
{
	//printf("AnalysisEtaP6GammaCanvas created. Name: %s\n", Name);
	
	strcpy(name,Name);
	
	Char_t	str[128];
	sprintf(str, "%s_InvMassPi0a", name);
	if(!(hInvMassPi0a		= (TH1D*)gROOT->Get(str)))
		hInvMassPi0a		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0b", name);
	if(!(hInvMassPi0b		= (TH1D*)gROOT->Get(str)))
		hInvMassPi0b		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0cEta", name);
	if(!(hInvMassPi0cEta	= (TH1D*)gROOT->Get(str)))
		hInvMassPi0cEta		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAll", name);
	if(!(hInvMassAll		= (TH1D*)gROOT->Get(str)))
		hInvMassAll			= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAllCut", name);
	if(!(hInvMassAllCut		= (TH1D*)gROOT->Get(str)))
		hInvMassAllCut		= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAllCutSet", name);
	if(!(hInvMassAllCutSet	= (TH1D*)gROOT->Get(str)))
		hInvMassAllCutSet	= new TH1D(str, str, 1600, 0, 1600);
		
	Clear();
}
AnalysisEtaP6GammaCanvas::~AnalysisEtaP6GammaCanvas()
{
	
}
	
void	AnalysisEtaP6GammaCanvas::Draw()
{
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name)))
		canvas	= new TCanvas(name, name, 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 2, 0.001, 0.001);
	
	canvas->cd(1);	hInvMassPi0a->Draw();
	canvas->cd(2);	hInvMassPi0b->Draw();
	canvas->cd(3);	hInvMassPi0cEta->Draw();
	canvas->cd(4);	hInvMassAll->Draw();	
	canvas->cd(5);	hInvMassAllCut->Draw();
	canvas->cd(6);	hInvMassAllCutSet->Draw();
}

void	AnalysisEtaP6GammaCanvas::Save()
{
	hInvMassPi0a->Write();
	hInvMassPi0b->Write();
	hInvMassPi0cEta->Write();
	hInvMassAll->Write();	
	hInvMassAllCut->Write();	
	hInvMassAllCutSet->Write();
}
