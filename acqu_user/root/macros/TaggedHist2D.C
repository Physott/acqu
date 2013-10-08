

class	TaggedHist2D
{
private:
	TString	name;
	TH2D*	hist[5];
	Bool_t	BG_Substracted;
	
protected:

public:
	TaggedHist2D(const Char_t* Name, const Int_t XNBin, const Double_t XMin, const Double_t XMax, const Int_t YNBin, const Double_t YMin, const Double_t YMax, const Char_t* Title = "");
	TaggedHist2D(const Char_t* Name, const TaggedHist2D* source);
	~TaggedHist2D();
	
	void	Clear();
	void	Add(const TaggedHist2D* source, const Double_t f = 1);
	void	Fill(const Int_t type, const Double_t XValue, const Double_t YValue)												{hist[type]->Fill(XValue, YValue);}
	void	Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t XValue, const Double_t YValue);
	void	Fill(const Int_t* nTagged, const Double_t XValue, const Double_t YValue)											{Fill(nTagged[0], nTagged[1], nTagged[2], XValue, YValue);}
	void	SubstractBackground();
	void	Save(const Int_t index);
	void	SaveSubs()		{Save(0); Save(1); Save(2); Save(3);}
	void	SaveResult()	{Save(4);}
	void	Save()			{SaveSubs(); SaveResult();}
};


TaggedHist2D::TaggedHist2D(const Char_t* Name, const Int_t XNBin, const Double_t XMin, const Double_t XMax, const Int_t YNBin, const Double_t YMin, const Double_t YMax, const Char_t* Title)	: name(Name), BG_Substracted(false)
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
		if((hist[i]	= (TH2D*)gROOT->Get(str.Data())))
			hist[i]->Delete();
		if(Title[0]=='\0')
			hist[i]	= new TH2D(str.Data(), str.Data(), XNBin, XMin, XMax, YNBin, YMin, YMax);
		else
			hist[i]	= new TH2D(str.Data(), Title, XNBin, XMin, XMax, YNBin, YMin, YMax);
		if(!hist[i])
		{
			cout << "Could not create histogram " << name.Data() << ". Exiting!" << endl;
			exit(1);
		}
	}
}
TaggedHist2D::TaggedHist2D(const Char_t* Name, const TaggedHist2D* source)	: name(Name), BG_Substracted(false)
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
		if((hist[i]	= (TH2D*)gROOT->Get(str)))
			hist[i]->Delete();
		hist[i]	= (TH2D*)source->hist[i]->Clone(str);
		if(!hist[i])
		{
			cout << "Could not create histogram " << name.Data() << " from source. Exiting!" << endl;
			exit(1);
		}
	}
}

TaggedHist2D::~TaggedHist2D()
{
}
	
inline	void	TaggedHist2D::Clear()
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

void	TaggedHist2D::Add(const TaggedHist2D* source, const Double_t f)
{
	hist[0]->Add(source->hist[0], f);
	hist[1]->Add(source->hist[1], f);
	hist[2]->Add(source->hist[2], f);
}

inline	void	TaggedHist2D::Fill(const Int_t nPrompt, const Int_t nRand1, const Int_t nRand2, const Double_t XValue, const Double_t YValue)
{
	for(int i=0; i<nPrompt; i++)
		hist[0]->Fill(XValue, YValue);
	for(int i=0; i<nRand1; i++)
		hist[1]->Fill(XValue, YValue);
	for(int i=0; i<nRand2; i++)
		hist[2]->Fill(XValue, YValue);
}

void	TaggedHist2D::SubstractBackground()
{
	TString	str;
	str	 = name;
	str	+= "_BG";
	if((hist[3]	= (TH2D*)gROOT->Get(str.Data())))
			hist[3]->Delete();
	hist[3]	= (TH2D*)hist[1]->Clone(str.Data());
	hist[3]->Add(hist[2]);
	hist[3]->Scale(0.5);
	
	if((hist[4]	= (TH2D*)gROOT->Get(name.Data())))
			hist[4]->Delete();
	hist[4]	= (TH2D*)hist[0]->Clone(name.Data());
	hist[4]->Add(hist[3], -1);
	BG_Substracted = true;
}

void	TaggedHist2D::Save(const Int_t index)
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











