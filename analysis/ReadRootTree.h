#ifndef	_ReadRootTree_h__
#define	_ReadRootTree_h__


#include <stdlib.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1I.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLorentzVector.h"
#include "TStyle.h"

#include "ReadRootTreeHist.h"
#include "Cut1Value.h"


class ReadRootTree
{
private:

	//general
	TFile*				file;
	TTree*				tree;
	Char_t				treeFileName[256];
	Char_t				treeName[64];
	bool				isOpened;
	TCanvas*			canvas;
	ReadRootTreeHist*	hist;
	int					period;
	int					nPeriod;
	
	//variables	
	Int_t				nCBHits;
	Double_t			Px[32];
	Double_t			Py[32];
	Double_t			Pz[32];
	Double_t			E[32];
	Double_t			Time[32];
	
	Int_t		nTagged;
	Double_t	TaggedEnergy[64];
	Double_t	TaggedTime[64];
	
	//cuts
	Cut1Value*			cutCBTime;
	TCanvas*			cutCanvas;
	ReadRootTreeHist*	cutHist;

	//functions	
	bool	openTree();
	
	
protected:

	//general
	TFile*	outFile;

	//variables
	TLorentzVector		vec[10];
	TLorentzVector		vecAll;
	
	
	bool	AnalyseEvent(const int index);				// no index checking
	bool	OpenOutputFile(const Char_t* outputFileName);
	void	Save();
	
public:
	
			ReadRootTree(const char* _treeFileName, const char* _treeName);
	virtual	~ReadRootTree();
	
	virtual	void	Clear()											{hist->Clear();}
	
			void	Fill(ReadRootTreeHist* h)						{h->Fill(nTagged, TaggedTime, TaggedEnergy, nCBHits, Time, vecAll.E());}
			
	virtual	void	Analyse(const int min, const int max);
	
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
	
	const	Double_t*	GetCutCBTime()		const		{return cutCBTime->GetCut();}
	const	Double_t	GetCutCBTimeMin()	const		{return cutCBTime->GetCutMin();}
	const	Double_t	GetCutCBTimeMax()	const		{return cutCBTime->GetCutMax();}
	
	void	SetCutCBTime(const Double_t min, const Double_t max)	{cutCBTime->SetCut(min, max);}
};




#endif

