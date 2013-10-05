#include "TreeReadTagged.C"
#include "TreeHist6gTagged.C"


#define MASS_PI0	135
#define MASS_ETA	547

#define MASS_PROTON	938.27203


class	TreeAnalyse6GammaMissMass	: public TreeReadTagged
{
private:
	Double_t	cut[2];
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Double_t		massAll;
	TLorentzVector	vecPart[3];
	TLorentzVector	partSet[3];
	Double_t		massPart[3];
	TLorentzVector	allSet;
	Double_t		massSet;
	
	Int_t			nBeam[3];
	TLorentzVector	beam[3][8];
	Double_t		beamEnergy[3][8];
	TLorentzVector	miss[3][8];
	Double_t		missMass[3][8];
	
	TH1I*				hCutCount;
	TH1D*				hMissMass;
	TH1D*				hCutCheck;
	Bool_t				isEta2Pi0;
	TreeHist6gTagged	hist[2];


protected:	
	
	void	SetMass(const int index, const Double_t mass);
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse6GammaMissMass(const Char_t* FileName, const Bool_t IsEta2Pi0 = false);
	~TreeAnalyse6GammaMissMass();
	
			void	Clear();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	

	static	TreeAnalyse6GammaMissMass*	test()
	{
		TreeAnalyse6GammaMissMass* c = new TreeAnalyse6GammaMissMass("tree_TTreeOutput_41943_6g_IM3Pi0.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};








TreeAnalyse6GammaMissMass::TreeAnalyse6GammaMissMass(const Char_t* FileName, const Bool_t IsEta2Pi0)	: TreeReadTagged(FileName), isEta2Pi0(IsEta2Pi0)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount	= new TH1I("CutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hMissMass	= (TH1D*)gROOT->Get("MissMass")))
		hMissMass	= new TH1D("MissMass", "MissMass", 4000, -2000, 2000);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("CutCheck")))
		hCutCheck	= new TH1D("CutCheck", "CutCheck", 450, 700, 1150);
	
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
	BaseName[25]	= "Prompt_Pi0c";
	BaseName[26]	= "Rand1_Pi0c";
	BaseName[27]	= "Rand2_Pi0c";
	BaseName[28]	= "Prompt_Pi0aDiced";
	BaseName[29]	= "Rand1_Pi0aDiced";
	BaseName[30]	= "Rand2_Pi0aDiced";
	BaseName[31]	= "Prompt_Pi0bDiced";
	BaseName[32]	= "Rand1_Pi0bDiced";
	BaseName[33]	= "Rand2_Pi0bDiced";
	BaseName[34]	= "Prompt_Pi0cDiced";
	BaseName[35]	= "Rand1_Pi0cDiced";
	BaseName[36]	= "Rand2_Pi0cDiced";
	BaseName[37]	= "Prompt_IMAllSet";
	BaseName[38]	= "Rand1_IMAllSet";
	BaseName[39]	= "Rand2_IMAllSet";
	if(isEta2Pi0)
	{
		BaseName[25]	= "Prompt_Eta";
		BaseName[26]	= "Rand1_Eta";
		BaseName[27]	= "Rand2_Eta";
		BaseName[34]	= "Prompt_EtaDiced";
		BaseName[35]	= "Rand1_EtaDiced";
		BaseName[36]	= "Rand2_EtaDiced";
		
	}
	Int_t		NBin[14]	=	{8,  200, 16, 2000, 2000,  180,  360, 500, 500, 1000, 500, 500, 1000, 2000};
	Double_t	Min[14]		=	{0, 1400,  0,    0,    0,    0, -180,   0,   0,    0,   0,   0,    0,    0};
	Double_t	Max[14]		=	{8, 1600, 16, 2000, 2000,  180,  180, 500, 500, 1000, 500, 500, 1000, 2000};
	if(!hist[0].Init(BaseName, BaseName, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hist[0] could not been initiated\n");
	}
	TString	Name[40];
	for(int i=0; i<40; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("CutMM_");
	}
	if(!hist[1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hist[0][0] could not been initiated\n");
	}
	
	if(isEta2Pi0)
	{
		hist[0].SetEta2Pi0();
		hist[1].SetEta2Pi0();
	}
	
	Clear();
}
TreeAnalyse6GammaMissMass::~TreeAnalyse6GammaMissMass()
{
	
}


void	TreeAnalyse6GammaMissMass::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}

inline	void	TreeAnalyse6GammaMissMass::Clear()
{
	hCutCount->Reset("M");
	hMissMass->Reset("M");
	hCutCheck->Reset("M");
	hist[0].Clear();
	hist[1].Clear();
}

bool	TreeAnalyse6GammaMissMass::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
	vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
	vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
	vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
	vecPart[0]	= vec[0] + vec[1];
	vecPart[1]	= vec[2] + vec[3];
	vecPart[2]	= vec[4] + vec[5];
	partSet[0]	= vecPart[0];
	partSet[1]	= vecPart[1];
	partSet[2]	= vecPart[2];
	vecAll	= vecPart[0] + vecPart[1] + vecPart[2];
	massAll	= vecAll.M();
	if(isEta2Pi0)
	{
		massPart[0]	= vecPart[1].M();
		massPart[1]	= vecPart[2].M();
		massPart[2]	= vecPart[0].M();
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
	}
	else
	{
		massPart[0]	= vecPart[0].M();
		massPart[1]	= vecPart[1].M();
		massPart[2]	= vecPart[2].M();
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
	}
	allSet	= partSet[0] + partSet[1] + partSet[2];
	massSet	= allSet.M();
	
	hCutCount->Fill(1);
	hist[0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
	for(int l=0; l<3; l++)
	{
		nBeam[l]	= 0;
		for(int i=0; i<nTagged[l]; i++)
		{
			beamEnergy[l][nBeam[l]]	= TaggedEnergy[l][i];
			beam[l][nBeam[l]].SetPxPyPzE(TaggedEnergy[l][i], 0.0, 0.0, TaggedEnergy[l][i] + MASS_PROTON);
			miss[l][nBeam[l]]		= beam[l][nBeam[l]] - vecAll;
			missMass[l][nBeam[l]]	= miss[l][nBeam[l]].M();
			hMissMass->Fill(missMass[l][nBeam[l]]);
			if(missMass[l][nBeam[l]] >= cut[0] && missMass[l][nBeam[l]] <= cut[1])
			{
				nBeam[l]++;
				hCutCount->Fill(l+6);
				hCutCheck->Fill(missMass[l][nBeam[l]]);
			}
		}
	}
	
	bool	ret = false;
	for(int l=0; l<3; l++)
	{
		if(nBeam[l] > 0)
		{
			hCutCount->Fill(l+2);
			ret	= true;
		}
	}
	if(ret)
	{
		hist[1].Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCBHits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi());
		return true;
	}
	
	return false;

}
void	TreeAnalyse6GammaMissMass::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse6GammaMissMass::Save()
{
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
	BaseName[14]	= "BG_IMPi0c";
	BaseName[15]	= "Result_IMPi0c";
	BaseName[16]	= "BG_IMPi0aDiced";
	BaseName[17]	= "Result_IMPi0aDiced";
	BaseName[18]	= "BG_IMPi0bDiced";
	BaseName[19]	= "Result_IMPi0bDiced";
	BaseName[20]	= "BG_IMPi0cDiced";
	BaseName[21]	= "Result_IMPi0cDiced";
	BaseName[22]	= "BG_IMAllSet";
	BaseName[23]	= "Result_IMAllSet";
	if(isEta2Pi0)
	{
		BaseName[14]	= "BG_IMEta";
		BaseName[15]	= "Result_IMEta";
		BaseName[20]	= "BG_IMEtaDiced";
		BaseName[21]	= "Result_IMEtaDiced";
	}
	hist[0].SubstractBackground(BaseName);
	
	TString	Name[24];
	for(int i=0; i<24; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("CutMM_");
	}
	hist[1].SubstractBackground(Name);
	
	Char_t	str[128];
	sprintf(str, "hist_%s_MM.root", GetFileName());
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hist[0].Save(true);
	hCutCount->Write();
	hMissMass->Write();
	hCutCheck->Write();
	hist[1].Save(true);
	result->Close();
	delete result;
}

