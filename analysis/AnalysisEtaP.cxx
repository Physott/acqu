
#include "AnalysisEtaP.h"
#include "AnalysisEtaP2Gamma.h"
//#include "AnalysisEtaP6Gamma.h"



AnalysisEtaP::AnalysisEtaP(const char* _treeFileName, const char* _treeName)	: ReadRootTree(_treeFileName, _treeName)
{
	analysis2	= new AnalysisEtaP2Gamma();
	//analysis6	= new AnalysisEtaP6Gamma();
	
}
AnalysisEtaP::~AnalysisEtaP()
{
	if(analysis2)
		delete analysis2;
	/*if(analysis6)
		delete analysis6;*/
}

void	AnalysisEtaP::Clear()
{
	ReadRootTree::Clear();
	
	analysis2->Clear();
	//analysis6->Clear();
}

bool	AnalysisEtaP::AnalyseEvent(const int index)
{
	if(ReadRootTree::AnalyseEvent(index))
	{
		if(GetNCBHits()==2)
		{
			return analysis2->Analyse(this);
		}
		else if(GetNCBHits()==6)
		{
			//return analysis6->Analyse(this);
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
	
	/*if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisEtaP")))
		canvas	= new TCanvas("AnalysisEtaP", "AnalysisEtaP", 50, 50, 1600, 800);
	canvas->Clear();
	
	canvas->Divide(2, 1, 0.001, 0.001);*/
	
	
	analysis2->Draw();
	//analysis6->Draw();
}

void	AnalysisEtaP::Save()
{
	outFile->cd();
	ReadRootTree::Save();
		
	
	analysis2->Save();
	//analysis6->Save();
}

void	AnalysisEtaP::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
