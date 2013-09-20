#include <stdlib.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TStyle.h"



class ReadRootTree
{
private:

	//general
	TFile*		file;
	TTree*		tree;
	Char_t		treeFileName[256];
	Char_t		treeName[64];
	bool		isOpened;
	TCanvas*	canvas;
	int			period;
	int			nPeriod;
	
	//variables
		
	Int_t				nCBHits;
	Double_t			Px[32];
	Double_t			Py[32];
	Double_t			Pz[32];
	Double_t			E[32];
	Double_t			Time[32];
	
	//histograms
	Int_t		nTagged;
	Double_t	TaggedEnergy[64];
	Double_t	TaggedTime[64];
	
	TH1I*		hNTagged;
	TH1D*		hTaggedTime;
	TH1I*		hCountAll;
	TH1I*		hNCBHits;
	TH1D*		CBEnergyAll;
	
	//counters
	Int_t		countMultiplicity[21];		// up to M20

	bool	openTree();
	
	
protected:

	//general
	TFile*	outFile;

	//variables
	TLorentzVector		vec[10];
	TLorentzVector		all;
	
	//counters
	Int_t		countAll;
	
	bool	AnalyseEvent(const int index);				// no index checking
	bool	OpenOutputFile(const Char_t* outputFileName);
	void	Save();
	
public:
	
			ReadRootTree(const char* _treeFileName, const char* _treeName);
	virtual	~ReadRootTree();
	
	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	PrintCounters();
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
	
	const	bool		IsOpened()							const	{return isOpened;}
			Int_t		GetNEvents()								{return tree->GetEntries();}
	const	Int_t		GetNTagged()						const	{return nTagged;}
	const	Int_t		GetNCBHits()						const	{return nCBHits;}
	const	Double_t*	GetTaggedEnergy()					const	{return TaggedEnergy;}
	const	Double_t	GetTaggedEnergy(const Int_t index)	const	{return TaggedEnergy[index];}
	const	Double_t*	GetTaggedTime()						const	{return TaggedTime;}
	const	Double_t	GetTaggedTime(const Int_t index)	const	{return TaggedTime[index];}
};



