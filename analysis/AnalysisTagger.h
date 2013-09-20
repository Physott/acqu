#ifndef	_AnalysisTagger_h__
#define	_AnalysisTagger_h__

#include "AnalysisEtaP.h"
#include "AnalysisEtaP6GammaTaggedCanvas.h"


#define MASS_PROTON	938.27203



class AnalysisTagger	: public AnalysisEtaP
{
private:

	//general
	TCanvas*	canvas;
	
	//histograms
	TH1D*				hMissMass;
	TH1D*				hCheckCutMissMass;
	TH1I*				hCountWindow;
	TH1I*				hCountWindowAccumulated;
	TH1I*				hCountWindowN[3];
	TH1I*				hCountWindowCut;
	TH1I*				hCountWindowAccumulatedCut;
	TH1I*				hCountWindowNCut[3];
	
	//cuts
	Double_t			cutTaggerTime[6];
	Double_t			cutMissMass[2];
	
protected:
	
	//variables
	TLorentzVector		beam[3][32];
	Double_t			missMass[3][32];
	Int_t				nBeam[3];
	TLorentzVector		beamCut[3][16];
	Double_t			missMassCut[3][16];
	Int_t				nBeamCut[3];
	
	void	AnalyseTagged();
	void	CutMissMass();
	bool	AnalyseEvent(const int index);				// no index checking
	void	Save();
	
	
public:
			AnalysisTagger(const char* _treeFileName, const char* _treeName);
	virtual	~AnalysisTagger();
	
	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
	
	const	Double_t*	GetCutTaggerTimes()	const	{return cutTaggerTime;}
	const	Double_t	GetCutTaggerTimePromptMin()	const	{return cutTaggerTime[0];}
	const	Double_t	GetCutTaggerTimePromptMax()	const	{return cutTaggerTime[1];}
	const	Double_t	GetCutTaggerTimeRand1Min()	const	{return cutTaggerTime[2];}
	const	Double_t	GetCutTaggerTimeRand1Max()	const	{return cutTaggerTime[3];}
	const	Double_t	GetCutTaggerTimeRand2Min()	const	{return cutTaggerTime[4];}
	const	Double_t	GetCutTaggerTimeRand2Max()	const	{return cutTaggerTime[5];}
	const	Double_t*	GetCutMissMass()			const	{return cutMissMass;}
	const	Double_t	GetCutMissMassMin()			const	{return cutMissMass[0];}
	const	Double_t	GetCutMissMassMax()			const	{return cutMissMass[1];}
	
	void	SetCutTaggerTime(const Double_t promptMin, const Double_t promptMax, const Double_t rand1Min, const Double_t rand1Max, const Double_t rand2Min, const Double_t rand2Max)	{cutTaggerTime[0]=promptMin; cutTaggerTime[1]=promptMax; cutTaggerTime[2]=rand1Min; cutTaggerTime[3]=rand1Max; cutTaggerTime[4]=rand2Min; cutTaggerTime[5]=rand2Max;}
	void	SetCutMissMass(const Double_t Min, const Double_t Max)	{cutMissMass[0]=Min; cutMissMass[1]=Max;}
		
};




#endif
