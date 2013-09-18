

#include "AnalysisTagger.h"
#include "AnalysisEtaP6GammaCanvas.h"

#define		MASS_PI0	134.9766
#define		MASS_ETA	547.51
#define		MASS_ETAP	957.78


class AnalysisEtaP6Gamma	: public AnalysisTagger
{
private:
	
	//general
	static	int		perm[15][6];
	
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	
	AnalysisEtaP6GammaCanvas*	canvasAll[2];				// [Eta, 3Pi0]
	AnalysisEtaP6GammaCanvas*	canvasUntagged[2];			// [Eta, 3Pi0]
	AnalysisEtaP6GammaCanvas*	canvasTagged[2][4];			// [Eta, 3Pi0][promtp, rand1, rand2, singlePrompt]
	AnalysisEtaP6GammaCanvas*	canvasTaggedMulti[2][3];	// [Eta, 3Pi0][promtp, rand1, rand2]
	
	//buffer
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	Double_t		massAll;
	
	//counters
	Int_t		countDecaysAll[3];				// [Eta, 3Pi0, checked]
	Int_t		countDecaysUntagged[3];			// [Eta, 3Pi0, checked]
	Int_t		countDecaysTagged[2][4];		// [Eta, 3Pi0][promtp, rand1, rand2, singlePrompt]
	Int_t		countDecaysTaggedMulti[2][3];	// [Eta, 3Pi0][promtp, rand1, rand2]
	
	//variables
	TLorentzVector		partSet[3];
	TLorentzVector		allSet;
	Double_t			massSet;
	
	void	SetMass(const int index, const Double_t mass);
	void	calcEvent();
	void	Reconstruct();
	
	
protected:
	
	bool	AnalyseEvent(const int index);
	void	Save();
	
	
public:
			AnalysisEtaP6Gamma(const char* _treeFileName, const char* _treeName);
	virtual	~AnalysisEtaP6Gamma();
	
	virtual	void	Clear();
	
	virtual	void	Analyse(const int min = 0, const int max = -1);
		
	virtual	void	PrintCounters();
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
	
	
	bool		is3Pi0()		{if(bestEta==3) return true; return false;}
	bool		isEta2Pi0()		{if(bestEta==3) return false; return true;}
};
