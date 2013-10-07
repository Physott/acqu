
#include "TaggedHist.C"



class	GeneralHist
{
private:
	TString		name;
	TH1I*		hMultiplicity;
	TaggedHist*	nTagged;
	TaggedHist*	hTaggedEnergy;
	TaggedHist*	hCBEnergyAll;
	TaggedHist*	hIMAll;
	TaggedHist*	hThetaAll;
	TaggedHist*	hPhiAll;
	Bool_t		BG_Substracted;

protected:
	
public:
	GeneralHist(const Char_t* Name);
	GeneralHist(const Char_t* Name, const GeneralHist* source);
	~GeneralHist();
	
	void	Clear();
	void	Add(const GeneralHist* source, const Double_t f = 1);
	void	Fill(const Int_t NCBHits, const Int_t NTagged, const Double_t* TaggedEnergy, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll);
	void	SubstractBackground();
	void	SaveSubs();
	void	SaveResult();
	void	Save()			{SaveSubs(); SaveResult();}
};


GeneralHist::GeneralHist(const Char_t* Name)	: name(Name), BG_Substracted(false)
{
	TString	str;
	str	 = name;
	str	+= "_NCBHits";
	if((hMultiplicity	= (TH1D*)gROOT->Get(str.Data())))
		hMultiplicity->Delete();
	hMultiplicity	= new TH1D(str.Data(), str.Data(), 16, 0, 16);
	if(!hMultiplicity)
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
		
	str	 = name;
	str	+= "_NTagged";
	nTagged	= new TaggedHist(str, 32, 0, 32);
	
	str	 = name;
	str	+= "_TaggedEnergy";
	hTaggedEnergy	= new TaggedHist(str, 200, 1400, 1600);
	
	str	 = name;
	str	+= "_CBEnergyAll";
	hCBEnergyAll	= new TaggedHist(str, 2000, 0, 2000);
	
	str	 = name;
	str	+= "_IMAll";
	hIMAll	= new TaggedHist(str, 1600, 0, 1600);
	
	str	 = name;
	str	+= "_ThetaAll";
	hThetaAll	= new TaggedHist(str, 180, 0, 180);
	
	str	 = name;
	str	+= "_PhiAll";
	hPhiAll	= new TaggedHist(str, 360, -180, 180);
}

GeneralHist::GeneralHist(const Char_t* Name, const GeneralHist* source)	: name(Name), BG_Substracted(false)
{
	TString	str;
	str	 = name;
	str	+= "_NCBHits";
	if((hMultiplicity	= (TH1D*)gROOT->Get(str.Data())))
		hMultiplicity->Delete();
	hMultiplicity	= (TH1D*)source->hMultiplicity->Clone(str.Data());
	if(!hMultiplicity)
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
		
	str	 = name;
	str	+= "_NTagged";
	nTagged	= new TaggedHist(str, source->nTagged);
	
	str	 = name;
	str	+= "_TaggedEnergy";
	hTaggedEnergy	= new TaggedHist(str, source->hTaggedEnergy);
	
	str	 = name;
	str	+= "_CBEnergyAll";
	hCBEnergyAll	= new TaggedHist(str, source->hCBEnergyAll);
	
	str	 = name;
	str	+= "_IMAll";
	hIMAll	= new TaggedHist(str, source->hIMAll);
	
	str	 = name;
	str	+= "_ThetaAll";
	hThetaAll	= new TaggedHist(str, source->hThetaAll);
	
	str	 = name;
	str	+= "_PhiAll";
	hPhiAll	= new TaggedHist(str, source->hPhiAll);
}

GeneralHist::~GeneralHist()
{
}



void	GeneralHist::Clear()
{
	hMultiplicity->Reset("M");
	nTagged->Clear();
	hTaggedEnergy->Clear();
	hCBEnergyAll->Clear();
	hIMAll->Clear();
	hThetaAll->Clear();
	hPhiAll->Clear();
}

void	GeneralHist::Add(const GeneralHist* source, const Double_t f)
{
	hMultiplicity->Add(source->hMultiplicity, f);
	nTagged->Add(source->nTagged, f);
	hTaggedEnergy->Add(source->hTaggedEnergy, f);
	hCBEnergyAll->Add(source->hCBEnergyAll, f);
	hIMAll->Add(source->hIMAll, f);
	hThetaAll->Add(source->hThetaAll, f);
	hPhiAll->Add(source->hPhiAll, f);
}
	
inline	void	GeneralHist::Fill(const Int_t NCBHits, const Int_t NTagged, const Double_t* TaggedEnergy, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll)
{
	hMultiplicity->Fill(NCBHits);
	nTagged->Fill(NTagged);
	for(int i=0; i<NTagged; i++)
		hTaggedEnergy->Fill(TaggedEnergy[i]);
	hCBEnergyAll->Fill(CBEnergyAll);
	hIMAll->Fill(IMAll);
	hThetaAll->Fill(ThetaAll);
	hPhiAll->Fill(PhiAll);
}
	
void	GeneralHist::SubstractBackground()
{
	nTagged->SubstractBackground();
	hTaggedEnergy->SubstractBackground();
	hCBEnergyAll->SubstractBackground();
	hIMAll->SubstractBackground();
	hThetaAll->SubstractBackground();
	hPhiAll->SubstractBackground();
}


void	GeneralHist::SaveSubs()
{
	nTagged->SaveSubs();
	hTaggedEnergy->SaveSubs();
	hCBEnergyAll->SaveSubs();
	hIMAll->SaveSubs();
	hThetaAll->SaveSubs();
	hPhiAll->SaveSubs();
}
void	GeneralHist::SaveResult()
{
	nTagged->SaveResult();
	hTaggedEnergy->SaveResult();
	hCBEnergyAll->SaveResult();
	hIMAll->SaveResult();
	hThetaAll->SaveResult();
	hPhiAll->SaveResult();
}






