#include "TreeRead.C"
#include "TreeHistGeneralTagged.C"

class	TreeAnalyseTagger	: public TreeRead
{
private:
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutCBTime[2];
	Double_t	cutTaggerTime[3][2];
	
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Int_t			nBeam[3];
	Double_t		beamEnergy[3][16];
	Double_t		beamTime[3][16];
	
	TH1I*					hCBTimeCutCount;
	TH1D*					hCBTime;
	TH1D*					hCBTimeCutCheck;
	TH1I*					hTaggerTimeCutCount[3];		//[2g, 6g, 10g]
	TH1D*					hTaggerTime[3];				//[2g, 6g, 10g]
	TH1D*					hTaggerTimeCutCheck[3];		//[Prompt, Rand1, Rand2]
	TreeHistGeneralTagged	hist[3];					//[2g, 6g, 10g]
	
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
	
	const	Double_t*	GetCutCBTime()				const	{return cutCBTime;}
	const	Double_t	GetCutCBTimeMin()			const	{return cutCBTime[0];}
	const	Double_t	GetCutCBTimeMax()			const	{return cutCBTime[1];}
	const	Double_t*	GetCutTaggerTime()			const	{return cutTaggerTime;}
	const	Double_t*	GetCutTaggerTimePrompt()	const	{return cutTaggerTime[0];}
	const	Double_t	GetCutTaggerTimePromptMin()	const	{return cutTaggerTime[0][0];}
	const	Double_t	GetCutTaggerTimePromptMax()	const	{return cutTaggerTime[0][1];}
	const	Double_t*	GetCutTaggerTimeRand1()		const	{return cutTaggerTime[1];}
	const	Double_t	GetCutTaggerTimeRand1Min()	const	{return cutTaggerTime[1][0];}
	const	Double_t	GetCutTaggerTimeRand1Max()	const	{return cutTaggerTime[1][1];}
	const	Double_t*	GetCutTaggerTimeRand2()		const	{return cutTaggerTime[2];}
	const	Double_t	GetCutTaggerTimeRand2Min()	const	{return cutTaggerTime[2][0];}
	const	Double_t	GetCutTaggerTimeRand2Max()	const	{return cutTaggerTime[2][1];}	
	
	
			void		SetCutCBTime(const Double_t Min, const Double_t Max)			{cutCBTime[0] = Min; cutCBTime[1] = Max;}
			void		SetCutTaggerTimePrompt(const Double_t Min, const Double_t Max)	{cutTaggerTime[0][0] = Min; cutTaggerTime[0][1] = Max;}
			void		SetCutTaggerTimeRand1(const Double_t Min, const Double_t Max)	{cutTaggerTime[1][0] = Min; cutTaggerTime[1][1] = Max;}
			void		SetCutTaggerTimeRand2(const Double_t Min, const Double_t Max)	{cutTaggerTime[2][0] = Min; cutTaggerTime[2][1] = Max;}
	
	static	TreeAnalyseTagger*	test()
	{
		printf("Creating\n");
		TreeAnalyseTagger* c = new TreeAnalyseTagger("TTreeOutput_41941");
		printf("Opening\n");
		c->Open();
		printf("Setting CBTime Cut\n");
		c->SetCutCBTime(-50, 50);
		printf("Setting Tagger Prompt Cut\n");
		c->SetCutTaggerTimePrompt(-6,  6);
		printf("Setting Tagger Rand1 Cut\n");
		c->SetCutTaggerTimeRand1(-20, -8);
		printf("Setting Tagger Rand2 Cut\n");
		c->SetCutTaggerTimeRand2(  8, 20);
		printf("Analysing\n");
		c->Analyse(10000);
		printf("Saving\n");
		c->Save();
		printf("End\n");
		return c;
	}
};


TreeAnalyseTagger::TreeAnalyseTagger(const Char_t* FileName)	: TreeRead(FileName)
{		
	cutCBTime[0]	= -100000;
	cutCBTime[1]	= 100000;
	
	cutTaggerTime[0][0]	= -30;
	cutTaggerTime[0][1]	= 30;
	cutTaggerTime[1][0]	= -90;
	cutTaggerTime[1][1]	= -30;
	cutTaggerTime[2][0]	= 30;
	cutTaggerTime[2][1]	= 90;
	
	if(!(hCBTimeCutCount	= (TH1I*)gROOT->Get("CBTimeCutCount")))
		hCBTimeCutCount		= new TH1I("CBTimeCutCount", "1:All/2:Passed", 4, 0, 4);
	if(!(hCBTime			= (TH1D*)gROOT->Get("CBTime")))
		hCBTime				= new TH1D("CBTime", "CBTime", 10000, -500, 500);	
	if(!(hCBTimeCutCheck	= (TH1D*)gROOT->Get("CBTimeCutCheck")))
		hCBTimeCutCheck		= new TH1D("CBTimeCutCheck", "CBTimeCutCheck", 2000, -100, 100);	
	if(!(hTaggerTimeCutCount[0]	= (TH1I*)gROOT->Get("2g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[0]	= new TH1I("2g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTimeCutCount[1]	= (TH1I*)gROOT->Get("6g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[1]	= new TH1I("6g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTimeCutCount[2]	= (TH1I*)gROOT->Get("10g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[2]	= new TH1I("10g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTime[0]		= (TH1D*)gROOT->Get("2g_TaggerTime")))
		hTaggerTime[0]		= new TH1D("2g_TaggerTime", "2g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTime[1]		= (TH1D*)gROOT->Get("6g_TaggerTime")))
		hTaggerTime[1]		= new TH1D("6g_TaggerTime", "6g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTime[2]		= (TH1D*)gROOT->Get("10g_TaggerTime")))
		hTaggerTime[2]		= new TH1D("10g_TaggerTime", "10g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[0]		= (TH1D*)gROOT->Get("2g_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[0]		= new TH1D("2g_TaggerTimeCutCheck", "2g_TaggerTimeCutCheck", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[1]		= (TH1D*)gROOT->Get("6g_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[1]		= new TH1D("6g_TaggerTimeCutCheck", "6g_TaggerTimeCutCheck", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[2]		= (TH1D*)gROOT->Get("10g_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[2]		= new TH1D("10g_TaggerTimeCutCheck", "10g_TaggerTimeCutCheck", 2000, -100, 100);	
	
	TString	BaseName[19];
	BaseName[0]		= "Prompt_NTagged";
	BaseName[1]		= "Rand1_NTagged";
	BaseName[2]		= "Rand2_NTagged";
	BaseName[3]		= "Prompt_TaggedEnergy";
	BaseName[4]		= "Rand1_TaggedEnergy";
	BaseName[5]		= "Rand2_TaggedEnergy";
	BaseName[6]		= "Multiplicity";
	BaseName[7]		= "Prompt_CBEnergyAll";
	BaseName[8]		= "Rand1_CBEnergyAll";
	BaseName[9]		= "Rand2_CBEnergyAll";
	BaseName[10]	= "Prompt_IMAll";
	BaseName[11]	= "Rand1_IMAll";
	BaseName[12]	= "Rand2_IMAll";
	BaseName[13]	= "Prompt_ThetaAll";
	BaseName[14]	= "Rand1_ThetaAll";
	BaseName[15]	= "Rand2_ThetaAll";
	BaseName[16]	= "Prompt_PhiAll";
	BaseName[17]	= "Rand1_PhiAll";
	BaseName[18]	= "Rand2_PhiAll";
	Int_t		NBin[9]	=	{8,  200, 16, 2000, 2000,  180,  360};
	Double_t	Min[9]	=	{0, 1400,  0,    0,    0,    0, -180};
	Double_t	Max[9]	=	{8, 1600, 16, 2000, 2000,  180,  180};
	TString	Name[19];
	
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("2g_");
	}
	if(!hist[0].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hist[0] could not been initiated\n");
	}
	
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("6g_");
	}
	if(!hist[1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hist[1] could not been initiated\n");
	}
	
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("10g_");
	}
	if(!hist[2].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hist[2] could not been initiated\n");
	}
	
	Clear();
	
}
TreeAnalyseTagger::~TreeAnalyseTagger()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

bool	TreeAnalyseTagger::CutCBTime()
{
	
	hCBTimeCutCount->Fill(1);
	
	bool	ret	= true;
	for(int i=0; i<nCBHits; i++)
	{
		hCBTime->Fill(CBTime[i]);
		if(CBTime[i] < cutCBTime[0] || CBTime[i] > cutCBTime[1])
			ret = false;
		else
			hCBTimeCutCheck->Fill(CBTime[i]);
	}
	if(!ret)
		return false;
	hCBTimeCutCount->Fill(2);
	return true;
}

inline	void	TreeAnalyseTagger::Clear()	
{
	hCBTimeCutCount->Reset("M");
	hCBTime->Reset("M");
	hCBTimeCutCheck->Reset("M");
	for(int i=0; i<3; i++)
	{
		hTaggerTimeCutCount[i]->Reset("M");
		hTaggerTime[i]->Reset("M");
		hTaggerTimeCutCheck[i]->Reset("M");
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
			sprintf(str, "tree_%s_2g.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_6g.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_10g.root", GetFileName());
			
		outFile[i]		= new TFile(str, "RECREATE");
		outTree[i]		= new TTree("tree", "tree");
		
		outTree[i]->Branch("nPrompt",&nBeam[0],"nPrompt/I");
		outTree[i]->Branch("PromptEnergy", beamEnergy[0], "PromptEnergy[nPrompt]/D");
		outTree[i]->Branch("PromptTime", beamTime[0], "PromptTime[nPrompt]/D");
		outTree[i]->Branch("nRand1",&nBeam[1],"nRand1/I");
		outTree[i]->Branch("Rand1Energy", beamEnergy[1], "Rand1Energy[nRand1]/D");
		outTree[i]->Branch("Rand1Time", beamTime[1], "Rand1Time[nRand1]/D");
		outTree[i]->Branch("nRand2",&nBeam[2],"nRand2/I");
		outTree[i]->Branch("Rand2Energy", beamEnergy[2], "Rand2Energy[nRand2]/D");
		outTree[i]->Branch("Rand2Time", beamTime[2], "Rand2Time[nRand2]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

void	TreeAnalyseTagger::AnalyseEvent(const Int_t i)
{
	//printf("%d\n",i);
	TreeRead::AnalyseEvent(i);
	
	Int_t	multiplicityEnum = -1;
	
	if(nCBHits == 2)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vecAll	= vec[0] + vec[1];
			multiplicityEnum	= 0;
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
			multiplicityEnum	= 1;
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
			multiplicityEnum	= 2;
		}
	}
	
	
	if(multiplicityEnum == -1)
		return;
	
	nBeam[0]	= 0;
	nBeam[1]	= 0;
	nBeam[2]	= 0;
	
	hTaggerTimeCutCount[multiplicityEnum]->Fill(1);
	
	for(int i=0; i<nTagged; i++)
	{
		hTaggerTime[multiplicityEnum]->Fill(TaggedTime[i]);
		if(TaggedTime[i] >= cutTaggerTime[0][0] && TaggedTime[i] <= cutTaggerTime[0][1])
		{
			beamEnergy[0][nBeam[0]]	= TaggedEnergy[i];
			beamTime[0][nBeam[0]]	= TaggedTime[i];
			nBeam[0]++;
			hTaggerTimeCutCount[multiplicityEnum]->Fill(7);
			hTaggerTimeCutCheck[0]->Fill(TaggedTime[i]);
		}
		if(TaggedTime[i] >= cutTaggerTime[1][0] && TaggedTime[i] <= cutTaggerTime[1][1])
		{
			beamEnergy[1][nBeam[1]]	= TaggedEnergy[i];
			beamTime[1][nBeam[1]]	= TaggedTime[i];
			nBeam[1]++;
			hTaggerTimeCutCount[multiplicityEnum]->Fill(8);
			hTaggerTimeCutCheck[1]->Fill(TaggedTime[i]);
		}
		if(TaggedTime[i] >= cutTaggerTime[2][0] && TaggedTime[i] <= cutTaggerTime[2][1])
		{
			beamEnergy[2][nBeam[2]]	= TaggedEnergy[i];
			beamTime[2][nBeam[2]]	= TaggedTime[i];
			nBeam[2]++;
			hTaggerTimeCutCount[multiplicityEnum]->Fill(9);
			hTaggerTimeCutCheck[2]->Fill(TaggedTime[i]);
		}
	}
	
	if(nBeam[0] > 0)
	{
		hTaggerTimeCutCount[multiplicityEnum]->Fill(2);
	}
	if(nBeam[1] > 0)
	{
		hTaggerTimeCutCount[multiplicityEnum]->Fill(3);
	}
	if(nBeam[2] > 0)
	{
		hTaggerTimeCutCount[multiplicityEnum]->Fill(4);
	}
	
	if(nBeam[0] == 0 && nBeam[1] == 0 && nBeam[2] == 0)
	{
		hTaggerTimeCutCount[multiplicityEnum]->Fill(5);
	}
	else
	{
		outTree[multiplicityEnum]->Fill();
		hist[multiplicityEnum].Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
	for(int i=0; i<3; i++)
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
	
	//**********************************************************************************
	//**********************************************************************************
	//*********************************    TO DO    ************************************
	//**********************************************************************************
	//**********************************************************************************
	
	TreeHistGeneralTagged	histAll;
	TString	BaseName[19];
	BaseName[0]		= "Prompt_NTagged";
	BaseName[1]		= "Rand1_NTagged";
	BaseName[2]		= "Rand2_NTagged";
	BaseName[3]		= "Prompt_TaggedEnergy";
	BaseName[4]		= "Rand1_TaggedEnergy";
	BaseName[5]		= "Rand2_TaggedEnergy";
	BaseName[6]		= "Multiplicity";
	BaseName[7]		= "Prompt_CBEnergyAll";
	BaseName[8]		= "Rand1_CBEnergyAll";
	BaseName[9]		= "Rand2_CBEnergyAll";
	BaseName[10]	= "Prompt_IMAll";
	BaseName[11]	= "Rand1_IMAll";
	BaseName[12]	= "Rand2_IMAll";
	BaseName[13]	= "Prompt_ThetaAll";
	BaseName[14]	= "Rand1_ThetaAll";
	BaseName[15]	= "Rand2_ThetaAll";
	BaseName[16]	= "Prompt_PhiAll";
	BaseName[17]	= "Rand1_PhiAll";
	BaseName[18]	= "Rand2_PhiAll";
	Int_t		NBin[9]	=	{8,  200, 16, 2000, 2000,  180,  360};
	Double_t	Min[9]	=	{0, 1400,  0,    0,    0,    0, -180};
	Double_t	Max[9]	=	{8, 1600, 16, 2000, 2000,  180,  180};
	if(!histAll.Init(hist[0], BaseName))
		return false;
	histAll.Add(hist[1]);
	histAll.Add(hist[2]);
	
	printf("hier\n");
	
	BaseName[0]		= "BG_TaggedEnergy";
	BaseName[1]		= "Result_TaggedEnergy";
	BaseName[2]		= "BG_CBEnergyAll";
	BaseName[3]		= "Result_CBEnergyAll";
	BaseName[4]		= "BG_IMAll";
	BaseName[5]		= "Result_IMAll";
	BaseName[6]		= "BG_ThetaAll";
	BaseName[7]		= "Result_ThetaAll";
	BaseName[8]		= "BG_PhiAll";
	BaseName[9]		= "Result_PhiAll";
	histAll.SubstractBackground(BaseName);
	
	printf("hier\n");
	
	TString	Name[10];
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("2g_");
	}
	hist[0].SubstractBackground(Name);
	printf("hier\n");
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("6g_");
	}
	hist[1].SubstractBackground(Name);
	printf("hier\n");
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("10g_");
	}
	hist[2].SubstractBackground(Name);
	printf("hier\n");
	
	TH1D*	hTaggerTimeAll;
	if(hTaggerTimeAll			= (TH1D*)gROOT->Get("TaggerTime"))
		hTaggerTimeAll->Delete();
	hTaggerTimeAll			= (TH1D*)hTaggerTime[0]->Clone("TaggerTime");
	hTaggerTimeAll->Add(hTaggerTime[1]);
	hTaggerTimeAll->Add(hTaggerTime[2]);
	
	TH1D*	hTaggerTimeCutCountAll;
	if(hTaggerTimeCutCountAll			= (TH1D*)gROOT->Get("TaggerTimeCutCount"))
		hTaggerTimeCutCountAll->Delete();
	hTaggerTimeCutCountAll	= (TH1D*)hTaggerTimeCutCount[0]->Clone("TaggerTimeCutCount");
	hTaggerTimeCutCountAll->Add(hTaggerTimeCutCount[1]);
	hTaggerTimeCutCountAll->Add(hTaggerTimeCutCount[2]);
	
	TH1D*	hTaggerTimeCutCheckAll;
	if(hTaggerTimeCutCheckAll			= (TH1D*)gROOT->Get("TaggerTimeCutCheck"))
		hTaggerTimeCutCheckAll->Delete();
	hTaggerTimeCutCheckAll	= (TH1D*)hTaggerTimeCutCheck[0]->Clone("TaggerTimeCutCheck");
	hTaggerTimeCutCheckAll->Add(hTaggerTimeCutCheck[1]);
	hTaggerTimeCutCheckAll->Add(hTaggerTimeCutCheck[2]);
	
	/*TreeHistGeneral	histAll[4];
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
	TString	Name[9];
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("2g_");
	histAll[0].Init(hist[0][0], Name);
	histAll[0].Add(hist[0][1]);
	histAll[0].Add(hist[0][2]);
	histAll[0].Add(hist[0][3]);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("6g_");
	histAll[1].Init(hist[1][0], Name);
	histAll[1].Add(hist[1][1]);
	histAll[1].Add(hist[1][2]);
	histAll[1].Add(hist[1][3]);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("10g_");
	histAll[2].Init(hist[2][0], Name);
	histAll[2].Add(hist[2][1]);
	histAll[2].Add(hist[2][2]);
	histAll[2].Add(hist[2][3]);
	
	histAll[3].Init(histAll[0], BaseName);
	histAll[3].Add(histAll[1]);
	histAll[3].Add(histAll[2]);
	
	
	TH1D*	hTaggerTimeCutCountAll	= (TH1D*)hTaggerTimeCutCount[0]->Clone("TaggerTimeCutCount");
	hTaggerTimeCutCountAll->Add(hTaggerTimeCutCount[1]);
	hTaggerTimeCutCountAll->Add(hTaggerTimeCutCount[2]);
	
	
	TreeHistGeneral	BG[3];
	TreeHistGeneral	Result[3];
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("2g_BG_");
	CalcBackground(BG[0], hist[0][1], hist[0][2], Name);
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("6g_BG_");
	CalcBackground(BG[1], hist[1][1], hist[1][2], Name);
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("10g_BG_");
	CalcBackground(BG[2], hist[2][1], hist[2][2], Name);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("2g_Result_");
	CalcResult(Result[0], hist[0][0], BG[0], Name);
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("6g_Result_");
	CalcResult(Result[1], hist[1][0], BG[1], Name);
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("10g_Result_");
	CalcResult(Result[2], hist[2][0], BG[2], Name);*/
	
	printf("hier\n");
	
	result->cd();
	hCBTimeCutCount->Write();
	hCBTime->Write();
	hCBTimeCutCheck->Write();
	hTaggerTimeAll->Write();
	hTaggerTimeCutCountAll->Write();
	hTaggerTimeCutCheckAll->Write();
	histAll.Save(true);
	hTaggerTime[0]->Write();
	hTaggerTimeCutCount[0]->Write();
	hTaggerTimeCutCheck[0]->Write();
	hist[0].Save(true);	
	hTaggerTime[1]->Write();
	hTaggerTimeCutCount[1]->Write();
	hTaggerTimeCutCheck[1]->Write();
	hist[1].Save();	
	hTaggerTime[2]->Write();
	hTaggerTimeCutCount[2]->Write();
	hTaggerTimeCutCheck[2]->Write();
	hist[2].Save();	
	result->Flush();
	result->Close();
	delete result;
}
