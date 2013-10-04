



class	TreeReadTagged
{
private:
	//general
	Char_t	fileName[128];
	TFile*	file;
	Bool_t	isOpen;


protected:
	TTree*	tree;
	
	//variables	
	Int_t		nTagged[3];
	Double_t	TaggedEnergy[3][8];
	Double_t	TaggedTime[3][8];
	
	Int_t		nCBHits;
	Double_t	Px[32];
	Double_t	Py[32];
	Double_t	Pz[32];
	Double_t	E[32];
	Double_t	CBTime[32];
	
	
public:	
	TreeReadTagged(const Char_t* FileName);
	~TreeReadTagged();
	
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t i)		{tree->GetEntry(i);}
	virtual	void	Print();
	
	const	Bool_t	IsOpen()		const	{return isOpen;}
	const	Char_t*	GetFileName()	const	{return fileName;}
	const	TTree*	GetTree()		const	{return tree;}
	
	static	void	FindName(const Char_t* FileName, Char_t* Name);
	static	TreeReadTagged*	test()
	{
		TreeReadTagged* c = new TreeReadTagged("TTreeOutput_41948_CUT_2_6_10.root");
		c->Open();
		c->Print();
		return c;
	}
};

void	TreeReadTagged::FindName(const Char_t* FileName, Char_t* Name)
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



TreeReadTagged::TreeReadTagged(const Char_t* FileName)	: file(0), tree(0), isOpen(false)
{
	//printf("TreeReadTagged constructor %s\n", FileName);
	FindName(FileName, fileName);
	//printf("TreeReadTagged constructor %s\n", FileName);
}
TreeReadTagged::~TreeReadTagged()
{
	if(tree)
		delete	tree;
	if(file)
		delete	file;
}

bool	TreeReadTagged::Open()
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
	bool	oldVersion = false;
	tree	= (TTree*)file->Get("tree");
	if(!tree)
	{
		printf("Could not open tree in file %s\n", str);
		return false;
	}
	
	tree->SetBranchAddress("nPrompt",&nTagged[0]);
	tree->SetBranchAddress("PromptEnergy",&TaggedEnergy[0]);
	tree->SetBranchAddress("PromptTime", &TaggedTime[0]);
	
	tree->SetBranchAddress("nRand1",&nTagged[1]);
	tree->SetBranchAddress("Rand1Energy",&TaggedEnergy[1]);
	tree->SetBranchAddress("Rand1Time", &TaggedTime[1]);
	
	tree->SetBranchAddress("nRand2",&nTagged[2]);
	tree->SetBranchAddress("Rand2Energy",&TaggedEnergy[2]);
	tree->SetBranchAddress("Rand2Time", &TaggedTime[2]);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	tree->SetBranchAddress("CBTime", &CBTime);
	
	printf("Open file %s and load tree successfully.    %ld Events at all\n", str, (long int)tree->GetEntries());

	isOpen = true;
}

void	TreeReadTagged::Print()
{
	printf("Entry:\tNTagged: %d\tNCB: %d\n", nTagged, nCBHits);
}
