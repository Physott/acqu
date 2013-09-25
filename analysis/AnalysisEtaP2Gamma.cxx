#include "AnalysisEtaP2Gamma.h"




AnalysisEtaP2Gamma::AnalysisEtaP2Gamma()
{
	if(!(hInvMass		= (TH1D*)gROOT->Get("2G_InvMass")))
		hInvMass		= new TH1D("2G_InvMass", "2G_InvMass", 2000, 0, 2000);
	
	rawHist	= new ReadRootTreeHist("2G");
	
	cutInvMass		= new Cut1Value("2G_CutIM", 400, 750, 1150);
	cutInvMass->SetCut(850, 1050);
	cutRawHist		= new ReadRootTreeHist("2G_CutIM");
	
	Clear();
}
AnalysisEtaP2Gamma::~AnalysisEtaP2Gamma()
{
	if(rawHist)
		delete	rawHist;
	if(cutRawHist)
		delete	cutRawHist;
	if(cutInvMass)
		delete	cutInvMass;
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
	rawHist->Clear();
	
	cutInvMass->Clear();
	cutRawHist->Clear();
}


bool	AnalysisEtaP2Gamma::Analyse(AnalysisEtaP* analysis)
{	
	hInvMass->Fill(analysis->vecAll.M());
	analysis->Fill(rawHist);
	
	if(cutInvMass->Analyse(analysis->vecAll.M()))
	{
		analysis->Fill(cutRawHist);
		return true;
	}
	
	return false;
}


void	AnalysisEtaP2Gamma::Draw()
{	
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("2G")))
		canvas	= new TCanvas("2G", "2G", 50, 50, 1600, 800);
	canvas->Clear();
	canvas->Divide(3, 3, 0.001, 0.001);
	
	canvas->cd(1);	hInvMass->Draw();
	rawHist->Draw(canvas, 4, 5, 6, 7, 8, 9);
	
	
	if(!(cutCanvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("2G_CutIM")))
		cutCanvas	= new TCanvas("2G_CutIM", "2G_CutIM", 50, 50, 1600, 800);
	cutCanvas->Clear();
	cutCanvas->Divide(3, 3, 0.001, 0.001);
	
	cutInvMass->Draw(cutCanvas, 1, 2, 3);
	cutRawHist->Draw(cutCanvas, 4, 5, 6, 7, 8, 9);
}
void	AnalysisEtaP2Gamma::Save(TFile* outFile, const Char_t* nameParent)
{	
	hInvMass->Write();
	rawHist->Save();
	
	Char_t	str[64];
	sprintf(str,"%s/CutIM", nameParent);
	outFile->mkdir(str);
	outFile->cd(str);
	
	cutInvMass->Save();
	cutRawHist->Save();
}

