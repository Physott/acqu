
#include "AnalysisEtaP6GammaCanvas.h"


AnalysisEtaP6GammaCanvas::AnalysisEtaP6GammaCanvas(const Char_t* Name)
{
	//printf("AnalysisEtaP6GammaCanvas created. Name: %s\n", Name);
	
	strcpy(name,Name);
	
	Char_t	str[128];
	sprintf(str, "%s_InvMassPi0a", name);
	hInvMassPi0a	= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0b", name);
	hInvMassPi0b	= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassPi0cEta", name);
	hInvMassPi0cEta	= new TH1D(str, str, 1600, 0, 1600);
	sprintf(str, "%s_InvMassAll", name);
	hInvMassAll	= new TH1D(str, str, 1600, 0, 1600);
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
	
}
