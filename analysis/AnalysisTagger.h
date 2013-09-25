#ifndef	_AnalysisTagger_h__
#define	_AnalysisTagger_h__

#include "AnalysisEtaP.h"
#include "CutWindows.h"
#include "AnalysisEtaP6Gamma.h"


#define MASS_PROTON	938.27203



class AnalysisTagger	: public AnalysisEtaP
{
private:

	//general
	TCanvas*		canvas2Gamma;					
	TCanvas*		canvas6Gamma[2];			// [Eta, 3Pi0]
	TCanvas*		cutWinCanvas2Gamma;					
	TCanvas*		cutWinCanvas6Gamma[2];		// [Eta, 3Pi0]								
	
	//histograms
	ReadRootTreeHist*			cutWinRaw2Gamma[4];				// [Prompt, Rand1, Rand2, (All)]
	TH1D*						cutWinInvMass2Gamma[4];
	ReadRootTreeHist*			cutWinRaw6Gamma[2][4];			// [Eta, 3Pi0][Prompt, Rand1, Rand2, (All)]
	AnalysisEtaP6GammaCanvas*	cutWinAnalysis6Gamma[2][4];		// [Eta, 3Pi0][Prompt, Rand1, Rand2, (All)]
	
	//cuts
	CutWindows*		cutWindows2Gamma;
	CutWindows*		cutWindows6Gamma[2];		// [Eta, 3Pi0]
	Cut1Value*		cutMissMass2Gamma;
	Cut1Value*		cutMissMass6Gamma[2];		// [Eta, 3Pi0]
	
protected:
	
	//variables
	Double_t			beamEnergy[3][32];
	Double_t			beamTime[3][32];
	TLorentzVector		beam[3][32];
	Double_t			missMass[3][32];
	Int_t				nBeam[3];
	TLorentzVector		beamCut[3][16];
	Double_t			missMassCut[3][16];
	Int_t				nBeamCut[3];
	
	
	void	FillTaggedWindows(const int window, ReadRootTreeHist* h)	{h->Fill(nBeam[window], beamTime[window], beamEnergy[window], GetNCBHits(), GetCBTime(), vecAll.E());}
			
	void	SetBeam(const int window, const int index);
	//void	CutMissMass();
	bool	AnalyseEvent(const int index);				// no index checking
	void	Save();
	
	
public:
			AnalysisTagger(const char* _treeFileName, const char* _treeName);
	virtual	~AnalysisTagger();
	
	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
	
	//const	Double_t*	GetCut()			const	{return cutWindows->GetCut();}
	//const	Double_t	GetCutPromptMin()	const	{return cutWindows->GetCutPromptMin();}
	//const	Double_t	GetCutPromptMax()	const	{return cutWindows->GetCutPromptMax();}
	//const	Double_t	GetCutRand1Min()	const	{return cutWindows->GetCutRand1Min();}
	//const	Double_t	GetCutRand1Max()	const	{return cutWindows->GetCutRand1Max();}
	//const	Double_t	GetCutRand2Min()	const	{return cutWindows->GetCutRand2Min();}
	//const	Double_t	GetCutRand2Max()	const	{return cutWindows->GetCutRand2Max();}
	//const	Double_t*	GetCutMissMass()			const	{return cutMissMass;}
	//const	Double_t	GetCutMissMassMin()			const	{return cutMissMass[0];}
	//const	Double_t	GetCutMissMassMax()			const	{return cutMissMass[1];}
	
	void	SetCutPrompt(const Double_t min, const Double_t max)	{cutWindows2Gamma->SetCutPrompt(min, max); cutWindows6Gamma[0]->SetCutPrompt(min, max); cutWindows6Gamma[1]->SetCutPrompt(min, max);}
	void	SetCutRand1(const Double_t min, const Double_t max)		{cutWindows2Gamma->SetCutRand1(min, max); cutWindows6Gamma[0]->SetCutRand1(min, max); cutWindows6Gamma[1]->SetCutRand1(min, max);}
	void	SetCutRand2(const Double_t min, const Double_t max)		{cutWindows2Gamma->SetCutRand2(min, max); cutWindows6Gamma[0]->SetCutRand2(min, max); cutWindows6Gamma[1]->SetCutRand2(min, max);}
	void	SetCut(const Double_t promptMin, const Double_t promptMax, const Double_t rand1Min, const Double_t rand1Max, const Double_t rand2Min, const Double_t rand2Max)	{SetCutPrompt(promptMin, promptMax); SetCutRand1(rand1Min, rand1Max); SetCutRand2(rand2Min, rand2Max);}
	//void	SetCutMissMass(const Double_t Min, const Double_t Max)	{cutMissMass[0]=Min; cutMissMass[1]=Max;}
		
	friend	class	CutWindows;
};




#endif
