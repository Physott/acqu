#include "TreeRead.C"
#include "TaggedHistSet.C"

class	TreeAnalyseTagger	: public TreeRead
{
private:
	TFile*		outFile[4];
	TTree*		outTree[4];
	
	Double_t	cutCBTime[2];
	Double_t	cutTaggerTime[3][2];
	
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Int_t			nBeam[3];
	Double_t		beamEnergy[3][16];
	Double_t		beamTime[3][16];
	
	TH1I*			hCBTimeCutCount;
	TH1D*			hCBTime;
	TH1D*			hCBTimeCutCheck;
	TH1I*			hTaggerTimeCutCount[4];		//[2g, 3g, 6g, 10g]
	TH1D*			hTaggerTime[4];				//[2g, 3g, 6g, 10g]
	TH1D*			hTaggerTimeCutCheck[3];		//[Prompt, Rand1, Rand2]
	TaggedHistSet*	hist[4];					//[2g, 3g, 6g, 10g]
	
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
		c->Analyse(1000);
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
	if(!(hTaggerTimeCutCount[1]	= (TH1I*)gROOT->Get("3g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[1]	= new TH1I("3g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTimeCutCount[2]	= (TH1I*)gROOT->Get("6g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[2]	= new TH1I("6g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTimeCutCount[3]	= (TH1I*)gROOT->Get("10g_TaggerTimeCutCount")))
		hTaggerTimeCutCount[3]	= new TH1I("10g_TaggerTimeCutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hTaggerTime[0]		= (TH1D*)gROOT->Get("2g_TaggerTime")))
		hTaggerTime[0]		= new TH1D("2g_TaggerTime", "2g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTime[1]		= (TH1D*)gROOT->Get("3g_TaggerTime")))
		hTaggerTime[1]		= new TH1D("3g_TaggerTime", "3g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTime[2]		= (TH1D*)gROOT->Get("6g_TaggerTime")))
		hTaggerTime[2]		= new TH1D("6g_TaggerTime", "6g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTime[3]		= (TH1D*)gROOT->Get("10g_TaggerTime")))
		hTaggerTime[3]		= new TH1D("10g_TaggerTime", "10g_TaggerTime", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[0]		= (TH1D*)gROOT->Get("Prompt_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[0]		= new TH1D("Prompt_TaggerTimeCutCheck", "Prompt_TaggerTimeCutCheck", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[1]		= (TH1D*)gROOT->Get("Rand1_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[1]		= new TH1D("Rand1_TaggerTimeCutCheck", "Rand1_TaggerTimeCutCheck", 2000, -100, 100);	
	if(!(hTaggerTimeCutCheck[2]		= (TH1D*)gROOT->Get("Rand2_TaggerTimeCutCheck")))
		hTaggerTimeCutCheck[2]		= new TH1D("Rand2_TaggerTimeCutCheck", "Rand2_TaggerTimeCutCheck", 2000, -100, 100);	
	
	hist[0]	= new TaggedHistSet("2g_");
	hist[1]	= new TaggedHistSet("3g_");
	hist[2]	= new TaggedHistSet("6g_");
	hist[3]	= new TaggedHistSet("10g_");
	
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
		hist[i]->Clear(); 
	}
	hTaggerTimeCutCount[3]->Reset("M");
	hTaggerTime[3]->Reset("M");
	hist[3]->Clear(); 
}
	
void	TreeAnalyseTagger::Open()
{
	//printf("TreeAnalyseMultiplicity::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
	
	Char_t	str[128];
	for(int i=0; i<4; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_2g.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_3g.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_6g.root", GetFileName());
		else if(i==3)
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
	if(nCBHits == 3)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
			vecAll	= vec[0] + vec[1] + vec[2];
			multiplicityEnum	= 1;
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
			multiplicityEnum	= 2;
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
			multiplicityEnum	= 3;
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
		hist[multiplicityEnum]->Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
	
	TaggedHistSet	histAll("", hist[0]);
	histAll.Add(hist[1]);
	histAll.Add(hist[2]);
	histAll.Add(hist[3]);
	
	histAll.SubstractBackground();
	hist[0]->SubstractBackground();
	hist[1]->SubstractBackground();
	hist[2]->SubstractBackground();
	hist[3]->SubstractBackground();
	
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
	
	result->cd();
	hCBTimeCutCount->Write();
	hCBTime->Write();
	hCBTimeCutCheck->Write();
	hTaggerTimeAll->Write();
	hTaggerTimeCutCountAll->Write();
	hTaggerTimeCutCheckAll->Write();
	hTaggerTimeCutCheck[0]->Write();
	hTaggerTimeCutCheck[1]->Write();
	hTaggerTimeCutCheck[2]->Write();
	histAll.SaveResult();
	histAll.SaveSubs();
	
	result->cd();
	result->mkdir("2g");
	result->cd("2g");
	hTaggerTime[0]->Write();
	hTaggerTimeCutCount[0]->Write();
	hist[0]->SaveResult();
	hist[0]->SaveSubs();
	
	result->cd();
	result->mkdir("3g");
	result->cd("3g");
	hTaggerTime[1]->Write();
	hTaggerTimeCutCount[1]->Write();
	hist[1]->SaveResult();
	hist[1]->SaveSubs();
	
	result->cd();
	result->mkdir("6g");
	result->cd("6g");
	hTaggerTime[2]->Write();
	hTaggerTimeCutCount[2]->Write();
	hist[2]->SaveResult();
	hist[2]->SaveSubs();
	
	result->cd();
	result->mkdir("10g");
	result->cd("10g");
	hTaggerTime[3]->Write();
	hTaggerTimeCutCount[3]->Write();
	hist[3]->SaveResult();
	hist[3]->SaveSubs();
	
	result->Flush();
	result->Close();
	delete result;
}
