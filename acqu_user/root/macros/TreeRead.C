



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
	
	Int_t		nCBHits;
	Double_t	Px[32];
	Double_t	Py[32];
	Double_t	Pz[32];
	Double_t	E[32];
	Double_t	CBTime[32];
	
	
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
	bool	oldVersion = false;
	tree	= (TTree*)file->Get("tree");
	if(!tree)
	{
		tree	= (TTree*)file->Get(fileName);
		if(!tree)
		{
			printf("Could not open tree in file %s\n", str);
			return false;
		}
		oldVersion = true;
	}
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	if(oldVersion)
		tree->SetBranchAddress("Time", &CBTime);
	else
		tree->SetBranchAddress("CBTime", &CBTime);
	
	printf("Open file %s and load tree successfully.    %ld Events at all\n", str, (long int)tree->GetEntries());

	isOpen = true;
}

void	TreeRead::Print()
{
	printf("Entry:\tNTagged: %d\tNCB: %d\n", nTagged, nCBHits);
}
