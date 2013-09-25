#ifndef	_CutWindows_h__
#define	_CutWindows_h__


#include "Cut1Value.h"


class	AnalysisTagger;



class CutWindows
{
private:

	//general
	Char_t	 	name[128];
	
	//histograms
		
	TH1D*		hValue;
	TH1D*		hValueCut[3];
	TH1I*		hCountSingle;
	TH1I*		hCountMulti;
	TH1I*		hCountN[3];
	
	//cuts
	Double_t	cut[6];

protected:

public:
	
	CutWindows(const Char_t* Name, const Double_t promptMin, const Double_t promptMax);
	~CutWindows();
	
	const	Double_t*	GetCut()			const		{return cut;}
	const	Double_t	GetCutPromptMin()	const		{return cut[0];}
	const	Double_t	GetCutPromptMax()	const		{return cut[1];}
	const	Double_t	GetCutRand1Min()	const		{return cut[2];}
	const	Double_t	GetCutRand1Max()	const		{return cut[3];}
	const	Double_t	GetCutRand2Min()	const		{return cut[4];}
	const	Double_t	GetCutRand2Max()	const		{return cut[5];}
	
	void	SetCutPrompt(const Double_t min, const Double_t max)			{cut[0] = min; cut[1] = max;}
	void	SetCutRand1(const Double_t min, const Double_t max)				{cut[2] = min; cut[3] = max;}
	void	SetCutRand2(const Double_t min, const Double_t max)				{cut[4] = min; cut[5] = max;}
	void	SetCut(const Double_t promptMin, const Double_t promptMax, const Double_t rand1Min, const Double_t rand1Max, const Double_t rand2Min, const Double_t rand2Max) {SetCutPrompt(promptMin, promptMax); SetCutRand1(rand1Min, rand1Max); SetCutRand2(rand2Min, rand2Max);}
	
	
	bool	Analyse(AnalysisTagger* analysis);
	
	void	Clear();
	
	void	Draw(TCanvas* canvas, const int posValue, const int posValuePromptCut, const int posValueRand1Cut, const int posValueRand2Cut, const int posCountSingle, const int posCountMulti, const int posCountNPrompt, const int posCountNRand1, const int posCountNRand2);
	void	Save();
};


#endif
