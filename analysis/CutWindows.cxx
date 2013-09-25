#include "CutWindows.h"
#include "AnalysisTagger.h"

CutWindows::CutWindows(const Char_t* Name, const Double_t promptMin, const Double_t promptMax)
{
	strcpy(name, Name);
	
	SetCutPrompt(promptMin, promptMax);
	SetCutRand1(promptMin-(3*(promptMax-promptMin)/2), promptMin-((promptMax-promptMin)/2));
	SetCutRand2(promptMax+((promptMax-promptMin)/2), promptMax+(3*(promptMax-promptMin)/2));
	
	
	Char_t	str[64];
	sprintf(str, "%s_Cut", name);
	if(!(hValue			= (TH1D*)gROOT->Get(str)))
		hValue			= new TH1D(str, str, 5*(promptMax-promptMin), promptMin-(5*(promptMax-promptMin)/2), promptMax+(5*(promptMax-promptMin)/2));
	sprintf(str, "%s_CutCheckPrompt", name);
	if(!(hValueCut[0]	= (TH1D*)gROOT->Get(str)))
		hValueCut[0]	= new TH1D(str, str, 5*(promptMax-promptMin), promptMin-(5*(promptMax-promptMin)/2), promptMax+(5*(promptMax-promptMin)/2));
	sprintf(str, "%s_CutCheckRand1", name);
	if(!(hValueCut[1]	= (TH1D*)gROOT->Get(str)))
		hValueCut[1]	= new TH1D(str, str, 5*(promptMax-promptMin), promptMin-(5*(promptMax-promptMin)/2), promptMax+(5*(promptMax-promptMin)/2));
	sprintf(str, "%s_CutCheckRand2", name);
	if(!(hValueCut[2]	= (TH1D*)gROOT->Get(str)))
		hValueCut[2]	= new TH1D(str, str, 5*(promptMax-promptMin), promptMin-(5*(promptMax-promptMin)/2), promptMax+(5*(promptMax-promptMin)/2));
	sprintf(str, "%s_CutCountSingle", name);
	if(!(hCountSingle	= (TH1I*)gROOT->Get(str)))
		hCountSingle	= new TH1I(str, "1:All / 2:Prompt / 3:Rand1 / 4:Rand2 (Per Event)", 6, 0, 6);
	sprintf(str, "%s_CutCountMulti", name);
	if(!(hCountMulti	= (TH1I*)gROOT->Get(str)))
		hCountMulti		= new TH1I(str, "1:All / 2:Prompt / 3:Rand1 / 4:Rand2 (Multi filled)", 6, 0, 6);
	sprintf(str, "%s_CutCountNPrompt", name);
	if(!(hCountN[0]		= (TH1I*)gROOT->Get(str)))
		hCountN[0]		= new TH1I(str, str, 6, 0, 6);
	sprintf(str, "%s_CutCountNRand1", name);
	if(!(hCountN[1]		= (TH1I*)gROOT->Get(str)))
		hCountN[1]		= new TH1I(str, str, 6, 0, 6);
	sprintf(str, "%s_CutCountNRand2", name);
	if(!(hCountN[2]		= (TH1I*)gROOT->Get(str)))
		hCountN[2]		= new TH1I(str, str, 6, 0, 6);
		
	Clear();	
}
CutWindows::~CutWindows()
{
}	
	
	
void	CutWindows::Clear()
{
	hValue->Reset("M"); 
	hValueCut[0]->Reset("M"); 
	hValueCut[1]->Reset("M"); 
	hValueCut[2]->Reset("M"); 
	hCountSingle->Reset("M");
	hCountMulti->Reset("M");
	hCountN[0]->Reset("M");
	hCountN[1]->Reset("M");
	hCountN[2]->Reset("M");
}

bool	CutWindows::Analyse(AnalysisTagger* analysis)
{
	for(int i=0; i< analysis->GetNTagged(); i++)
	{
		hValue->Fill(analysis->GetTaggedTime(i));
		if(analysis->GetTaggedTime(i)>=cut[0] && analysis->GetTaggedTime(i)<=cut[1])
		{
			hValueCut[0]->Fill(analysis->GetTaggedTime(i));
			analysis->SetBeam(0, i);
		}
		else if(analysis->GetTaggedTime(i)>=cut[2] && analysis->GetTaggedTime(i)<=cut[3])
		{
			hValueCut[1]->Fill(analysis->GetTaggedTime(i));
			analysis->SetBeam(1, i);
		}
		else if(analysis->GetTaggedTime(i)>=cut[4] && analysis->GetTaggedTime(i)<=cut[5])
		{
			hValueCut[2]->Fill(analysis->GetTaggedTime(i));
			analysis->SetBeam(2, i);
		}
	}
	
	hCountSingle->Fill(1);
	hCountMulti->Fill(1);
	for(int i=0; i<3; i++)
	{
		if(analysis->nBeam[i] > 0)
		{
			hCountSingle->Fill(i+2);
			for(int l=0; l< analysis->nBeam[i]; l++)
				hCountMulti->Fill(i+2);
			hCountN[i]->Fill(analysis->nBeam[i]);
		}
	}
	if(analysis->nBeam[0] > 0 || analysis->nBeam[1] > 0 || analysis->nBeam[2] > 0)
		return true;
	return false;
}

	
void	CutWindows::Draw(TCanvas* canvas, const int posValue, const int posValuePromptCut, const int posValueRand1Cut, const int posValueRand2Cut, const int posCountSingle, const int posCountMulti, const int posCountNPrompt, const int posCountNRand1, const int posCountNRand2)
{
	canvas->cd(posValue);	hValue->Draw();
	canvas->cd(posValuePromptCut);	hValueCut[0]->Draw();
	canvas->cd(posValueRand1Cut);	hValueCut[1]->Draw();
	canvas->cd(posValueRand2Cut);	hValueCut[2]->Draw();
	canvas->cd(posCountSingle);		hCountSingle->Draw();
	canvas->cd(posCountMulti);		hCountMulti->Draw();
	canvas->cd(posCountNPrompt);	hCountN[0]->Draw();
	canvas->cd(posCountNRand1);		hCountN[1]->Draw();
	canvas->cd(posCountNRand2);		hCountN[2]->Draw();
	
}

void	CutWindows::Save()
{
	hValue->Write();
	hValueCut[0]->Write();
	hValueCut[1]->Write();
	hValueCut[2]->Write();
	hCountSingle->Write();
	hCountMulti->Write();
	hCountN[0]->Write();
	hCountN[1]->Write();
	hCountN[2]->Write();
}


