#ifndef	_AnalysisEtaP6GammaTaggedCanvas_h__
#define	_AnalysisEtaP6GammaTaggedCanvas_h__



#include <stdlib.h>
#include <stdio.h>

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1D.h"

#include "AnalysisEtaP6Gamma.h"

class	AnalysisEtaP6GammaTaggedCanvas
{
private:
	TCanvas*		canvas;
	Char_t			name[64];
	
	TH1D*			hInvMassPi0a[5];
	TH1D*			hInvMassPi0b[5];
	TH1D*			hInvMassPi0cEta[5];
	TH1D*			hInvMassAll[5];
	TH1D*			hInvMassAllSet[5];
	TH1D*			hMissingMass[5];
	
	void	SubstractBackground();
	
	
public:
	AnalysisEtaP6GammaTaggedCanvas(const Char_t* Name);
	~AnalysisEtaP6GammaTaggedCanvas();
	
	void	Clear();
	void	Fill(const Double_t IMPi0a, const Double_t IMPi0b, const Double_t IMPi0cEta, const Double_t IMAll, const Double_t IMSet, const Double_t missMass, const int TaggerWindow, const int Hits = 1);
	void	Draw();
	void	Save();
};






#endif
