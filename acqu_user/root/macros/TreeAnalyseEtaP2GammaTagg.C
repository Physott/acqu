#include "TreeRead.C"


#define MASS_PROTON	938.27203


class	TreeAnalyseEtaP2GammaTagg	: public TreeRead
{
private:
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutTagg[3][2];
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		beamEnergy[3][16];
	Double_t		beamTime[3][16];
	Int_t			nBeam[3];
	TLorentzVector	beam[3][64];
	
	TH1I*	hCutTagg;
	TH1D*	hCutCheck[3];
	TH1D*	hIM[3];
	
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyseEtaP2GammaTagg(const Char_t* FileName);
	~TreeAnalyseEtaP2GammaTagg();
	
			void	Clear()													{hCutTagg->Reset("M"); hCutCheck[0]->Reset("M"); hCutCheck[1]->Reset("M"); hCutCheck[2]->Reset("M"); hIM[0]->Reset("M"); hIM[1]->Reset("M"); hIM[2]->Reset("M");}
	virtual	bool	Open();
			void	AnalyseEvent(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCutTagg()			const	{return cutTagg;}
	const	Double_t*	GetCutTaggPrompt()		const	{return cutTagg[0];}
	const	Double_t	GetCutTaggPromptMin()	const	{return cutTagg[0][0];}
	const	Double_t	GetCutTaggPromptMax()	const	{return cutTagg[0][1];}
	const	Double_t*	GetCutTaggRand1()		const	{return cutTagg[1];}
	const	Double_t	GetCutTaggRand1Min()	const	{return cutTagg[1][0];}
	const	Double_t	GetCutTaggRand1Max()	const	{return cutTagg[1][1];}
	const	Double_t*	GetCutTaggRand2()		const	{return cutTagg[2];}
	const	Double_t	GetCutTaggRand2Min()	const	{return cutTagg[2][0];}
	const	Double_t	GetCutTaggRand2Max()	const	{return cutTagg[2][1];}	
	
	
			void		SetCutTaggPrompt(const Double_t Min, const Double_t Max)	{cutTagg[0][0] = Min; cutTagg[0][1] = Max;}
			void		SetCutTaggRand1(const Double_t Min, const Double_t Max)		{cutTagg[1][0] = Min; cutTagg[1][1] = Max;}
			void		SetCutTaggRand2(const Double_t Min, const Double_t Max)		{cutTagg[2][0] = Min; cutTagg[2][1] = Max;}
	
	
	static	TreeAnalyseEtaP2GammaTagg*	test()
	{
		TreeAnalyseEtaP2GammaTagg* c = new TreeAnalyseEtaP2GammaTagg("tree_2g.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};






TreeAnalyseEtaP2GammaTagg::TreeAnalyseEtaP2GammaTagg(const Char_t* FileName)	: TreeRead(FileName)
{	
	for(int i=0; i<3; i++)
	{
		 outTree[i]	= 0;
		 outFile[i]	= 0;
	}
	
	cutTagg[0][0]	= -30;
	cutTagg[0][1]	= 30;
	cutTagg[1][0]	= -95;
	cutTagg[1][1]	= -35;
	cutTagg[2][0]	= 35;
	cutTagg[2][1]	= 95;
	
	if(!(hCutTagg	= (TH1I*)gROOT->Get("2G_Tagg_Cut")))
		hCutTagg	= new TH1I("2G_Tagg_Cut", "1:All/2,3,4:(Prompt,Rand1,Rand2)/6,7,8:MultiFilled(Prompt,Rand1,Rand2)", 10, 0, 10);
	if(!(hCutCheck[0]	= (TH1D*)gROOT->Get("2G_Prompt_CutCheck")))
		hCutCheck[0]	= new TH1D("2G_Prompt_CutCheck", "2G_Prompt_CutCheck", 200, -100, 100);
	if(!(hCutCheck[1]	= (TH1D*)gROOT->Get("2G_Rand1_CutCheck")))
		hCutCheck[1]	= new TH1D("2G_Rand1_CutCheck", "2G_Rand1_CutCheck", 200, -100, 100);
	if(!(hCutCheck[2]	= (TH1D*)gROOT->Get("2G_Rand2_CutCheck")))
		hCutCheck[2]	= new TH1D("2G_Rand2_CutCheck", "2G_Rand2_CutCheck", 200, -100, 100);
	if(!(hIM[0]	= (TH1D*)gROOT->Get("2G_Prompt_IM")))
		hIM[0]	= new TH1D("2G_Prompt_IM", "2G_Prompt_IM", 300, 800, 1100);
	if(!(hIM[1]	= (TH1D*)gROOT->Get("2G_Rand1_IM")))
		hIM[1]	= new TH1D("2G_Rand1_IM", "2G_Rand1_IM", 300, 800, 1100);
	if(!(hIM[2]	= (TH1D*)gROOT->Get("2G_Rand2_IM")))
		hIM[2]	= new TH1D("2G_Rand2_IM", "2G_Rand2_IM", 300, 800, 1100);
		
	Clear();
}
TreeAnalyseEtaP2GammaTagg::~TreeAnalyseEtaP2GammaTagg()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}


void	TreeAnalyseEtaP2GammaTagg::Open()
{
	//printf("TreeAnalyseEtaP2GammaTagg::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyseEtaP2GammaTagg::Open 2()\n");
	
	Char_t	str[128];
	sprintf(str, "%s_2g_Prompt.root", GetFileName());
	outFile[0]	= new TFile(str, "RECREATE");
	outTree[0]	= new TTree("tree", "tree");
	sprintf(str, "%s_2g_Rand1.root", GetFileName());
	outFile[1]	= new TFile(str, "RECREATE");
	outTree[1]	= new TTree("tree", "tree");
	sprintf(str, "%s_2g_Rand2.root", GetFileName());
	outFile[2]	= new TFile(str, "RECREATE");
	outTree[2]	= new TTree("tree", "tree");
		
	for(int i=0; i<3; i++)
	{
		outTree[i]->Branch("nTagged",&nBeam[i],"nTagged/I");
		outTree[i]->Branch("BeamEnergy", beamEnergy[i], "BeamEnergy[nTagged]/D");
		outTree[i]->Branch("BeamTime", beamTime[i], "BeamTime[nTagged]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", Px, "Px[nCBHits]/D");
		outTree[i]->Branch("Py", Py, "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", Pz, "Pz[nCBHits]/D");
		outTree[i]->Branch("E", E, "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
}

bool	TreeAnalyseEtaP2GammaTagg::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vecAll	= vec[0] + vec[1];
	
	nBeam[0]	= 0;
	nBeam[1]	= 0;
	nBeam[2]	= 0;
	
	hCutTagg->Fill(1);
	
	for(int i=0; i<nTagged; i++)
	{
		if(TaggedTime[i] >= cutTagg[0][0] && TaggedTime[i] <= cutTagg[0][1])
		{
			hCutTagg->Fill(6);
			hCutCheck[0]->Fill(TaggedTime[i]);
			beamEnergy[0][nBeam[0]]	= TaggedEnergy[i];
			beamTime[0][nBeam[0]]	= TaggedTime[i];
			beam[0][nBeam[0]].SetPxPyPzE(TaggedEnergy[i], 0, 0, TaggedEnergy[i] + MASS_PROTON);
			nBeam[0]++;
		}
		else if(TaggedTime[i] >= cutTagg[1][0] && TaggedTime[i] <= cutTagg[1][1])
		{
			hCutTagg->Fill(7);
			hCutCheck[1]->Fill(TaggedTime[i]);
			beamEnergy[1][nBeam[1]]	= TaggedEnergy[i];
			beamTime[1][nBeam[1]]	= TaggedTime[i];
			beam[1][nBeam[1]].SetPxPyPzE(TaggedEnergy[i], 0, 0, TaggedEnergy[i] + MASS_PROTON);
			nBeam[1]++;
		}
		else if(TaggedTime[i] >= cutTagg[2][0] && TaggedTime[i] <= cutTagg[2][1])
		{
			hCutTagg->Fill(8);
			hCutCheck[2]->Fill(TaggedTime[i]);
			beamEnergy[2][nBeam[2]]	= TaggedEnergy[i];
			beamTime[2][nBeam[2]]	= TaggedTime[i];
			beam[2][nBeam[2]].SetPxPyPzE(TaggedEnergy[i], 0, 0, TaggedEnergy[i] + MASS_PROTON);
			nBeam[2]++;
		}
	}
	
	bool	ret	= false;
	for(int i=0; i<3; i++)
	{
		if(nBeam[i] > 0)
		{
			hCutTagg->Fill(i+2);
			hIM[i]->Fill(vecAll.M());
			outTree[i]->Fill();
			ret	= true;
		}
	}
	return ret;
}
void	TreeAnalyseEtaP2GammaTagg::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyseEtaP2GammaTagg::Save()
{
	outFile[0]->cd();
	outTree[0]->Write();
	outFile[0]->Flush();
	
	outFile[1]->cd();
	outTree[1]->Write();
	outFile[1]->Flush();
	
	outFile[2]->cd();
	outTree[2]->Write();
	outFile[2]->Flush();
	
	TH1D*	BG	= hIM[1]->Clone("2G_BG_IM");
			BG->Add(hIM[2]);
			BG->Scale(0.5);
	TH1D*	res	= hIM[0]->Clone("2G_IM");
			res->Add(BG, -1);
	
	Char_t	str[128];
	sprintf(str, "%s_hist_2g_Tagg.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hCutTagg->Write();
	hCutCheck[0]->Write();
	hCutCheck[1]->Write();
	hCutCheck[2]->Write();
	hIM[0]->Write();
	hIM[1]->Write();
	hIM[2]->Write();
	BG->Write();
	res->Write();
	result->Close();
	delete result;
}

