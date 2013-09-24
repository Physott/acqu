#include "CutNValues.h"

CutNValues::CutNValues(const Char_t* Name, const int NValues, const Double_t xBins, const Double_t xMin, const Double_t xMax)	: nValues(NValues)
{
	strcpy(name, Name);
	
	cut	= new Cut1Value*[nValues];
	
	
	Char_t	str[64];
	for(int i=0; i<nValues; i++)
	{
		sprintf(str, "%s_Cut%d", name, i);
		cut[i]	= new Cut1Value(str, xBins, xMin, xMax);
	}
		
	Clear();	
}
CutNValues::~CutNValues()
{
}	

void	CutNValues::SetCut(const Double_t min, const Double_t max)
{
	for(int i=0; i<nValues; i++)
		cut[i]->SetCut(min, max);
}
	
void	CutNValues::Draw(TCanvas* canvas, const int posValue1, const int posValue2, const int posValue1Cut, const int posValue2Cut, const int posCount1, const int posCount2)
{
	cut[0]->Draw(canvas, posValue1, posValue1Cut, posCount1);
	cut[1]->Draw(canvas, posValue2, posValue2Cut, posCount2);
}
void	CutNValues::Draw(TCanvas* canvas, const int posValue1, const int posValue2, const int posValue3, const int posValue1Cut, const int posValue2Cut, const int posValue3Cut, const int posCount1, const int posCount2, const int posCount3)
{
	cut[0]->Draw(canvas, posValue1, posValue1Cut, posCount1);
	cut[1]->Draw(canvas, posValue2, posValue2Cut, posCount2);
	cut[2]->Draw(canvas, posValue3, posValue3Cut, posCount3);
}
void	CutNValues::Draw(TCanvas* canvas, const int* posValue, const int* posValueCut, const int* posCount)
{
	for(int i=0; i<nValues; i++)
		cut[i]->Draw(canvas, posValue[i], posValueCut[i], posCount[i]);
}

void	CutNValues::Save()
{
	for(int i=0; i<nValues; i++)
		cut[i]->Save();
}

