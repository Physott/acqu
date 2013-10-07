

class	TaggedHist
{
private:
	TString	name;
	TH1D*	hist[5];
	Bool_t	BG_Substracted;
	
protected:

public:
	TaggedHist(const Char_t* Name, const Int_t NBin, const Double_t Min, const Double_t Max, const Char_t* Title = "");
	TaggedHist(const Char_t* Name, const TaggedHist* source);
	~TaggedHist();
	
	void	Clear();
	void	Add(const TaggedHist* source, const Double_t f = 1);
	void	Fill(const Int_t type, const Double_t value)														{hist[type]->Fill(value);}
	void	FillPrompt(const Int_t nTagged, const Double_t* value);
	void	FillRand1(const Int_t nTagged, const Double_t* value);
	void	FillRand2(const Int_t nTagged, const Double_t* value);
	void	Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t value);
	void	Fill(const Int_t* nTagged, const Double_t value)													{Fill(nTagged[0], nTagged[1], nTagged[2], value);}
	void	Fill(const Int_t* nTagged, const Double_t* prompt, const Double_t* rand1, const Double_t* rand2)	{FillPrompt(nTagged[0], prompt); FillRand1(nTagged[1], rand1); FillRand2(nTagged[2], rand2);}
	void	SubstractBackground();
	void	Save(const Int_t index);
	void	SaveSubs()		{Save(0); Save(1); Save(2); Save(3);}
	void	SaveResult()	{Save(4);}
	void	Save()			{SaveSubs(); SaveResult();}
};


TaggedHist::TaggedHist(const Char_t* Name, const Int_t NBin, const Double_t Min, const Double_t Max, const Char_t* Title)	: name(Name), BG_Substracted(false)
{
	TString	map[3];
	map[0]	= "_Prompt";
	map[1]	= "_Rand1";
	map[2]	= "_Rand2";
	TString	str;
	for(int i=0; i<3; i++)
	{
		str	 = name;
		str	+= map[i];
		if((hist[i]	= (TH1D*)gROOT->Get(str.Data())))
			hist[i]->Delete();
		if(Title[0]=='\0')
			hist[i]	= new TH1D(str.Data(), str.Data(), NBin, Min, Max);
		else
			hist[i]	= new TH1D(str.Data(), Title, NBin, Min, Max);
		if(!hist[i])
		{
			cout << "Could not create histogram " << name.Data() << ". Exiting!" << endl;
			exit(1);
		}
	}
}
TaggedHist::TaggedHist(const Char_t* Name, const TaggedHist* source)	: name(Name), BG_Substracted(false)
{
	TString	map[3];
	map[0]	= "_Prompt";
	map[1]	= "_Rand1";
	map[2]	= "_Rand2";
	TString	str;
	for(int i=0; i<3; i++)
	{
		str	 = name;
		str	+= map[i];
		if((hist[i]	= (TH1D*)gROOT->Get(str)))
			hist[i]->Delete();
		hist[i]	= (TH1D*)source->hist[i]->Clone(str);
		if(!hist[i])
		{
			cout << "Could not create histogram " << name.Data() << " from source. Exiting!" << endl;
			exit(1);
		}
	}
}

TaggedHist::~TaggedHist()
{
}
	
inline	void	TaggedHist::Clear()
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

void	TaggedHist::Add(const TaggedHist* source, const Double_t f)
{
	hist[0]->Add(source->hist[0], f);
	hist[1]->Add(source->hist[1], f);
	hist[2]->Add(source->hist[2], f);
}
inline	void	TaggedHist::FillPrompt(const Int_t nTagged, const Double_t* value)
{
	for(int i=0; i<nTagged; i++)
		hist[0]->Fill(value[i]);
}
inline	void	TaggedHist::FillRand1(const Int_t nTagged, const Double_t* value)
{
	for(int i=0; i<nTagged; i++)
		hist[1]->Fill(value[i]);
}
inline	void	TaggedHist::FillRand2(const Int_t nTagged, const Double_t* value)
{
	for(int i=0; i<nTagged; i++)
		hist[2]->Fill(value[i]);
}
	
inline	void	TaggedHist::Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t value)
{
	for(int i=0; i<nPrompt; i++)
		hist[0]->Fill(value);
	for(int i=0; i<nRand1; i++)
		hist[1]->Fill(value);
	for(int i=0; i<nRand2; i++)
		hist[2]->Fill(value);
}

void	TaggedHist::SubstractBackground()
{
	TString	str;
	str	 = name;
	str	+= "_BG";
	if((hist[3]	= (TH1D*)gROOT->Get(str.Data())))
			hist[3]->Delete();
	hist[3]	= (TH1D*)hist[1]->Clone(str.Data());
	hist[3]->Add(hist[2]);
	hist[3]->Scale(0.5);
	
	if((hist[4]	= (TH1D*)gROOT->Get(name.Data())))
			hist[4]->Delete();
	hist[4]	= (TH1D*)hist[0]->Clone(name.Data());
	hist[4]->Add(hist[3], -1);
	BG_Substracted = true;
}

void	TaggedHist::Save(const Int_t index)
{
	if(index<0)
		return;
	if(index>4)
		return;
	if(index>2)
	{
		if(!BG_Substracted)
			return;
	}
	//cout << "Saving histogram " << hist[index]->GetName() << endl;
	hist[index]->Write();
}











