#include "TreeRead.C"


#define MASS_PROTON	938.27203


enum	ETA2G
{
	ETA2G_EtaP	= 0,
	ETA2G_Eta,
	ETA2G_Pi0
};


class	TreeAnalyseEtaP2GammaTaggMissMass	: public TreeRead
{
private:
	ETA2G		type;
	TFile*		outFile;
	TTree*		outTree;
	
	Double_t	cut[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		beamEnergy[16];
	Double_t		beamTime[16];
	Int_t			nBeam;
	TLorentzVector	beam[16];
	TLorentzVector	miss[16];
	Double_t		missMass[16];
	
	TH1I*	hCutCount;
	TH1D*	hCut;
	TH1D*	hCutCheck;
	TH1D*	hIM;

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseEtaP2GammaTaggMissMass(const Char_t* FileName, const ETA2G Type = ETA2G_EtaP);
	~TreeAnalyseEtaP2GammaTaggMissMass();
	
			void	Clear()													{hCutCount->Reset("M"); hCut->Reset("M"); hCutCheck->Reset("M"); hIM->Reset("M");}
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	
	static	void	Analyse(const Char_t* FileName)
	{
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(FileName);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
	}
	static	TreeAnalyseEtaP2GammaTaggMissMass*	test()
	{
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass("tree_2g_Prompt.root");
		c->Open();
		//c->Analyse();
		//c->Save();
		return c;
	}
};








TreeAnalyseEtaP2GammaTaggMissMass::TreeAnalyseEtaP2GammaTaggMissMass(const Char_t* FileName, const ETA2G Type)	: TreeRead(FileName), type(Type), outTree(0), outFile(0)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if(!(hCutCount	= (TH1I*)gROOT->Get("2G_MM_CutCount")))
		hCutCount	= new TH1I("2G_MM_CutCount", "1:All/2:Passed", 5, 0, 5);
	if(!(hCut		= (TH1D*)gROOT->Get("2G_MM_Cut")))
		hCut		= new TH1D("2G_MM_Cut", "2G_MM_Cut", 3200, -1600, 1600);
	if(!(hCutCheck	= (TH1D*)gROOT->Get("2G_MM_CutCheck")))
		hCutCheck	= new TH1D("2G_MM_CutCheck", "2G_MM_CutCheck", 450, 700, 1150);
	
	Double_t	min	= 800;
	Double_t	max	= 1100;	
	if(type == ETA2G_Eta)
	{
		min	= 400;
		max	= 700;
	}
	else if(type == ETA2G_Pi0)
	{
		min	= 0;
		max	= 300;
	}	
	if((hIM	= (TH1D*)gROOT->Get("2G_IM")))
		hIM->Delete();
	hIM		= new TH1D("2G_IM", "2G_IM", max-min, min, max);
		
	/*TString	str(FileName);
	if(str.EndsWith("_Prompt.root"))
	{
		printf("TaggMarker found: Prompt data\n");
	}
	else if(str.EndsWith("_Rand1.root"))
	{
		printf("TaggMarker found: Rand1 data\n");
	}
	else if(str.EndsWith("_Rand2.root"))
	{
		printf("TaggMarker found: Rand2 data\n");
	}
	else
	{
		printf("ERROR: No TaggMarker found in Filename (_Prompt.root, _Rand1.root, _Rand2.root)\n");
	}*/
	
	Clear();
}
TreeAnalyseEtaP2GammaTaggMissMass::~TreeAnalyseEtaP2GammaTaggMissMass()
{
	if(outTree)
		delete	outTree;
	if(outFile)
		delete	outFile;
}


void	TreeAnalyseEtaP2GammaTaggMissMass::Open()
{
	//printf("TreeAnalyseEtaP2GammaTaggMissMass::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseEtaP2GammaTaggMissMass::Open 2()\n");
	
	Char_t	str[128];
	sprintf(str, "%s_MM.root", GetFileName());
	
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

bool	TreeAnalyseEtaP2GammaTaggMissMass::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	
	nBeam	= 0;
	
	hCutCount->Fill(1);
	
	for(int i=0; i<nTagged; i++)
	{
		beam[nBeam].SetPxPyPzE(TaggedEnergy[i], 0, 0, TaggedEnergy[i] + MASS_PROTON);
		miss[nBeam]		= beam[nBeam] - vecAll;
		missMass[nBeam]	= miss[nBeam].M();
		hCut->Fill(missMass[nBeam]);
		
		if(missMass[nBeam] >= cut[0] && missMass[nBeam] <= cut[1])
		{
			hCutCount->Fill(3);
			hCutCheck->Fill(missMass[nBeam]);
			beamEnergy[nBeam]	= TaggedEnergy[i];
			beamTime[nBeam]		= TaggedTime[i];
			nBeam++;
		}
	}
	
	if(nBeam[i] > 0)
	{
		hCutCount->Fill(2);
		hIM->Fill(vecAll.M());
		outTree->Fill();
		return true;
	}
	return false;
}
void	TreeAnalyseEtaP2GammaTaggMissMass::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseEtaP2GammaTaggMissMass::Save()
{
	outFile->cd();
	outTree->Write();
	outFile->Flush();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_MM.root", GetFileName()+5);
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCutCount->Write();
	hCut->Write();
	hCutCheck->Write();
	hIM->Write();
	result->Close();
	delete result;
}

