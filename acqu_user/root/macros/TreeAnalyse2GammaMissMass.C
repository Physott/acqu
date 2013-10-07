#include "TreeReadTagged.C"
#include "TreeHistGeneralTagged.C"


#define MASS_PROTON	938.27203

class	TreeAnalyse2GammaMissMass	: public TreeReadTagged
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
	TreeHistTagged			hMissP[5][2];	//[x,y,z,E,Mag]
	TH1D*					hMissMass;
	TH1D*					hCutCheck;
	TreeHistGeneralTagged	hist[2];

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse2GammaMissMass(const Char_t* FileName);
	~TreeAnalyse2GammaMissMass();
	
			void	Clear();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	

	static	TreeAnalyse2GammaMissMass*	test()
	{
		TreeAnalyse2GammaMissMass* c = new TreeAnalyse2GammaMissMass("tree_TTreeOutput_41941_2g_IMPi0.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};








TreeAnalyse2GammaMissMass::TreeAnalyse2GammaMissMass(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount	= new TH1I("CutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
		
	TString	BaseName[19];
	BaseName[0]		= "Prompt_MissPx";
	BaseName[1]		= "Rand1_MissPx";
	BaseName[2]		= "Rand2_MissPx";
	if(!hMissP[0][0].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[0][0] could not been initiated\n");
	BaseName[0]		= "Prompt_MissPy";
	BaseName[1]		= "Rand1_MissPy";
	BaseName[2]		= "Rand2_MissPy";
	if(!hMissP[1][0].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[1][0] could not been initiated\n");
	BaseName[0]		= "Prompt_MissPz";
	BaseName[1]		= "Rand1_MissPz";
	BaseName[2]		= "Rand2_MissPz";
	if(!hMissP[2][0].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[2][0] could not been initiated\n");
	BaseName[0]		= "Prompt_MissE";
	BaseName[1]		= "Rand1_MissE";
	BaseName[2]		= "Rand2_MissE";
	if(!hMissP[3][0].Init(BaseName, BaseName, 3000, 0, 3000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[3][0] could not been initiated\n");
	BaseName[0]		= "Prompt_MissP";
	BaseName[1]		= "Rand1_MissP";
	BaseName[2]		= "Rand2_MissP";
	if(!hMissP[4][0].Init(BaseName, BaseName, 3000, 0, 3000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[4][0] could not been initiated\n");
	
	
	BaseName[0]		= "CutMM_Prompt_MissPx";
	BaseName[1]		= "CutMM_Rand1_MissPx";
	BaseName[2]		= "CutMM_Rand2_MissPx";
	if(!hMissP[0][1].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[0][0] could not been initiated\n");
	BaseName[0]		= "CutMM_Prompt_MissPy";
	BaseName[1]		= "CutMM_Rand1_MissPy";
	BaseName[2]		= "CutMM_Rand2_MissPy";
	if(!hMissP[1][1].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[1][0] could not been initiated\n");
	BaseName[0]		= "CutMM_Prompt_MissPz";
	BaseName[1]		= "CutMM_Rand1_MissPz";
	BaseName[2]		= "CutMM_Rand2_MissPz";
	if(!hMissP[2][1].Init(BaseName, BaseName, 4000, -2000, 2000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[2][0] could not been initiated\n");
	BaseName[0]		= "CutMM_Prompt_MissE";
	BaseName[1]		= "CutMM_Rand1_MissE";
	BaseName[2]		= "CutMM_Rand2_MissE";
	if(!hMissP[3][1].Init(BaseName, BaseName, 3000, 0, 3000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[3][0] could not been initiated\n");
	BaseName[0]		= "CutMM_Prompt_MissP";
	BaseName[1]		= "CutMM_Rand1_MissP";
	BaseName[2]		= "CutMM_Rand2_MissP";
	if(!hMissP[4][1].Init(BaseName, BaseName, 3000, 0, 3000))
		printf("ERROR: TreeAnalyse2Gamma Constructor: hMissP[4][0] could not been initiated\n");
	
	if(!(hMissMass	= (TH1D*)gROOT->Get("MissMass")))
		hMissMass	= new TH1D("MissMass", "MissMass", 4000, -2000, 2000);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("hCutCheck")))
		hCutCheck	= new TH1D("hCutCheck", "hCutCheck", 450, 700, 1150);
	
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
	
	Clear();
}
TreeAnalyse2GammaMissMass::~TreeAnalyse2GammaMissMass()
{
	
}

inline	void	TreeAnalyse2GammaMissMass::Clear()
{
	hCutCount->Reset("M");
	for(int i=0; i<5; i++)
	{
		hMissP[i][0].Clear();
		hMissP[i][1].Clear();
	}
	hMissMass->Reset("M");
	hCutCheck->Reset("M");
	hist[0].Clear();
	hist[1].Clear();
}

bool	TreeAnalyse2GammaMissMass::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	
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
			hMissP[0][0].Fill(l, miss[l][nBeam[l]].Px());
			hMissP[1][0].Fill(l, miss[l][nBeam[l]].Py());
			hMissP[2][0].Fill(l, miss[l][nBeam[l]].Pz());
			hMissP[3][0].Fill(l, miss[l][nBeam[l]].E());
			hMissP[4][0].Fill(l, miss[l][nBeam[l]].P());
			if(missMass[l][nBeam[l]] >= cut[0] && missMass[l][nBeam[l]] <= cut[1])
			{
				nBeam[l]++;
				hCutCount->Fill(l+6);
				hCutCheck->Fill(missMass[l][nBeam[l]]);
				hMissP[0][1].Fill(l, miss[l][nBeam[l]].Px());
				hMissP[1][1].Fill(l, miss[l][nBeam[l]].Py());
				hMissP[2][1].Fill(l, miss[l][nBeam[l]].Pz());
				hMissP[3][1].Fill(l, miss[l][nBeam[l]].E());
				hMissP[4][1].Fill(l, miss[l][nBeam[l]].P());
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
void	TreeAnalyse2GammaMissMass::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse2GammaMissMass::Save()
{
	TString	BaseName[10];
	TString	Name[10];
	
	hMissP[0][0].SubstractBackground("BG_MissPx", "Result_MissPx");
	hMissP[0][1].SubstractBackground("CutMM_BG_MissPx", "CutMM_Result_MissPx");
	
	hMissP[1][0].SubstractBackground("BG_MissPy", "Result_MissPy");
	hMissP[1][1].SubstractBackground("CutMM_BG_MissPy", "CutMM_Result_MissPy");
	
	hMissP[2][0].SubstractBackground("BG_MissPz", "Result_MissPz");
	hMissP[2][1].SubstractBackground("CutMM_BG_MissPz", "CutMM_Result_MissPz");
	
	hMissP[3][0].SubstractBackground("BG_MissE", "Result_MissE");
	hMissP[3][1].SubstractBackground("CutMM_BG_MissE", "CutMM_Result_MissE");
	
	hMissP[4][0].SubstractBackground("BG_MissP", "Result_MissP");
	hMissP[4][1].SubstractBackground("CutMM_BG_MissP", "CutMM_Result_MissP");
	
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
	hMissP[0][0].Save();	hMissP[0][0].Save(3); hMissP[0][0].Save(4);
	hMissP[1][0].Save();	hMissP[1][0].Save(3); hMissP[1][0].Save(4);
	hMissP[2][0].Save();	hMissP[2][0].Save(3); hMissP[2][0].Save(4);
	hMissP[3][0].Save();	hMissP[3][0].Save(3); hMissP[3][0].Save(4);
	hMissP[4][0].Save();	hMissP[4][0].Save(3); hMissP[4][0].Save(4);
	hMissP[0][1].Save();	hMissP[0][1].Save(3); hMissP[0][1].Save(4);
	hMissP[1][1].Save();	hMissP[1][1].Save(3); hMissP[1][1].Save(4);
	hMissP[2][1].Save();	hMissP[2][1].Save(3); hMissP[2][1].Save(4);
	hMissP[3][1].Save();	hMissP[3][1].Save(3); hMissP[3][1].Save(4);
	hMissP[4][1].Save();	hMissP[4][1].Save(3); hMissP[4][1].Save(4);
	hMissMass->Write();
	hCutCheck->Write();
	hist[1].Save(true);
	result->Close();
	delete result;
}

