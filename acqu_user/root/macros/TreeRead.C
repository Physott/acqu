



class	TreeRead
{
private:
	//general
	Char_t	fileName[128];
	TFile*	file;
	Bool_t	isOpen;


protected:
	TTree*	tree;
	
	//variables	
	Int_t		nTagged;
	Double_t	TaggedEnergy[64];
	Double_t	TaggedTime[64];
	
	Int_t		nCB_Hits;
	Double_t	CB_Px[32];
	Double_t	CB_Py[32];
	Double_t	CB_Pz[32];
	Double_t	CB_E[32];
	Double_t	CB_Time[32];
	
	Int_t		nTAPS_Hits;
	Double_t	TAPS_Px[32];
	Double_t	TAPS_Py[32];
	Double_t	TAPS_Pz[32];
	Double_t	TAPS_E[32];
	Double_t	TAPS_Time[32];
	
	
public:	
	TreeRead(const Char_t* FileName);
	~TreeRead();
	
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t i)		{tree->GetEntry(i);}
	virtual	void	Print();
	
	const	Bool_t	IsOpen()		const	{return isOpen;}
	const	Char_t*	GetFileName()	const	{return fileName;}
	const	TTree*	GetTree()		const	{return tree;}
	
	static	void	FindName(const Char_t* FileName, Char_t* Name);
	static	TreeRead*	test()
	{
		TreeRead* c = new TreeRead("TTreeOutput_41948_CUT_2_6_10.root");
		c->Open();
		c->Print();
		return c;
	}
};

void	TreeRead::FindName(const Char_t* FileName, Char_t* Name)
{
	TString	str(FileName);
	if(str.EndsWith(".root"))
	{
		str.Remove(TString::kTrailing,'t');
		str.Remove(TString::kTrailing,'o');
		str.Remove(TString::kTrailing,'o');
		str.Remove(TString::kTrailing,'r');
		str.Remove(TString::kTrailing,'.');
	}
	if(str.BeginsWith("tree_"))
	{
		str.Remove(TString::kLeading,'t');
		str.Remove(TString::kLeading,'r');
		str.Remove(TString::kLeading,'e');
		str.Remove(TString::kLeading,'e');
		str.Remove(TString::kLeading,'_');
	}
	
	strcpy(Name, str.Data());
}



TreeRead::TreeRead(const Char_t* FileName)	: file(0), tree(0), isOpen(false)
{
	//printf("TreeRead constructor\n");
	
	FindName(FileName, fileName);
	//strcpy(fileName, FileName);
}
TreeRead::~TreeRead()
{
	if(tree)
		delete	tree;
	if(file)
		delete	file;
}

bool	TreeRead::Open()
{
	if(isOpen)
		return true;
	
	Char_t	str[128];
	sprintf(str, "%s.root", fileName);
	
	file	= TFile::Open(str);
	if(!file )
	{
		sprintf(str, "tree_%s.root", fileName);
		file	= TFile::Open(str);
		if(!file )
		{
			printf("Could not open file %s\n", str);
			return false;
		}
	}
	tree	= (TTree*)file->Get("tree");
	if(!tree)
	{
		printf("Could not open tree in file %s\n", str);
		return false;
	}
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCB_Hits",&nCB_Hits);
	tree->SetBranchAddress("CB_Px", &CB_Px);
	tree->SetBranchAddress("CB_Py", &CB_Py);
	tree->SetBranchAddress("CB_Pz", &CB_Pz);
	tree->SetBranchAddress("CB_E", &CB_E);	
	tree->SetBranchAddress("CB_Time", &CB_Time);
	
	tree->SetBranchAddress("nTAPS_Hits",&nTAPS_Hits);
	tree->SetBranchAddress("TAPS_Px", &TAPS_Px);
	tree->SetBranchAddress("TAPS_Py", &TAPS_Py);
	tree->SetBranchAddress("TAPS_Pz", &TAPS_Pz);
	tree->SetBranchAddress("TAPS_E", &TAPS_E);	
	tree->SetBranchAddress("TAPS_Time", &TAPS_Time);
	
	printf("Open file %s and load tree successfully.    %ld Events at all\n", str, (long int)tree->GetEntries());

	isOpen = true;
}

void	TreeRead::Print()
{
	printf("Entry:\tNTagged: %d\tNCB: %d\tNTAPS: %d\n", nTagged, nCB_Hits, nTAPS_Hits);
}
