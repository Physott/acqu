#include "TreeRead.C"


#define MASS_PROTON	938.27203

class	TreeAnalyse6GammaTagg	: public TreeRead
{
private:
	bool		isEta2Pi0;
	TFile*		outFile[3];
	TTree*		outTree[3];
	
	Double_t	cutTagg[3][2];
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	
	Double_t		massAll;
	TLorentzVector	part[3];
	TLorentzVector	partSet[3];
	TLorentzVector	allSet;
	Double_t		massSet;
	
	Double_t		beamEnergy[3][16];
	Double_t		beamTime[3][16];
	Int_t			nBeam[3];
	TLorentzVector	beam[3][64];
	
	TH1I*	hCutTagg;
	TH1D*	hCutCheck[3]
	TH1D*	hIMPart[3][3];
	TH1D*	hIMPartDiced[3][3];
	TH1D*	hIM[3];
	TH1D*	hIMSet[3];
	
	
	void	SetMass(const int index, const Double_t mass);
	void	Reconstruct();
	
protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse6GammaTagg(const Char_t* FileName, const bool eta2Pi0);
	~TreeAnalyse6GammaTagg();
	
			void	Clear();
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
	
	
	static	TreeAnalyse6GammaTagg*	test()
	{
		TreeAnalyse6GammaTagg* c = new TreeAnalyse6GammaTagg("tree_2g.root");
		c->Open();
		c->Analyse();
		c->Save();
		return c;
	}
};






TreeAnalyse6GammaTagg::TreeAnalyse6GammaTagg(const Char_t* FileName, const bool Type)	: TreeRead(FileName), isEta2Pi0(eta2Pi0)
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
	
	if(!(hCutTagg	= (TH1I*)gROOT->Get("6G_Tagg_Cut")))
		hCutTagg	= new TH1I("6G_Tagg_Cut", "1:All/2,3,4:(Prompt,Rand1,Rand2)/6,7,8:MultiFilled(Prompt,Rand1,Rand2)", 10, 0, 10);
	if(!(hCutCheck[0]	= (TH1D*)gROOT->Get("6G_Prompt_CutCheck")))
		hCutCheck[0]	= new TH1D("6G_Prompt_CutCheck", "6G_Prompt_CutCheck", 200, -100, 100);
	if(!(hCutCheck[1]	= (TH1D*)gROOT->Get("6G_Rand1_CutCheck")))
		hCutCheck[1]	= new TH1D("6G_Rand1_CutCheck", "6G_Rand1_CutCheck", 200, -100, 100);
	if(!(hCutCheck[2]	= (TH1D*)gROOT->Get("6G_Rand2_CutCheck")))
		hCutCheck[2]	= new TH1D("6G_Rand2_CutCheck", "6G_Rand2_CutCheck", 200, -100, 100);
	
	Double_t	min	= 0;
	Double_t	max	= 300;	
	if(isEta2Pi0)
	{
		min	= 400;
		max	= 700;
	}
	
	Char_t	str[128];
	Char_t	help[3][16];
	sprintf(help[0], "Prompt");
	sprintf(help[1], "Rand1");
	sprintf(help[1], "Rand2");
	
	for(int i=0; i<3; i++)
	{
		sprintf(str, "6G_%s_IMPi0a", help[i]);
		if(!(hIMPart[i][0]	= (TH1D*)gROOT->Get(str)))
			hIMPart[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_IMPi0b", help[i]);
		if(!(hIMPart[i][1]	= (TH1D*)gROOT->Get(str)))
			hIMPart[i][1]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_IMPi0aDiced", help[i]);
		if(!(hIMPartDiced[i][0]	= (TH1D*)gROOT->Get(str)))
			hIMPartDiced[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_IMPi0bDiced", help[i]);
		if(!(hIMPartDiced[i][1]	= (TH1D*)gROOT->Get(str)))
			hIMPartDiced[i][1]	= new TH1D(str, str, 300, 0, 300);
		if(isEta2Pi0)
		{
			sprintf(str, "6G_%s_IMEta", help[i]);
			if(!(hIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hIMPart[i][2]	= new TH1D(str, str, 300, 400, 700);
			sprintf(str, "6G_%s_IMEtaDiced", help[i]);
			if(!(hIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hIMPartDiced[i][2]	= new TH1D(str, str, 300, 400, 700);
		}
		else
		{
			sprintf(str, "6G_%s_IMPi0c", help[i]);
			if(!(hIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hIMPart[i][2]	= new TH1D(str, str, 300, 0, 300);
			sprintf(str, "6G_%s_IMPi0cDiced", help[i]);
			if(!(hIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hIMPartDiced[i][2]	= new TH1D(str, str, 300, 0, 300);
		}
		sprintf(str, "6G_%s_IM", help[i]);
		if(!(hIM[i]	= (TH1D*)gROOT->Get(str)))
			hIM[i]	= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "6G_%s_IMSet", help[i]);
		if(!(hIMSet[i]	= (TH1D*)gROOT->Get(str)))
			hIMSet[i]	= new TH1D(str, str, 1600, 0, 1600);
	}
	Clear();
}
TreeAnalyse6GammaTagg::~TreeAnalyse6GammaTagg()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}

void	TreeAnalyse6GammaTagg::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}

void	TreeAnalyse6GammaTagg::Clear()
{
	hCutTagg->Reset("M");
	for(int i=0; i<3; i++)
	{
		hCutCheck->Reset("M");
		for(int l=0; l<3; l++)
		{
			hIMPart[i][l]->Reset("M");
			hIMPartDiced[i][l]->Reset("M");
		}
		hIM[i]->Reset("M");
		hIMSet[i]->Reset("M");
	}
}

void	TreeAnalyse6GammaTagg::Open()
{
	//printf("TreeAnalyse6GammaTagg::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyse6GammaTagg::Open 2()\n");
	
	Char_t	str[128];
	//printf("GetFileName: %s\n", GetFileName());
	sprintf(str, "%s_Prompt.root", GetFileName());
	outFile[0]	= new TFile(str, "RECREATE");
	outTree[0]	= new TTree("tree", "tree");
	sprintf(str, "%s_Rand1.root", GetFileName());
	outFile[1]	= new TFile(str, "RECREATE");
	outTree[1]	= new TTree("tree", "tree");
	sprintf(str, "%s_Rand2.root", GetFileName());
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

bool	TreeAnalyse6GammaTagg::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
	vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
	vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
	vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
	part[0]	= vec[0] + vec[1];
	part[1]	= vec[2] + vec[3];
	part[2]	= vec[4] + vec[5];
	partSet[0]	= part[0];
	partSet[1]	= part[1];
	partSet[2]	= part[2];
	vecAll	= part[0] + part[1] + part[2];
	massAll	= vecAll.M();
	if(isEta2Pi0)
		SetMass(0, MASS_ETA);
	else
		SetMass(0, MASS_PI0);
	SetMass(1, MASS_PI0);
	SetMass(2, MASS_PI0);
	allSet	= partSet[0] + partSet[1] + partSet[2];
	massSet	= allSet.M();
	
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
	static	bool	dice		= false;
	static	int		dice3pi0	= 0;
	for(int i=0; i<3; i++)
	{
		if(nBeam[i] > 0)
		{
			hCutTagg->Fill(i+2);
			hIMPart[i][0]->Fill(part[0].M());
			hIMPart[i][1]->Fill(part[1].M());
			hIMPart[i][2]->Fill(part[2].M());
			if(isEta2Pi0)
			{
				if(dice)
				{
					hIMPartDiced[i][0]->Fill(part[1].M());
					hIMPartDiced[i][1]->Fill(part[2].M());
					dice	= false;
				}
				else
				{
					hIMPartDiced[i][0]->Fill(part[2].M());
					hIMPartDiced[i][1]->Fill(part[1].M());
					dice	= true;
				}
				hIMPartDiced[i][2]->Fill(part[0].M());
			}
			else
			{
				if(dice3pi0==0)
				{
					hIMPartDiced[i][0]->Fill(part[0].M());
					hIMPartDiced[i][1]->Fill(part[1].M());
					hIMPartDiced[i][2]->Fill(part[2].M());
					dice3pi0	= 1;
				}
				else if(dice3pi0==1)
				{
					hIMPartDiced[i][0]->Fill(part[2].M());
					hIMPartDiced[i][1]->Fill(part[0].M());
					hIMPartDiced[i][2]->Fill(part[1].M());
					dice3pi0	= 2;
				}
				else
				{
					hIMPartDiced[i][0]->Fill(part[1].M());
					hIMPartDiced[i][1]->Fill(part[2].M());
					hIMPartDiced[i][2]->Fill(part[0].M());
					dice3pi0	= 0;
				}
				
			}
			hIM[i]->Fill(massAll);
			hIMSet[i]->Fill(massSet);
			outTree[i]->Fill();
			ret	= true;
		}
	}
	return ret;
}
void	TreeAnalyse6GammaTagg::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse6GammaTagg::Save()
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
	
	/*TH1D*	BG	= hIM[1]->Clone("6G_BG_IM");
			BG->Add(hIM[2]);
			BG->Scale(0.5);
	TH1D*	res	= hIM[0]->Clone("6G_IM");
			res->Add(BG, -1);*/
	
	Char_t	str[128];
	sprintf(str, "hist_%s_Tagg.root", GetFileName()+5);
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
	result->cd();
	hCutTagg->Write();
	hCutCheck[0]->Write();
	hCutCheck[1]->Write();
	hCutCheck[2]->Write();
	for(int i=0; i<3; i++)
	{
		for(int l=0; l<3; l++)
			hIMPart[i][l]->Write();
		for(int l=0; l<3; l++)
			hIMPartDiced[i][l]->Write();
		hIM[i]->Write();
		hIMSet[i]->Write();
	}
	result->Close();
	delete result;
}


