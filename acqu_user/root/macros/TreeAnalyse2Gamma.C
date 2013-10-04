#include "TreeRead.C"
#include "TreeHistTagged.C"


class	TreeAnalyse2Gamma	: public TreeRead
{
private:	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	Double_t	cutMM[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*			hCutIMCount;
	TH1I*			hCutMMCount[3];
	TH1D*			hMM[3];
	TH1D*			hCutMMCheck;
	TreeHistTagged	histAll;
	TreeHistTagged	hist[3][2];		//[Pi0, Eta, EtaP][All, MM]
	
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse2Gamma(const Char_t* FileName);
	~TreeAnalyse2Gamma();
	
			void	Clear();
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
	const	Double_t*	GetCutMM()			const	{return cutMM;}
	const	Double_t	GetCutMMMin()		const	{return cutMM[0];}
	const	Double_t	GetCutMMMax()		const	{return cutM[1];}
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			void		SetCutMM(const Double_t Min, const Double_t Max)		{cutMM[0] = Min    ; cutMM[1] = Max;}
			
	
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
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	cutMM[0]		= 800;
	cutMM[1]		= 1070;
	
	if(!(hCutIMCount	= (TH1I*)gROOT->Get("IMCutCount")))
		hCutIMCount		= new TH1I("IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
	if(!(hCutMMCount[0]	= (TH1I*)gROOT->Get("Pi0_MMCutCount")))
		hCutMMCount[0]	= new TH1I("Pi0_MMCutCount", "1:All/2:Passed", 4, 0, 4);
	if(!(hCutMMCount[1]	= (TH1I*)gROOT->Get("Eta_MMCutCount")))
		hCutMMCount[1]	= new TH1I("Eta_MMCutCount", "1:All/2:Passed", 4, 0, 4);
	if(!(hCutMMCount[2]	= (TH1I*)gROOT->Get("EtaP_MMCutCount")))
		hCutMMCount[2]	= new TH1I("EtaP_MMCutCount", "1:All/2:Passed", 4, 0, 4);
	if(!(hMM[0]			= (TH1D*)gROOT->Get("Pi0_MM")))
		hMM[0]			= new TH1D("Pi0_MM", "Pi0_MM", 4000, -2000, 2000);
	if(!(hMM[1]			= (TH1D*)gROOT->Get("Eta_MM")))
		hMM[1]			= new TH1D("Eta_MM", "Eta_MM", 4000, -2000, 2000);
	if(!(hMM[1]			= (TH1D*)gROOT->Get("EtaP_MM")))
		hMM[1]			= new TH1D("EtaP_MM", "EtaP_MM", 4000, -2000, 2000);
	if(!(hCutMMCheck	= (TH1D*)gROOT->Get("CutMMCheck")))
		hCutMMCheck		= new TH1D("CutMMCheck", "CutMMCheck", 4000, -2000, 2000);
		
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
	TString	Name[3][9];
	histAll.Init(BaseName, BaseName, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[i];
		Name[1][i]	= BaseName[i];
		Name[2][i]	= BaseName[i];
		Name[0][i].Prepend("Pi0_Prompt_");
		Name[1][i].Prepend("Pi0_Rand1_");
		Name[2][i].Prepend("Pi0_Rand2_");
	}
	hist[0][0].Init(Name, Name, NBin, Min, Max);
	
	Min[6]	=	400;
	Max[6]	=	700;
	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[0][i];
		Name[1][i]	= BaseName[1][i];
		Name[2][i]	= BaseName[2][i];
		Name[0][i].Prepend("Eta_Prompt_");
		Name[1][i].Prepend("Eta_Rand1_");
		Name[2][i].Prepend("Eta_Rand2_");
	}
	hist[1][0].Init(Name, Name, NBin, Min, Max);
	
	NBin[6]	=	450;
	Min[6]	=	700;
	Max[6]	=	1150;
	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[0][i];
		Name[1][i]	= BaseName[1][i];
		Name[2][i]	= BaseName[2][i];
		Name[0][i].Prepend("EtaP_Prompt_");
		Name[1][i].Prepend("EtaP_Rand1_");
		Name[2][i].Prepend("EtaP_Rand2_");
	}
	hist[2][0].Init(Name, Name, NBin, Min, Max);


	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[0][i];
		Name[1][i]	= BaseName[1][i];
		Name[2][i]	= BaseName[2][i];
		Name[0][i].Prepend("Pi0_MM_Prompt_");
		Name[1][i].Prepend("Pi0_MM_Rand1_");
		Name[2][i].Prepend("Pi0_MM_Rand2_");
	}
	hist[0][1].Init(Name, Name, NBin, Min, Max);
	
	Min[6]	=	400;
	Max[6]	=	700;
	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[0][i];
		Name[1][i]	= BaseName[1][i];
		Name[2][i]	= BaseName[2][i];
		Name[0][i].Prepend("Eta_MM_Prompt_");
		Name[1][i].Prepend("Eta_MM_Rand1_");
		Name[2][i].Prepend("Eta_MM_Rand2_");
	}
	hist[1][1].Init(Name, Name, NBin, Min, Max);
	
	NBin[6]	=	450;
	Min[6]	=	700;
	Max[6]	=	1150;
	for(int i=0; i<9; i++)
	{
		Name[0][i]	= BaseName[0][i];
		Name[1][i]	= BaseName[1][i];
		Name[2][i]	= BaseName[2][i];
		Name[0][i].Prepend("EtaP_MM_Prompt_");
		Name[1][i].Prepend("EtaP_MM__Rand1_");
		Name[2][i].Prepend("EtaP_MM__Rand2_");
	}
	hist[2][1].Init(Name, Name, NBin, Min, Max);
	
		
	Clear();
}
TreeAnalyse2Gamma::~TreeAnalyse2Gamma()
{
	
}

inline	void	TreeAnalyse2Gamma::Clear()	
{
	hCutCount->Reset("M");
	histAll->Clear();
	for(int i=0; i<3; i++)
	{
		hCutMMCount[i]->Reset("M");
		hMM[i]->Reset("M");
		hist[i][0].Clear();
		hist[i][1].Clear();
	}
}

bool	TreeAnalyse2Gamma::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	massAll	= vecAll.M();
	
	hCutIMCount->Fill(1);
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutIMCount->Fill(2);
		hist[0][0].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutIMCount->Fill(3);
		hist[1][0].Fill(1, nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutIMCount->Fill(4);
		hist[2][0].Fill(2, nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
	/*
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
	delete result;*/
}

