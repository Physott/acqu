#define		MASS_PI0	134.9766
#define		MASS_ETA	547.51
#define		MASS_ETAP	957.78

enum	STagged
{
	NO,
	PROMPT,
	RAND1,
	RAND2
};

class AnalyseEtap
{
	private:
		//general
		TFile*		file;
		TTree*		tree;
		TCanvas*	canvas[4];					// general, prompt, rand1, rand2
		char		treeFileName[256];
		char		treeName[256];
		bool		isOpened;
		int			thisEntry;		
		int			entries;					
		int			entriesTimeCB[2];			// tested, passed
		int			entriesTimeTagger[4];		// tested, prompt, rand1, rand2
		int			acceptedEntries;
		int			acceptedEntries6Hits;
		
		static	int		perm[15][6];
		
		//Variables
		Int_t				nTagged;
		Double_t			TaggedEnergy[64];
		Double_t			TaggedTime[64];
		
		Int_t				nCBHits;
		Double_t			Px[32];
		Double_t			Py[32];
		Double_t			Pz[32];
		Double_t			E[32];
		Double_t			Time[32];
		
		TLorentzVector		vec[32];
		TLorentzVector		beam[64];
		
		//histograms
		TH1D*		hTimeCB;
		TH1I*		hNTagged;		
		TH1D*		hTimeTagged;
		
		//cuts
		Double_t	cutTimeCB[2];
		Double_t	cutTimeTagger[6];
		
		//methodes
		STagged	TimeCutTagger();
		bool	TimeCutCB();
		bool	Analyse6Hits();
		void	PrintActualEvent();
    
	public:
		AnalyseEtap(const char* _treeFileName, const char* _treeName);
		~AnalyseEtap();
		
		bool	openTree();
		void	Clear()								{nTagged = 0; nCBHits = 0;}
		void	Analyse(int start = 0, int stop = -1);
		void	PrintEvent(const int event);
		void	Draw();
		
		const char*		GetTreeFileName()		const	{return treeFileName;}
		const char*		GetTreeName()			const	{return treeName;}
		const int		GetThisEntryNumber()	const	{return thisEntry;}
		const Double_t	GetCutTimeTaggerPromptMin()		const	{return cutTimeTagger[0];}
		const Double_t	GetCutTimeTaggerPromptMax()		const	{return cutTimeTagger[1];}
		const Double_t	GetCutTimeTaggerRand1Min()		const	{return cutTimeTagger[0];}
		const Double_t	GetCutTimeTaggerRand1Max()		const	{return cutTimeTagger[1];}		
		const Double_t	GetCutTimeTaggerRand2Min()		const	{return cutTimeTagger[0];}
		const Double_t	GetCutTimeTaggerRand2Max()		const	{return cutTimeTagger[1];}				
		const Double_t	GetCutTimeCBMin()		const	{return cutTimeCB[0];}
		const Double_t	GetCutTimeCBMax()		const	{return cutTimeCB[1];}
		
		void	SetTreeFileName(const char*	_treeFileName)				{strcpy(treeFileName,_treeFileName);}
		void	SetTreeName(const char*	_treeName)						{strcpy(treeName,_treeName);}
		void	SetCutTimeTagger(const Double_t PromptMin, const Double_t PromptMax, const Double_t Rand1Min, const Double_t Rand1Max, const Double_t Rand2Min, const Double_t Rand2Max)	{cutTimeTagger[0] = PromptMin; cutTimeTagger[1] = PromptMax; cutTimeTagger[2] = Rand1Min; cutTimeTagger[3] = Rand1Max; cutTimeTagger[4] = Rand2Min; cutTimeTagger[5] = Rand2Max;}
		void	SetCutTimeCB(const Double_t min, const Double_t max)	{cutTimeCB[0] = min; cutTimeCB[1] = max;}
};

AnalyseEtap::AnalyseEtap(const char* _treeFileName, const char* _treeName)	:	isOpened(false), 
																				thisEntry(0), 
																				entries(0),
																				acceptedEntries(0), 
																				acceptedEntries6Hits(0),
																				hTimeCB(0)
{
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	entriesTimeCB[0]		= 0;
	entriesTimeCB[1]		= 0;
	entriesTimeTagger[0]	= 0;
	entriesTimeTagger[1]	= 0;
	entriesTimeTagger[2]	= 0;
	entriesTimeTagger[3]	= 0;
	
	hNTagged	= new TH1I("NTagged", "NTagged", 16, 0, 16);
	hTimeTagged	= new TH1D("TimeTagged", "TimeTagged", 1000, -250, 250);
	SetCutTimeTagger(-250,250,0,0,0,0);
	hTimeCB		= new TH1D("TimeCB", "TimeCB", 1000, -250, 250);
	SetCutTimeCB(-250,250);
}

AnalyseEtap::~AnalyseEtap()
{
	if(file)
		delete file;
	if(tree)
		delete tree;
	if(canvas)
		delete canvas;
}


bool	AnalyseEtap::openTree()
{
	if(isOpened)
		return true;
		
	file	= new TFile(treeFileName);
	if(!file)
	{
		printf("Could not open file %s\n", treeFileName);
		return false;
	}	
	tree	= (TTree*)file->Get(treeName);
	if(!tree)
	{
		printf("Could not open tree %s in file %s\n", treeName, treeFileName);
		return false;
	}
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	tree->SetBranchAddress("Time", &Time);
	
	isOpened				= true;
	thisEntry				= 0;
	entries					= 0;
	entriesTimeCB[0]		= 0;
	entriesTimeCB[1]		= 0;
	entriesTimeTagger[0]	= 0;
	entriesTimeTagger[1]	= 0;
	entriesTimeTagger[2]	= 0;
	entriesTimeTagger[3]	= 0;
	acceptedEntries			= 0;
	acceptedEntries6Hits	= 0;
	Clear();
	return true;
}
STagged	AnalyseEtap::TimeCutTagger()
{
	entriesTimeTagger[0]++;
	
	hNTagged->Fill(nTagged);
	for(int i=0; i<nTagged; i++)
		hTimeTagged->Fill(TaggedTime[i]);
	
	if(nTagged!=1)
		return NO;
		
	beam.SetPxPyPzE(TaggedEnergy, 0, 0, TaggedEnergy);
		
	if(TaggedTime[0] >= cutTimeTagger[0]  &&  TaggedTime[0] <= cutTimeTagger[1])
	{
		entriesTimeTagger[1]++;
		return PROMPT;
	}
	
	if(TaggedTime[0] >= cutTimeTagger[2]  &&  TaggedTime[0] <= cutTimeTagger[3])
	{
		entriesTimeTagger[2]++;
		return RAND1;
	}
	
	if(TaggedTime[0] >= cutTimeTagger[4]  &&  TaggedTime[0] <= cutTimeTagger[5])
	{
		entriesTimeTagger[3]++;
		return RAND2;
	}
	
	return NO;
}
bool	AnalyseEtap::TimeCutCB()
{
	entriesTimeCB[0]++;
	
	for(int i=0; i<nCBHits; i++)
		hTimeCB->Fill(Time[i]);
		
	for(int i=0; i<nCBHits; i++)
		if(Time[i] < cutTimeCB[0]  ||  Time[i] > cutTimeCB[1])
			return false;
	
	entriesTimeCB[1]++;
	return true;
}
bool	AnalyseEtap::Analyse6Hits()
{
	if(!TimeCutCB())
		return false;
		
	STagged	res	= TimeCutTagger();
	
	Double_t		ChiSqr[15][4];
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	for(int i=0; i<15; i++)
	{
		part[i][0]	= vec[perm[i][0]] + vec[perm[i][1]];
		part[i][1]	= vec[perm[i][2]] + vec[perm[i][3]];
		part[i][2]	= vec[perm[i][4]] + vec[perm[i][5]];
		mass[i][0]	= part[i][0].Mag2();
		mass[i][1]	= part[i][1].Mag2();
		mass[i][2]	= part[i][2].Mag2();
		
		//**********************************************************************************
		//**********************************************************************************
		//************************         TO DO        ************************************
		//**********************************************************************************
		//**********************************************************************************
	}
	
	switch(res)
	{
		
	}
	
	acceptedEntries++;
	acceptedEntries6Hits++;
	
	return true;
}
void	AnalyseEtap::Analyse(int start, int stop)
{
	if(!isOpened)
	{
		printf("Tree not opened. Try opening ...");
		if(!openTree())
		{
			printf("failed. Exiting1\n");
			return;
		}
		printf("OK\n");
	}
	
	if(stop >= tree->GetEntries())
		stop = tree->GetEntries();
	if(stop < 0)
		stop = tree->GetEntries();
	if(start < 0)
		start = 0;
	if(start >= tree->GetEntries())
		start = tree->GetEntries();
		
	
	entries					= 0;
	entriesTimeCB[0]		= 0;
	entriesTimeCB[1]		= 0;
	entriesTimeTagger[0]	= 0;
	entriesTimeTagger[1]	= 0;
	entriesTimeTagger[2]	= 0;
	entriesTimeTagger[3]	= 0;
	acceptedEntries			= 0;
	acceptedEntries6Hits	= 0;
		
	for(int thisEntry=start; thisEntry<stop; thisEntry++)
	{
		tree->GetEntry(thisEntry);
		
		entries++;
		
		if(nCBHits == 6)
		{
			for(int i=0; i<6; i++)
				vec[i].SetPxPyPzE(Px[i], Py[i], Pz[i], E[i]);
			Analyse6Hits();
		}
	}
	
	printf("Analysis done! %d Entries checked\n", entries);
	printf("Cut TimeCB:     %d Entries tested. %d accepted (%lf %%)\n", entriesTimeCB[0], entriesTimeCB[1], 100.0*double(entriesTimeCB[1])/double(entriesTimeCB[0]) );
	printf("Cut TimeTagger: %d Entries tested. %d accepted Prompt (%lf %%). %d accepted Rand1 (%lf %%). %d accepted Rand2 (%lf %%)\n", entriesTimeTagger[0], entriesTimeTagger[1], 100.0*double(entriesTimeTagger[1])/double(entriesTimeTagger[0]), entriesTimeTagger[2], 100.0*double(entriesTimeTagger[2])/double(entriesTimeTagger[0]), entriesTimeTagger[3], 100.0*double(entriesTimeTagger[3])/double(entriesTimeTagger[0]));
	printf("%d Entries accepted with 6 Hits\n", acceptedEntries6Hits);
	printf("%d Entries accepted at all\n", acceptedEntries);
}
void	AnalyseEtap::PrintActualEvent()
{
	printf("Print Entry %d\n", thisEntry);
	
	printf("# of tagged Photons %d\n", nTagged);
	for(int i=0; i<nTagged; i++)
		printf("\tPhoton %d:   BeamEnergy: %lf   BeamTime: %lf\n", TaggedEnergy[i], TaggedTime[i]);
	
	printf("# of CB Hits %d\n", nCBHits);
	for(int i=0; i<nCBHits; i++)
		printf("\tHit %d:   Px: %lf   Py: %lf   Pz: %lf   E: %lf   Time: %lf\n", Px[i], Py[i], Pz[i], E[i], Time[i]);
}
void	AnalyseEtap::PrintEvent(const int event)
{
	if(!isOpened)
	{
		printf("PrintEvent:  tree is not opened\n");
		return;
	}
	
	if(event<0)
	{
		printf("PrintEvent:  event number &d is too small, set to 0\n", event);
		event = 0;
	}
	else if(event>=tree->GetEntries())
	{
		printf("PrintEvent:  event number &d is too large, set to max (%d)\n", event, tree->GetEntries());
		event = tree->GetEntries();
	}
	
	tree->GetEntry(event);
	
	printf("Print Entry %d\n", event);
	
	printf("# of tagged Photons %d\n", nTagged);
	for(int i=0; i<nTagged; i++)
		printf("\tPhoton %d:   BeamEnergy: %lf   BeamTime: %lf\n", TaggedEnergy[i], TaggedTime[i]);
	
	printf("# of CB Hits %d\n", nCBHits);
	for(int i=0; i<nCBHits; i++)
		printf("\tHit %d:   Px: %lf   Py: %lf   Pz: %lf   E: %lf   Time: %lf\n", Px[i], Py[i], Pz[i], E[i], Time[i]);
}
void	AnalyseEtap::Draw()
{
	if(!(canvas[0]	= (TCanvas*)gROOT->Get("canvasGeneral")))
		canvas[0]	= new TCanvas("canvasGeneral", "General", 0, 0, 800, 400);
		
	canvas[0]->Divide(2, 2, 0.001, 0.001);
	
	canvas[0]->cd(1);	hNTagged->Draw();
	canvas[0]->cd(2);	hTimeTagged->Draw();
	canvas[0]->cd(3);	hTimeCB->Draw();
	
	
	
	if(!(canvas[1]	= (TCanvas*)gROOT->Get("canvasPrompt")))
		canvas[1]	= new TCanvas("canvasPrompt", "Prompt", 0, 0, 800, 400);
		
	canvas[1]->Divide(2, 2, 0.001, 0.001);
	
	
	
	if(!(canvas[2]	= (TCanvas*)gROOT->Get("canvasRand1")))
		canvas[2]	= new TCanvas("canvasRand1", "Rand1", 0, 0, 800, 400);
		
	canvas[2]->Divide(2, 2, 0.001, 0.001);
	
	
	
	if(!(canvas[2]	= (TCanvas*)gROOT->Get("canvasRand2")))
		canvas[2]	= new TCanvas("canvasRand2", "Rand2", 0, 0, 800, 400);
		
	canvas[2]->Divide(2, 2, 0.001, 0.001);
}

int		AnalyseEtap::perm[15][6]=
{
	{0,1,2,3,4,5},
	{0,1,2,4,3,5},
	{0,1,2,5,4,3},
	
	{0,2,1,3,4,5},
	{0,2,1,4,3,5},
	{0,2,1,5,4,3},
	
	{0,3,2,1,4,5},
	{0,3,2,4,1,5},
	{0,3,2,5,4,1},
	
	{0,4,2,3,1,5},
	{0,4,2,1,3,5},
	{0,4,2,5,1,3},
	
	{0,5,2,3,4,1},
	{0,5,2,4,3,1},
	{0,5,2,1,4,3},
};
