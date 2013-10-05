#include "TreeReadTagged.C"
#include "TreeHist6gTagged.C"

#define MASS_PI0	135
#define MASS_ETA	547

class	TreeAnalyse6Gamma	: public TreeReadTagged
{
private:
	TFile*	outFile[2];
	TTree*	outTree[2];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	static	int		perm[15][6];
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	Double_t		massPart[3];
	Double_t		massAll;
	TLorentzVector	partSet[3];
	TLorentzVector	allSet;
	Double_t		massSet;
	
	Double_t		out[4][6];
	
	TH1I*				hCount;
	TreeHist6gTagged	hist[2][2];	//[Etap,3Pi0][All,IM]
	
	void	SetMass(const int index, const Double_t mass);
	void	Reconstruct();

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse6Gamma(const Char_t* FileName);
	~TreeAnalyse6Gamma();
	
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
			
	static	TreeAnalyse6Gamma*	test()
	{
		TreeAnalyse6Gamma* c = new TreeAnalyse6Gamma("tree_TTreeOutput_41941_6g.root");
		c->Open();
		c->Analyse(100);
		c->Save();
		return c;
	}
};





TreeAnalyse6Gamma::TreeAnalyse6Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
{
	outTree[0]	= 0;
	outTree[1]	= 0;
	outFile[0]	= 0;
	outFile[1]	= 0;
	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hCount	= (TH1I*)gROOT->Get("ReconstructCount")))
		hCount		= new TH1I("ReconstructCount", "1:All/2,3:(EtaP,3Pi0)/5,6:(EtaP,3Pi0)Passed IM", 8, 0, 8);
	
	TString	BaseName[40];
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
	BaseName[19]	= "Prompt_Pi0a";
	BaseName[20]	= "Rand1_Pi0a";
	BaseName[21]	= "Rand2_Pi0a";
	BaseName[22]	= "Prompt_Pi0b";
	BaseName[23]	= "Rand1_Pi0b";
	BaseName[24]	= "Rand2_Pi0b";
	BaseName[25]	= "Prompt_Eta";
	BaseName[26]	= "Rand1_Eta";
	BaseName[27]	= "Rand2_Eta";
	BaseName[28]	= "Prompt_Pi0aDiced";
	BaseName[29]	= "Rand1_Pi0aDiced";
	BaseName[30]	= "Rand2_Pi0aDiced";
	BaseName[31]	= "Prompt_Pi0bDiced";
	BaseName[32]	= "Rand1_Pi0bDiced";
	BaseName[33]	= "Rand2_Pi0bDiced";
	BaseName[34]	= "Prompt_EtaDiced";
	BaseName[35]	= "Rand1_EtaDiced";
	BaseName[36]	= "Rand2_EtaDiced";
	BaseName[37]	= "Prompt_IMAllSet";
	BaseName[38]	= "Rand1_IMAllSet";
	BaseName[39]	= "Rand2_IMAllSet";
	Int_t		NBin[14]	=	{8,  200, 16, 2000, 2000,  180,  360, 500, 500, 1000, 500, 500, 1000, 2000};
	Double_t	Min[14]		=	{0, 1400,  0,    0,    0,    0, -180,   0,   0,    0,   0,   0,    0,    0};
	Double_t	Max[14]		=	{8, 1600, 16, 2000, 2000,  180,  180, 500, 500, 1000, 500, 500, 1000, 2000};
	TString	Name[40];
	for(int i=0; i<40; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_");
	}
	if(!hist[0][0].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hist[0][0] could not been initiated\n");
	}
	hist[0][0].SetEta2Pi0();
	for(int i=0; i<40; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_IM_");
	}
	if(!hist[0][1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hist[0][1] could not been initiated\n");
	}
	hist[0][1].SetEta2Pi0();
	
	BaseName[25]	= "Prompt_Pi0c";
	BaseName[26]	= "Rand1_Pi0c";
	BaseName[27]	= "Rand2_Pi0c";
	BaseName[34]	= "Prompt_Pi0cDiced";
	BaseName[35]	= "Rand1_Pi0cDiced";
	BaseName[36]	= "Rand2_Pi0cDiced";
	NBin[9]			= 500;
	Max[9]			= 500;
	for(int i=0; i<40; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("3Pi0_");
	}
	if(!hist[1][0].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hist[1][0] could not been initiated\n");
	}
	for(int i=0; i<40; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("3Pi0_IM_");
	}
	if(!hist[1][1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyseTagger Constructor: hist[1][1] could not been initiated\n");
	}
}
TreeAnalyse6Gamma::~TreeAnalyse6Gamma()
{
	for(int i=0; i<2; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

void	TreeAnalyse6Gamma::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}

void	TreeAnalyse6Gamma::Clear()
{
	hCount->Reset("M");
	for(int i=0; i<2; i++)
	{
		for(int l=0; l<2; l++)
		{
			hist[i][l].Clear();
		}
	}
}

void	TreeAnalyse6Gamma::Open()
{
	//printf("TreeAnalyse6Gamma::Open()\n");
	TreeReadTagged::Open();
	//printf("TreeAnalyse6Gamma::Open 2()\n");
	
	Char_t	str[128];	
	
	for(int i=0; i<2; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_IMEtaP.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_IM3Pi0.root", GetFileName());
			
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
		outTree[i]->Branch("Px", out[0], "Px[nCBHits]/D");
		outTree[i]->Branch("Py", out[1], "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", out[2], "Pz[nCBHits]/D");
		outTree[i]->Branch("E", out[3], "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");

	}
}
bool	TreeAnalyse6Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
	vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
	vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
	vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
	vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
	massAll	= vecAll.M();
	
	Reconstruct();
	
	hCount->Fill(1);
	
	switch(bestEta)
	{
	case 0:
		massPart[0]	= mass[bestPerm][1];
		massPart[1]	= mass[bestPerm][2];
		massPart[2]	= mass[bestPerm][0];
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hist[0][0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][0]<cutIMEta[0] || mass[bestPerm][0]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hist[0][1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		outTree[0]->Fill();
		
		break;
	case 1:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][2];
		massPart[2]	= mass[bestPerm][1];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_ETA);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hist[0][0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMEta[0] || mass[bestPerm][1]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hist[0][1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		out[0][0]	= vec[perm[bestPerm][2]].Px();
		out[1][0]	= vec[perm[bestPerm][2]].Py();
		out[2][0]	= vec[perm[bestPerm][2]].Pz();
		out[3][0]	= vec[perm[bestPerm][2]].E();
		out[0][1]	= vec[perm[bestPerm][3]].Px();
		out[1][1]	= vec[perm[bestPerm][3]].Py();
		out[2][1]	= vec[perm[bestPerm][3]].Pz();
		out[3][1]	= vec[perm[bestPerm][3]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][4]].Px();
		out[1][4]	= vec[perm[bestPerm][4]].Py();
		out[2][4]	= vec[perm[bestPerm][4]].Pz();
		out[3][4]	= vec[perm[bestPerm][4]].E();
		out[0][5]	= vec[perm[bestPerm][5]].Px();
		out[1][5]	= vec[perm[bestPerm][5]].Py();
		out[2][5]	= vec[perm[bestPerm][5]].Pz();
		out[3][5]	= vec[perm[bestPerm][5]].E();
		
		outTree[0]->Fill();
		
		break;
	case 2:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][1];
		massPart[2]	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_ETA);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hist[0][0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMEta[0] || mass[bestPerm][2]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hist[0][1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		out[0][0]	= vec[perm[bestPerm][4]].Px();
		out[1][0]	= vec[perm[bestPerm][4]].Py();
		out[2][0]	= vec[perm[bestPerm][4]].Pz();
		out[3][0]	= vec[perm[bestPerm][4]].E();
		out[0][1]	= vec[perm[bestPerm][5]].Px();
		out[1][1]	= vec[perm[bestPerm][5]].Py();
		out[2][1]	= vec[perm[bestPerm][5]].Pz();
		out[3][1]	= vec[perm[bestPerm][5]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][2]].Px();
		out[1][4]	= vec[perm[bestPerm][2]].Py();
		out[2][4]	= vec[perm[bestPerm][2]].Pz();
		out[3][4]	= vec[perm[bestPerm][2]].E();
		out[0][5]	= vec[perm[bestPerm][3]].Px();
		out[1][5]	= vec[perm[bestPerm][3]].Py();
		out[2][5]	= vec[perm[bestPerm][3]].Pz();
		out[3][5]	= vec[perm[bestPerm][3]].E();
		
		outTree[0]->Fill();
		
		break;
	case 3:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][1];
		massPart[2]	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(3);
		hist[1][0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		
		hCount->Fill(6);
		hist[1][1].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTree[1]->Fill();
		
		break;
	}
	
	return true;
}
void	TreeAnalyse6Gamma::Reconstruct()
{
	for(int i=0; i<15; i++)
	{
		part[i][0]	= vec[perm[i][0]] + vec[perm[i][1]];
		part[i][1]	= vec[perm[i][2]] + vec[perm[i][3]];
		part[i][2]	= vec[perm[i][4]] + vec[perm[i][5]];
		
		mass[i][0]	= part[i][0].M();
		mass[i][1]	= part[i][1].M();
		mass[i][2]	= part[i][2].M();
		
		ChiSqr[i][0]	= ((mass[i][0]-MASS_ETA)*(mass[i][0]-MASS_ETA)/(1600)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
		ChiSqr[i][1]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_ETA)*(mass[i][1]-MASS_ETA)/(1600)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
		ChiSqr[i][2]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_ETA)*(mass[i][1]-MASS_ETA)/(1600));
		ChiSqr[i][3]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
	}
	Double_t		minChiSqr	= ChiSqr[0][0];
					bestPerm	= 0;
					bestEta		= 0;
	for(int i=0; i<15; i++)
	{
		for(int l=0; l<4; l++)
		{
			if(ChiSqr[i][l] < minChiSqr)
			{
				minChiSqr	= ChiSqr[i][l];
				bestPerm	= i;
				bestEta		= l;
			}
		}
	}
	
	massAll = vecAll.M();
	partSet[0]	= part[bestPerm][0];
	partSet[1]	= part[bestPerm][1];
	partSet[2]	= part[bestPerm][2];
	
}
void	TreeAnalyse6Gamma::Analyse(const Int_t Min, const Int_t Max)
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
	{
		if((i%1000)==0)
			printf("Analyse Event: %d\n", i);
		AnalyseEvent(i);
	}
}
bool	TreeAnalyse6Gamma::Save()
{
	
	outFile[0]->cd();
	outTree[0]->Write();
	outFile[0]->Flush();
	
	outFile[1]->cd();
	outTree[1]->Write();
	outFile[1]->Flush();
	
	TString	BaseName[24];
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
	BaseName[10]	= "BG_IMPi0a";
	BaseName[11]	= "Result_IMPi0a";
	BaseName[12]	= "BG_IMPi0b";
	BaseName[13]	= "Result_IMPi0b";
	BaseName[14]	= "BG_IMEta";
	BaseName[15]	= "Result_IMEta";
	BaseName[16]	= "BG_IMPi0aDiced";
	BaseName[17]	= "Result_IMPi0aDiced";
	BaseName[18]	= "BG_IMPi0bDiced";
	BaseName[19]	= "Result_IMPi0bDiced";
	BaseName[20]	= "BG_IMEtaDiced";
	BaseName[21]	= "Result_IMEtaDiced";
	BaseName[22]	= "BG_IMAllSet";
	BaseName[23]	= "Result_IMAllSet";
	TString	Name[24];
	for(int i=0; i<24; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_");
	}
	hist[0][0].SubstractBackground(Name);
	for(int i=0; i<24; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("EtaP_IM_");
	}
	hist[0][1].SubstractBackground(Name);
	
	BaseName[14]	= "BG_IMPi0c";
	BaseName[15]	= "Result_IMPi0c";
	BaseName[20]	= "BG_IMPi0cDiced";
	BaseName[21]	= "Result_IMPi0cDiced";
	for(int i=0; i<24; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("3Pi0_");
	}
	hist[1][0].SubstractBackground(Name);
	for(int i=0; i<24; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("3Pi0_IM_");
	}
	hist[1][1].SubstractBackground(Name);
	
	
	
	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCount->Write();
	hist[0][0].Save(true);
	hist[0][1].Save(true);
	hist[1][0].Save(true);
	hist[1][1].Save(true);
	result->Close();
	delete result;
	
	return true;
}


int		TreeAnalyse6Gamma::perm[15][6]=
{
	{0,1,2,3,4,5},
	{0,1,2,4,3,5},
	{0,1,2,5,4,3},
	
	{0,2,1,3,4,5},
	{0,2,1,4,3,5},
	{0,2,1,5,4,3},
	
	{0,3,2,1,4,5},
	{0,3,2,4,1,5},
	{0,3,2,5,4,1},
	
	{0,4,2,3,1,5},
	{0,4,2,1,3,5},
	{0,4,2,5,1,3},
	
	{0,5,2,3,4,1},
	{0,5,2,4,3,1},
	{0,5,2,1,4,3},
};

