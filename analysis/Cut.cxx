#include "Cut.h"

Cut::Cut(const Char_t* Name, const Double_t xBins, const Double_t xMin, const Double_t xMax, const Char_t* Title)	: canvas(0)
{
	strcpy(name, Name);
	strcpy(title, Title);
	
	Char_t	helpName[128];
	
	sprintf(helpName, "%s", Name);
	if(!(hValue		= (TH1D*)gROOT->Get(helpName)))
		hValue		= new TH1D(helpName, helpName, xBins, xMin, xMax);
	sprintf(helpName, "%s_checkCut", Name);
	if(!(hValueCut	= (TH1D*)gROOT->Get(helpName)))
		hValueCut	= new TH1D(helpName, helpName, xBins, xMin, xMax);
	sprintf(helpName, "%s_count", Name);
	if(!(hCount		= (TH1I*)gROOT->Get(helpName)))
		hCount		= new TH1I(helpName, "1:All / 2:Passed", 4, 0, 4);
		
	Clear();
	
	cut[0]	=	-1000000000;
	cut[1]	=	1000000000;
}
Cut::~Cut()
{
	if(canvas)
		delete	canvas;
}


bool	Cut::Analyse(const Double_t val)
{
	hCount->Fill(1);
	hValue->Fill(val);
	if(val<cut[0] || val>cut[1])
		return false;
	hCount->Fill(2);
	hValueCut->Fill(val);
	return true;	
}
bool	Cut::Analyse(const Double_t lengthArray, const Double_t* valArray)
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
	

void	Cut::Draw()
{
	Char_t	helpName[128];
	
	sprintf(helpName, "Cut_%s", name);
	if(!(canvas	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject(helpName)))
		canvas	= new TCanvas(helpName, title, 50, 50, 1600, 800);
	canvas->Clear();
		
	canvas->Divide(3, 1, 0.001, 0.001);
	
	canvas->cd(1);	hValue->Draw();
	canvas->cd(2);	hValueCut->Draw();
	canvas->cd(3);	hCount->Draw();
}
void	Cut::Save()
{
	hValue->Write();
	hValueCut->Write();
	hCount->Write();
}
