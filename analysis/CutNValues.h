#ifndef	_CutNValues_h__
#define	_CutNValues_h__


#include "Cut1Value.h"


class CutNValues
{
private:

	//general
	Char_t	 	name[128];
	int 		nValues;
	
	//cuts
	Cut1Value**	cut;

protected:

public:
	
	CutNValues(const Char_t* Name, const int NValues, const Double_t xBins, const Double_t xMin, const Double_t xMax);
	~CutNValues();
	
	const	Double_t*	GetCut()	const		{return cut[0]->GetCut();}
	const	Double_t	GetCutMin()	const		{return cut[0]->GetCutMin();}
	const	Double_t	GetCutMax()	const		{return cut[0]->GetCutMax();}
	
	void	SetCut(const Double_t min, const Double_t max);
	
	
	bool	Analyse(const Double_t val1, const Double_t val2)						{if(!cut[0]->Analyse(val1)) return false; return cut[1]->Analyse(val2);}
	bool	Analyse(const Double_t val1, const Double_t val2, const Double_t val3)	{if(!cut[0]->Analyse(val1)) return false; if(!cut[1]->Analyse(val2)) return false; return cut[2]->Analyse(val3);}
	bool	Analyse(const Double_t* val);
	
	void	Clear()			{for(int i=0; i<nValues; i++) cut[i]->Clear();}
	
	void	Draw(TCanvas* canvas, const int posValue1, const int posValue2, const int posValue1Cut, const int posValue2Cut, const int posCount1, const int posCount2);
	void	Draw(TCanvas* canvas, const int posValue1, const int posValue2, const int posValue3, const int posValue1Cut, const int posValue2Cut, const int posValue3Cut, const int posCount1, const int posCount2, const int posCount3);
	void	Draw(TCanvas* canvas, const int* posValue, const int* posValueCut, const int* posCount);
	void	Save();
};

inline	bool	CutNValues::Analyse(const Double_t* val)
{
	for(int i=0; i<(nValues-1); i++) 
	{
		if(!cut[i]->Analyse(val[i])) 
			return false;
	}
	return cut[nValues-1]->Analyse(val[nValues-1]);
}

#endif
