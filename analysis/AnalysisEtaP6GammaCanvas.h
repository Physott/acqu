
#include <stdlib.h>
#include <stdio.h>

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1D.h"


class	AnalysisEtaP6GammaCanvas
{
private:
	TCanvas*		canvas;
	Char_t			name[64];
	
	TH1D*			hInvMassPi0a;
	TH1D*			hInvMassPi0b;
	TH1D*			hInvMassPi0cEta;
	TH1D*			hInvMassAll;
	TH1D*			hInvMassAllSet;
	
public:
	AnalysisEtaP6GammaCanvas(const Char_t* Name);
	~AnalysisEtaP6GammaCanvas();
	
	void	Clear()																																					{hInvMassPi0a->Reset("M"); hInvMassPi0b->Reset("M"); hInvMassPi0cEta->Reset("M"); hInvMassAll->Reset("M");}
	//void	Fill(const Double_t invMassPi0a, const Double_t invMassPi0b, const Double_t invMassPi0cEta, const Double_t invMassAll)									{hInvMassPi0a->Fill(invMassPi0a); hInvMassPi0b->Fill(invMassPi0b); hInvMassPi0cEta->Fill(invMassPi0cEta); hInvMassAll->Fill(invMassAll);}
	void	Fill(const Double_t invMassPi0a, const Double_t invMassPi0b, const Double_t invMassPi0cEta, const Double_t invMassAll, const Double_t invMassAllSet)	{hInvMassPi0a->Fill(invMassPi0a); hInvMassPi0b->Fill(invMassPi0b); hInvMassPi0cEta->Fill(invMassPi0cEta); hInvMassAll->Fill(invMassAll); hInvMassAllSet->Fill(invMassAllSet);}
	void	Draw();
	void	Save();
};
