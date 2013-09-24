#ifndef	_AnalysisEtaP6Gamma_h__
#define	_AnalysisEtaP6Gamma_h__



//#include "Cut.h"
#include "AnalysisEtaP.h"
#include "AnalysisEtaP6GammaCanvas.h"
#include "CutNValues.h"

class AnalysisEtaP6Gamma
{
private:
	
	//general	
	TCanvas*		canvas[3];							// [All, Eta, 3Pi0]
	TCanvas*		cutCanvas[2];						// [ Eta, 3Pi0]
	static	int		perm[15][6];
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	
	AnalysisEtaP6GammaCanvas*	result[2];				// [Eta, 3Pi0]
	ReadRootTreeHist*			rawHist[3];				// [All, Eta, 3Pi0]
	AnalysisEtaP6GammaCanvas*	cutResult[2];				// [Eta, 3Pi0]
	ReadRootTreeHist*			cutRawHist[2];				// [All, Eta, 3Pi0]
	
	
	//buffer
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	Double_t		massPi0[2];
	Double_t		massPi0Eta;
	Double_t		massAll;
	
	//variables
	TLorentzVector		partSet[3];
	TLorentzVector		allSet;
	Double_t			massSet;
	
	//histograms
	TH1I*			hCountDecay;
	
	//cuts
	Cut1Value*		cutEta;
	CutNValues*		cut2Pi0;
	CutNValues*		cut3Pi0;
	
	
	void	SetMass(const int index, const Double_t mass);
	void	calcEvent(AnalysisEtaP* analysis);
	void	CalcHistograms();
	void	DrawCut();
	
	
protected:
	
	
public:
	AnalysisEtaP6Gamma();
	~AnalysisEtaP6Gamma();
	
	void	Clear();
	
	bool	Analyse(AnalysisEtaP* analysis);
		
	void	Draw();
	void	Save(TFile* outFile, const Char_t* nameParent);
	
	void	Fill(AnalysisEtaP6GammaCanvas* can);
	
	virtual	bool		IsEtaP()						{if(bestEta==3) return false; return true;}
	
	const	Double_t*	GetCutInvMassPi0()		const	{return cut2Pi0->GetCut();}
	const	Double_t	GetCutInvMassPi0Min()	const	{return cut2Pi0->GetCutMin();}
	const	Double_t	GetCutInvMassPi0Max()	const	{return cut2Pi0->GetCutMax();}
	const	Double_t*	GetCutInvMassEta()		const	{return cutEta->GetCut();}
	const	Double_t	GetCutInvMassEtaMin()	const	{return cutEta->GetCutMin();}
	const	Double_t	GetCutInvMassEtaMax()	const	{return cutEta->GetCutMax();}
	
	const	Double_t	GetMassPi0a()		const	{return massPi0[0];}
	const	Double_t	GetMassPi0b()		const	{return massPi0[1];}
	const	Double_t	GetMassPi0cEta()	const	{return massPi0Eta;}
	const	Double_t	GetMassAll()		const	{return massAll;}
	const	Double_t	GetMassSet()		const	{return massSet;}
	
	void	SetCutInvMassPi0(const Double_t min, const Double_t max)	{cut2Pi0->SetCut(min, max); cut3Pi0->SetCut(min, max);}
	void	SetCutInvMassEta(const Double_t min, const Double_t max)	{cutEta->SetCut(min, max);}
};





#endif
