
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
	sprintf(str, "%s_InvMassAllSet", name);
	hInvMassAllSet	= new TH1D(str, str, 1600, 0, 1600);
}
AnalysisEtaP6GammaCanvas::~AnalysisEtaP6GammaCanvas()
{
	
}

void	AnalysisEtaP6GammaCanvas::FillBackground(const AnalysisEtaP6GammaCanvas& rand1, const AnalysisEtaP6GammaCanvas& rand2)
{	
	Char_t	str[128];
	
	delete hInvMassPi0a;
	sprintf(str, "%s_InvMassPi0a", name);
	hInvMassPi0a = (TH1D*)rand1.hInvMassPi0a->Clone(str);	
	hInvMassPi0a->SetTitle(str);
	hInvMassPi0a->Add(rand2.hInvMassPi0a);
	hInvMassPi0a->Scale(0.5);
	
	delete hInvMassPi0b;
	sprintf(str, "%s_InvMassPi0b", name);
	hInvMassPi0b = (TH1D*)rand1.hInvMassPi0b->Clone(str);	
	hInvMassPi0b->SetTitle(str);
	hInvMassPi0b->Add(rand2.hInvMassPi0b);
	hInvMassPi0b->Scale(0.5);
	
	delete hInvMassPi0cEta;
	sprintf(str, "%s_InvMassPi0cEta", name);
	hInvMassPi0cEta = (TH1D*)rand1.hInvMassPi0cEta->Clone(str);	
	hInvMassPi0cEta->SetTitle(str);
	hInvMassPi0cEta->Add(rand2.hInvMassPi0cEta);
	hInvMassPi0cEta->Scale(0.5);
	
	delete hInvMassAll;
	sprintf(str, "%s_InvMassAll", name);
	hInvMassAll = (TH1D*)rand1.hInvMassAll->Clone(str);	
	hInvMassAll->SetTitle(str);
	hInvMassAll->Add(rand2.hInvMassAll);
	hInvMassAll->Scale(0.5);
	
	delete hInvMassAllSet;
	sprintf(str, "%s_InvMassAllSet", name);
	hInvMassAllSet = (TH1D*)rand1.hInvMassAllSet->Clone(str);	
	hInvMassAllSet->SetTitle(str);
	hInvMassAllSet->Add(rand2.hInvMassAllSet);
	hInvMassAllSet->Scale(0.5);
}

void	AnalysisEtaP6GammaCanvas::FillSubstract(const AnalysisEtaP6GammaCanvas& prompt, const AnalysisEtaP6GammaCanvas& background)
{
	Char_t	str[128];
	
	delete hInvMassPi0a;
	sprintf(str, "%s_InvMassPi0a", name);
	hInvMassPi0a = (TH1D*)prompt.hInvMassPi0a->Clone(str);
	hInvMassPi0a->SetTitle(str);
	hInvMassPi0a->Add(background.hInvMassPi0a, -1);
	
	delete hInvMassPi0b;
	sprintf(str, "%s_InvMassPi0b", name);
	hInvMassPi0b = (TH1D*)prompt.hInvMassPi0b->Clone(str);
	hInvMassPi0b->SetTitle(str);
	hInvMassPi0b->Add(background.hInvMassPi0b, -1);
	
	delete hInvMassPi0cEta;
	sprintf(str, "%s_InvMassPi0cEta", name);
	hInvMassPi0cEta = (TH1D*)prompt.hInvMassPi0cEta->Clone(str);
	hInvMassPi0cEta->SetTitle(str);
	hInvMassPi0cEta->Add(background.hInvMassPi0cEta, -1);
	
	delete hInvMassAll;
	sprintf(str, "%s_InvMassAll", name);
	hInvMassAll = (TH1D*)prompt.hInvMassAll->Clone(str);
	hInvMassAll->SetTitle(str);
	hInvMassAll->Add(background.hInvMassAll, -1);
	
	delete hInvMassAllSet;
	sprintf(str, "%s_InvMassAllSet", name);
	hInvMassAllSet = (TH1D*)prompt.hInvMassAllSet->Clone(str);
	hInvMassAllSet->SetTitle(str);
	hInvMassAllSet->Add(background.hInvMassAllSet, -1);
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
	canvas->cd(5);	hInvMassAllSet->Draw();
}

void	AnalysisEtaP6GammaCanvas::Save()
{
	hInvMassPi0a->Write();
	hInvMassPi0b->Write();
	hInvMassPi0cEta->Write();
	hInvMassAll->Write();	
	hInvMassAllSet->Write();
}
