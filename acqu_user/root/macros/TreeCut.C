


void	TreeCut(const char* treeFileName,const char* treeName,const int CBHits)
{
	TFile*	file	= new TFile(treeFileName);
	if(!file)
	{
		printf("Could not open file %s\n", treeFileName);
		return;
	}	
	TTree*	tree	= (TTree*)file->Get(treeName);
	if(!tree)
	{
		printf("Could not open tree %s in file %s\n", treeName, treeFileName);
		return;
	}
	
	Char_t	str[256];
	sprintf(str,"cutTree_%d.root",CBHits);
	TFile*	outFile	= new TFile(str, "RECREATE");
	if(!outFile)
	{
		printf("Could not open file %s\n", str);
		return;
	}
	sprintf(str,"cutTree_%d",CBHits);
	TTree*	outTree	= new TTree(str,str);
	if(!outTree)
	{
		printf("Could not create outputtree %s", str);
		return;
	}
	
	Int_t				nTagged;
    Double_t			TaggedEnergy[128];
    Double_t			TaggedTime[128];
    
    Int_t				nCBHits;
    Double_t			Px[64];
    Double_t			Py[64];
    Double_t			Pz[64];
    Double_t			E[64];
    Double_t			Time[64];
	
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	tree->SetBranchAddress("Time", &Time);
	
	
	outTree->Branch("nTagged",&nTagged,"nTagged/I");
	outTree->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	outTree->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
	
	outTree->Branch("nCBHits",&nCBHits,"nCBHits/I");
	outTree->Branch("Px", Px, "Px[nCBHits]/D");
	outTree->Branch("Py", Py, "Py[nCBHits]/D");
	outTree->Branch("Pz", Pz, "Pz[nCBHits]/D");
	outTree->Branch("E", E, "E[nCBHits]/D");	
	outTree->Branch("Time", Time, "Time[nCBHits]/D");
	
	printf("Number of Entries in origin:\t%d\n",tree->GetEntries());
	
	for(int i=0; i<tree->GetEntries(); i++)
	{
		tree->GetEntry(i);
		if(nCBHits==CBHits)
			outTree->Fill();
	}
	
	printf("Number of Entries in output:\t%d\n",outTree->GetEntries());
	
	//outTree->Print();
	outFile->Write();
	file->Close();
	outFile->Close();
}

void	TreeCreate(const char* treeFileName = "testTree.root",const char* treeName = "testTree")
{
	Int_t		nTagged;
	Double_t*	TaggedEnergy	= new Double_t[64];
    Double_t*	TaggedTime		= new Double_t[64];
    
    Int_t		nCBHits;
    Double_t*	Px				= new Double_t[128];
    Double_t*	Py				= new Double_t[128];
    Double_t*	Pz				= new Double_t[128];
    Double_t*	E				= new Double_t[128];
    Double_t*	Time			= new Double_t[128];
    
    printf("---------\n");
    printf("Init Tree\n");
    printf("---------\n");
    
    TFile*	file	= new TFile(treeFileName,"RECREATE");
	TTree*	tree	= new TTree(treeName, treeName);
    
	tree->Branch("nTagged",&nTagged,"nTagged/I");
	tree->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
	tree->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
	
	tree->Branch("nCBHits",&nCBHits,"nCBHits/I");
	tree->Branch("Px", Px, "Px[nCBHits]/D");
	tree->Branch("Py", Py, "Py[nCBHits]/D");
	tree->Branch("Pz", Pz, "Pz[nCBHits]/D");
	tree->Branch("E", E, "E[nCBHits]/D");	
	tree->Branch("Time", Time, "Time[nCBHits]/D");
	
	nTagged			= 3;
	TaggedEnergy[0]	= 1450;
	TaggedEnergy[1]	= 1460;
	TaggedEnergy[2]	= 1470;
	TaggedTime[0]	= 0;
	TaggedTime[1]	= 1;
	TaggedTime[2]	= 2;
	
	nCBHits			= 2;
	Px[0]			= 10;
	Px[1]			= 11;
	Py[0]			= 20;
	Py[1]			= 21;
	Pz[0]			= 30;
	Pz[1]			= 31;
	E[0]			= 40;
	E[1]			= 41;
	Time[0]			= -10;
	Time[1]			= -11;
	
	tree->Fill();
	
	nTagged			= 3;
	TaggedEnergy[0]	= 1450;
	TaggedEnergy[1]	= 1460;
	TaggedEnergy[2]	= 1470;
	TaggedTime[0]	= 0;
	TaggedTime[1]	= 1;
	TaggedTime[2]	= 2;
	
	nCBHits			= 3;
	Px[0]			= 10;
	Px[1]			= 11;
	Px[2]			= 12;
	Py[0]			= 20;
	Py[1]			= 21;
	Py[2]			= 22;
	Pz[0]			= 30;
	Pz[1]			= 31;
	Pz[2]			= 32;
	E[0]			= 40;
	E[1]			= 41;
	E[2]			= 42;
	Time[0]			= -10;
	Time[1]			= -11;
	Time[2]			= -12;
	
	tree->Fill();
	
	tree->Print();
	file->Write();
	file->Close();
}
