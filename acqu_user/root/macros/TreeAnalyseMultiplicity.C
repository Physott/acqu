#include "TreeRead.C"
#include "TreeHistGeneral.C"

class	TreeAnalyseMultiplicity	: public TreeRead
{
private:
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutCBTime[2];
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	
	TH1I*			hCutCount;
	TH1D*			hCutCheck;
	TreeHistGeneral	hist[3];
	
	bool	CutCBTime();

protected:
	void	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseMultiplicity(const Char_t* FileName);
	~TreeAnalyseMultiplicity();
	
			void	Clear();
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutCBTime()		const	{return cutCBTime;}
	const	Double_t	GetCutCBTimeMin()	const	{return cutCBTime[0];}
	const	Double_t	GetCutCBTimeMax()	const	{return cutCBTime[1];}
	const	Double_t*	GetCutIMPi0()		const	{return cutIMPi0;}
	const	Double_t	GetCutIMPi0Min()	const	{return cutIMPi0[0];}
	const	Double_t	GetCutIMPi0Max()	const	{return cutIMPi0[1];}
	const	Double_t*	GetCutIMEta()		const	{return cutIMEta;}
	const	Double_t	GetCutIMEtaMin()	const	{return cutIMEta[0];}
	const	Double_t	GetCutIMEtaMax()	const	{return cutIMEta[1];}
	const	Double_t*	GetCutIMEtaP()		const	{return cutIMEtaP;}
	const	Double_t	GetCutIMEtaPMin()	const	{return cutIMEtaP[0];}
	const	Double_t	GetCutIMEtaPMax()	const	{return cutIMEtaP[1];}
	
			void		SetCutCBTime(const Double_t Min, const Double_t Max)	{cutCBTime[0] = Min; cutCBTime[1] = Max;}
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
	
	/*static	void	Analyse(const Char_t* FileName, const Double_t CBTimeMin, const Double_t CBTimeMax, const Double_t IMEtaPMin, const Double_t IMEtaPMax)
	{
		TreeAnalyseMultiplicity* c = new TreeAnalyseMultiplicity(FileName);
		c->Open();
		c->SetCutCBTime(CBTimeMin, CBTimeMax);
		c->SetCutIMEtaP(IMEtaPMin, IMEtaPMax);
		c->Analyse();
		c->Save();
		delete c;
	}		*/
	static	TreeAnalyseMultiplicity*	test()
	{
		TreeAnalyseMultiplicity* c = new TreeAnalyseMultiplicity("TTreeOutput_41941");
		c->Open();
		c->SetCutCBTime(-50, 50);
		c->Analyse();
		c->Save();
		return c;
	}
};





bool	TreeAnalyseMultiplicity::CutCBTime()
{
	hCutCount->Fill(1);
	for(int i=0; i<nCBHits; i++)
	{
		if(CBTime[i] < cutCBTime[0] || CBTime[i] > cutCBTime[1])
			return false;
		hCutCheck->Fill(CBTime[i]);
	}
	hCutCount->Fill(2);
	return true;
}



TreeAnalyseMultiplicity::TreeAnalyseMultiplicity(const Char_t* FileName)	: TreeRead(FileName)
{	
	//printf("TreeAnalyseMultiplicity constructor\n");
	cutCBTime[0]	= -100000;
	cutCBTime[1]	= 100000;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CBTimeCutCount")))
		hCutCount	= new TH1I("CBTimeCutCount", "1:All/2:Passed", 4, 0, 4);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("CBTimeCutCheck")))
		hCutCheck	= new TH1D("CBTimeCutCheck", "CBTimeCutCheck", 2000, -100, 100);
	
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
		Name[i].Prepend("2g_");
	hist[0].Init(Name, Name, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("6g_");
	hist[1].Init(Name, Name, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("10g_");
	hist[2].Init(Name, Name, NBin, Min, Max);
	
	Clear();
}
TreeAnalyseMultiplicity::~TreeAnalyseMultiplicity()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

inline	void	TreeAnalyseMultiplicity::Clear()	
{
	hCutCount->Reset("M");
	hCutCheck->Reset("M");
	
	for(int i=0; i<3; i++)
	{
		hist[i].Clear(); 
	}
}
	
void	TreeAnalyseMultiplicity::Open()
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
		
		outTree[i]->Branch("nTagged",&nTagged,"nTagged/I");
		outTree[i]->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
		outTree[i]->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

void	TreeAnalyseMultiplicity::AnalyseEvent(const Int_t i)
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
			outTree[0]->Fill();
			hist[0].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
			outTree[1]->Fill();
			hist[1].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
			outTree[2]->Fill();
			hist[2].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		}
	}
}
void	TreeAnalyseMultiplicity::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseMultiplicity::Save()
{
	for(int i=0; i<3; i++)
	{
		outFile[i]->cd();
		outTree[i]->Write();
		outFile[i]->Flush();
	}
	
	Char_t	str[128];
	sprintf(str, "hist_%s_Multiplicity.root", GetFileName());
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
	hCutCheck->Write();
	hist[0].Save();
	hist[1].Save();
	hist[2].Save();
	result->Close();
	delete result;
}

