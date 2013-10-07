
#include "TaggedHist.C"



class	TaggedHistSet
{
private:
	Bool_t		BG_Substracted;

protected:
	TString		name;
	TH1I*		hMultiplicity;
	TH1I*		nTagged[3];
	TaggedHist*	hTaggedEnergy;
	TaggedHist*	hCBEnergyAll;
	TaggedHist*	hIMAll;
	TaggedHist*	hThetaAll;
	TaggedHist*	hPhiAll;
	
public:
	TaggedHistSet(const Char_t* Name);
	TaggedHistSet(const Char_t* Name, const TaggedHistSet* source);
	~TaggedHistSet();
	
	void	Clear();
	void	Add(const TaggedHistSet* source, const Double_t f = 1);
	void	Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t NCBHits, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll);
	void	SubstractBackground();
	void	SaveSubs();
	void	SaveResult();
	void	Save()			{SaveSubs(); SaveResult();}
};


TaggedHistSet::TaggedHistSet(const Char_t* Name)	: name(Name), BG_Substracted(false)
{
	TString	str;
	str	 = name;
	str	+= "_NCBHits";
	if((hMultiplicity	= (TH1I*)gROOT->Get(str.Data())))
		hMultiplicity->Delete();
	hMultiplicity	= new TH1I(str.Data(), str.Data(), 16, 0, 16);
	if(!hMultiplicity)
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
		
	str	 = name;
	str	+= "_NTagged_Prompt";
	if((nTagged[0]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[0]->Delete();
	nTagged[0]	= new TH1I(str.Data(), str.Data(), 32, 0, 32);
	if(!nTagged[0])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	str	 = name;
	str	+= "_NTagged_Rand1";
	if((nTagged[1]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[1]->Delete();
	nTagged[1]	= new TH1I(str.Data(), str.Data(), 32, 0, 32);
	if(!nTagged[1])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	str	 = name;
	str	+= "_NTagged_Rand2";
	if((nTagged[2]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[2]->Delete();
	nTagged[2]	= new TH1I(str.Data(), str.Data(), 32, 0, 32);
	if(!nTagged[2])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	
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

TaggedHistSet::TaggedHistSet(const Char_t* Name, const TaggedHistSet* source)	: name(Name), BG_Substracted(false)
{
	TString	str;
	str	 = name;
	str	+= "_NCBHits";
	if((hMultiplicity	= (TH1I*)gROOT->Get(str.Data())))
		hMultiplicity->Delete();
	hMultiplicity	= (TH1I*)source->hMultiplicity->Clone(str.Data());
	if(!hMultiplicity)
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
		
	str	 = name;
	str	+= "_NTagged_Prompt";
	if((nTagged[0]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[0]->Delete();
	nTagged[0]	= (TH1I*)source->nTagged[0]->Clone(str.Data());
	if(!nTagged[0])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	str	 = name;
	str	+= "_NTagged_Rand1";
	if((nTagged[1]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[1]->Delete();
	nTagged[1]	= (TH1I*)source->nTagged[1]->Clone(str.Data());
	if(!nTagged[1])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	str	 = name;
	str	+= "_NTagged_Rand2";
	if((nTagged[2]	= (TH1I*)gROOT->Get(str.Data())))
		nTagged[2]->Delete();
	nTagged[2]	= (TH1I*)source->nTagged[2]->Clone(str.Data());
	if(!nTagged[2])
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
	
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

TaggedHistSet::~TaggedHistSet()
{
}



void	TaggedHistSet::Clear()
{
	hMultiplicity->Reset("M");
	nTagged[0]->Reset("M");
	nTagged[1]->Reset("M");
	nTagged[2]->Reset("M");
	hTaggedEnergy->Clear();
	hCBEnergyAll->Clear();
	hIMAll->Clear();
	hThetaAll->Clear();
	hPhiAll->Clear();
}

void	TaggedHistSet::Add(const TaggedHistSet* source, const Double_t f)
{
	hMultiplicity->Add(source->hMultiplicity, f);
	nTagged[0]->Add(source->nTagged[0], f);
	nTagged[1]->Add(source->nTagged[1], f);
	nTagged[2]->Add(source->nTagged[2], f);
	hTaggedEnergy->Add(source->hTaggedEnergy, f);
	hCBEnergyAll->Add(source->hCBEnergyAll, f);
	hIMAll->Add(source->hIMAll, f);
	hThetaAll->Add(source->hThetaAll, f);
	hPhiAll->Add(source->hPhiAll, f);
}
	
inline	void	TaggedHistSet::Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t NCBHits, const Double_t CBEnergyAll, const Double_t IMAll, const Double_t ThetaAll, const Double_t PhiAll)
{
	hMultiplicity->Fill(NCBHits);
	nTagged[0]->Fill(NTagged[0]);
	nTagged[1]->Fill(NTagged[1]);
	nTagged[2]->Fill(NTagged[2]);
	hTaggedEnergy->Fill(NTagged, PromptEnergy, Rand1Energy, Rand2Energy);
	hCBEnergyAll->Fill(NTagged, CBEnergyAll);
	hIMAll->Fill(NTagged, IMAll);
	hThetaAll->Fill(NTagged, ThetaAll);
	hPhiAll->Fill(NTagged, PhiAll);
}
	
void	TaggedHistSet::SubstractBackground()
{
	hTaggedEnergy->SubstractBackground();
	hCBEnergyAll->SubstractBackground();
	hIMAll->SubstractBackground();
	hThetaAll->SubstractBackground();
	hPhiAll->SubstractBackground();
}


void	TaggedHistSet::SaveSubs()
{
	hTaggedEnergy->SaveSubs();
	hCBEnergyAll->SaveSubs();
	hIMAll->SaveSubs();
	hThetaAll->SaveSubs();
	hPhiAll->SaveSubs();
}
void	TaggedHistSet::SaveResult()
{
	hMultiplicity->Write();
	nTagged[0]->Write();
	nTagged[1]->Write();
	nTagged[2]->Write();
	hTaggedEnergy->SaveResult();
	hCBEnergyAll->SaveResult();
	hIMAll->SaveResult();
	hThetaAll->SaveResult();
	hPhiAll->SaveResult();
}






