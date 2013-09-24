#ifndef	_AnalysisEtaP6Gamma_h__
#define	_AnalysisEtaP6Gamma_h__



#include "AnalysisDecay.h"
#include "Cut.h"
#include "AnalysisEtaP6GammaCanvas.h"


class AnalysisEtaP6Gamma	: public AnalysisDecay
{
private:
	
	//general	
	static	int		perm[15][6];
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	
	AnalysisEtaP6GammaCanvas*	result[2];				// [Eta, 3Pi0]
	
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
	
	//cuts
	Double_t			cutInvMassPi0[2];
	Double_t			cutInvMassEta[2];
	
	
	void	SetMass(const int index, const Double_t mass);
	void	calcEvent(AnalysisEtaP* analysis);
	void	CalcHistograms();
	bool	CutInvariantMass(AnalysisEtaP* analysis);
	
	
protected:
	
	
public:
	AnalysisEtaP6Gamma(const char* Name, const char* Title);
	~AnalysisEtaP6Gamma();
	
	void	Clear();
	
	bool	Analyse(AnalysisEtaP* analysis);
		
	void	Draw();
	void	Save(TFile* outFile);
	
	void	Fill(AnalysisEtaP6GammaCanvas* can);
	
	virtual	bool		IsEtaP()						{if(bestEta==3) return false; return true;}
	
	const	Double_t*	GetCutInvMassPi0()		const	{return cutInvMassPi0;}
	const	Double_t	GetCutInvMassPi0Min()	const	{return cutInvMassPi0[0];}
	const	Double_t	GetCutInvMassPi0Max()	const	{return cutInvMassPi0[1];}
	const	Double_t*	GetCutInvMassEta()		const	{return cutInvMassEta;}
	const	Double_t	GetCutInvMassEtaMin()	const	{return cutInvMassEta[0];}
	const	Double_t	GetCutInvMassEtaMax()	const	{return cutInvMassEta[1];}
	
	const	Double_t	GetMassPi0a()		const	{return massPi0[0];}
	const	Double_t	GetMassPi0b()		const	{return massPi0[1];}
	const	Double_t	GetMassPi0cEta()	const	{return massPi0Eta;}
	const	Double_t	GetMassAll()		const	{return massAll;}
	const	Double_t	GetMassSet()		const	{return massSet;}
	
	void	SetCutInvMassPi0(const Double_t min, const Double_t max)	{cutInvMassPi0[0] = min; cutInvMassPi0[1] = max;}
	void	SetCutInvMassEta(const Double_t min, const Double_t max)	{cutInvMassEta[0] = min; cutInvMassEta[1] = max;}
};





#endif
