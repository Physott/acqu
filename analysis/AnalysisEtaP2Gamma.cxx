#include "AnalysisEtaP2Gamma.h"




AnalysisEtaP2Gamma::AnalysisEtaP2Gamma(const char* Name, const char* Title)	: AnalysisDecay(Name, Title)
{
	Char_t	helpName[128];
	
	sprintf(helpName, "%s_InvMass", Name);
	if(!(hInvMass		= (TH1D*)gROOT->Get(helpName)))
		hInvMass		= new TH1D(helpName, helpName, 2000, 0, 2000);
	
	sprintf(helpName, "%s_CutIM", Name);
	cutInvMass			= new Cut(helpName, 500, 700, 1200, helpName);
	
	cutInvMass->SetCut(850, 1050);
	
	Clear();
}
AnalysisEtaP2Gamma::~AnalysisEtaP2Gamma()
{
}

/*bool	AnalysisEtaP2Gamma::CutInvariantMass(AnalysisEtaP* analysis)
{
	//if(mass[bestPerm][0]<cutInvMassPi0[0] || mass[bestPerm][0]>cutInvMassPi0[1])
	//		return false;
	
	
	return true;
}*/



bool	AnalysisEtaP2Gamma::Analyse(AnalysisEtaP* analysis)
{	
	hInvMass->Fill(analysis->vecAll.M());
	
	cutInvMass->Analyse(analysis->vecAll.M());
	
	return false;
}


void	AnalysisEtaP2Gamma::Draw()
{	
	Char_t	helpName[128];
	
	sprintf(helpName, "%s_InvMass", name);
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(name)))
		canvas	= new TCanvas(name, name, 50, 50, 1600, 800);
	canvas->Clear();
	
	//canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd();	hInvMass->Draw();
	
	cutInvMass->Draw();
}


void	AnalysisEtaP2Gamma::Save(TFile* outFile)
{
	outFile->cd();	
	outFile->mkdir("2G");
	outFile->cd("2G");
	
	hInvMass->Write();	
	cutInvMass->Save();
	
	outFile->cd();
}

