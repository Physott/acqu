


class	TreeHistGeneral
{
private:
	TH1I*	hNTagged;
	TH1D*	hTaggedEnergy;
	TH1D*	hTaggedTime;
	TH1I*	hMultiplicity;
	TH1D*	hCBTime;
	TH1D*	hCBEnergyAll;
	TH1D*	hIMAll;
	TH1D*	hThetaAll;
	TH1D*	hPhiAll;
	
protected:

public:
	TreeHistGeneral();
	~TreeHistGeneral();
	
	Bool_t	Init(const Char_t** Name, const Char_t** Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max);
	Bool_t	Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max);
	Bool_t	Init(const TreeHistGeneral& source, const Char_t** Name);
	Bool_t	Init(const TreeHistGeneral& source, const TString* Name);
	void	Clear();
	void	Fill(const Int_t NTagged, const Double_t* TaggedEnergy, const Double_t* TaggedTime, const Int_t Multiplicity, const Double_t* CBTime, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll);
	void	Add(const TreeHistGeneral& source, const Double_t f = 1);
	void	Scale(const Double_t s);
	void	Save();
};

void	CalcBackground(TreeHistGeneral& BG, const TreeHistGeneral& BG1, const TreeHistGeneral& BG2, const TString* Name);
void	CalcResult(TreeHistGeneral& Result, const TreeHistGeneral& FG, const TreeHistGeneral& BG, const TString* Name);


TreeHistGeneral::TreeHistGeneral()	:	hNTagged(0), hTaggedEnergy(0), hTaggedTime(0), hMultiplicity(0), hCBTime(0), hCBEnergyAll(0)
{
}

TreeHistGeneral::~TreeHistGeneral()
{
}


Bool_t	TreeHistGeneral::Init(const Char_t** Name, const Char_t** Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max)
{
	if((hNTagged	= (TH1I*)gROOT->Get(Name[0])))
		hNTagged->Delete();
	hNTagged	= new TH1I(Name[0], Title[0], NBin[0], Min[0], Max[0]);
	if(!hNTagged)
		return false;
	
	if((hTaggedEnergy	= (TH1D*)gROOT->Get(Name[1])))
		hTaggedEnergy->Delete();
	hTaggedEnergy	= new TH1D(Name[1], Title[1], NBin[1], Min[1], Max[1]);
	if(!hTaggedEnergy)
		return false;
	
	if((hTaggedTime	= (TH1D*)gROOT->Get(Name[2])))
		hTaggedTime->Delete();
	hTaggedTime	= new TH1D(Name[2], Title[2], NBin[2], Min[2], Max[2]);
	if(!hTaggedTime)
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[3])))
		hMultiplicity->Delete();
	hMultiplicity	= new TH1I(Name[3], Title[3], NBin[3], Min[3], Max[3]);
	if(!hMultiplicity)
		return false;
	
	if((hCBTime	= (TH1D*)gROOT->Get(Name[4])))
		hCBTime->Delete();
	hCBTime	= new TH1D(Name[4], Title[4], NBin[4], Min[4], Max[4]);
	if(!hCBTime)
		return false;
	
	if((hCBEnergyAll	= (TH1D*)gROOT->Get(Name[5])))
		hCBEnergyAll->Delete();
	hCBEnergyAll	= new TH1D(Name[5], Title[5], NBin[5], Min[5], Max[5]);
	if(!hCBEnergyAll)
		return false;
		
	if((hIMAll	= (TH1D*)gROOT->Get(Name[6])))
		hIMAll->Delete();
	hIMAll	= new TH1D(Name[6], Title[6], NBin[6], Min[6], Max[6]);
	if(!hIMAll)
		return false;
	
	if((hThetaAll	= (TH1D*)gROOT->Get(Name[7])))
		hThetaAll->Delete();
	hThetaAll	= new TH1D(Name[7], Title[7], NBin[7], Min[7], Max[7]);
	if(!hThetaAll)
		return false;
	
	if((hPhiAll	= (TH1D*)gROOT->Get(Name[8])))
		hPhiAll->Delete();
	hPhiAll	= new TH1D(Name[8], Title[8], NBin[8], Min[8], Max[8]);
	if(!hPhiAll)
		return false;
		
	return true;
}
Bool_t	TreeHistGeneral::Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max)
{
	if((hNTagged	= (TH1I*)gROOT->Get(Name[0].Data())))
		hNTagged->Delete();
	hNTagged	= new TH1I(Name[0].Data(), Title[0].Data(), NBin[0], Min[0], Max[0]);
	if(!hNTagged)
		return false;
	
	if((hTaggedEnergy	= (TH1D*)gROOT->Get(Name[1].Data())))
		hTaggedEnergy->Delete();
	hTaggedEnergy	= new TH1D(Name[1].Data(), Title[1].Data(), NBin[1], Min[1], Max[1]);
	if(!hTaggedEnergy)
		return false;
	
	if((hTaggedTime	= (TH1D*)gROOT->Get(Name[2].Data())))
		hTaggedTime->Delete();
	hTaggedTime	= new TH1D(Name[2].Data(), Title[2].Data(), NBin[2], Min[2], Max[2]);
	if(!hTaggedTime)
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[3].Data())))
		hMultiplicity->Delete();
	hMultiplicity	= new TH1I(Name[3].Data(), Title[3].Data(), NBin[3], Min[3], Max[3]);
	if(!hMultiplicity)
		return false;
	
	if((hCBTime	= (TH1D*)gROOT->Get(Name[4].Data())))
		hCBTime->Delete();
	hCBTime	= new TH1D(Name[4].Data(), Title[4].Data(), NBin[4], Min[4], Max[4]);
	if(!hCBTime)
		return false;
	
	if((hCBEnergyAll	= (TH1D*)gROOT->Get(Name[5].Data())))
		hCBEnergyAll->Delete();
	hCBEnergyAll	= new TH1D(Name[5].Data(), Title[5].Data(), NBin[5], Min[5], Max[5]);
	if(!hCBEnergyAll)
		return false;
		
	if((hIMAll	= (TH1D*)gROOT->Get(Name[6].Data())))
		hIMAll->Delete();
	hIMAll	= new TH1D(Name[6].Data(), Title[6].Data(), NBin[6], Min[6], Max[6]);
	if(!hIMAll)
		return false;
	
	if((hThetaAll	= (TH1D*)gROOT->Get(Name[7].Data())))
		hThetaAll->Delete();
	hThetaAll	= new TH1D(Name[7].Data(), Title[7].Data(), NBin[7], Min[7], Max[7]);
	if(!hThetaAll)
		return false;
	
	if((hPhiAll	= (TH1D*)gROOT->Get(Name[8].Data())))
		hPhiAll->Delete();
	hPhiAll	= new TH1D(Name[8].Data(), Title[8].Data(), NBin[8], Min[8], Max[8]);
	if(!hPhiAll)
		return false;
		
	return true;
}
Bool_t	TreeHistGeneral::Init(const TreeHistGeneral& source, const Char_t** Name)
{
	if((hNTagged	= (TH1I*)gROOT->Get(Name[0])))
		hNTagged->Delete();
	hNTagged	= (TH1I*)source.hNTagged->Clone(Name[0]);
	if(!hNTagged)
		return false;
	
	if((hTaggedEnergy	= (TH1D*)gROOT->Get(Name[1])))
		hTaggedEnergy->Delete();
	hTaggedEnergy	= (TH1D*)source.hTaggedEnergy->Clone(Name[1]);
	if(!hTaggedEnergy)
		return false;
	
	if((hTaggedTime	= (TH1D*)gROOT->Get(Name[2])))
		hTaggedTime->Delete();
	hTaggedTime	= (TH1D*)source.hTaggedTime->Clone(Name[2]);
	if(!hTaggedTime)
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[3])))
		hMultiplicity->Delete();
	hMultiplicity	= (TH1I*)source.hMultiplicity->Clone(Name[3]);
	if(!hMultiplicity)
		return false;
	
	if((hCBTime	= (TH1D*)gROOT->Get(Name[4])))
		hCBTime->Delete();
	hCBTime	= (TH1D*)source.hCBTime->Clone(Name[4]);
	if(!hCBTime)
		return false;
	
	if((hCBEnergyAll	= (TH1D*)gROOT->Get(Name[5])))
		hCBEnergyAll->Delete();
	hCBEnergyAll	= (TH1D*)source.hCBEnergyAll->Clone(Name[5]);
	if(!hCBEnergyAll)
		return false;
		
	if((hIMAll	= (TH1D*)gROOT->Get(Name[6])))
		hIMAll->Delete();
	hIMAll	= (TH1D*)source.hIMAll->Clone(Name[6]);
	if(!hIMAll)
		return false;
	
	if((hThetaAll	= (TH1D*)gROOT->Get(Name[7])))
		hThetaAll->Delete();
	hThetaAll	= (TH1D*)source.hThetaAll->Clone(Name[7]);
	if(!hThetaAll)
		return false;
	
	if((hPhiAll	= (TH1D*)gROOT->Get(Name[8])))
		hPhiAll->Delete();
	hPhiAll	= (TH1D*)source.hPhiAll->Clone(Name[8]);
	if(!hPhiAll)
		return false;
		
	return true;
}
Bool_t	TreeHistGeneral::Init(const TreeHistGeneral& source, const TString* Name)
{
	if((hNTagged	= (TH1I*)gROOT->Get(Name[0].Data())))
		hNTagged->Delete();
	hNTagged	= (TH1I*)source.hNTagged->Clone(Name[0].Data());
	if(!hNTagged)
		return false;
	
	if((hTaggedEnergy	= (TH1D*)gROOT->Get(Name[1].Data())))
		hTaggedEnergy->Delete();
	hTaggedEnergy	= (TH1D*)source.hTaggedEnergy->Clone(Name[1].Data());
	if(!hTaggedEnergy)
		return false;
	
	if((hTaggedTime	= (TH1D*)gROOT->Get(Name[2].Data())))
		hTaggedTime->Delete();
	hTaggedTime	= (TH1D*)source.hTaggedTime->Clone(Name[2].Data());
	if(!hTaggedTime)
		return false;
	
	if((hMultiplicity	= (TH1I*)gROOT->Get(Name[3].Data())))
		hMultiplicity->Delete();
	hMultiplicity	= (TH1I*)source.hMultiplicity->Clone(Name[3].Data());
	if(!hMultiplicity)
		return false;
	
	if((hCBTime	= (TH1D*)gROOT->Get(Name[4].Data())))
		hCBTime->Delete();
	hCBTime	= (TH1D*)source.hCBTime->Clone(Name[4].Data());
	if(!hCBTime)
		return false;
	
	if((hCBEnergyAll	= (TH1D*)gROOT->Get(Name[5].Data())))
		hCBEnergyAll->Delete();
	hCBEnergyAll	= (TH1D*)source.hCBEnergyAll->Clone(Name[5].Data());
	if(!hCBEnergyAll)
		return false;
		
	if((hIMAll	= (TH1D*)gROOT->Get(Name[6].Data())))
		hIMAll->Delete();
	hIMAll	= (TH1D*)source.hIMAll->Clone(Name[6].Data());
	if(!hIMAll)
		return false;
	
	if((hThetaAll	= (TH1D*)gROOT->Get(Name[7].Data())))
		hThetaAll->Delete();
	hThetaAll	= (TH1D*)source.hThetaAll->Clone(Name[7].Data());
	if(!hThetaAll)
		return false;
	
	if((hPhiAll	= (TH1D*)gROOT->Get(Name[8].Data())))
		hPhiAll->Delete();
	hPhiAll	= (TH1D*)source.hPhiAll->Clone(Name[8].Data());
	if(!hPhiAll)
		return false;
		
	return true;
}
	
inline	void	TreeHistGeneral::Clear()
{
	hNTagged->Reset("M");
	hTaggedEnergy->Reset("M");
	hTaggedTime->Reset("M");
	hMultiplicity->Reset("M");
	hCBTime->Reset("M");
	hCBEnergyAll->Reset("M");
	hIMAll->Reset("M");
	hThetaAll->Reset("M");
	hPhiAll->Reset("M");
}

inline	void	TreeHistGeneral::Fill(const Int_t NTagged, const Double_t* TaggedEnergy, const Double_t* TaggedTime, const Int_t Multiplicity, const Double_t* CBTime, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll)
{
	hNTagged->Fill(NTagged);
	for(int i=0; i<NTagged; i++)
	{
		hTaggedEnergy->Fill(TaggedEnergy[i]);
		hTaggedTime->Fill(TaggedTime[i]);
	}
	hMultiplicity->Fill(Multiplicity);
	for(int i=0; i<Multiplicity; i++)
		hCBTime->Fill(CBTime[i]);
	hCBEnergyAll->Fill(CBEnergyAll);
	hIMAll->Fill(IMAll);
	hThetaAll->Fill(TMath::RadToDeg()*ThetaAll);
	hPhiAll->Fill(TMath::RadToDeg()*PhiAll);
}

void	TreeHistGeneral::Add(const TreeHistGeneral& source, const Double_t f)
{
	hNTagged->Add(source.hNTagged, f);
	hTaggedEnergy->Add(source.hTaggedEnergy, f);
	hTaggedTime->Add(source.hTaggedTime, f);
	hMultiplicity->Add(source.hMultiplicity, f);
	hCBTime->Add(source.hCBTime, f);
	hCBEnergyAll->Add(source.hCBEnergyAll, f);
	hIMAll->Add(source.hIMAll, f);
	hThetaAll->Add(source.hThetaAll, f);
	hPhiAll->Add(source.hPhiAll, f);
}

void	TreeHistGeneral::Scale(const Double_t s)
{
	hNTagged->Scale(s);
	hTaggedEnergy->Scale(s);
	hTaggedTime->Scale(s);
	hMultiplicity->Scale(s);
	hCBTime->Scale(s);
	hCBEnergyAll->Scale(s);
	hIMAll->Scale(s);
	hThetaAll->Scale(s);
	hPhiAll->Scale(s);
}

void	TreeHistGeneral::Save()
{
	hNTagged->Write();
	hTaggedEnergy->Write();
	hTaggedTime->Write();
	hMultiplicity->Write();
	hCBTime->Write();
	hCBEnergyAll->Write();
	hIMAll->Write();
	hThetaAll->Write();
	hPhiAll->Write();
}





void	CalcBackground(TreeHistGeneral& BG, const TreeHistGeneral& BG1, const TreeHistGeneral& BG2, const TString* Name)
{
	BG.Init(BG1, Name);
	BG.Add(BG2);
	BG.Scale(0.5);
}
void	CalcResult(TreeHistGeneral& Result, const TreeHistGeneral& FG, const TreeHistGeneral& BG, const TString* Name)
{
	Result.Init(FG, Name);
	Result.Add(BG, -1);
}









