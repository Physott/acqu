
#include "Analysis.h"
#include "AnalysisEtaP6Gamma.h"


Analysis::Analysis(const char* _treeFileName, const char* _treeName)	:	AnalysisTagger(_treeFileName, _treeName)
{
	result2		= new AnalysisEtaP2GammaTaggedCanvas("2G_EtaP_CutMM");
	result6[0]	= new AnalysisEtaP6GammaTaggedCanvas("6G_EtaP_CutMM");
	result6[1]	= new AnalysisEtaP6GammaTaggedCanvas("6G_3Oi0_CutMM");
}
	
Analysis::~Analysis()
{
}

void	Analysis::Clear()
{
	result2->Clear();
	result6[0]->Clear();
	result6[1]->Clear();
}

bool	Analysis::AnalyseEvent(const int index)
{	
	if(AnalysisTagger::AnalyseEvent(index))
	{
		for(int l=0; l<3; l++)
		{
			for(int i=0; i<nBeamCut[l]; i++)
			{
				
				if(nBeamCut[l])
				{
					if(GetNCBHits()==2)
						result2->Fill(vecAll.M(), missMassCut[l][i], l, nBeamCut[l]);
					else if(GetNCBHits()==6)
					{
						if(GetAnalysis6Gamma()->Is3Pi0())
							result6[1]->Fill(analysis6->GetMassPi0a(), analysis6->GetMassPi0b(), analysis6->GetMassPi0cEta(), analysis6->GetMassAll(), analysis6->GetMassSet(), missMassCut[l][i], l, nBeamCut[l]);
						else
							result6[0]->Fill(analysis6->GetMassPi0a(), analysis6->GetMassPi0b(), analysis6->GetMassPi0cEta(), analysis6->GetMassAll(), analysis6->GetMassSet(), missMassCut[l][i], l, nBeamCut[l]);
					}
				}
			}
		}
	}
	return true;
}

void	Analysis::Analyse(const int min, const int max)
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
		AnalyseEvent(i);
}

void	Analysis::Draw()
{
	AnalysisTagger::Draw();
	
	/*if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisEtaP")))
		canvas	= new TCanvas("AnalysisEtaP", "AnalysisEtaP", 50, 50, 1600, 800);
	canvas->Clear();
	
	canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd(1);	hCheckInvMassCutPi0->Draw();
	canvas->cd(2);	hCheckInvMassCutEta->Draw();*/
	
	result2->Draw();
	result6[0]->Draw();
	result6[1]->Draw();
}

void	Analysis::Save()
{
	outFile->cd();
	
	result2->Save();
	result6[0]->Save();
	result6[1]->Save();
}

void	Analysis::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		ReadRootTree::Save();
		AnalysisEtaP::Save();
		AnalysisTagger::Save();
		Save();
		delete outFile;
	}
}
