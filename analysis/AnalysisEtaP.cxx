
#include "AnalysisEtaP.h"
#include "AnalysisEtaP2Gamma.h"
#include "AnalysisEtaP6Gamma.h"



AnalysisEtaP::AnalysisEtaP(const char* _treeFileName, const char* _treeName)	: ReadRootTree(_treeFileName, _treeName), analysis2(0)
{
	analysis2	= new AnalysisEtaP2Gamma("2G", "2G");
	analysis6	= new AnalysisEtaP6Gamma("6G", "6G");
	
	if(!(hCountDecay	= (TH1I*)gROOT->Get("CountDecay")))
		hCountDecay		= new TH1I("CountDecay", "1:all / 2:EtaP / 3:3Pi0 ", 5, 0, 5);
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
	
	hCountDecay->Reset("M");
	
	analysis2->Clear();
	analysis6->Clear();
}

bool	AnalysisEtaP::AnalyseEvent(const int index)
{
	if(ReadRootTree::AnalyseEvent(index))
	{
		hCountDecay->Fill(1);
		if(GetNCBHits()==2)
		{
			hCountDecay->Fill(2);
			multiplicity	= MULTIPLICITY_2;
			return analysis2->Analyse(this);
		}
		else if(GetNCBHits()==6)
		{
			hCountDecay->Fill(3);
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
	
	//canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd();	hCountDecay->Draw();
	
	analysis2->Draw();
	analysis6->Draw();
}

void	AnalysisEtaP::Save()
{
	outFile->cd();
	ReadRootTree::Save();
	
	outFile->mkdir("AnalysisEtaP");
	outFile->cd("AnalysisEtaP");
	
	hCountDecay->Write();
	
	outFile->cd();
	analysis2->Save(outFile);
	analysis6->Save(outFile);
}

void	AnalysisEtaP::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
