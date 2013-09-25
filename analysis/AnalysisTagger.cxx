#include "AnalysisTagger.h"




AnalysisTagger::AnalysisTagger(const char* _treeFileName, const char* _treeName)	: AnalysisEtaP(_treeFileName, _treeName)
{
	cutWinRaw2Gamma[0]			= new ReadRootTreeHist("2G_CutIM_CutPrompt");
	cutWinRaw2Gamma[1]			= new ReadRootTreeHist("2G_CutIM_CutRand1");
	cutWinRaw2Gamma[2]			= new ReadRootTreeHist("2G_CutIM_CutRand2");
	cutWinInvMass2Gamma[0]		= new TH1D("2G_CutIM_CutPrompt_InvMass", "2G_CutIM_CutPrompt_InvMass", 1600, 0, 1600);
	cutWinInvMass2Gamma[1]		= new TH1D("2G_CutIM_CutRand1_InvMass", "2G_CutIM_CutRand1_InvMass", 1600, 0, 1600);
	cutWinInvMass2Gamma[2]		= new TH1D("2G_CutIM_CutRand2_InvMass", "2G_CutIM_CutRand2_InvMass", 1600, 0, 1600);
	cutWinRaw6Gamma[0][0]		= new ReadRootTreeHist("6G_EtaP_CutIM_CutPrompt");
	cutWinRaw6Gamma[0][1]		= new ReadRootTreeHist("6G_EtaP_CutIM_CutRand1");
	cutWinRaw6Gamma[0][2]		= new ReadRootTreeHist("6G_EtaP_CutIM_CutRand2");
	cutWinRaw6Gamma[1][0]		= new ReadRootTreeHist("6G_3Pi0_CutIM_CutPrompt");
	cutWinRaw6Gamma[1][1]		= new ReadRootTreeHist("6G_3Pi0_CutIM_CutRand1");
	cutWinRaw6Gamma[1][2]		= new ReadRootTreeHist("6G_3Pi0_CutIM_CutRand2");
	cutWinAnalysis6Gamma[0][0]		= new AnalysisEtaP6GammaCanvas("6G_EtaP_CutIM_CutPrompt", true);
	cutWinAnalysis6Gamma[0][1]		= new AnalysisEtaP6GammaCanvas("6G_EtaP_CutIM_CutRand1", true);
	cutWinAnalysis6Gamma[0][2]		= new AnalysisEtaP6GammaCanvas("6G_EtaP_CutIM_CutRand2", true);
	cutWinAnalysis6Gamma[1][0]		= new AnalysisEtaP6GammaCanvas("6G_3Pi0_CutIM_CutPrompt", false);
	cutWinAnalysis6Gamma[1][1]		= new AnalysisEtaP6GammaCanvas("6G_3Pi0_CutIM_CutRand1", false);
	cutWinAnalysis6Gamma[1][2]		= new AnalysisEtaP6GammaCanvas("6G_3Pi0_CutIM_CutRand2", false);
	
	cutWindows2Gamma	= new CutWindows("2G_CutIM_CutWin", -6, 6);
	cutWindows6Gamma[0]	= new CutWindows("6G_EtaP_CutIM_CutWin", -6, 6);
	cutWindows6Gamma[1]	= new CutWindows("6G_3Pi0_CutIM_CutWin", -6, 6);
	
	cutMissMass2Gamma		= new Cut1Value("2G_CutIM_CutWin_CutMM", 1600, 0, 1600);
	cutMissMass6Gamma[0]	= new Cut1Value("6G_EtaP_CutIM_CutWin_CutMM", 1600, 0, 1600);
	cutMissMass6Gamma[1]	= new Cut1Value("6G_3Pi0_CutIM_CutWin_CutMM", 1600, 0, 1600);
	
	Clear();
}
AnalysisTagger::~AnalysisTagger()
{
	if(cutWindows2Gamma)
		delete	cutWindows2Gamma;
	if(cutMissMass2Gamma)
		delete	cutMissMass2Gamma;
			
			
	for(int i=0; i<2; i++)
	{	
		if(cutWindows6Gamma[i])
			delete	cutWindows6Gamma[i];
		if(cutMissMass6Gamma[i])
			delete	cutMissMass6Gamma[i];
	}
			
			
	for(int i=0; i<4; i++)
	{
		if(cutWinRaw2Gamma[i])
			delete	cutWinRaw2Gamma[i];
		if(cutWinInvMass2Gamma[i])
			delete	cutWinInvMass2Gamma[i];
			
		
		for(int l=0; l<2; l++)
		{
			if(cutWinRaw6Gamma[l][i])
				delete	cutWinRaw6Gamma[l][i];
			if(cutWinAnalysis6Gamma[l][i])
				delete	cutWinAnalysis6Gamma[l][i];
		}
	}
}

void	AnalysisTagger::SetBeam(const int window, const int index)
{
	beam[window][nBeam[window]].SetPxPyPzE(GetTaggedEnergy(index), 0.0, 0.0, GetTaggedEnergy(index) + MASS_PROTON);
	missMass[window][nBeam[window]]	= ((beam[window][nBeam[window]]) -vecAll).M();
	nBeam[window]++;
}
	

void	AnalysisTagger::Clear()
{
	AnalysisEtaP::Clear();

	cutWindows2Gamma->Clear();
	cutWindows6Gamma[0]->Clear();
	cutWindows6Gamma[1]->Clear();
	
	cutMissMass2Gamma->Clear();
	cutMissMass6Gamma[0]->Clear();
	cutMissMass6Gamma[1]->Clear();
}
/*
void	AnalysisTagger::CutMissMass()
{
	nBeamCut[0]	= 0;
	nBeamCut[1]	= 0;
	nBeamCut[2]	= 0;
		
	for(int l=0; l<3; l++)
	{
		for(int i=0; i<nBeam[l]; i++)
		{
			if(missMass[l][i]>=cutMissMass[0] && missMass[l][i]<=cutMissMass[1])
			{
				beamCut[l][nBeamCut[l]]		= beam[l][i];
				missMassCut[l][nBeamCut[l]]	= missMass[l][i];
				hCheckCutMissMass->Fill(missMassCut[l][nBeamCut[l]]);
				hCountWindowAccumulatedCut->Fill(l);
				nBeamCut[l]++;
			}
		}
	}
}*/
bool	AnalysisTagger::AnalyseEvent(const int index)
{	
	if(AnalysisEtaP::AnalyseEvent(index))
	{		
		nBeam[0]	= 0;
		nBeam[1]	= 0;
		nBeam[2]	= 0;
		
		if(GetNCBHits()==2)
		{
			if(cutWindows2Gamma->Analyse(this))
			{
				for(int i=0; i<3; i++)
				{
					for(int l=0; l<nBeam[i]; l++)
					{
						Fill(cutWinRaw2Gamma[i]);
						cutWinInvMass2Gamma[i]->Fill(vecAll.M());
					}
				}
				return true;
			}
		}
		else if(GetNCBHits()==6)
		{
			if(GetAnalysis6Gamma()->IsEtaP())
				return cutWindows6Gamma[0]->Analyse(this);		
			else
				return cutWindows6Gamma[1]->Analyse(this);
		}
	}
	return false;
}
void	AnalysisTagger::Analyse(const int min, const int max)
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
void	AnalysisTagger::Draw()
{
	AnalysisEtaP::Draw();
	
	if(!(canvas2Gamma	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("2G_CutIM_CutWin")))
		canvas2Gamma	= new TCanvas("2G_CutIM_CutWin", "2G_CutIM_CutWin", 50, 50, 1600, 800);
	canvas2Gamma->Clear();
	canvas2Gamma->Divide(3, 3, 0.001, 0.001);
	
	cutWindows2Gamma->Draw(canvas2Gamma, 1, 4, 5, 6, 2, 3, 7, 8, 9);
	
	
	if(!(canvas6Gamma[0]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_EtaP_CutIM_CutWin")))
		canvas6Gamma[0]	= new TCanvas("6G_EtaP_CutIM_CutWin", "6G_EtaP_CutIM_CutWin", 50, 50, 1600, 800);
	canvas6Gamma[0]->Clear();
	canvas6Gamma[0]->Divide(3, 3, 0.001, 0.001);
	
	cutWindows6Gamma[0]->Draw(canvas6Gamma[0], 1, 4, 5, 6, 2, 3, 7, 8, 9);
	
	
	if(!(canvas6Gamma[1]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_3Pi0_CutIM_CutWin")))
		canvas6Gamma[1]	= new TCanvas("6G_3Pi0_CutIM_CutWin", "6G_3Pi0_CutIM_CutWin", 50, 50, 1600, 800);
	canvas6Gamma[1]->Clear();
	canvas6Gamma[1]->Divide(3, 3, 0.001, 0.001);
	
	cutWindows6Gamma[1]->Draw(canvas6Gamma[1], 1, 4, 5, 6, 2, 3, 7, 8, 9);
	
	
	
	if(!(cutWinCanvas2Gamma	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("2G_CutIM_CutWinRes")))
		cutWinCanvas2Gamma	= new TCanvas("2G_CutIM_CutWinRes", "2G_CutIM_CutWinRes", 50, 50, 1600, 800);
	cutWinCanvas2Gamma->Clear();
	cutWinCanvas2Gamma->Divide(5, 3, 0.001, 0.001);
	
	cutWinCanvas2Gamma->cd(1);	cutWinInvMass2Gamma[0]->Draw();
	cutWinCanvas2Gamma->cd(2);	cutWinInvMass2Gamma[1]->Draw();
	cutWinCanvas2Gamma->cd(3);	cutWinInvMass2Gamma[2]->Draw();
	cutWinRaw2Gamma[0]->Draw(cutWinCanvas2Gamma, 6, 7, 8, 11, 12, 13);
}
void	AnalysisTagger::Save()
{
	AnalysisEtaP::Save();
	
	outFile->cd();
	outFile->mkdir("2G/CutIM/CutWin");
	outFile->cd("2G/CutIM/CutWin");
	cutWindows2Gamma->Save();
	
	outFile->cd();
	outFile->mkdir("6G/EtaP/CutIM/CutWin");
	outFile->cd("6G/EtaP/CutIM/CutWin");
	cutWindows6Gamma[0]->Save();
	
	outFile->cd();
	outFile->mkdir("6G/3Pi0/CutIM/CutWin");
	outFile->cd("6G/3Pi0/CutIM/CutWin");
	cutWindows6Gamma[1]->Save();
}
void	AnalysisTagger::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		Save();
		delete outFile;
	}
}
