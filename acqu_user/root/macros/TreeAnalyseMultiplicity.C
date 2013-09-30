#include "TreeRead.C"


class	TreeAnalyseMultiplicity	: public TreeRead
{
private:
	TFile*		outFile2g[3];
	TTree*		out2g[3];
	TFile*		outFile6g;
	TTree*		out6g;
	
	Double_t	cutCBTime[2];
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1D*	hIM;
	TH1D*	hIMCut[3];
	TH1D*	hIM6g;
	
	bool	CutCBTime();

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseMultiplicity(const Char_t* FileName);
	~TreeAnalyseMultiplicity();
	
			void	Clear()													{hIM->Reset("M"); hIMCut[0]->Reset("M"); hIMCut[1]->Reset("M"); hIMCut[2]->Reset("M"); hIM6g->Reset("M");}
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
		TreeAnalyseMultiplicity* c = new TreeAnalyseMultiplicity("TTreeOutput_41948.root");
		c->Open();
		c->SetCutCBTime(-50, 50);
		c->Analyse();
		c->Save();
		return c;
	}
};





bool	TreeAnalyseMultiplicity::CutCBTime()
{
	for(int i=0; i<nCBHits; i++)
	{
		if(CBTime[i] < cutCBTime[0] || CBTime[i] > cutCBTime[1])
			return false;
	}
	return true;
}



TreeAnalyseMultiplicity::TreeAnalyseMultiplicity(const Char_t* FileName)	: TreeRead(FileName), out2g(0), outFile2g(0)
{	
	//printf("TreeAnalyseMultiplicity constructor\n");
	cutCBTime[0]	= -100000;
	cutCBTime[1]	= 100000;
	cutIMPi0[0]		= 120;
	cutIMPi0[1]		= 150;
	cutIMEta[0]		= 517;
	cutIMEta[1]		= 577;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hIM		= (TH1D*)gROOT->Get("2G_IM")))
		hIM			= new TH1D("2G_IM", "2G_IM", 1600, 0, 1600);
	if(!(hIMCut[0]	= (TH1D*)gROOT->Get("2G_IM_CutPi0")))
		hIMCut[0]	= new TH1D("2G_IM_CutPi0", "2G_IM_CutPi0", 300, 0, 300);
	if(!(hIMCut[1]	= (TH1D*)gROOT->Get("2G_IM_CutEta")))
		hIMCut[1]	= new TH1D("2G_IM_CutEta", "2G_IM_CutEta", 300, 400, 700);
	if(!(hIMCut[2]	= (TH1D*)gROOT->Get("2G_IM_CutEtaP")))
		hIMCut[2]	= new TH1D("2G_IM_CutEtaP", "2G_IM_CutEtaP", 300, 800, 1100);
	if(!(hIM6g		= (TH1D*)gROOT->Get("6G_IM")))
		hIM6g		= new TH1D("6G_IM", "6G_IM", 1600, 0, 1600);
		
	Clear();
}
TreeAnalyseMultiplicity::~TreeAnalyseMultiplicity()
{
	for(int i=0; i<3; i++)
	{
		if(out2g[i])
			delete	out2g[i];
		if(outFile2g[i])
			delete	outFile2g[i];
	}
	
	if(out6g)
		delete	out6g;
	if(outFile6g)
		delete	outFile6g;
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
			sprintf(str, "tree_%s_2g_IMPi0.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_2g_IMEta.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_2g_IMEtaP.root", GetFileName());
			
		outFile2g[i]		= new TFile(str, "RECREATE");
		out2g[i]			= new TTree("tree", "tree");
		
		out2g[i]->Branch("nTagged",&nTagged,"nTagged/I");
		out2g[i]->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
		out2g[i]->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
		out2g[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		out2g[i]->Branch("Px", Px, "Px[nCBHits]/D");
		out2g[i]->Branch("Py", Py, "Py[nCBHits]/D");
		out2g[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		out2g[i]->Branch("E", E, "E[nCBHits]/D");	
		out2g[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
	
	sprintf(str, "tree_%s_6g.root", GetFileName());
			
	outFile6g		= new TFile(str, "RECREATE");
	out6g			= new TTree("tree", "tree");
		
	out6g->Branch("nTagged",&nTagged,"nTagged/I");
	out6g->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	out6g->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
	out6g->Branch("nCBHits",&nCBHits,"nCBHits/I");
	out6g->Branch("Px", Px, "Px[nCBHits]/D");
	out6g->Branch("Py", Py, "Py[nCBHits]/D");
	out6g->Branch("Pz", Pz, "Pz[nCBHits]/D");
	out6g->Branch("E", E, "E[nCBHits]/D");	
	out6g->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

bool	TreeAnalyseMultiplicity::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	if(nCBHits == 2)
	{
		if(CutCBTime())
		{
			vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
			vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
			vecAll	= vec[0] + vec[1];
			massAll	= vecAll.M();
			hIM->Fill(massAll);
			if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
			{
				out2g[0]->Fill();
				hIMCut[0]->Fill(massAll);
			}
			else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
			{
				out2g[1]->Fill();
				hIMCut[1]->Fill(massAll);
			}
			else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
			{
				out2g[2]->Fill();
				hIMCut[2]->Fill(massAll);
			}
		}
	}	
	else if(nCBHits == 6)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
		massAll	= vecAll.M();
		hIM6g->Fill(massAll);
		out6g->Fill();
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
		outFile2g[i]->cd();
		out2g[i]->Write();
		outFile2g[i]->Flush();
	}
	outFile6g->cd();
	out6g->Write();
	outFile6g->Flush();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_Multiplicity.root", GetFileName());
	TFile*		result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hIM->Write();
	hIMCut[0]->Write();
	hIMCut[1]->Write();
	hIMCut[2]->Write();
	hIM6g->Write();
	result->Close();
	delete result;
}

