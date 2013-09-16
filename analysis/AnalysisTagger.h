

#include "ReadRootTree.h"


#define MASS_PROTON	938.27203


class AnalysisTagger	: public ReadRootTree
{
private:

	//general
	TCanvas*	canvas;
	
	//types
	bool				untagged;			// untagged
	bool				uniqueWindow;		// only 1 hit in 1 window
	
	//counters
	Int_t		countTaggerWindow[5];		// prompt, rand1, rand2, prompt(only 1 Hit), checked
	Int_t		countTaggerWindowMulti[3];	// prompt, rand1, rand2		(filled multi)
	
	//cuts
	Double_t	cutTaggerTime[6];	// prompt, rand1, rand2
	Double_t	cutMissMass[2];
	
	//histogramms
	TH1I*	hCountTaggerWindow;
	TH1I*	hCountTaggerWindowMulti;
	TH1D*	hMissMass[4];
	TH1D*	hMissMassMulti[3];
	
	
protected:
	
	//variables
	TLorentzVector		beam[3][10];
	Double_t			missMass[3][10];
	Int_t				nBeam[3];
	
	
	void	CutMissMass();
	void	AnalyseTagged();
	bool	AnalyseEvent(const int index);				// no index checking
	
	
public:
			AnalysisTagger(const char* _treeFileName, const char* _treeName);
	virtual	~AnalysisTagger();
	
	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	PrintCounters();
	virtual	void	Draw();
	
	const	bool	isUntagged()		const	{return untagged;}
	const	bool	isUniqueWindow()	const	{return uniqueWindow;}
	
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
	void	SetCutMissMass(const Double_t Min, const Double_t Max)	{cutMissMass[0]=Min; cutMissMass[1]=Max}
		
};
