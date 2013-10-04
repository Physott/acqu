#include "TreeReadTagged.C"
#include "TreeHistGeneralTagged.C"

#define MASS_PROTON	938.27203

class	TreeAnalyse2Gamma	: public TreeReadTagged
{
private:	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*					hCutIMCount;
	TreeHistGeneralTagged	hIMAll;
	TreeHistGeneralTagged	hIM[3];		//[Pi0, Eta, EtaP]
	
	
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
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			
	
	static	TreeAnalyse2Gamma*	test()
	{
		printf("Creating\n");
		TreeAnalyse2Gamma* c = new TreeAnalyse2Gamma("tree_TTreeOutput_41941_2g.root");
		printf("Opening\n");
		c->Open();
		printf("Analysing\n");
		c->Analyse();
		printf("Saving\n");
		c->Save();
		printf("End\n");
		return c;
	}
};





TreeAnalyse2Gamma::TreeAnalyse2Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hCutIMCount	= (TH1I*)gROOT->Get("IMCutCount")))
		hCutIMCount		= new TH1I("IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
			
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
	if(!hIMAll.Init(BaseName, BaseName, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hIMAll could not been initiated\n");
	}
	
	TString	Name[19];
	NBin[4]	= 300;
	Max[4]	= 300;
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("Pi0_");
	}
	if(!hIM[0].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hIM[0][0] could not been initiated\n");
	}
	
	Min[4]	=	400;
	Max[4]	=	700;
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("Eta_");
	}
	if(!hIM[1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hIM[1][0] could not been initiated\n");
	}
	
	NBin[4]	=	450;
	Min[4]	=	700;
	Max[4]	=	1150;
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_");
	}
	if(!hIM[2].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hIM[2][0] could not been initiated\n");
	}
		
	Clear();
}
TreeAnalyse2Gamma::~TreeAnalyse2Gamma()
{
	
}

inline	void	TreeAnalyse2Gamma::Clear()	
{
	hCutIMCount->Reset("M");
	hIMAll.Clear();
	for(int i=0; i<3; i++)
		hIM[i].Clear();
}


/*bool	TreeAnalyse2Gamma::CutMissMass(const Int_t type)
{
	hCutMMCount[type]->Fill(1);
	for(int l=0; l<3; l++)
	{
		nBeam[l]	= 0;
		for(int i=0; i<nTagged[l]; i++)
		{
			beamEnergy[l][nBeam[l]]	= TaggedEnergy[l][i];
			beam[l][nBeam[l]].SetPxPyPzE(TaggedEnergy[l][i], 0.0, 0.0, TaggedEnergy[l][i] + MASS_PROTON);
			miss[l][nBeam[l]]		= beam[l][nBeam[l]] - vecAll;
			missMass[l][nBeam[l]]	= miss[l][nBeam[l]].M();
			hMM[type][0].Fill(l, missMass[l][nBeam[l]]);
			if(missMass[l][nBeam[l]] >= cutMM[0] && missMass[l][nBeam[l]] <= cutMM[1])
			{
				nBeam[l]++;
				hCutMMCount[type]->Fill(l+6);
			}
		}
	}
	
	bool	ret = false;
	for(int l=0; l<3; l++)
	{
		if(nBeam[l] > 0)
		{
			hCutMMCount[type]->Fill(l+2);
			ret	= true;
		}
	}
	if(ret)
	{
		hIM[type][1].Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		hMM[type][1].Fill(nBeam, missMass[0], missMass[1], missMass[2]);
		return true;
	}
	
	return false;
}*/

bool	TreeAnalyse2Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	massAll	= vecAll.M();
	
	hCutIMCount->Fill(1);
	hIMAll.Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutIMCount->Fill(2);
		hIM[0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		return true;
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutIMCount->Fill(3);
		hIM[1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		return true;
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutIMCount->Fill(4);
		hIM[2].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		return true;
	}
	
	return false;
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
	TString	BaseName[10];
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
	hIMAll.SubstractBackground(BaseName);
	
	TString	Name[10];
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("Pi0_");
	}
	hIM[0].SubstractBackground(Name);
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("Eta_");
	}
	hIM[1].SubstractBackground(Name);
	for(int i=0; i<10; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_");
	}
	hIM[2].SubstractBackground(Name);

	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hCutIMCount->Write();
	hIMAll.Save(true);
	hIM[0].Save(true);
	hIM[1].Save(true);
	hIM[2].Save(true);
	result->Close();
	delete result;
}

