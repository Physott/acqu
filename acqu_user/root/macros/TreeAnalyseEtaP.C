#include "TreeRead.C"


class	TreeAnalyseEtaP	: public TreeRead
{
private:
	TFile*	outFile2g;
	TFile*	outFile6g[2];
	TTree*	out2g;
	TTree*	out6g[2];
	
	Double_t	cutCBTime[2];
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	bool	CutCBTime();

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseEtaP(const Char_t* FileName);
	~TreeAnalyseEtaP();
	
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save(const Char_t* FileName);
	
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
			
	static	TreeAnalyseEtaP*	test()
	{
		TreeAnalyseEtaP* c = new TreeAnalyseEtaP("TTreeOutput_41948_CUT_2_6_10.root");
		c->Open();
		c->SetCutCBTime(-50, 50);
		c->Analyse(0,1);
		c->Print();
		return c;
	}
};





bool	TreeAnalyseEtaP::CutCBTime()
{
	for(int i=0; i<nCBHits; i++)
	{
		if(CBTime[i] < cutCBTime[0] || CBTime[i] > cutCBTime[1])
			return false;
	}
	return true;
}



TreeAnalyseEtaP::TreeAnalyseEtaP(const Char_t* FileName)	: TreeRead(FileName), out2g(0), outFile2g(0)
{
	out6g[0]	= 0;
	out6g[1]	= 0;
	outFile6g[0]	= 0;
	outFile6g[1]	= 0;
	
	cutCBTime[0]	= -100000;
	cutCBTime[1]	= 100000;
	cutIMPi0[0]		= 120;
	cutIMPi0[1]		= 150;
	cutIMEta[0]		= 417;
	cutIMEta[1]		= 477;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
}
TreeAnalyseEtaP::~TreeAnalyseEtaP()
{
	if(out2g)
		delete	out2g;
	if(outFile2g)
		delete	outFile2g;
	if(out6g[0])
		delete	out6g[0];
	if(outFile6g[0])
		delete	outFile6g[0];
	if(out6g[1])
		delete	out6g[1];
	if(outFile6g[1])
		delete	outFile6g[1];
}


void	TreeAnalyseEtaP::Open()
{
	//printf("TreeAnalyseEtaP::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseEtaP::Open 2()\n");
	
	outFile2g		= new TFile("tree_2g", "RECREATE");
	out2g			= new TTree("tree2g", "tree2g");
	outFile6g[0]	= new TFile("tree_6g_EtaP", "RECREATE");
	out6g[0]		= new TTree("tree6gEtaP", "tree6gEtaP");
	outFile6g[1]	= new TFile("tree_6g_3Pi0", "RECREATE");
	out6g[1]		= new TTree("tree6g3Pi0", "tree6g3Pi0");
	
	out2g->Branch("nTagged",&nTagged,"nTagged/I");
	out2g->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	out2g->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
	
	out2g->Branch("nCBHits",&nCBHits,"nCBHits/I");
	out2g->Branch("Px", Px, "Px[nCBHits]/D");
	out2g->Branch("Py", Py, "Py[nCBHits]/D");
	out2g->Branch("Pz", Pz, "Pz[nCBHits]/D");
	out2g->Branch("E", E, "E[nCBHits]/D");	
	out2g->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	
	for(int i=0; i<2; i++)
	{
		out6g[i]->Branch("nTagged",&nTagged,"nTagged/I");
		out6g[i]->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
		out6g[i]->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
		out6g[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		out6g[i]->Branch("Px", Px, "Px[nCBHits]/D");
		out6g[i]->Branch("Py", Py, "Py[nCBHits]/D");
		out6g[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		out6g[i]->Branch("E", E, "E[nCBHits]/D");	
		out6g[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
}
bool	TreeAnalyseEtaP::AnalyseEvent(const Int_t i)
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
			if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
				out2g->Fill();
		}
	}
	else if(nCBHits == 6)
	{
		if(CutCBTime())
		{
		}
	}
	
}
void	TreeAnalyseEtaP::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseEtaP::Save(const Char_t* FileName)
{
	Char_t	str[128];
	
	outFile2g->cd();
	out2g->Write();
	outFile2g->Flush();
	
	outFile6g[0]->cd();
	out6g[0]->Write();
	outFile6g[0]->Flush();
	
	outFile6g[1]->cd();
	out6g[1]->Write();
	outFile6g[1]->Flush();
}


