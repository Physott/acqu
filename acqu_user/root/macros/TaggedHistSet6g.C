#include "TaggedHistSet.C"


class	TaggedHistSet6g	: public TaggedHistSet
{
private:
	Bool_t		isEta2Pi0;
	TaggedHist*	hIMPart[3];
	TaggedHist*	hIMPartDiced[3];
	TaggedHist*	hIMAllSet;
	
protected:

public:
	TaggedHistSet6g(const Char_t* Name, const Bool_t Eta2Pi0 = false);
	TaggedHistSet6g(const Char_t* Name, const TaggedHistSet6g* source);
	~TaggedHistSet6g();
	
	
	Bool_t	Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max);
	Bool_t	Init(const TaggedHistSet6g* source, const TString* Name, const TString* Title);
	void	Clear();
	void	Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t* IMPart, const Double_t IMAll, const Double_t IMAllSet, const Double_t ThetaAll, const Double_t PhiAll);
	void	SubstractBackground();	
	void	Add(const TaggedHistSet6g* source, const Double_t f = 1);
	void	SaveSubs();
	void	SaveResult();
	void	Save()			{SaveSubs(); SaveResult();}
	
	const	Bool_t	IsEta2Pi0()	const					{return isEta2Pi0;}
			void	SetEta2Pi0()						{isEta2Pi0	= true;}
			void	SetEta2Pi0(const Bool_t IsEta2Pi0)	{isEta2Pi0	= IsEta2Pi0;}
};


TaggedHistSet6g::TaggedHistSet6g(const Char_t* Name, const Bool_t Eta2Pi0)			: TaggedHistSet(Name), isEta2Pi0(Eta2Pi0)
{
	TString	str;
	str	 = name;
	str	+= "_IMPi0a";
	hIMPart[0]	= new TaggedHist(str, 300, 0, 300);
	str	 = name;
	str	+= "_IMPi0b";
	hIMPart[1]	= new TaggedHist(str, 300, 0, 300);
	str	 = name;
	if(isEta2Pi0)
	{
		str	+= "_IMEta";
		hIMPart[2]	= new TaggedHist(str, 300, 400, 700);
	}
	else
	{
		str	+= "_IMPi0c";
		hIMPart[2]	= new TaggedHist(str, 300, 0, 300);
	}
	
	str	 = name;
	str	+= "_IMPi0aDiced";
	hIMPartDiced[0]	= new TaggedHist(str, 300, 0, 300);
	str	 = name;
	str	+= "_IMPi0bDiced";
	hIMPartDiced[1]	= new TaggedHist(str, 300, 0, 300);
	str	 = name;
	if(isEta2Pi0)
	{
		str	+= "_IMEtaDiced";
		hIMPartDiced[2]	= new TaggedHist(str, 300, 400, 700);
	}
	else
	{
		str	+= "_IMPi0cDiced";
		hIMPartDiced[2]	= new TaggedHist(str, 300, 0, 300);
	}
	
	str	 = name;
	str	+= "_IMAllSet";
	hIMAllSet	= new TaggedHist(str, 1600, 0, 1600);
}
TaggedHistSet6g::TaggedHistSet6g(const Char_t* Name, const TaggedHistSet6g* source)	: TaggedHistSet(Name), isEta2Pi0(source->Eta2Pi0)
{
	TString	str;
	str	 = name;
	str	+= "_IMPi0a";
	hIMPart[0]	= new TaggedHist(str, source->hIMPart[0]);
	str	 = name;
	str	+= "_IMPi0b";
	hIMPart[1]	= new TaggedHist(str, source->hIMPart[1]);
	str	 = name;
	if(isEta2Pi0)
	{
		str	+= "_IMEta";
		hIMPart[2]	= new TaggedHist(str, source->hIMPart[2]);
	}
	else
	{
		str	+= "_IMPi0c";
		hIMPart[2]	= new TaggedHist(str, source->hIMPart[2]);
	}
	
	str	 = name;
	str	+= "_IMPi0aDiced";
	hIMPartDiced[0]	= new TaggedHist(str, source->hIMPartDiced[0]);
	str	 = name;
	str	+= "_IMPi0bDiced";
	hIMPartDiced[1]	= new TaggedHist(str, source->hIMPartDiced[1]);
	str	 = name;
	if(isEta2Pi0)
	{
		str	+= "_IMEtaDiced";
		hIMPartDiced[2]	= new TaggedHist(str, source->hIMPartDiced[2]);
	}
	else
	{
		str	+= "_IMPi0cDiced";
		hIMPartDiced[2]	= new TaggedHist(str, source->hIMPartDiced[2]);
	}
	
	str	 = name;
	str	+= "_IMAllSet";
	hIMAllSet	= new TaggedHist(str, source->hIMAllSet);
}
TaggedHistSet6g::~TaggedHistSet6g()
{
}

inline	void	TaggedHistSet6g::Clear()
{
	TaggedHistSet::Clear();
	for(int i=0; i<3; i++)
	{
		hIMPart[i]->Clear();
		hIMPartDiced[i]->Clear();
	}
	hIMAllSet->Clear();
}

inline	void	TaggedHistSet6g::Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t* IMPart, const Double_t IMAll, const Double_t IMAllSet, const Double_t ThetaAll, const Double_t PhiAll)
{	
	TaggedHistSet::Fill(NTagged, PromptEnergy, Rand1Energy, Rand2Energy, Multiplicity, CBEnergyAll, IMAll, ThetaAll, PhiAll);
	
	hIMPart[0]->Fill(NTagged, IMPart[0]);
	hIMPart[1]->Fill(NTagged, IMPart[1]);
	hIMPart[2]->Fill(NTagged, IMPart[2]);
	
	static int	dice	= 0;
	
	if(isEta2Pi0)
	{
		if(dice == 0)
		{
			hIMPartDiced[0]->Fill(NTagged, IMPart[0]);
			hIMPartDiced[1]->Fill(NTagged, IMPart[1]);
			dice	= 1;
		}
		else if(dice == 1)
		{
			hIMPartDiced[0]->Fill(NTagged, IMPart[1]);
			hIMPartDiced[1]->Fill(NTagged, IMPart[0]);
			dice	= 0;
		}
		hIMPartDiced[2]->Fill(NTagged, IMPart[2]);
	}
	else
	{
		if(dice == 0)
		{
			hIMPartDiced[0]->Fill(NTagged, IMPart[0]);
			hIMPartDiced[1]->Fill(NTagged, IMPart[1]);
			hIMPartDiced[2]->Fill(NTagged, IMPart[2]);
			dice	= 1;
		}
		else if(dice == 1)
		{
			hIMPartDiced[0]->Fill(NTagged, IMPart[1]);
			hIMPartDiced[1]->Fill(NTagged, IMPart[2]);
			hIMPartDiced[2]->Fill(NTagged, IMPart[0]);
			dice	= 2;
		}
		else if(dice == 2)
		{
			hIMPartDiced[0]->Fill(NTagged, IMPart[2]);
			hIMPartDiced[1]->Fill(NTagged, IMPart[0]);
			hIMPartDiced[2]->Fill(NTagged, IMPart[1]);
			dice	= 0;
		}
	}
	
	hIMAllSet->Fill(NTagged, IMAllSet);
}

void	TaggedHistSet6g::SubstractBackground()
{
	TaggedHistSet::SubstractBackground();
	
	hIMPart[0]->SubstractBackground();
	hIMPart[1]->SubstractBackground();
	hIMPart[2]->SubstractBackground();
	hIMPartDiced[0]->SubstractBackground();
	hIMPartDiced[1]->SubstractBackground();
	hIMPartDiced[2]->SubstractBackground();
	hIMAllSet->SubstractBackground();
}
	
void	TaggedHistSet6g::Add(const TaggedHistSet6g* source, const Double_t f)
{
	TaggedHistSet::Add(Name);
	
	hIMPart[0]->Add(source->hIMPart[0], f);
	hIMPart[1]->Add(source->hIMPart[1], f);
	hIMPart[2]->Add(source->hIMPart[2], f);
	hIMPartDiced[0]->Add(source->hIMPartDiced[0], f);
	hIMPartDiced[1]->Add(source->hIMPartDiced[1], f);
	hIMPartDiced[2]->Add(source->hIMPartDiced[2], f);
	hIMAllSet->Add(source->hIMAllSet, f);
}


void	TaggedHistSet6g::SaveSubs()
{
	hTaggedEnergy->SaveSubs();
	hCBEnergyAll->SaveSubs();
	
	hIMPart[0]->SaveSubs();
	hIMPart[1]->SaveSubs();
	hIMPart[2]->SaveSubs();
	hIMPartDiced[0]->SaveSubs();
	hIMPartDiced[1]->SaveSubs();
	hIMPartDiced[2]->SaveSubs();
	
	hIMAll->SaveSubs();
	
	hIMAllSet->SaveSubs();
	
	hThetaAll->SaveSubs();
	hPhiAll->SaveSubs();
}
void	TaggedHistSet6g::SaveResult()
{
	hMultiplicity->Write();
	nTagged[0]->Write();
	nTagged[1]->Write();
	nTagged[2]->Write();	
	
	hTaggedEnergy->SaveResult();
	hCBEnergyAll->SaveResult();
	
	hIMPart[0]->SaveResult();
	hIMPart[1]->SaveResult();
	hIMPart[2]->SaveResult();
	hIMPartDiced[0]->SaveResult();
	hIMPartDiced[1]->SaveResult();
	hIMPartDiced[2]->SaveResult();
	
	hIMAll->SaveResult();
	
	hIMAllSet->SaveResult();
	
	hThetaAll->SaveResult();
	hPhiAll->SaveResult();
}
