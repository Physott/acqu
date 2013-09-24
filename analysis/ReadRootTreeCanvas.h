#ifndef	_ReadRootTreeCanvas_h__
#define	_ReadRootTreeCanvas_h__


#include <stdlib.h>

#include "TROOT.h"
#include "TObject.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TCanvas.h"



class ReadRootTreeCanvas	: public TObject
{
private:
	//general
	Char_t	 	name[128];
	Char_t	 	title[512];
	TCanvas*	canvas;
	
	//histograms
	TH1I*		hNTagged;
	TH1D*		hTaggedTime;
	TH1D*		hTaggedEnergy;
	TH1I*		hNCBHits;
	TH1D*		hCBTime;
	TH1D*		hCBEnergyAll;
	
public:

	ReadRootTreeCanvas(const Char_t* Name, const Char_t* title = "");
	~ReadRootTreeCanvas();
	
	void	Clear();

	void	Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy);
	void	Fill(const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll);
	void	Fill(const Int_t NTagged, const Double_t* TaggedTime, const Double_t* TaggedEnergy, const Int_t NCB, const Double_t* CBTime, const Double_t CBEnergyAll);
	
	void	Draw();
	void	Save();
};




#endif
