

class	TreeHistTagged
{
private:
	TH1D*	hist[5];
	Bool_t	BG_Substracted;
	
protected:

public:
	TreeHistTagged();
	~TreeHistTagged();
	
	Bool_t	Init(const TString* Name, const TString* Title, const Int_t NBin, const Double_t Min, const Double_t Max);
	Bool_t	Init(const TreeHistTagged& source, const TString* Name);
	void	Clear();
	void	Add(const TreeHistTagged& source, const Double_t f = 1);
	void	Fill(const Int_t* NTagged, const Double_t* Prompt, const Double_t* Rand1, const Double_t* Rand2);
	void	Fill(const Int_t type, const Double_t value);
	void	Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t value);
	void	Fill(const Int_t* nTagged, const Double_t value)														{Fill(nTagged[0], nTagged[1], nTagged[2], value);}
	void	SubstractBackground(const Char_t* NameBG, const Char_t* NameResult);
	void	Save(const Int_t index = -1);
};


TreeHistTagged::TreeHistTagged()	: BG_Substracted(false)
{
	for(int i=0; i<5; i++)
		hist[i]	= 0;
}

TreeHistTagged::~TreeHistTagged()
{
}



Bool_t	TreeHistTagged::Init(const TString* Name, const TString* Title, const Int_t NBin, const Double_t Min, const Double_t Max)
{
	if((hist[0]	= (TH1D*)gROOT->Get(Name[0].Data())))
		hist[0]->Delete();
	hist[0]		= new TH1D(Name[0].Data(), Title[0].Data(), NBin, Min, Max);
	if(!hist[0])
		return false;
	
	if((hist[1]	= (TH1D*)gROOT->Get(Name[1].Data())))
		hist[1]->Delete();
	hist[1]		= new TH1D(Name[1].Data(), Title[1].Data(), NBin, Min, Max);
	if(!hist[1])
		return false;
		
	if((hist[2]	= (TH1D*)gROOT->Get(Name[2].Data())))
		hist[2]->Delete();
	hist[2]		= new TH1D(Name[2].Data(), Title[2].Data(), NBin, Min, Max);
	if(!hist[2])
		return false;
	
	return true;
}
Bool_t	TreeHistTagged::Init(const TreeHistTagged& source, const TString* Name)
{
	for(int i=0; i<3; i++)
	{
		if((hist[i]	= (TH1D*)gROOT->Get(Name[i].Data())))
			hist[i]->Delete();
		hist[i]	= (TH1D*)source.hist[i]->Clone(Name[i].Data());
		if(!hist[i])
			return false;
	}
	return true;
}
	
inline	void	TreeHistTagged::Clear()
{
	hist[0]->Reset("M");
	hist[1]->Reset("M");
	hist[2]->Reset("M");
	BG_Substracted	= false;
	/*if(hist[3])
		hist[3]->Delete();
	if(hist[4])
		hist[4]->Delete();*/
}

void	TreeHistTagged::Add(const TreeHistTagged& source, const Double_t f)
{
	hist[0]->Add(source.hist[0], f);
	hist[1]->Add(source.hist[1], f);
	hist[2]->Add(source.hist[2], f);
}
inline	void	TreeHistTagged::Fill(const Int_t* NTagged, const Double_t* Prompt, const Double_t* Rand1, const Double_t* Rand2)
{
	for(int i=0; i<NTagged[0]; i++)
		hist[0]->Fill(Prompt[i]);
	for(int i=0; i<NTagged[1]; i++)
		hist[1]->Fill(Rand1[i]);
	for(int i=0; i<NTagged[2]; i++)
		hist[2]->Fill(Rand2[i]);
}
inline	void	TreeHistTagged::Fill(const Int_t type, const Double_t value)
{
	hist[type]->Fill(value);
}
inline	void	TreeHistTagged::Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t value)
{
	for(int i=0; i<nPrompt; i++)
		hist[0]->Fill(value);
	for(int i=0; i<nRand1; i++)
		hist[1]->Fill(value);
	for(int i=0; i<nRand2; i++)
		hist[2]->Fill(value);
}

void	TreeHistTagged::SubstractBackground(const Char_t* NameBG, const Char_t* NameResult)
{
	hist[3]	= (TH1D*)hist[1]->Clone(NameBG);
	hist[3]->Add(hist[2]);
	hist[3]->Scale(0.5);
	hist[4]	= (TH1D*)hist[0]->Clone(NameResult);
	hist[4]->Add(hist[3], -1);
	BG_Substracted = true;
}

void	TreeHistTagged::Save(const Int_t index)
{
	if(index<0)
	{
		//printf("Save %d %s %s %s\n", index, hist[0]->GetName(), hist[1]->GetName(), hist[2]->GetName());
		hist[0]->Write();
		hist[1]->Write();
		hist[2]->Write();
		return;
	}
	
	if(index>4)
		return;
		
	if(index>2)
	{
		if(!BG_Substracted)
			return;
	}
	//printf("Save %d %s \n", index, hist[index]->GetName());
	hist[index]->Write();
}











