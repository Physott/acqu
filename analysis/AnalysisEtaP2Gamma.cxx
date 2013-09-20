#include "AnalysisEtaP2Gamma.h"




AnalysisEtaP2Gamma::AnalysisEtaP2Gamma()
{
	if(!(hInvMass		= (TH1D*)gROOT->Get("2G_InvMass")))
		hInvMass		= new TH1D("2G_InvMass", "2G_InvMass", 2000, 0, 2000);
	
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

void	AnalysisEtaP2Gamma::Clear()
{
	hInvMass->Reset("M");
}


bool	AnalysisEtaP2Gamma::Analyse(AnalysisEtaP* analysis)
{	
	hInvMass->Fill(analysis->vecAll.M());
	
	return true;
}


void	AnalysisEtaP2Gamma::Draw()
{	
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisEtaP2Gamma")))
		canvas	= new TCanvas("AnalysisEtaP2Gamma", "AnalysisEtaP2Gamma", 50, 50, 1600, 800);
	canvas->Clear();
	
	//canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd();	hInvMass->Draw();
}
void	AnalysisEtaP2Gamma::Save()
{	
	hInvMass->Write();
}

