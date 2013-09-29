#include "TreeRead.C"


class	TreeAnalyseEtaP2Gamma	: public TreeRead
{
private:
	TFile*		outFile2g;
	TTree*		out2g;
	
	Double_t	cutCBTime[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1D*	hIM[2];
	
	bool	CutCBTime();

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseEtaP2Gamma(const Char_t* FileName);
	~TreeAnalyseEtaP2Gamma();
	
			void	Clear()													{hIM[0]->Reset("M"); hIM[1]->Reset("M");}
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutCBTime()		const	{return cutCBTime;}
	const	Double_t	GetCutCBTimeMin()	const	{return cutCBTime[0];}
	const	Double_t	GetCutCBTimeMax()	const	{return cutCBTime[1];}
	const	Double_t*	GetCutIMEtaP()		const	{return cutIMEtaP;}
	const	Double_t	GetCutIMEtaPMin()	const	{return cutIMEtaP[0];}
	const	Double_t	GetCutIMEtaPMax()	const	{return cutIMEtaP[1];}
	
			void		SetCutCBTime(const Double_t Min, const Double_t Max)	{cutCBTime[0] = Min; cutCBTime[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
	
	/*static	void	Analyse(const Char_t* FileName, const Double_t CBTimeMin, const Double_t CBTimeMax, const Double_t IMEtaPMin, const Double_t IMEtaPMax)
	{
		TreeAnalyseEtaP2Gamma* c = new TreeAnalyseEtaP2Gamma(FileName);
		c->Open();
		c->SetCutCBTime(CBTimeMin, CBTimeMax);
		c->SetCutIMEtaP(IMEtaPMin, IMEtaPMax);
		c->Analyse();
		c->Save();
		delete c;
	}		*/
	static	TreeAnalyseEtaP2Gamma*	test()
	{
		TreeAnalyseEtaP2Gamma* c = new TreeAnalyseEtaP2Gamma("TTreeOutput_41948.root");
		c->Open();
		c->SetCutCBTime(-50, 50);
		//c->Analyse(0,1);
		//c->Print();
		return c;
	}
};





bool	TreeAnalyseEtaP2Gamma::CutCBTime()
{
	for(int i=0; i<nCBHits; i++)
	{
		if(CBTime[i] < cutCBTime[0] || CBTime[i] > cutCBTime[1])
			return false;
	}
	return true;
}



TreeAnalyseEtaP2Gamma::TreeAnalyseEtaP2Gamma(const Char_t* FileName)	: TreeRead(FileName), out2g(0), outFile2g(0)
{	
	//printf("TreeAnalyseEtaP2Gamma constructor\n");
	cutCBTime[0]	= -100000;
	cutCBTime[1]	= 100000;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hIM[0]	= (TH1D*)gROOT->Get("2G_IM")))
		hIM[0]		= new TH1D("2G_IM", "2G_IM", 1600, 0, 1600);
	if(!(hIM[1]	= (TH1D*)gROOT->Get("2G_IM_Cut")))
		hIM[1]		= new TH1D("2G_IM_Cut", "2G_IM_Cut", 300, 800, 1100);
		
	Clear();
}
TreeAnalyseEtaP2Gamma::~TreeAnalyseEtaP2Gamma()
{
	if(out2g)
		delete	out2g;
	if(outFile2g)
		delete	outFile2g;
}


void	TreeAnalyseEtaP2Gamma::Open()
{
	//printf("TreeAnalyseEtaP2Gamma::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseEtaP2Gamma::Open 2()\n");
	
	Char_t	str[128];
	sprintf(str, "%s_2g_IM.root", GetFileName());
	outFile2g		= new TFile(str, "RECREATE");
	out2g			= new TTree("tree", "tree");
	
	out2g->Branch("nTagged",&nTagged,"nTagged/I");
	out2g->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	out2g->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
	
	out2g->Branch("nCBHits",&nCBHits,"nCBHits/I");
	out2g->Branch("Px", Px, "Px[nCBHits]/D");
	out2g->Branch("Py", Py, "Py[nCBHits]/D");
	out2g->Branch("Pz", Pz, "Pz[nCBHits]/D");
	out2g->Branch("E", E, "E[nCBHits]/D");	
	out2g->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	
	printf("TreeAnalyseEtaP2Gamma::Open 2()\n");
}

bool	TreeAnalyseEtaP2Gamma::AnalyseEvent(const Int_t i)
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
			hIM[0]->Fill(massAll);
			if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
			{
				out2g->Fill();
				hIM[1]->Fill(massAll);
			}
		}
	}	
}
void	TreeAnalyseEtaP2Gamma::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseEtaP2Gamma::Save()
{
	outFile2g->cd();
	out2g->Write();
	outFile2g->Flush();
	
	Char_t	str[128];
	sprintf(str, "%s_hist_2g_IM.root", GetFileName());
	TFile*		result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hIM[0]->Write();
	hIM[1]->Write();
	result->Close();
	delete result;
}

