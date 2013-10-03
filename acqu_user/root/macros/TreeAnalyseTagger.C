#include "TreeRead.C"
#include "TreeHistGeneral.C"

class	TreeAnalyseTagger	: public TreeRead
{
private:
	TFile*		outFile[4];
	TTree*		outTree[4];
	
	Double_t	cutTaggerTime[3][2];
	
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Int_t			nBeam[3];
	Double_t		beamEnergy[3][16];
	Double_t		beamTime[3][16];
	
	TH1I*			hCutCount;
	TreeHistGeneral	hist[4];
	
	bool	CutCBTime();

protected:
	void	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseTagger(const Char_t* FileName);
	~TreeAnalyseTagger();
	
			void	Clear();
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutTaggerTime()			const	{return cutTaggerTime;}
	const	Double_t*	GetCutTaggerTimePrompt()		const	{return cutTaggerTime[0];}
	const	Double_t	GetCutTaggerTimePromptMin()	const	{return cutTaggerTime[0][0];}
	const	Double_t	GetCutTaggerTimePromptMax()	const	{return cutTaggerTime[0][1];}
	const	Double_t*	GetCutTaggerTimeRand1()		const	{return cutTaggerTime[1];}
	const	Double_t	GetCutTaggerTimeRand1Min()	const	{return cutTaggerTime[1][0];}
	const	Double_t	GetCutTaggerTimeRand1Max()	const	{return cutTaggerTime[1][1];}
	const	Double_t*	GetCutTaggerTimeRand2()		const	{return cutTaggerTime[2];}
	const	Double_t	GetCutTaggerTimeRand2Min()	const	{return cutTaggerTime[2][0];}
	const	Double_t	GetCutTaggerTimeRand2Max()	const	{return cutTaggerTime[2][1];}	
	
	
			void		SetCutTaggerTimePrompt(const Double_t Min, const Double_t Max)	{cutTaggerTime[0][0] = Min; cutTaggerTime[0][1] = Max;}
			void		SetCutTaggerTimeRand1(const Double_t Min, const Double_t Max)		{cutTaggerTime[1][0] = Min; cutTaggerTime[1][1] = Max;}
			void		SetCutTaggerTimeRand2(const Double_t Min, const Double_t Max)		{cutTaggerTime[2][0] = Min; cutTaggerTime[2][1] = Max;}
	
	/*static	void	Analyse(const Char_t* FileName, const Double_t CBTimeMin, const Double_t CBTimeMax, const Double_t IMEtaPMin, const Double_t IMEtaPMax)
	{
		TreeAnalyseTagger* c = new TreeAnalyseTagger(FileName);
		c->Open();
		c->SetCutCBTime(CBTimeMin, CBTimeMax);
		c->SetCutIMEtaP(IMEtaPMin, IMEtaPMax);
		c->Analyse();
		c->Save();
		delete c;
	}		*/
	static	TreeAnalyseTagger*	test()
	{
		TreeAnalyseTagger* c = new TreeAnalyseTagger("tree_TTreeOutput_41941_2g.root");
		c->Open();
		c->SetCutTaggerTimePrompt(-6,  6);
		c->SetCutTaggerTimeRand1(-20, -8);
		c->SetCutTaggerTimeRand2(  8, 20);
		c->Analyse(10000);
		c->Save();
		return c;
	}
};


TreeAnalyseTagger::TreeAnalyseTagger(const Char_t* FileName)	: TreeRead(FileName)
{	
	//printf("TreeAnalyseMultiplicity constructor\n");
	cutTaggerTime[0][0]	= -30;
	cutTaggerTime[0][1]	= 30;
	cutTaggerTime[1][0]	= -90;
	cutTaggerTime[1][1]	= -30;
	cutTaggerTime[2][0]	= 30;
	cutTaggerTime[2][1]	= 90;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount	= new TH1I("CutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	
	
	TString	BaseName[9];
	BaseName[0]	= "NTagged";
	BaseName[1]	= "TaggedEnergy";
	BaseName[2]	= "TaggedTime";
	BaseName[3]	= "Multiplicity";
	BaseName[4]	= "CBTime";
	BaseName[5]	= "CBEnergyAll";
	BaseName[6]	= "IMAll";
	BaseName[7]	= "ThetaAll";
	BaseName[8]	= "PhiAll";
	Int_t		NBin[9]	=	{32,  200, 2000, 16, 2000, 2000, 2000,  180,  360};
	Double_t	Min[9]	=	{ 0, 1400, -100,  0, -100,    0,    0,    0, -180};
	Double_t	Max[9]	=	{32, 1600,  100, 16,  100, 2000, 2000,  180,  180};
	TString	Name[9];
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Prompt_");
	hist[0].Init(Name, Name, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Rand1_");
	hist[1].Init(Name, Name, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Rand2_");
	hist[2].Init(Name, Name, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Untagged_");
	hist[3].Init(Name, Name, NBin, Min, Max);
	
	Clear();
}
TreeAnalyseTagger::~TreeAnalyseTagger()
{
	for(int i=0; i<4; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

inline	void	TreeAnalyseTagger::Clear()	
{
	hCutCount->Reset("M");
	for(int i=0; i<4; i++)
	{
		hist[i].Clear(); 
	}
}
	
void	TreeAnalyseTagger::Open()
{
	//printf("TreeAnalyseMultiplicity::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
	
	Char_t	str[128];
	for(int i=0; i<3; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_Prompt.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_Rand1.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_Rand2.root", GetFileName());
			
		outFile[i]		= new TFile(str, "RECREATE");
		outTree[i]		= new TTree("tree", "tree");
		
		outTree[i]->Branch("nTagged",&nBeam[i],"nTagged/I");
		outTree[i]->Branch("BeamEnergy", beamEnergy[i], "BeamEnergy[nTagged]/D");
		outTree[i]->Branch("BeamTime", beamTime[i], "BeamTime[nTagged]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
	
	sprintf(str, "tree_%s_Untagged.root", GetFileName());
			
	outFile[3]		= new TFile(str, "RECREATE");
	outTree[3]		= new TTree("tree", "tree");
	
	outTree[3]->Branch("nCBHits",&nCBHits,"nCBHits/I");
	outTree[3]->Branch("Px", Px, "Px[nCBHits]/D");
	outTree[3]->Branch("Py", Py, "Py[nCBHits]/D");
	outTree[3]->Branch("Pz", Pz, "Pz[nCBHits]/D");
	outTree[3]->Branch("E", E, "E[nCBHits]/D");	
	outTree[3]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

void	TreeAnalyseTagger::AnalyseEvent(const Int_t i)
{
	//printf("%d\n",i);
	TreeRead::AnalyseEvent(i);
	
	if(nCBHits == 2)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vecAll	= vec[0] + vec[1];
		}
	}	
	else if(nCBHits == 6)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
			vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
			vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
			vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
			vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
		}
	}
	else if(nCBHits == 10)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
			vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
			vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
			vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
			vec[6].SetPxPyPzE(Px[6], Py[6], Pz[6], E[6]);
			vec[7].SetPxPyPzE(Px[7], Py[7], Pz[7], E[7]);
			vec[8].SetPxPyPzE(Px[8], Py[8], Pz[8], E[8]);
			vec[9].SetPxPyPzE(Px[9], Py[9], Pz[9], E[9]);
			vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5] + vec[6] + vec[7] + vec[8] + vec[9];
		}
	}
	
	nBeam[0]	= 0;
	nBeam[1]	= 0;
	nBeam[2]	= 0;
	
	hCutCount->Fill(1);
	
	for(int i=0; i<nTagged; i++)
	{
		if(TaggedTime[i] >= cutTaggerTime[0][0] && TaggedTime[i] <= cutTaggerTime[0][1])
		{
			beamEnergy[0][nBeam[0]]	= TaggedEnergy[i];
			beamTime[0][nBeam[0]]		= TaggedTime[i];
			nBeam[0]++;
			hCutCount->Fill(7);
		}
		if(TaggedTime[i] >= cutTaggerTime[1][0] && TaggedTime[i] <= cutTaggerTime[1][1])
		{
			beamEnergy[1][nBeam[1]]	= TaggedEnergy[i];
			beamTime[1][nBeam[1]]		= TaggedTime[i];
			nBeam[1]++;
			hCutCount->Fill(8);
		}
		if(TaggedTime[i] >= cutTaggerTime[2][0] && TaggedTime[i] <= cutTaggerTime[2][1])
		{
			beamEnergy[2][nBeam[2]]	= TaggedEnergy[i];
			beamTime[2][nBeam[2]]		= TaggedTime[i];
			nBeam[2]++;
			hCutCount->Fill(9);
		}
	}
	
	if(nBeam[0] > 0)
	{
		hCutCount->Fill(2);
		hist[0].Fill(nBeam[0], beamEnergy[0], beamTime[0], nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[0]->Fill();
	}
	if(nBeam[1] > 0)
	{
		hCutCount->Fill(3);
		hist[1].Fill(nBeam[1], beamEnergy[1], beamTime[1], nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[1]->Fill();
	}
	if(nBeam[2] > 0)
	{
		hCutCount->Fill(4);
		hist[2].Fill(nBeam[2], beamEnergy[2], beamTime[2], nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[2]->Fill();
	}
	
	if(nBeam[0] == 0 && nBeam[1] == 0 && nBeam[2] == 0)
	{
		hCutCount->Fill(5);
		hist[3].Fill(0, 0, 0, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[3]->Fill();
	}
	
}
void	TreeAnalyseTagger::Analyse(const Int_t Min, const Int_t Max)
{
	Double_t	min = Min;
	Double_t	max = Max;
	if(min < 0)
		min = 0;
	if(min > tree->GetEntries())
		min = tree->GetEntries();
	if(max < 0 || max > tree->GetEntries())
		max = tree->GetEntries();
		
	for(int i=min; i<max; i++)
		AnalyseEvent(i);
}
bool	TreeAnalyseTagger::Save()
{
	for(int i=0; i<4; i++)
	{
		outFile[i]->cd();
		outTree[i]->Write();
		outFile[i]->Flush();
	}
	
	Char_t	str[128];
	sprintf(str, "hist_%s_Tagger.root", GetFileName());
	TFile*		result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	
	TreeHistGeneral	histAll;
	TString	BaseName[9];
	BaseName[0]	= "NTagged";
	BaseName[1]	= "TaggedEnergy";
	BaseName[2]	= "TaggedTime";
	BaseName[3]	= "Multiplicity";
	BaseName[4]	= "CBTime";
	BaseName[5]	= "CBEnergyAll";
	BaseName[6]	= "IMAll";
	BaseName[7]	= "ThetaAll";
	BaseName[8]	= "PhiAll";
	histAll.Init(hist[0], BaseName);
	histAll.Add(hist[1]);
	histAll.Add(hist[2]);
	result->cd();
	histAll.Save();
	hCutCount->Write();
	hist[0].Save();
	hist[1].Save();
	hist[2].Save();
	hist[3].Save();
	result->Close();
	delete result;
}
