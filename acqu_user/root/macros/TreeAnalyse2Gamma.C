#include "TreeRead.C"
#include "TreeHistGeneral.C"


class	TreeAnalyse2Gamma	: public TreeRead
{
private:
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*			hCutCount;
	TreeHistGeneral	hist[3];
	
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse2Gamma(const Char_t* FileName);
	~TreeAnalyse2Gamma();
	
			void	Clear();
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutIMPi0()		const	{return cutIMPi0;}
	const	Double_t	GetCutIMPi0Min()	const	{return cutIMPi0[0];}
	const	Double_t	GetCutIMPi0Max()	const	{return cutIMPi0[1];}
	const	Double_t*	GetCutIMEta()		const	{return cutIMEta;}
	const	Double_t	GetCutIMEtaMin()	const	{return cutIMEta[0];}
	const	Double_t	GetCutIMEtaMax()	const	{return cutIMEta[1];}
	const	Double_t*	GetCutIMEtaP()		const	{return cutIMEtaP;}
	const	Double_t	GetCutIMEtaPMin()	const	{return cutIMEtaP[0];}
	const	Double_t	GetCutIMEtaPMax()	const	{return cutIMEtaP[1];}
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
	
	
	static	TreeAnalyse2Gamma*	test()
	{
		TreeAnalyse2Gamma* c = new TreeAnalyse2Gamma("tree_TTreeOutput_41941_2g_Prompt.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};






TreeAnalyse2Gamma::TreeAnalyse2Gamma(const Char_t* FileName)	: TreeRead(FileName)
{	
	for(int i=0; i<3; i++)
	{
		 outTree[i]	= 0;
		 outFile[i]	= 0;
	}
	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("IMCutCount")))
		hCutCount	= new TH1I("IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
		
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
	Int_t		NBin[9]	=	{32,  200, 2000, 16, 2000, 2000, 300,  180,  360};
	Double_t	Min[9]	=	{ 0, 1400, -100,  0, -100,    0,   0,    0, -180};
	Double_t	Max[9]	=	{32, 1600,  100, 16,  100, 2000, 300,  180,  180};
	TString	Name[9];
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Pi0_");
	hist[0].Init(Name, Name, NBin, Min, Max);
	
	Min[6]	=	400;
	Max[6]	=	700;
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("Eta_");
	hist[1].Init(Name, Name, NBin, Min, Max);
	
	NBin[6]	=	450;
	Min[6]	=	700;
	Max[6]	=	1150;
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("EtaP_");
	hist[2].Init(Name, Name, NBin, Min, Max);
	
		
	Clear();
}
TreeAnalyse2Gamma::~TreeAnalyse2Gamma()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

inline	void	TreeAnalyse2Gamma::Clear()	
{
	hCutCount->Reset("M");
	hist[0].Clear();
	hist[1].Clear();
	hist[2].Clear();
}

void	TreeAnalyse2Gamma::Open()
{
	//printf("TreeAnalyse2Gamma::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyse2Gamma::Open 2()\n");
	
	Char_t	str[128];
	//printf("GetFileName: %s\n", GetFileName());
	sprintf(str, "tree_%s_IMPi0.root", GetFileName());
	outFile[0]	= new TFile(str, "RECREATE");
	outTree[0]	= new TTree("tree", "tree");
	sprintf(str, "tree_%s_IMEta.root", GetFileName());
	outFile[1]	= new TFile(str, "RECREATE");
	outTree[1]	= new TTree("tree", "tree");
	sprintf(str, "tree_%s_IMEtaP.root", GetFileName());
	outFile[2]	= new TFile(str, "RECREATE");
	outTree[2]	= new TTree("tree", "tree");
		
	for(int i=0; i<3; i++)
	{
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
}

bool	TreeAnalyse2Gamma::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	massAll	= vecAll.M();
	
	hCutCount->Fill(1);
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutCount->Fill(2);		
		outTree[0]->Fill();
		hist[0].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutCount->Fill(3);
		outTree[1]->Fill();
		hist[1].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutCount->Fill(4);
		outTree[2]->Fill();
		hist[2].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	}
	
}
void	TreeAnalyse2Gamma::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse2Gamma::Save()
{
	outFile[0]->cd();
	outTree[0]->Write();
	outFile[0]->Flush();
	
	outFile[1]->cd();
	outTree[1]->Write();
	outFile[1]->Flush();
	
	outFile[2]->cd();
	outTree[2]->Write();
	outFile[2]->Flush();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hCutCount->Write();
	hist[0].Save();
	hist[1].Save();
	hist[2].Save();
	result->Close();
	delete result;
}

