#include "TreeHistTagged.C"


class	TreeHistGeneralTagged
{
private:
	TH1I*			hNTagged[3];
	TreeHistTagged	hTaggedEnergy;
	TH1I*			hMultiplicity;
	TreeHistTagged	hCBEnergyAll;
	TreeHistTagged	hIMAll;
	TreeHistTagged	hThetaAll;
	TreeHistTagged	hPhiAll;
	
protected:

public:
	TreeHistGeneralTagged();
	~TreeHistGeneralTagged();
	
	Bool_t	Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max);
	Bool_t	Init(const TreeHistGeneralTagged& source, const TString* Name, const TString* Title);
	void	Clear();
	void	Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll);
	void	SubstractBackground(const TString* Name);	
	void	Add(const TreeHistGeneralTagged& source, const Double_t f = 1);
	void	Scale(const Double_t s);
	void	Save(const Bool_t withBGCorrection = false);
};

void	CalcBackground(TreeHistGeneralTagged& BG, const TreeHistGeneralTagged& BG1, const TreeHistGeneralTagged& BG2, const TString* Name);
void	CalcResult(TreeHistGeneralTagged& Result, const TreeHistGeneralTagged& FG, const TreeHistGeneralTagged& BG, const TString* Name);


TreeHistGeneralTagged::TreeHistGeneralTagged()	:	hNTagged(0), hTaggedEnergy(0), hTaggedTime(0), hMultiplicity(0), hCBTime(0), hCBEnergyAll(0)
{
}

TreeHistGeneralTagged::~TreeHistGeneralTagged()
{
}


Bool_t	TreeHistGeneralTagged::Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max)
{
	if((hNTagged[0]	= (TH1I*)gROOT->Get(Name[0].Data())))
		hNTagged[0]->Delete();
	hNTagged[0]	= new TH1I(Name[0].Data(), Title[0].Data(), NBin[0], Min[0], Max[0]);
	if(!hNTagged[0])
		return false;
	if((hNTagged[1]	= (TH1I*)gROOT->Get(Name[1].Data())))
		hNTagged[1]->Delete();
	hNTagged[1]	= new TH1I(Name[1].Data(), Title[1].Data(), NBin[0], Min[0], Max[0]);
	if(!hNTagged[1])
		return false;
	if((hNTagged[2]	= (TH1I*)gROOT->Get(Name[2].Data())))
		hNTagged[2]->Delete();
	hNTagged[2]	= new TH1I(Name[2].Data(), Title[2].Data(), NBin[0], Min[0], Max[0]);
	if(!hNTagged[2])
		return false;
		
	if(!hTaggedEnergy.Init(&Name[3], &Title[3], NBin[1], Min[1], Max[1]))
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[6].Data())))
		hMultiplicity->Delete();
	hMultiplicity	= new TH1I(Name[6].Data(), Title[6].Data(), NBin[2], Min[2], Max[2]);
	if(!hMultiplicity)
		return false;
	
	if(!hCBEnergyAll.Init(&Name[7], &Title[7], NBin[3], Min[3], Max[3]))
		return false;
	
	if(!hIMAll.Init(&Name[10], &Title[10], NBin[4], Min[4], Max[4]))
		return false;
	
	if(!hThetaAll.Init(&Name[13], &Title[13], NBin[5], Min[5], Max[5]))
		return false;
	
	if(!hPhiAll.Init(&Name[16], &Title[16], NBin[6], Min[6], Max[6]))
		return false;
		
	return true;
}
Bool_t	TreeHistGeneralTagged::Init(const TreeHistGeneralTagged& source, const TString* Name)
{
	if((hNTagged[0]	= (TH1I*)gROOT->Get(Name[0].Data())))
		hNTagged[0]->Delete();
	hNTagged[0]	= (TH1I*)source.hNTagged[0]->Clone(Name[0].Data());
	if(!hNTagged[0])
		return false;
		
	if((hNTagged[1]	= (TH1I*)gROOT->Get(Name[1].Data())))
		hNTagged[1]->Delete();
	hNTagged[1]	= (TH1I*)source.hNTagged[1]->Clone(Name[1].Data());
	if(!hNTagged[1])
		return false;
		
	if((hNTagged[2]	= (TH1I*)gROOT->Get(Name[2].Data())))
		hNTagged[2]->Delete();
	hNTagged[2]	= (TH1I*)source.hNTagged[2]->Clone(Name[2].Data());
	if(!hNTagged[2])
		return false;
		
	if(!hTaggedEnergy.Init(source.hTaggedEnergy, &Name[3]))
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[6].Data())))
		hMultiplicity->Delete();
	hMultiplicity	= (TH1I*)source.hMultiplicity->Clone(Name[6].Data());
	if(!hMultiplicity)
		return false;
	
	if(!hCBEnergyAll.Init(source.hCBEnergyAll, &Name[7]))
		return false;
	
	if(!hIMAll.Init(source.hIMAll, &Name[10]))
		return false;
	
	if(!hThetaAll.Init(source.hThetaAll, &Name[13]))
		return false;
	
	if(!hPhiAll.Init(source.hPhiAll, &Name[16]))
		return false;
		
	return true;
}
	
inline	void	TreeHistGeneralTagged::Clear()
{
	hNTagged[0]->Reset("M");
	hNTagged[1]->Reset("M");
	hNTagged[2]->Reset("M");
	hTaggedEnergy.Clear();
	hMultiplicity->Reset("M");
	hCBEnergyAll.Clear();
	hIMAll.Clear();
	hThetaAll.Clear();
	hPhiAll.Clear();
}

inline	void	TreeHistGeneralTagged::Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll)
{	
	hNTagged[0]->Fill(NTagged[0]);
	hNTagged[1]->Fill(NTagged[1]);
	hNTagged[2]->Fill(NTagged[2]);
	hTaggedEnergy.Fill(NTagged, PromptEnergy, Rand1Energy, Rand2Energy);
	hMultiplicity->Fill(Multiplicity);
	hCBEnergyAll.Fill(NTagged[0], NTagged[1], NTagged[2], CBEnergyAll);
	hIMAll.Fill(NTagged[0], NTagged[1], NTagged[2], IMAll);
	hThetaAll.Fill(NTagged[0], NTagged[1], NTagged[2], TMath::RadToDeg()*ThetaAll);
	hPhiAll.Fill(NTagged[0], NTagged[1], NTagged[2], TMath::RadToDeg()*PhiAll);
}

void	TreeHistGeneralTagged::SubstractBackground(const TString* Name)
{
	hTaggedEnergy.SubstractBackground(Name[0].Data(), Name[1].Data());
	hCBEnergyAll.SubstractBackground(Name[2].Data(), Name[3].Data());
	hIMAll.SubstractBackground(Name[4].Data(), Name[5].Data());
	hThetaAll.SubstractBackground(Name[6].Data(), Name[7].Data());
	hPhiAll.SubstractBackground(Name[8].Data(), Name[9].Data());
}
	
void	TreeHistGeneralTagged::Add(const TreeHistGeneralTagged& source, const Double_t f)
{
	hNTagged[0]->Add(source.hNTagged[0], f);
	hNTagged[1]->Add(source.hNTagged[1], f);
	hNTagged[2]->Add(source.hNTagged[2], f);
	hTaggedEnergy.Add(source.hTaggedEnergy, f);
	hMultiplicity->Add(source.hMultiplicity, f);
	hCBEnergyAll.Add(source.hCBEnergyAll, f);
	hIMAll.Add(source.hIMAll, f);
	hThetaAll.Add(source.hThetaAll, f);
	hPhiAll.Add(source.hPhiAll, f);
}

void	TreeHistGeneralTagged::Scale(const Double_t s)
{
	hNTagged[0]->Scale();
	hNTagged[1]->Scale();
	hNTagged[2]->Scale();
	hTaggedEnergy.Scale(s);
	hMultiplicity->Scale(s);
	hCBEnergyAll.Scale(s);
	hIMAll.Scale(s);
	hThetaAll.Scale(s);
	hPhiAll.Scale(s);
}

void	TreeHistGeneralTagged::Save(const Bool_t withBGCorrection)
{
	hNTagged[0]->Write();
	hNTagged[1]->Write();
	hNTagged[2]->Write();
	hMultiplicity->Write();
	printf("bool %d\n", withBGCorrection);
	Int_t	max = 3;
	if(withBGCorrection)
		max	= 5;
	for(int i=0; i<max; i++)
	{
		hTaggedEnergy.Save(i);
		hCBEnergyAll.Save(i);
		hIMAll.Save(i);
		hThetaAll.Save(i);
		hPhiAll.Save(i);
	}
}








