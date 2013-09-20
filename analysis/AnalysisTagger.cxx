#include "AnalysisTagger.h"




AnalysisTagger::AnalysisTagger(const char* _treeFileName, const char* _treeName)	: ReadRootTree(_treeFileName, _treeName)
{
	//printf("AnalysisTagger created. Parameters %s %s\n", _treeFileName, _treeName);
	
	for(int i=0; i<3; i++)
	{
		cutTaggerTime[2*i]		= -100000;
		cutTaggerTime[(2*i)+1]	= 100000;
	}
	cutMissMass[0]		= -100000;
	cutMissMass[1]		= 100000;
	
	if(!(hCountTaggerWindow		= (TH1I*)gROOT->Get("CountTimeWindow")))
		hCountTaggerWindow		= new TH1I("CountTimeWindow", "CountTimeWindow", 5, 0, 5);
	if(!(hCountTaggerWindowMulti= (TH1I*)gROOT->Get("CountTimeWindowMulti")))
		hCountTaggerWindowMulti	= new TH1I("CountTimeWindowMulti", "CountTimeWindowMulti", 3, 0, 3);
	if(!(hMissMass[0]			= (TH1D*)gROOT->Get("MissMassPrompt")))
		hMissMass[0]			= new TH1D("MissMassPrompt", "MissMassPrompt", 1600, 0, 1600);
	if(!(hMissMass[1]			= (TH1D*)gROOT->Get("MissMassRand1")))
		hMissMass[1]			= new TH1D("MissMassRand1", "MissMassRand1", 1600, 0, 1600);
	if(!(hMissMass[2]			= (TH1D*)gROOT->Get("MissMassRand2")))
		hMissMass[2]			= new TH1D("MissMassRand2", "MissMassRand2", 1600, 0, 1600);
	if(!(hMissMass[3]			= (TH1D*)gROOT->Get("MissMassSinglePrompt")))
		hMissMass[3]			= new TH1D("MissMassSinglePrompt", "MissMassSinglePrompt", 1600, 0, 1600);
	if(!(hMissMassMulti[0]		= (TH1D*)gROOT->Get("MissMassMultiPrompt")))
		hMissMassMulti[0]		= new TH1D("MissMassMultiPrompt", "MissMassMultiPrompt", 1600, 0, 1600);
	if(!(hMissMassMulti[1]		= (TH1D*)gROOT->Get("MissMassMultiRand1")))
		hMissMassMulti[1]		= new TH1D("MissMassMultiRand1", "MissMassMultiRand1", 1600, 0, 1600);
	if(!(hMissMassMulti[2]		= (TH1D*)gROOT->Get("MissMassMultiRand2")))
		hMissMassMulti[2]		= new TH1D("MissMassMultiRand2", "MissMassMultiRand2", 1600, 0, 1600);
	
	Clear();
}
AnalysisTagger::~AnalysisTagger()
{
	/*if(canvas)
		delete canvas;
	
	if(hCountTaggerWindow)
		delete hCountTaggerWindow;
	if(hCountTaggerWindowMulti)
		delete hCountTaggerWindowMulti;
		
	for(int i=0; i<3; i++)
	{
		if(hMissMass[i])
			delete hMissMass[i];
		if(hMissMassMulti[i])
			delete hMissMassMulti[i];
		if(hMissMassAll[i])
			delete hMissMassAll[i];
		if(hMissMassBackground[i])
			delete hMissMassBackground[i];
		if(hMissMassBackground[i])
			delete hMissMassSubstract[i];
	}
	if(hMissMass[3])
			delete hMissMass[3];*/
}


void	AnalysisTagger::Clear()
{
	ReadRootTree::Clear();
	
	hCountTaggerWindow->Reset("M");
	hCountTaggerWindowMulti->Reset("M");
	
	for(int i=0; i<3; i++)
	{
		countTaggerWindow[i]		= 0;
		countTaggerWindowMulti[i]	= 0;
		hMissMass[i]->Reset("M");
		hMissMassMulti[i]->Reset("M");
		//hMissMassAll[i]->Reset("M");
		//hMissMassSubstract[i]->Reset("M");
	}
	hMissMass[3]->Reset("M");
	countTaggerWindow[3]		= 0;
	countTaggerWindow[4]		= 0;
}

void	AnalysisTagger::CutMissMass()
{
	for(int i=0; i<3; i++)
	{
		for(int l=nBeam[i]-1; l>-1; l--)
		{
			if(missMass[i][l]<=cutMissMass[0] || missMass[i][l]>=cutMissMass[1])
			{
				for(int m=l+1; m<nBeam[i]; m++)
				{
					beam[i][m-1] = beam[i][m];
					nBeam[i]--;
				}
			}
		}
	}
}
void	AnalysisTagger::AnalyseTagged()
{
	TLorentzVector	res;
	
	for(int i=0; i<GetNTagged(); i++)
	{
		if(GetTaggedTime(i)>cutTaggerTime[0] && GetTaggedTime(i)<cutTaggerTime[1])
		{
			beam[0][nBeam[0]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[0][nBeam[0]]	= ((beam[0][nBeam[0]]) -all).M();
			nBeam[0]++;
		}
		if(GetTaggedTime(i)>cutTaggerTime[2] && GetTaggedTime(i)<cutTaggerTime[3])
		{
			beam[1][nBeam[1]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[1][nBeam[1]]	= ((beam[1][nBeam[1]]) -all).M();
			nBeam[1]++;
		}
		if(GetTaggedTime(i)>cutTaggerTime[4] && GetTaggedTime(i)<cutTaggerTime[5])
		{
			beam[2][nBeam[2]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i) + MASS_PROTON);
			missMass[2][nBeam[2]]	= ((beam[2][nBeam[2]]) -all).M();
			nBeam[2]++;
		}
	}
}
bool	AnalysisTagger::AnalyseEvent(const int index)
{	
	if(ReadRootTree::AnalyseEvent(index))
	{		
		countTaggerWindow[4]++;
		hCountTaggerWindow->Fill(4);
		nBeam[0]	= 0;
		nBeam[1]	= 0;
		nBeam[2]	= 0;
		untagged	= false;
		
		AnalyseTagged();
		//CutMissMass();
		
		if(nBeam[0]>0 || nBeam[1]>0 || nBeam[2]>0)
		{
			if(nBeam[0]>1)
			{
				for(int i=0; i<nBeam[0]; i++)
					hMissMassMulti[0]->Fill(missMass[0][i]);
				countTaggerWindowMulti[0]++;
				hCountTaggerWindowMulti->Fill(0);
			}
			else if(nBeam[0] == 1)
			{
				hMissMass[0]->Fill(missMass[0][0]);
				countTaggerWindow[0]++;
				hCountTaggerWindow->Fill(0);
			}
			
			if(nBeam[1]>1)
			{
				for(int i=0; i<nBeam[1]; i++)
					hMissMassMulti[1]->Fill(missMass[1][i]);
				countTaggerWindowMulti[1]++;
				hCountTaggerWindowMulti->Fill(1);
			}
			else if(nBeam[1] == 1)
			{
				hMissMass[1]->Fill(missMass[1][0]);
				countTaggerWindow[1]++;
				hCountTaggerWindow->Fill(1);
			}
				
			if(nBeam[2]>1)
			{
				for(int i=0; i<nBeam[2]; i++)
					hMissMassMulti[2]->Fill(missMass[2][i]);
				countTaggerWindowMulti[2]++;
				hCountTaggerWindowMulti->Fill(2);
			}
			else if(nBeam[2] == 1)
			{
				hMissMass[2]->Fill(missMass[2][0]);
				countTaggerWindow[2]++;
				hCountTaggerWindow->Fill(2);
			}
			
			if(GetNTagged() == 1)
			{
				if(nBeam[0] == 1)
				{
					hMissMass[3]->Fill(missMass[0][0]);
					countTaggerWindow[3]++;
					hCountTaggerWindow->Fill(3);
				}
				uniqueWindow	= true;
			}
			else
				uniqueWindow	= false;
		}
		else
			untagged	= true;
			
		return true;
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
	{
		AnalyseEvent(i);
	}
	
	PrintCounters();
}
void	AnalysisTagger::PrintCounters()
{
	ReadRootTree::PrintCounters();
	
	printf("\tTaggerTime:\t%d events checked.\t%d in Prompt Window (%lf %%).\t%d in Rand1 Window (%lf %%).\t%d in Rand2 Window (%lf %%).\t%d have only prompt hit (%lf %%)\n", countTaggerWindow[4], countTaggerWindow[0], 100.0 * double(countTaggerWindow[0])/double(countTaggerWindow[4]), countTaggerWindow[1], 100.0*double(countTaggerWindow[1])/double(countTaggerWindow[4]), countTaggerWindow[2], 100.0*double(countTaggerWindow[2])/double(countTaggerWindow[4]), countTaggerWindow[3], 100.0 * double(countTaggerWindow[3])/double(countTaggerWindow[4]));
	printf("\t   MultiHits:\t\t\t\t%d in Prompt Window (%lf %%).\t%d in Rand1 Window (%lf %%). \t%d in Rand2 Window (%lf %%)\n", countTaggerWindowMulti[0], 100.0 * double(countTaggerWindowMulti[0])/double(countTaggerWindow[4]), countTaggerWindowMulti[1], 100.0*double(countTaggerWindowMulti[1])/double(countTaggerWindow[4]), countTaggerWindowMulti[2], 100.0*double(countTaggerWindowMulti[2])/double(countTaggerWindow[4]));
}
void	AnalysisTagger::Draw()
{
	ReadRootTree::Draw();
	
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("AnalysisTaggerCanvas")))
		canvas	= new TCanvas("AnalysisTaggerCanvas", "AnalysisTagger", 50, 50, 1600, 800);
	canvas->Clear();
	
	canvas->Divide(7, 3, 0.001, 0.001);
	
	canvas->cd(1);	hCountTaggerWindow->Draw();
	canvas->cd(2);	hMissMass[0]->Draw();
	canvas->cd(3);	hMissMass[1]->Draw();
	canvas->cd(4);	hMissMass[2]->Draw();
	canvas->cd(5);	hMissMass[3]->Draw();
	canvas->cd(8);	hCountTaggerWindowMulti->Draw();
	canvas->cd(9);	hMissMassMulti[0]->Draw();
	canvas->cd(10);	hMissMassMulti[1]->Draw();
	canvas->cd(11);	hMissMassMulti[2]->Draw();
	
	CalcHistograms();
	
	canvas->cd(6);	hMissMassBackground[0]->Draw();
	canvas->cd(7);	hMissMassSubstract[0]->Draw();
	canvas->cd(13);	hMissMassBackground[1]->Draw();
	canvas->cd(14);	hMissMassSubstract[1]->Draw();
	canvas->cd(16);	hMissMassAll[0]->Draw();
	canvas->cd(17);	hMissMassAll[1]->Draw();
	canvas->cd(18);	hMissMassAll[2]->Draw();
	canvas->cd(20);	hMissMassBackground[2]->Draw();
	canvas->cd(21);	hMissMassSubstract[2]->Draw();
}
void	AnalysisTagger::CalcHistograms()
{
	hMissMassAll[0] = (TH1D*)hMissMass[0]->Clone("MissMassAllPrompt");
	hMissMassAll[0]->SetTitle("MissMassAllPrompt");
	hMissMassAll[1] = (TH1D*)hMissMass[1]->Clone("MissMassAllRand1");
	hMissMassAll[1]->SetTitle("MissMassAllRand1");
	hMissMassAll[2] = (TH1D*)hMissMass[2]->Clone("MissMassAllRand2");
	hMissMassAll[2]->SetTitle("MissMassAllRand2");
	hMissMassAll[0]->Add(hMissMassMulti[0]);
	hMissMassAll[1]->Add(hMissMassMulti[1]);
	hMissMassAll[2]->Add(hMissMassMulti[2]);
	
	hMissMassBackground[0] = (TH1D*)hMissMass[1]->Clone("MissMassBackgroundPrompt");
	hMissMassBackground[0]->SetTitle("MissMassBackgroundPrompt");
	hMissMassBackground[0]->Add(hMissMass[2]);
	hMissMassBackground[0]->Scale(0.5);
	hMissMassSubstract[0] = (TH1D*)hMissMass[0]->Clone("MissMassSubstractPrompt");
	hMissMassSubstract[0]->SetTitle("MissMassSubstractPrompt");
	hMissMassSubstract[0]->Add(hMissMassBackground[0], -1);
	
	hMissMassBackground[1] = (TH1D*)hMissMassMulti[1]->Clone("MissMassBackgroundRand1");
	hMissMassBackground[1]->SetTitle("MissMassBackgroundRand1");
	hMissMassBackground[1]->Add(hMissMassMulti[2]);
	hMissMassBackground[1]->Scale(0.5);
	hMissMassSubstract[1] = (TH1D*)hMissMassMulti[0]->Clone("MissMassSubstractRand1");
	hMissMassSubstract[1]->SetTitle("MissMassSubstractRand1");
	hMissMassSubstract[1]->Add(hMissMassBackground[1], -1);
	
	hMissMassBackground[2] = (TH1D*)hMissMassAll[1]->Clone("MissMassBackgroundRand2");
	hMissMassBackground[2]->SetTitle("MissMassBackgroundRand2");
	hMissMassBackground[2]->Add(hMissMassAll[2]);
	hMissMassBackground[2]->Scale(0.5);
	hMissMassSubstract[2] = (TH1D*)hMissMassAll[0]->Clone("MissMassSubstractRand2");
	hMissMassSubstract[2]->SetTitle("MissMassSubstractRand2");
	hMissMassSubstract[2]->Add(hMissMassBackground[2], -1);
}
void	AnalysisTagger::Save()
{
	outFile->cd();
		
	hCountTaggerWindow->Write();
	hMissMass[0]->Write();
	hMissMass[1]->Write();
	hMissMass[2]->Write();
	hMissMass[3]->Write();
	hCountTaggerWindowMulti->Write();
	hMissMassMulti[0]->Write();
	hMissMassMulti[1]->Write();
	hMissMassMulti[2]->Write();
	
	CalcHistograms();
	
	hMissMassBackground[0]->Write();
	hMissMassSubstract[0]->Write();
	hMissMassBackground[1]->Write();
	hMissMassSubstract[1]->Write();
	hMissMassAll[0]->Write();
	hMissMassAll[1]->Write();
	hMissMassAll[2]->Write();
	hMissMassBackground[2]->Write();
	hMissMassSubstract[2]->Write();
}
void	AnalysisTagger::Save(const Char_t* outputFileName)
{
	if(OpenOutputFile(outputFileName))
	{
		ReadRootTree::Save();
		Save();
		
		delete outFile;
	}
}
