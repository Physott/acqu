
#include "AnalysisEtaP.h"
#include "AnalysisEtaP2Gamma.h"
#include "AnalysisEtaP6Gamma.h"



AnalysisEtaP::AnalysisEtaP(const char* _treeFileName, const char* _treeName)	: ReadRootTree(_treeFileName, _treeName), analysis6(0)
{
	analysis2	= new AnalysisEtaP2Gamma();
	analysis6	= new AnalysisEtaP6Gamma();
	
	if(!(hCheckInvMassCutPi0		= (TH1D*)gROOT->Get("CheckInvMassCutPi0")))
		hCheckInvMassCutPi0		= new TH1D("CheckInvMassCutPi0", "CheckInvMassCutPi0", 300, 0, 300);
	if(!(hCheckInvMassCutEta		= (TH1D*)gROOT->Get("CheckInvMassCutEta")))
		hCheckInvMassCutEta		= new TH1D("CheckInvMassCutEta", "CheckInvMassCutEta", 300, 400, 700);
}
AnalysisEtaP::~AnalysisEtaP()
{
	if(analysis2)
		delete analysis2;
	if(analysis6)
		delete analysis6;
}

void	AnalysisEtaP::Clear()
{
	ReadRootTree::Clear();
	
	hCheckInvMassCutPi0->Reset("M");
	hCheckInvMassCutEta->Reset("M");
	
	analysis2->Clear();
	analysis6->Clear();
}

bool	AnalysisEtaP::AnalyseEvent(const int index)
{
	if(ReadRootTree::AnalyseEvent(index))
	{
		if(GetNCBHits()==2)
		{
			multiplicity	= MULTIPLICITY_2;
			return analysis2->Analyse(this);
		}
		else if(GetNCBHits()==6)
		{
			multiplicity	= MULTIPLICITY_6;
			return analysis6->Analyse(this);
		}
	}
	return false;
}
void	AnalysisEtaP::Analyse(const int min, const int max)
{
	int start=min;
	if(start < 0)
		start	= 0;
	if(start >= GetNEvents())
		start	= GetNEvents()-1;
	int stop=max;
	if(stop < 0)
		stop	= GetNEvents();
	if(stop > GetNEvents())
		stop	= GetNEvents();
	
	for(int i=start; i<stop; i++)
	{
		AnalyseEvent(i);
	}
}

void	AnalysisEtaP::Draw()
{
	ReadRootTree::Draw();
	
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisEtaP")))
		canvas	= new TCanvas("AnalysisEtaP", "AnalysisEtaP", 50, 50, 1600, 800);
	canvas->Clear();
	
	canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd(1);	hCheckInvMassCutPi0->Draw();
	canvas->cd(2);	hCheckInvMassCutEta->Draw();
	
	analysis2->Draw();
	analysis6->Draw();
}

void	AnalysisEtaP::Save()
{
	outFile->cd();
	
	hCheckInvMassCutPi0->Write();
	hCheckInvMassCutEta->Write();
	
	analysis2->Save();
	analysis6->Save();
}

void	AnalysisEtaP::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		ReadRootTree::Save();
		Save();
		delete outFile;
	}
}
