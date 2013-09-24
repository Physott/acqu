#ifndef	_Cut_h__
#define	_Cut_h__


#include <stdlib.h>

#include "TROOT.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TCanvas.h"


class Cut
{
private:

	//general
	Char_t	 	name[128];
	Char_t	 	title[512];
	TCanvas*	canvas;
	
	//histograms
		
	TH1D*		hValue;
	TH1D*		hValueCut;
	TH1I*		hCount;
	
	//cuts
	Double_t	cut[2];

protected:

public:
	
	Cut(const Char_t* Name, const Double_t xBins, const Double_t xMin, const Double_t xMax, const Char_t* Title = "");
	~Cut();
	
	const	Double_t*	GetCut()	const		{return cut;}
	const	Double_t	GetCutMin()	const		{return cut[0];}
	const	Double_t	GetCutMax()	const		{return cut[1];}
	
	void	SetCut(const Double_t min, const Double_t max)	{cut[0] = min; cut[1] = max;}
	
	bool	Analyse(const Double_t val);
	bool	Analyse(const Double_t lengthArray, const Double_t* valArray);
	
	void	Clear()			{hValue->Reset("M"); hValueCut->Reset("M"); hCount->Reset("M");}
	
	void	Draw();
	void	Save();
};



#endif
