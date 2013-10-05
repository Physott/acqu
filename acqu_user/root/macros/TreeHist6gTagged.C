#include "TreeHistGeneralTagged.C"


class	TreeHist6gTagged	: public TreeHistGeneralTagged
{
private:
	Bool_t			isEta2Pi0;
	TreeHistTagged	hIMPart[3];
	TreeHistTagged	hIMPartDiced[3];
	TreeHistTagged	hIMAllSet;
	
protected:

public:
	TreeHist6gTagged();
	~TreeHist6gTagged();
	
	Bool_t	Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max);
	Bool_t	Init(const TreeHist6gTagged& source, const TString* Name, const TString* Title);
	void	Clear();
	void	Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t* IMPart, const Double_t IMAll, const Double_t IMAllSet, const Double_t ThetaAll, const Double_t PhiAll);
	void	SubstractBackground(const TString* Name);	
	void	Add(const TreeHist6gTagged& source, const Double_t f = 1);
	void	Save(const Bool_t withBGCorrection = false);
	
	const	Bool_t	GetEta2Pi0()	const				{return isEta2Pi0;}
			void	SetEta2Pi0()						{isEta2Pi0	= true;}
			void	SetEta2Pi0(const Bool_t IsEta2Pi0)	{isEta2Pi0	= IsEta2Pi0;}
};


TreeHist6gTagged::TreeHist6gTagged()	: isEta2Pi0(false)
{
}

TreeHist6gTagged::~TreeHist6gTagged()
{
}


Bool_t	TreeHist6gTagged::Init(const TString* Name, const TString* Title, const Int_t* NBin, const Double_t* Min, const Double_t* Max)
{
	TreeHistGeneralTagged::Init(Name, Title, NBin, Min, Max);
	
	if(!hIMPart[0].Init(&Name[19], &Title[19], NBin[7], Min[7], Max[7]))
		return false;
	if(!hIMPart[1].Init(&Name[22], &Title[22], NBin[8], Min[8], Max[8]))
		return false;
	if(!hIMPart[2].Init(&Name[25], &Title[25], NBin[9], Min[9], Max[9]))
		return false;
		
	if(!hIMPartDiced[0].Init(&Name[28], &Title[28], NBin[10], Min[10], Max[10]))
		return false;
	if(!hIMPartDiced[1].Init(&Name[31], &Title[31], NBin[11], Min[11], Max[11]))
		return false;
	if(!hIMPartDiced[2].Init(&Name[34], &Title[34], NBin[12], Min[12], Max[12]))
		return false;
		
	if(!hIMAllSet.Init(&Name[37], &Title[37], NBin[13], Min[13], Max[13]))
		return false;
		
	return true;
}
Bool_t	TreeHist6gTagged::Init(const TreeHist6gTagged& source, const TString* Name)
{
	TreeHistGeneralTagged::Init(source, Name);
	
		if(!hIMPart[0].Init(source.hIMPart[0], &Name[19]))
		return false;
	if(!hIMPart[1].Init(source.hIMPart[1], &Name[22]))
		return false;
	if(!hIMPart[2].Init(source.hIMPart[2], &Name[25]))
		return false;
		
	if(!hIMPartDiced[0].Init(source.hIMPartDiced[0], &Name[28]))
		return false;
	if(!hIMPartDiced[1].Init(source.hIMPartDiced[1], &Name[31]))
		return false;
	if(!hIMPartDiced[2].Init(source.hIMPartDiced[2], &Name[34]))
		return false;
		
	if(!hIMAllSet.Init(source.hIMAllSet, &Name[37]))
		return false;

	return true;
}
	
inline	void	TreeHist6gTagged::Clear()
{
	TreeHistGeneralTagged::Clear();
	for(int i=0; i<3; i++)
	{
		hIMPart[i].Clear();;
		hIMPartDiced[i].Clear();;
	}
	hIMAllSet.Clear();;
}

inline	void	TreeHist6gTagged::Fill(const Int_t* NTagged, const Double_t* PromptEnergy, const Double_t* Rand1Energy, const Double_t* Rand2Energy, const Int_t Multiplicity, const Double_t CBEnergyAll, const Double_t* IMPart, const Double_t IMAll, const Double_t IMAllSet, const Double_t ThetaAll, const Double_t PhiAll)
{	
	TreeHistGeneralTagged::Fill(NTagged, PromptEnergy, Rand1Energy, Rand2Energy, Multiplicity, CBEnergyAll, IMAll, ThetaAll, PhiAll);
	
	hIMPart[0].Fill(NTagged, IMPart[0]);
	hIMPart[1].Fill(NTagged, IMPart[1]);
	hIMPart[2].Fill(NTagged, IMPart[2]);
	
	static int	dice	= 0;
	
	if(isEta2Pi0)
	{
		if(dice == 0)
		{
			hIMPartDiced[0].Fill(NTagged, IMPart[0]);
			hIMPartDiced[1].Fill(NTagged, IMPart[1]);
			dice	= 1;
		}
		else if(dice == 1)
		{
			hIMPartDiced[0].Fill(NTagged, IMPart[1]);
			hIMPartDiced[1].Fill(NTagged, IMPart[0]);
			dice	= 0;
		}
		hIMPartDiced[2].Fill(NTagged, IMPart[2]);
	}
	else
	{
		if(dice == 0)
		{
			hIMPartDiced[0].Fill(NTagged, IMPart[0]);
			hIMPartDiced[1].Fill(NTagged, IMPart[1]);
			hIMPartDiced[2].Fill(NTagged, IMPart[2]);
			dice	= 1;
		}
		else if(dice == 1)
		{
			hIMPartDiced[0].Fill(NTagged, IMPart[1]);
			hIMPartDiced[1].Fill(NTagged, IMPart[2]);
			hIMPartDiced[2].Fill(NTagged, IMPart[0]);
			dice	= 2;
		}
		else if(dice == 2)
		{
			hIMPartDiced[0].Fill(NTagged, IMPart[2]);
			hIMPartDiced[1].Fill(NTagged, IMPart[0]);
			hIMPartDiced[2].Fill(NTagged, IMPart[1]);
			dice	= 0;
		}
	}
	
	hIMAllSet.Fill(NTagged, IMAllSet);
}

void	TreeHist6gTagged::SubstractBackground(const TString* Name)
{
	TreeHistGeneralTagged::SubstractBackground(Name);
	
	hIMPart[0].SubstractBackground(Name[10].Data(), Name[11].Data());
	hIMPart[1].SubstractBackground(Name[12].Data(), Name[13].Data());
	hIMPart[2].SubstractBackground(Name[14].Data(), Name[15].Data());
	hIMPartDiced[0].SubstractBackground(Name[16].Data(), Name[17].Data());
	hIMPartDiced[1].SubstractBackground(Name[18].Data(), Name[19].Data());
	hIMPartDiced[2].SubstractBackground(Name[20].Data(), Name[21].Data());
	hIMAllSet.SubstractBackground(Name[22].Data(), Name[23].Data());
}
	
void	TreeHist6gTagged::Add(const TreeHist6gTagged& source, const Double_t f)
{
	TreeHistGeneralTagged::Add(Name);
	
	hIMPart[0]->Add(source.hIMPart[0], f);
	hIMPart[1]->Add(source.hIMPart[1], f);
	hIMPart[2]->Add(source.hIMPart[2], f);
	hIMPartDiced[0]->Add(source.hIMPartDiced[0], f);
	hIMPartDiced[1]->Add(source.hIMPartDiced[1], f);
	hIMPartDiced[2]->Add(source.hIMPartDiced[2], f);
	hIMAllSet->Add(source.hIMAllSet, f);
}


void	TreeHist6gTagged::Save(const Bool_t withBGCorrection)
{
	hMultiplicity->Write();
	Int_t	max = 3;
	if(withBGCorrection)
		max	= 5;
	for(int i=0; i<max; i++)
	{
		if(i<3)
			hNTagged[i]->Write();
		hTaggedEnergy.Save(i);
		hCBEnergyAll.Save(i);
		hIMPart[0].Save(i);
		hIMPart[1].Save(i);
		hIMPart[2].Save(i);
		hIMPartDiced[0].Save(i);
		hIMPartDiced[1].Save(i);
		hIMPartDiced[2].Save(i);
		hIMAll.Save(i);
		hIMAllSet.Save(i);
		hThetaAll.Save(i);
		hPhiAll.Save(i);
	}
}
