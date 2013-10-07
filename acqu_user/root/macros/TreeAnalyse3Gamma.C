#include "TreeReadTagged.C"
#include "TreeHistGeneralTagged.C"
#include "TreeHistTagged2D.C"

#define MASS_PROTON	938.27203

class	TreeAnalyse3Gamma	: public TreeReadTagged
{
private:	
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[3];
	TLorentzVector	vecParticle[3];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*					hCutIMCount;
	TreeHistTagged			hIMParticle[3];
	TreeHistTagged			hIMParticleDiced[3];
	TreeHistTagged2D		hProtonAngleDiff[3];
	TreeHistGeneralTagged	hIMAll;
	TreeHistGeneralTagged	hIM[3];		//[Pi0, Eta, EtaP]
	
	bool	FindProton();
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse3Gamma(const Char_t* FileName);
	~TreeAnalyse3Gamma();
	
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
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			
	
	static	TreeAnalyse3Gamma*	test()
	{
		printf("Creating\n");
		TreeAnalyse3Gamma* c = new TreeAnalyse3Gamma("tree_TTreeOutput_41941_3g.root");
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





TreeAnalyse3Gamma::TreeAnalyse3Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
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
	for(int i=0; i<3; i++)
	{
		BaseName[0]		= "Prompt_IMParticlePerm_";
		BaseName[0]		+= i; 
		BaseName[1]		= "Rand1_IMParticlePerm_";
		BaseName[1]		+= i; 
		BaseName[2]		= "Rand2_IMParticlePerm_";
		BaseName[2]		+= i; 
		if(!hIMParticle[i].Init(BaseName, BaseName, 1600, 0, 1600))
		{
			printf("ERROR: TreeAnalyse3Gamma Constructor: hIMParticle[%d] could not been initiated\n", i);
		}
		
		BaseName[0]		= "Prompt_IMParticlePermDiced_";
		BaseName[0]		+= i; 
		BaseName[1]		= "Rand1_IMParticlePermDiced_";
		BaseName[1]		+= i; 
		BaseName[2]		= "Rand2_IMParticlePermDiced_";
		BaseName[2]		+= i; 
		if(!hIMParticle[i].Init(BaseName, BaseName, 1600, 0, 1600))
		{
			printf("ERROR: TreeAnalyse3Gamma Constructor: hIMParticle[%d] could not been initiated\n", i);
		}
		
		BaseName[0]		= "Prompt_AngleDiff_";
		BaseName[0]		+= i; 
		BaseName[1]		= "Rand1_AngleDiff_";
		BaseName[1]		+= i; 
		BaseName[2]		= "Rand2_AngleDiff_";
		BaseName[2]		+= i; 
		if(!hProtonAngleDiff[i].Init(BaseName, BaseName, 1600, 0, 1600))
		{
			printf("ERROR: TreeAnalyse3Gamma Constructor: hIMParticle[%d] could not been initiated\n", i);
		}
	}
		
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
	Int_t		NBin[7]	=	{8,  200, 16, 2000, 2000,  180,  360};
	Double_t	Min[7]	=	{0, 1400,  0,    0,    0,    0, -180};
	Double_t	Max[7]	=	{8, 1600, 16, 2000, 2000,  180,  180};
	if(!hIMAll.Init(BaseName, BaseName, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse3Gamma Constructor: hIMAll could not been initiated\n");
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
TreeAnalyse3Gamma::~TreeAnalyse3Gamma()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

inline	void	TreeAnalyse3Gamma::Clear()	
{
	hCutIMCount->Reset("M");
	hIMAll.Clear();
	for(int i=0; i<3; i++)
	{
		hIMParticle[i].Clear();
		hIMParticleDiced[i].Clear();
		hProtonAngleDiff[i].Clear();
		hIM[i].Clear();
	}
}

void	TreeAnalyse3Gamma::Open()
{
	//printf("TreeAnalyseMultiplicity::Open()\n");
	TreeReadTagged::Open();
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
	
	Char_t	str[128];
	for(int i=0; i<3; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_IMPi0.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_IMEta.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_IMEtaP.root", GetFileName());
			
		outFile[i]		= new TFile(str, "RECREATE");
		outTree[i]		= new TTree("tree", "tree");
		
		outTree[i]->Branch("nPrompt",&nTagged[0],"nPrompt/I");
		outTree[i]->Branch("PromptEnergy", TaggedEnergy[0], "PromptEnergy[nPrompt]/D");
		outTree[i]->Branch("PromptTime", TaggedTime[0], "PromptTime[nPrompt]/D");
		outTree[i]->Branch("nRand1",&nTagged[1],"nRand1/I");
		outTree[i]->Branch("Rand1Energy", TaggedEnergy[1], "Rand1Energy[nRand1]/D");
		outTree[i]->Branch("Rand1Time", TaggedTime[1], "Rand1Time[nRand1]/D");
		outTree[i]->Branch("nRand2",&nTagged[2],"nRand2/I");
		outTree[i]->Branch("Rand2Energy", TaggedEnergy[2], "Rand2Energy[nRand2]/D");
		outTree[i]->Branch("Rand2Time", TaggedTime[2], "Rand2Time[nRand2]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

bool	TreeAnalyse3Gamma::FindProton()
{
	vecParticle[0]	= vec[1] + vec[2];
	vecParticle[1]	= vec[0] + vec[2];
	vecParticle[2]	= vec[0] + vec[1];
	
	hIMParticle[0].Fill(nTagged, vecParticle[0].M());
	hIMParticle[1].Fill(nTagged, vecParticle[1].M());
	hIMParticle[2].Fill(nTagged, vecParticle[2].M());
	
	static	int	dice = 0;
	if(dice == 0)
	{
		hIMParticleDiced[0].Fill(nTagged, vecParticle[0].M());
		hIMParticleDiced[1].Fill(nTagged, vecParticle[1].M());
		hIMParticleDiced[2].Fill(nTagged, vecParticle[2].M());
		dice = 1;
	}
	else if(dice == 1)
	{
		hIMParticleDiced[0].Fill(nTagged, vecParticle[1].M());
		hIMParticleDiced[1].Fill(nTagged, vecParticle[2].M());
		hIMParticleDiced[2].Fill(nTagged, vecParticle[0].M());
		dice = 2;
	}
	else
	{
		hIMParticleDiced[0].Fill(nTagged, vecParticle[2].M());
		hIMParticleDiced[1].Fill(nTagged, vecParticle[0].M());
		hIMParticleDiced[2].Fill(nTagged, vecParticle[1].M());
		dice = 0;
	}
	
	TLorentzVector	beam;
	TLorentzVector	miss;
}

bool	TreeAnalyse3Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
	
	FindProton();
	//***********************************************************************************************
	
	vecAll	= vec[0] + vec[1] + vec[2];
	massAll	= vecAll.M();
	
	hCutIMCount->Fill(1);
	hIMAll.Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutIMCount->Fill(2);
		hIM[0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[0]->Fill();
		return true;
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutIMCount->Fill(3);
		hIM[1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[1]->Fill();
		return true;
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutIMCount->Fill(4);
		hIM[2].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[2]->Fill();
		return true;
	}
	
	return false;
}
void	TreeAnalyse3Gamma::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse3Gamma::Save()
{
	for(int i=0; i<3; i++)
	{
		outFile[i]->cd();
		outTree[i]->Write();
		outFile[i]->Flush();
	}
	
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
	
	hIMParticle[0].SubstractBackground("BG_ParticlePerm_0", "Result_ParticlePerm_0");
	hIMParticle[1].SubstractBackground("BG_ParticlePerm_1", "Result_ParticlePerm_1");
	hIMParticle[2].SubstractBackground("BG_ParticlePerm_2", "Result_ParticlePerm_2");

	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hCutIMCount->Write();
	hIMParticle[0].Save();	hIMParticle[0].Save(3);	hIMParticle[0].Save(4);
	hIMParticle[1].Save();	hIMParticle[1].Save(3);	hIMParticle[1].Save(4);
	hIMParticle[2].Save();	hIMParticle[2].Save(3);	hIMParticle[2].Save(4);
	hIMAll.Save(true);
	hIM[0].Save(true);
	hIM[1].Save(true);
	hIM[2].Save(true);
	result->Close();
	delete result;
}

