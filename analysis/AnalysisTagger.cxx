#include "AnalysisTagger.h"




AnalysisTagger::AnalysisTagger(const char* _treeFileName, const char* _treeName)	: AnalysisEtaP(_treeFileName, _treeName)
{
	if(!(hMissMass			= (TH1D*)gROOT->Get("MissingMass")))
		hMissMass			= new TH1D("MissingMass", "MissingMass", 1600, 0, 1600);
	if(!(hCheckCutMissMass	= (TH1D*)gROOT->Get("CheckCutMissingMass")))
		hCheckCutMissMass	= new TH1D("CheckCutMissingMass", "CheckCutMissingMass", 1600, 0, 1600);
	if(!(hCountWindow		= (TH1I*)gROOT->Get("CountTaggerWindow")))
		hCountWindow		= new TH1I("CountTaggerWindow", "CountTaggerWindow", 5, 0, 5);
	if(!(hCountWindowAccumulated	= (TH1I*)gROOT->Get("CountTaggerWindowAccumulated")))
		hCountWindowAccumulated		= new TH1I("CountTaggerWindowAccumulated", "CountTaggerWindowAccumulated", 5, 0, 5);
	if(!(hCountWindowN[0]	= (TH1I*)gROOT->Get("CountTaggerWindowNPrompt")))
		hCountWindowN[0]	= new TH1I("CountTaggerWindowNPrompt", "CountTaggerWindowNPrompt", 5, 0, 5);
	if(!(hCountWindowN[1]	= (TH1I*)gROOT->Get("CountTaggerWindowNRand1")))
		hCountWindowN[1]	= new TH1I("CountTaggerWindowNRand1", "CountTaggerWindowNRand1", 5, 0, 5);
	if(!(hCountWindowN[2]	= (TH1I*)gROOT->Get("CountTaggerWindowNRand2")))
		hCountWindowN[2]	= new TH1I("CountTaggerWindowNRand2", "CountTaggerWindowNRand2", 5, 0, 5);
	if(!(hCountWindowCut	= (TH1I*)gROOT->Get("CountTaggerWindowCut")))
		hCountWindowCut		= new TH1I("CountTaggerWindowCut", "CountTaggerWindowCut", 5, 0, 5);
	if(!(hCountWindowAccumulatedCut	= (TH1I*)gROOT->Get("CountTaggerWindowAccumulatedCut")))
		hCountWindowAccumulatedCut	= new TH1I("CountTaggerWindowAccumulatedCut", "CountTaggerWindowAccumulatedCut", 5, 0, 5);
	if(!(hCountWindowNCut[0]	= (TH1I*)gROOT->Get("CountTaggerWindowNCutPrompt")))
		hCountWindowNCut[0]	= new TH1I("CountTaggerWindowNCutPrompt", "CountTaggerWindowNCutPrompt", 5, 0, 5);
	if(!(hCountWindowNCut[1]	= (TH1I*)gROOT->Get("CountTaggerWindowNCutRand1")))
		hCountWindowNCut[1]	= new TH1I("CountTaggerWindowNCutRand1", "CountTaggerWindowNCutRand1", 5, 0, 5);
	if(!(hCountWindowNCut[2]	= (TH1I*)gROOT->Get("CountTaggerWindowNCutRand2")))
		hCountWindowNCut[2]	= new TH1I("CountTaggerWindowNCutRand2", "CountTaggerWindowNCutRand2", 5, 0, 5);
		
	Clear();
	
	for(int i=0; i<3; i++)
	{
		cutTaggerTime[2*i]		= -100000;
		cutTaggerTime[(2*i)+1]	= 100000;
	}
	cutMissMass[0]		= -100000;
	cutMissMass[1]		= 100000;
}
AnalysisTagger::~AnalysisTagger()
{
}


void	AnalysisTagger::Clear()
{
	AnalysisEtaP::Clear();

	hMissMass->Reset("M");
	hCheckCutMissMass->Reset("M");
	hCountWindow->Reset("M");
	hCountWindowAccumulated->Reset("M");
	hCountWindowN[0]->Reset("M");
	hCountWindowN[1]->Reset("M");
	hCountWindowN[2]->Reset("M");
	hCountWindowCut->Reset("M");
	hCountWindowAccumulatedCut->Reset("M");
	hCountWindowNCut[0]->Reset("M");
	hCountWindowNCut[1]->Reset("M");
	hCountWindowNCut[2]->Reset("M");
}

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
}
void	AnalysisTagger::AnalyseTagged()
{	
	for(int i=0; i<GetNTagged(); i++)
	{
		//printf("%d",i);
		//printf("%d\n",GetTaggedTime(i));
		if(GetTaggedTime(i)>=cutTaggerTime[0] && GetTaggedTime(i)<=cutTaggerTime[1])
		{
			beam[0][nBeam[0]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[0][nBeam[0]]	= ((beam[0][nBeam[0]]) -vecAll).M();
			hMissMass->Fill(missMass[0][nBeam[0]]);
			hCountWindowAccumulated->Fill(0);
			nBeam[0]++;
		}
		if(GetTaggedTime(i)>=cutTaggerTime[2] && GetTaggedTime(i)<=cutTaggerTime[3])
		{
			beam[1][nBeam[1]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[1][nBeam[1]]	= ((beam[1][nBeam[1]]) -vecAll).M();
			hMissMass->Fill(missMass[1][nBeam[1]]);
			hCountWindowAccumulated->Fill(1);
			nBeam[1]++;
		}
		if(GetTaggedTime(i)>=cutTaggerTime[4] && GetTaggedTime(i)<=cutTaggerTime[5])
		{
			beam[2][nBeam[2]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[2][nBeam[2]]	= ((beam[2][nBeam[2]]) -vecAll).M();
			hMissMass->Fill(missMass[2][nBeam[2]]);
			hCountWindowAccumulated->Fill(2);
			nBeam[2]++;
		}
	}
}
bool	AnalysisTagger::AnalyseEvent(const int index)
{	
	if(AnalysisEtaP::AnalyseEvent(index))
	{		
		nBeam[0]	= 0;
		nBeam[1]	= 0;
		nBeam[2]	= 0;
		
		hCountWindow->Fill(4);
		
		AnalyseTagged();
		
		//printf("%d\t%d\t%d\n",nBeam[0],nBeam[1],nBeam[2]);
		
		if(nBeam[0]>0)
		{
			hCountWindow->Fill(0);
			hCountWindowN[0]->Fill(nBeam[0]);
		}
		if(nBeam[1]>0)
		{
			hCountWindow->Fill(1);
			hCountWindowN[1]->Fill(nBeam[1]);
		}
		if(nBeam[2]>0)
		{
			hCountWindow->Fill(2);
			hCountWindowN[2]->Fill(nBeam[2]);
		}
			
		CutMissMass();
		
		bool ret	= false;
		
		if(nBeamCut[0]>0)
		{
			hCountWindowCut->Fill(0);
			hCountWindowNCut[0]->Fill(nBeamCut[0]);
			ret	= true;
		}
		if(nBeamCut[1]>0)
		{
			hCountWindowCut->Fill(1);
			hCountWindowNCut[1]->Fill(nBeamCut[1]);
			ret	= true;
		}
		if(nBeamCut[2]>0)
		{
			hCountWindowCut->Fill(2);
			hCountWindowNCut[2]->Fill(nBeamCut[2]);
			ret	= true;
		}
		return ret;
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
	
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisTaggerCanvas")))
		canvas	= new TCanvas("AnalysisTaggerCanvas", "AnalysisTagger", 50, 50, 1600, 800);
	canvas->Clear();
	
	canvas->Divide(3, 4, 0.001, 0.001);
	
	canvas->cd(1);	hMissMass->Draw();
	canvas->cd(2);	hCountWindow->Draw();
	canvas->cd(3);	hCountWindowAccumulated->Draw();
	canvas->cd(4);	hCountWindowN[0]->Draw();
	canvas->cd(5);	hCountWindowN[1]->Draw();
	canvas->cd(6);	hCountWindowN[2]->Draw();
	
	canvas->cd(7);	hCheckCutMissMass->Draw();
	canvas->cd(8);	hCountWindowCut->Draw();
	canvas->cd(9);	hCountWindowAccumulatedCut->Draw();
	canvas->cd(10);	hCountWindowNCut[0]->Draw();
	canvas->cd(11);	hCountWindowNCut[1]->Draw();
	canvas->cd(12);	hCountWindowNCut[2]->Draw();
}
void	AnalysisTagger::Save()
{
	outFile->cd();
	
	hMissMass->Write();
	hCheckCutMissMass->Write();
	hCountWindow->Write();
	hCountWindowAccumulated->Write();
	hCountWindowN[0]->Write();
	hCountWindowN[1]->Write();
	hCountWindowN[2]->Write();
	hCountWindowCut->Write();
	hCountWindowAccumulatedCut->Write();
	hCountWindowNCut[0]->Write();
	hCountWindowNCut[1]->Write();
	hCountWindowNCut[2]->Write();
}
void	AnalysisTagger::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		ReadRootTree::Save();
		AnalysisEtaP::Save();
		Save();
		
		delete outFile;
	}
}
