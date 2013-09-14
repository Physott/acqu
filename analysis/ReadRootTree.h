#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TH1I.h"
#include "TH1D.h"

#include "Analysis6GammaCanvas.h"

#define		MASS_PI0	134.9766
#define		MASS_ETA	547.51
#define		MASS_ETAP	957.78


class ReadRootTree
{
private:

	TFile*		file;
	TTree*		tree;
	Char_t		treeFileName[256];
	Char_t		treeName[64];
	bool		isOpened;
	TCanvas*	canvas;
	
	static	int		perm[15][6];
	
	int				bestPerm;
	int				bestEta;
	
	Analysis6GammaCanvas*	analysisAll[2];					//[Eta, Pi0]
	Analysis6GammaCanvas*	analysisDecayTagger[2][4];		//[Eta, Pi0][PROMPT, RAND1, RAND2, prompt(only 1 Hit)]
	Analysis6GammaCanvas*	analysisDecayTaggerMulti[2][3];		//[Eta, Pi0][PROMPT, RAND1, RAND2]	(filled multi)
	
	
	//variables
	Int_t				nTagged;
	Double_t			TaggedEnergy[64];
	Double_t			TaggedTime[64];
		
	Int_t				nCBHits;
	Double_t			Px[32];
	Double_t			Py[32];
	Double_t			Pz[32];
	Double_t			E[32];
	Double_t			Time[32];
	
	//histograms
	TH1I*		hNTagged;
	TH1D*		hTaggedTime;
	TH1I*		hNCBHits;
	
	//buffer
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	TLorentzVector	partAll;
	Double_t		massAll;
	
	//counters
	Int_t		countDecays[3];				// checked, Eta, 3Pi0
	Int_t		countTaggerTime[5];			// prompt, rand1, rand2, prompt(only 1 Hit), checked
	Int_t		countTaggerTimeMulti[3];	// prompt, rand1, rand2		(filled multi)
	
	//cuts
	Double_t	cutTaggerTime[6];	// prompt, rand1, rand2
	
protected:
	//variables
	TLorentzVector		vec[6];
	TLorentzVector		beam[3][8];
	int					nBeam[3];

public:
	
	ReadRootTree(const char* _treeFileName, const char* _treeName);
	~ReadRootTree();
	
	void	Clear();
	bool	openTree();
	void	Analyse(const int min = 0, const int max = -1);
	void	AnalyseEntry(const int index);
	void	AnalyseTaggerTime();
	void	Reconstruct();
	
	void	PrintCounters();
	void	Draw();
	
	void	SetCutTaggerTime(const Double_t promptMin, const Double_t promptMax, const Double_t rand1Min, const Double_t rand1Max, const Double_t rand2Min, const Double_t rand2Max)	{cutTaggerTime[0]=promptMin; cutTaggerTime[1]=promptMax; cutTaggerTime[2]=rand1Min; cutTaggerTime[3]=rand1Max; cutTaggerTime[4]=rand2Min; cutTaggerTime[5]=rand2Max;}
};
