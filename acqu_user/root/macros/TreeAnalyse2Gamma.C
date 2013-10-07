#include "TreeReadTagged.C"
#include "TaggedHistSet.C"

#define MASS_PROTON	938.27203

class	TreeAnalyse2Gamma	: public TreeReadTagged
{
private:	
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*			hCutIMCount;
	TaggedHistSet*	hAll;
	TaggedHistSet*	h[3];		//[Pi0, Eta, EtaP]
	
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse2Gamma(const Char_t* FileName);
	~TreeAnalyse2Gamma();
	
			void	Clear();
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutIMPi0()		const	{return cutIMPi0;}
	const	Double_t	GetCutIMPi0Min()	const	{return cutIMPi0[0];}
	const	Double_t	GetCutIMPi0Max()	const	{return cutIMPi0[1];}
	const	Double_t*	GetCutIMEta()		const	{return cutIMEta;}
	const	Double_t	GetCutIMEtaMin()	const	{return cutIMEta[0];}
	const	Double_t	GetCutIMEtaMax()	const	{return cutIMEta[1];}
	const	Double_t*	GetCutIMEtaP()		const	{return cutIMEtaP;}
	const	Double_t	GetCutIMEtaPMin()	const	{return cutIMEtaP[0];}
	const	Double_t	GetCutIMEtaPMax()	const	{return cutIMEtaP[1];}
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			
	
	static	TreeAnalyse2Gamma*	test()
	{
		printf("Creating\n");
		TreeAnalyse2Gamma* c = new TreeAnalyse2Gamma("tree_TTreeOutput_41941_2g.root");
		printf("Opening\n");
		c->Open();
		printf("Analysing\n");
		c->Analyse();
		printf("Saving\n");
		c->Save();
		printf("End\n");
		return c;
	}
};





TreeAnalyse2Gamma::TreeAnalyse2Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if((hCutIMCount	= (TH1I*)gROOT->Get("IMCutCount")))
		hCutIMCount->Delete();
	hCutIMCount		= new TH1I("IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
	if(!hCutIMCount)
	{
		cout << "Could not create histogram " << "IMCutCount" << ". Exiting!" << endl;
		exit(1);
	}
			
	hAll = new TaggedHistSet("");
	h[0] = new TaggedHistSet("Pi0");
	h[1] = new TaggedHistSet("Eta");
	h[2] = new TaggedHistSet("EtaP");
		
	Clear();
}
TreeAnalyse2Gamma::~TreeAnalyse2Gamma()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

inline	void	TreeAnalyse2Gamma::Clear()	
{
	hCutIMCount->Reset("M");
	hAll->Clear();
	for(int i=0; i<3; i++)
		h[i]->Clear();
}

void	TreeAnalyse2Gamma::Open()
{
	//printf("TreeAnalyseMultiplicity::Open()\n");
	TreeReadTagged::Open();
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
	
	Char_t	str[128];
	for(int i=0; i<3; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_IMPi0.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_IMEta.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%s_IMEtaP.root", GetFileName());
			
		outFile[i]		= new TFile(str, "RECREATE");
		outTree[i]		= new TTree("tree", "tree");
		
		outTree[i]->Branch("nPrompt",&nTagged[0],"nPrompt/I");
		outTree[i]->Branch("PromptEnergy", TaggedEnergy[0], "PromptEnergy[nPrompt]/D");
		outTree[i]->Branch("PromptTime", TaggedTime[0], "PromptTime[nPrompt]/D");
		outTree[i]->Branch("nRand1",&nTagged[1],"nRand1/I");
		outTree[i]->Branch("Rand1Energy", TaggedEnergy[1], "Rand1Energy[nRand1]/D");
		outTree[i]->Branch("Rand1Time", TaggedTime[1], "Rand1Time[nRand1]/D");
		outTree[i]->Branch("nRand2",&nTagged[2],"nRand2/I");
		outTree[i]->Branch("Rand2Energy", TaggedEnergy[2], "Rand2Energy[nRand2]/D");
		outTree[i]->Branch("Rand2Time", TaggedTime[2], "Rand2Time[nRand2]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

bool	TreeAnalyse2Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	massAll	= vecAll.M();
	
	hCutIMCount->Fill(1);
	hAll->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutIMCount->Fill(2);
		h[0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[0]->Fill();
		return true;
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutIMCount->Fill(3);
		h[1]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[1]->Fill();
		return true;
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutIMCount->Fill(4);
		h[2]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCBHits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi());
		outTree[2]->Fill();
		return true;
	}
	
	return false;
}
void	TreeAnalyse2Gamma::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse2Gamma::Save()
{
	for(int i=0; i<3; i++)
	{
		outFile[i]->cd();
		outTree[i]->Write();
		outFile[i]->Flush();
	}
	
	hAll->SubstractBackground();
	h[0]->SubstractBackground();
	h[1]->SubstractBackground();
	h[2]->SubstractBackground();

	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCutIMCount->Write();
	hAll->Save();
	
	result->cd();
	result->mkdir("Pi0");
	result->cd("Pi0");
	h[0]->Save();
	
	result->cd();
	result->mkdir("Eta");
	result->cd("Eta");
	h[1]->Save();
	
	result->cd();
	result->mkdir("EtaP");
	result->cd("EtaP");
	h[2]->Save();
	
	result->Close();
	delete result;
}

