#include "TreeRead.C"
#include "TreeHistGeneral.C"


#define MASS_PROTON	938.27203

class	TreeAnalyseMissMass	: public TreeRead
{
private:
	TFile*		outFile;
	TTree*		outTree;
	
	Double_t	cut[2];
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	TLorentzVector	beam[16];
	TLorentzVector	miss[16];
	Double_t		missMass[16];
	Double_t		beamEnergy[16];
	Double_t		beamTime[16];
	Int_t			nBeam;
	
	TH1I*			hCutCount;
	TH1D*			hMissMass;
	TH1D*			hCutCheck;
	TreeHistGeneral	hist[2];

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseMissMass(const Char_t* FileName);
	~TreeAnalyseMissMass();
	
			void	Clear();
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	

	static	TreeAnalyseMissMass*	test()
	{
		TreeAnalyseMissMass* c = new TreeAnalyseMissMass("tree_TTreeOutput_41941_2g_Prompt_IMPi0.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};








TreeAnalyseMissMass::TreeAnalyseMissMass(const Char_t* FileName)	: TreeRead(FileName), outTree(0), outFile(0)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount	= new TH1I("CutCount", "1:All/2:Passed/4:Passed (MultiFilled)", 6, 0, 6);
	if(!(hMissMass	= (TH1D*)gROOT->Get("MissMass")))
		hMissMass	= new TH1D("MissMass", "MissMass", 4000, -2000, 2000);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("MMCutCheck")))
		hCutCheck	= new TH1D("MMCutCheck", "MMCutCheck", 450, 700, 1150);
	
	TString	BaseName[9];
	BaseName[0]	= "NTagged";
	BaseName[1]	= "TaggedEnergy";
	BaseName[2]	= "TaggedTime";
	BaseName[3]	= "Multiplicity";
	BaseName[4]	= "CBTime";
	BaseName[5]	= "CBEnergyAll";
	BaseName[6]	= "IMAll";
	BaseName[7]	= "ThetaAll";
	BaseName[8]	= "PhiAll";
	Int_t		NBin[9]	=	{32,  200, 2000, 16, 2000, 2000, 2000,  180,  360};
	Double_t	Min[9]	=	{ 0, 1400, -100,  0, -100,    0,    0,    0, -180};
	Double_t	Max[9]	=	{32, 1600,  100, 16,  100, 2000, 2000,  180,  180};
	TString	Name[9];
	hist[0].Init(BaseName, BaseName, NBin, Min, Max);
	
	for(int i=0; i<9; i++)
		Name[i]	= BaseName[i];
	for(int i=0; i<9; i++)
		Name[i].Prepend("CutMM_");
	hist[1].Init(Name, Name, NBin, Min, Max);
	
	Clear();
}
TreeAnalyseMissMass::~TreeAnalyseMissMass()
{
	if(outTree)
		delete	outTree;
	if(outFile)
		delete	outFile;
}

inline	void	TreeAnalyseMissMass::Clear()
{
	hCutCount->Reset("M");
	hMissMass->Reset("M");
	hCutCheck->Reset("M");
	hist[0].Clear();
	hist[1].Clear();
}
void	TreeAnalyseMissMass::Open()
{
	//printf("TreeAnalyseMissMass::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseMissMass::Open 2()\n");
	
	Char_t	str[128];
	sprintf(str, "tree_%s_MM.root", GetFileName());
	
	outFile	= new TFile(str, "RECREATE");
	outTree	= new TTree("tree", "tree");
		
	outTree->Branch("nTagged",&nBeam,"nTagged/I");
	outTree->Branch("BeamEnergy", beamEnergy, "BeamEnergy[nTagged]/D");
	outTree->Branch("BeamTime", beamTime, "BeamTime[nTagged]/D");
		
	outTree->Branch("nCBHits",&nCBHits,"nCBHits/I");
	outTree->Branch("Px", Px, "Px[nCBHits]/D");
	outTree->Branch("Py", Py, "Py[nCBHits]/D");
	outTree->Branch("Pz", Pz, "Pz[nCBHits]/D");
	outTree->Branch("E", E, "E[nCBHits]/D");	
	outTree->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
}

bool	TreeAnalyseMissMass::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	if(nCBHits == 2)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
		vecAll	= vec[0] + vec[1];
	}	
	else if(nCBHits == 6)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
	}
	else if(nCBHits == 10)
	{
		vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
		vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
		vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
		vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
		vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
		vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
		vec[6].SetPxPyPzE(Px[6], Py[6], Pz[6], E[6]);
		vec[7].SetPxPyPzE(Px[7], Py[7], Pz[7], E[7]);
		vec[8].SetPxPyPzE(Px[8], Py[8], Pz[8], E[8]);
		vec[9].SetPxPyPzE(Px[9], Py[9], Pz[9], E[9]);
		vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5] + vec[6] + vec[7] + vec[8] + vec[9];
	}
	
	nBeam	= 0;
	
	hCutCount->Fill(1);
	
	for(int i=0; i<nTagged; i++)
	{
		beam[nBeam].SetPxPyPzE(TaggedEnergy[i], 0, 0, TaggedEnergy[i] + MASS_PROTON);
		miss[nBeam]		= beam[nBeam] - vecAll;
		missMass[nBeam]	= miss[nBeam].M();
		hMissMass->Fill(missMass[nBeam]);
		
		if(missMass[nBeam] >= cut[0] && missMass[nBeam] <= cut[1])
		{
			hCutCount->Fill(4);
			hCutCheck->Fill(missMass[nBeam]);
			beamEnergy[nBeam]	= TaggedEnergy[i];
			beamTime[nBeam]		= TaggedTime[i];
			nBeam++;
		}
	}
	
	hist[0].Fill(nTagged, TaggedEnergy, TaggedTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		
	if(nBeam > 0)
	{
		hCutCount->Fill(2);
		outTree->Fill();
		hist[1].Fill(nBeam, beamEnergy, beamTime, nCBHits, CBTime, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		return true;
	}
	
	return false;
}
void	TreeAnalyseMissMass::Analyse(const Int_t Min, const Int_t Max)
{
	Double_t	min = Min;
	Double_t	max = Max;
	if(min < 0)
		min = 0;
	if(min > tree->GetEntries())
		min = tree->GetEntries();
	if(max < 0 || max > tree->GetEntries())
		max = tree->GetEntries();
		
	for(int i=min; i<max; i++)
		AnalyseEvent(i);
}
bool	TreeAnalyseMissMass::Save()
{
	outFile->cd();
	outTree->Write();
	outFile->Flush();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_MM.root", GetFileName());
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hist[0].Save();
	hCutCount->Write();
	hMissMass->Write();
	hCutCheck->Write();
	hist[1].Save();
	result->Close();
	delete result;
}

