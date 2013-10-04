#include "TreeReadTagged.C"
#include "TreeHistGeneralTagged.C"


#define MASS_PROTON	938.27203

class	TreeAnalyseMissMass	: public TreeReadTagged
{
private:
	Double_t	cut[2];
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Int_t			nBeam[3];
	TLorentzVector	beam[3][8];
	Double_t		beamEnergy[3][8];
	TLorentzVector	miss[3][8];
	Double_t		missMass[3][8];
	
	TH1I*					hCutCount;
	TH1D*					hMissMass;
	TH1D*					hCutCheck;
	TreeHistGeneralTagged	hist[2];

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseMissMass(const Char_t* FileName);
	~TreeAnalyseMissMass();
	
			void	Clear();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	

	static	TreeAnalyseMissMass*	test()
	{
		TreeAnalyseMissMass* c = new TreeAnalyseMissMass("tree_TTreeOutput_41941_2g.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};








TreeAnalyseMissMass::TreeAnalyseMissMass(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount	= new TH1I("CutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!(hMissMass	= (TH1D*)gROOT->Get("MissMass")))
		hMissMass	= new TH1D("MissMass", "MissMass", 4000, -2000, 2000);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("CutCheck")))
		hCutCheck	= new TH1D("CutCheck", "CutCheck", 450, 700, 1150);
	
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
	if(!hist[0].Init(BaseName, BaseName, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hIMAll could not been initiated\n");
	}
	
	TString	Name[19];
	for(int i=0; i<19; i++)
	{
		Name[i]	= BaseName[i];
		Name[i].Prepend("CutMM_");
	}
	if(!hist[1].Init(Name, Name, NBin, Min, Max))
	{
		printf("ERROR: TreeAnalyse2Gamma Constructor: hIMAll could not been initiated\n");
	}
	
	printf("Constructor\n");
	
	Clear();
	printf("Constructor\n");
}
TreeAnalyseMissMass::~TreeAnalyseMissMass()
{
	
}

inline	void	TreeAnalyseMissMass::Clear()
{
	hCutCount->Reset("M");
	hMissMass->Reset("M");
	hCutCheck->Reset("M");
	hist[0].Clear();
	hist[1].Clear();
}

bool	TreeAnalyseMissMass::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	if(nCBHits == 2)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
		vecAll	= vec[0] + vec[1];
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
	}
	else if(nCBHits == 10)
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
	}
	
	hCutCount->Fill(1);
	hist[0].Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
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
		hist[1].Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		return true;
	}
	
	return false;

}
void	TreeAnalyseMissMass::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseMissMass::Save()
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
	hist[0].SubstractBackground(BaseName);
	
	TString	Name[10];
	for(int i=0; i<10; i++)
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

