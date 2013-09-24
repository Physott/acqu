#ifndef	_AnalysisEtaP6GammaCanvas_h__
#define	_AnalysisEtaP6GammaCanvas_h__



#include <stdlib.h>
#include <stdio.h>

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1D.h"


class	AnalysisEtaP6GammaCanvas
{
private:
	//TCanvas*		canvas;
	Char_t			name[64];
	bool			isEta2Pi0;
	
	TH1D*			hInvMassPi0a[2];
	TH1D*			hInvMassPi0b[2];
	TH1D*			hInvMassPi0cEta[2];
	TH1D*			hInvMassAll;
	TH1D*			hInvMassAllSet;
	
public:
	AnalysisEtaP6GammaCanvas(const Char_t* Name, const bool Eta2Pi0);
	~AnalysisEtaP6GammaCanvas();
	
	void	Clear()																													{hInvMassPi0a[0]->Reset("M"); hInvMassPi0b[0]->Reset("M"); hInvMassPi0cEta[0]->Reset("M"); hInvMassPi0a[1]->Reset("M"); hInvMassPi0b[1]->Reset("M"); hInvMassPi0cEta[1]->Reset("M"); hInvMassAll->Reset("M"); hInvMassAllSet->Reset("M");}
	void	Fill(const Double_t invMassPi0a, const Double_t invMassPi0b, const Double_t invMassPi0cEta, const Double_t invMassAll, const Double_t invMassAllSet);
	void	Draw(TCanvas* canvas, const int posInvMassPi0a, const int posInvMassPi0b, const int posInvMassPi0cEta, const int posInvMassPi0aDiced, const int posInvMassPi0bDiced, const int posInvMassPi0cEtaDiced, const int posInvMassAll, const int posInvMassAllSet);
	void	Save();
};






#endif
