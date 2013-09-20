#ifndef	_AnalysisEtaP2GammaTaggedCanvas_h__
#define	_AnalysisEtaP2GammaTaggedCanvas_h__



#include <stdlib.h>
#include <stdio.h>

#include "TROOT.h"
#include "TCanvas.h"
#include "TH1D.h"


class	AnalysisEtaP2GammaTaggedCanvas
{
private:
	TCanvas*		canvas;
	Char_t			name[64];
	
	TH1D*			hInvMass[5];
	TH1D*			hMissingMass[5];
	
	void	SubstractBackground();
	
public:
	AnalysisEtaP2GammaTaggedCanvas(const Char_t* Name);
	~AnalysisEtaP2GammaTaggedCanvas();
	
	void	Clear();
	void	Fill(const Double_t invMass, const Double_t missMass, const int TaggerWindow, const int Hits = 1);
	void	Draw();
	void	Save();
};






#endif
