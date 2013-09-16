#include "AnalysisTagger.h"




AnalysisTagger::AnalysisTagger(const char* _treeFileName, const char* _treeName)	: ReadRootTree(_treeFileName, _treeName)
{
	//printf("AnalysisTagger created. Parameters %s %s\n", _treeFileName, _treeName);
	
	for(int i=0; i<3; i++)
	{
		cutTaggerTime[2*i]		= -100000;
		cutTaggerTime[(2*i)+1]	= 100000;
	}
	
	hTaggerTimeWindow		= new TH1I("TaggerTimeWindow", "TaggerTimeWindow", 5, 0, 5);
	hTaggerTimeWindowMulti	= new TH1I("TaggerTimeWindowMulti", "TaggerTimeWindowMulti", 3, 0, 3);
	
	Clear();
}
AnalysisTagger::~AnalysisTagger()
{
}


void	AnalysisTagger::Clear()
{
	ReadRootTree::Clear();
	
	hTaggerTimeWindow->Reset("M");
	hTaggerTimeWindowMulti->Reset("M");
	
	for(int i=0; i<3; i++)
	{
		countTaggerWindow[i]		= 0;
		countTaggerWindowMulti[i]	= 0;
	}
	countTaggerWindow[3]		= 0;
	countTaggerWindow[4]		= 0;
}

bool	AnalysisTagger::AnalyseEvent(const int index)
{	
	if(ReadRootTree::AnalyseEvent(index))
	{		
		countTaggerWindow[4]++;
		hTaggerTimeWindow->Fill(4);
		nBeam[0]	= 0;
		nBeam[1]	= 0;
		nBeam[2]	= 0;
		untagged	= false;
		
		for(int i=0; i<GetNTagged(); i++)
		{
			if(GetTaggedTime(i)>cutTaggerTime[0] && GetTaggedTime(i)<cutTaggerTime[1])
			{
				beam[0][nBeam[0]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i));
				nBeam[0]++;
			}
			if(GetTaggedTime(i)>cutTaggerTime[2] && GetTaggedTime(i)<cutTaggerTime[3])
			{
				beam[1][nBeam[1]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i));
				nBeam[1]++;
			}
			if(GetTaggedTime(i)>cutTaggerTime[4] && GetTaggedTime(i)<cutTaggerTime[5])
			{
				beam[2][nBeam[2]].SetPxPyPzE(GetTaggedEnergy(i), 0.0, 0.0, GetTaggedEnergy(i));
				nBeam[2]++;
			}
		}
		if(nBeam[0]>0 || nBeam[1]>0 || nBeam[2]>0)
		{
			if(nBeam[0]>1)
			{
				countTaggerWindowMulti[0]++;
				hTaggerTimeWindowMulti->Fill(0);
			}
			else if(nBeam[0] == 1)
			{
				countTaggerWindow[0]++;
				hTaggerTimeWindow->Fill(0);
			}
			
			if(nBeam[1]>1)
			{
				countTaggerWindowMulti[1]++;
				hTaggerTimeWindowMulti->Fill(1);
			}
			else if(nBeam[1] == 1)
			{
				countTaggerWindow[1]++;
				hTaggerTimeWindow->Fill(1);
			}
				
			if(nBeam[2]>1)
			{
				countTaggerWindowMulti[2]++;
				hTaggerTimeWindowMulti->Fill(2);
			}
			else if(nBeam[2] == 1)
			{
				countTaggerWindow[2]++;
				hTaggerTimeWindow->Fill(2);
			}
			
			if((nBeam[0]	== 1) && (nBeam[1]	== 0) && (nBeam[2]	== 0))
			{
				countTaggerWindow[3]++;
				hTaggerTimeWindow->Fill(3);
				uniqueWindow	= true;
			}
			else if((nBeam[0]	== 0) && (nBeam[1]	== 1) && (nBeam[2]	== 0))
				uniqueWindow	= true;
			else if((nBeam[0]	== 0) && (nBeam[1]	== 0) && (nBeam[2]	== 1))
				uniqueWindow	= true;
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
	
	canvas->Divide(2, 1, 0.001, 0.001);
	
	canvas->cd(1);	hTaggerTimeWindow->Draw();
	canvas->cd(2);	hTaggerTimeWindowMulti->Draw();
}
