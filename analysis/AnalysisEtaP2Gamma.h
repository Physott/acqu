#ifndef	_AnalysisEtaP2Gamma_h__
#define	_AnalysisEtaP2Gamma_h__





#include "AnalysisEtaP.h"


class AnalysisEtaP2Gamma
{
private:
	
	//general	
	TCanvas*			canvas;
	TCanvas*			cutCanvas;
	
	//histograms
	TH1D*				hInvMass;
	ReadRootTreeHist*	rawHist;
	
	ReadRootTreeHist*	cutRawHist;
	
	//cuts
	Cut1Value*			cutInvMass;
	
	
protected:
	
	
public:
	AnalysisEtaP2Gamma();
	~AnalysisEtaP2Gamma();
	
	void	Clear();
	
	bool	Analyse(AnalysisEtaP* analysis);
		
	void	Draw();
	void	Save(TFile* outFile, const Char_t* nameParent);
	
	const Double_t*	GetCutInvMassEtaP()		const	{return cutInvMass->GetCut();}
	const Double_t	GetCutInvMassEtaPMin()	const	{return cutInvMass->GetCutMin();}
	const Double_t	GetCutInvMassEtaPMax()	const	{return cutInvMass->GetCutMax();}
	
	void	SetCutInvMassEtaP(const Double_t min, const Double_t max)	{cutInvMass->SetCut(min, max);}
};



#endif
