#include "Cut1Value.h"

Cut1Value::Cut1Value(const Char_t* Name, const Double_t xBins, const Double_t xMin, const Double_t xMax)	
{
	strcpy(name, Name);
	
	Char_t	helpName[128];
	
	sprintf(helpName, "%s_Cut", Name);
	if(!(hValue		= (TH1D*)gROOT->Get(helpName)))
		hValue		= new TH1D(helpName, helpName, xBins, xMin, xMax);
	sprintf(helpName, "%s_CutCheck", Name);
	if(!(hValueCut	= (TH1D*)gROOT->Get(helpName)))
		hValueCut	= new TH1D(helpName, helpName, xBins, xMin, xMax);
	sprintf(helpName, "%s_CutCount", Name);
	if(!(hCount		= (TH1I*)gROOT->Get(helpName)))
		hCount		= new TH1I(helpName, "1:All / 2:Passed", 4, 0, 4);
		
	Clear();
	
	cut[0]	=	-1000000000;
	cut[1]	=	1000000000;
}
Cut1Value::~Cut1Value()
{
}


bool	Cut1Value::Analyse(const Double_t val)
{
	hCount->Fill(1);
	hValue->Fill(val);
	if(val<cut[0] || val>cut[1])
		return false;
	hCount->Fill(2);
	hValueCut->Fill(val);
	return true;	
}
bool	Cut1Value::Analyse(const Double_t lengthArray, const Double_t* valArray)
{
	hCount->Fill(1);
	for(int i=0; i<lengthArray; i++)
		hValue->Fill(valArray[i]);
	
	for(int i=0; i<lengthArray; i++)
	{
		if(valArray[i]<cut[0] || valArray[i]>cut[1])
			return false;
		hValueCut->Fill(valArray[i]);
	}
	hCount->Fill(2);
	return true;	
}
	

void	Cut1Value::Draw(TCanvas* canvas, const int posValue, const int posValueCut, const int posCount)
{
	canvas->cd(posValue);	hValue->Draw();
	canvas->cd(posValueCut);	hValueCut->Draw();
	canvas->cd(posCount);	hCount->Draw();
}
void	Cut1Value::Save()
{
	hValue->Write();
	hValueCut->Write();
	hCount->Write();
}
