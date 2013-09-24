#ifndef	_ReadRootTreeHist_h__
#define	_ReadRootTreeHist_h__


#include "TROOT.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TCanvas.h"



class ReadRootTreeHist
{
private:
	//general
	Char_t	 	name[128];
	
	//histograms
	TH1I*		hNTagged;
	TH1D*		hTaggedTime;
	TH1D*		hTaggedEnergy;
	TH1I*		hNCBHits;
	TH1D*		hCBTime;
	TH1D*		hCBEnergyAll;
	
public:

	ReadRootTreeHist(const Char_t* Name);
	~ReadRootTreeHist();
	
	void	Clear();

	void	Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy);
	void	Fill(const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll);
	void	Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy, const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll);
	
	void	Draw(TCanvas* canvas, const int posNTagged = 1, const int posTaggedTime = 2, const int posTaggedEnergy = 3, const int posNCBHits = 4, const int posCBTime = 5, const int posCBEnergyAll = 6);
	void	Save();
};




#endif
