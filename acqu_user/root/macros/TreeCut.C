


void	TreeCut(const char* treeFileName,const char* treeName,const int CBHits, const char* outputFileName = "cutTree.root")
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
	
	tree->GetEntry(1);
	printf("nTagged: %d\n",nTagged);
	for(int i=0; i<nTagged; i++)
		printf("BeamEnergy: %d\tBeamTime: %lf\n",TaggedEnergy[i], TaggedTime[i]);
}
