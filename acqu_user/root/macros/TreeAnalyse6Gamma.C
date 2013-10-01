#include "TreeRead.C"

#define MASS_PI0	135
#define MASS_ETA	547

class	TreeAnalyse6Gamma	: public TreeRead
{
private:
	TFile*	outFile[2];
	TTree*	outTree[2];
	TFile*	outFileCutAll[2];
	TTree*	outTreeCutAll[2];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	static	int		perm[15][6];
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	Double_t		massAll;
	TLorentzVector	partSet[3];
	TLorentzVector	allSet;
	Double_t		massSet;
	
	Double_t		out[4][6];
	
	TH1I*	hCount;
	TH1D*	hIMPart[2][3];
	TH1D*	hIMPartDiced[2][3];
	TH1D*	hIM[2];
	TH1D*	hIMSet[2];
	TH1D*	hCutIMPart[2][3];
	TH1D*	hCutIMPartDiced[2][3];
	TH1D*	hCutIM[2];
	TH1D*	hCutIMSet[2];
	TH1D*	hCutAllIMPart[2][3];
	TH1D*	hCutAllIMPartDiced[2][3];
	TH1D*	hCutAllIM[2];
	TH1D*	hCutAllIMSet[2];
	
	void	SetMass(const int index, const Double_t mass);
	void	Reconstruct();

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse6Gamma(const Char_t* FileName);
	~TreeAnalyse6Gamma();
	
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
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			
	static	TreeAnalyse6Gamma*	test()
	{
		TreeAnalyse6Gamma* c = new TreeAnalyse6Gamma("tree_TTreeOutput_41948_6g.root");
		c->Open();
		c->Analyse(1001);
		c->Save();
		return c;
	}
};





TreeAnalyse6Gamma::TreeAnalyse6Gamma(const Char_t* FileName)	: TreeRead(FileName)
{
	outTree[0]	= 0;
	outTree[1]	= 0;
	outFile[0]	= 0;
	outFile[1]	= 0;
	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if(!(hCount	= (TH1I*)gROOT->Get("6G_Count")))
		hCount	= new TH1I("6G_Count", "1:All/2,3:(EtaP,3Pi0)/5,6:Passed(EtaP,3Pi0)/8,9:PassedAll(EtaP,3Pi0)", 11, 0, 11);
	
	Char_t	str[128];
	Char_t	help[2][16];
	sprintf(help[0], "EtaP");
	sprintf(help[1], "3Pi0");
	
	for(int i=0; i<2; i++)
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
		if(i==0)
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
	
	for(int i=0; i<2; i++)
	{
		sprintf(str, "6G_%s_CutIMPi0a", help[i]);
		if(!(hCutIMPart[i][0]	= (TH1D*)gROOT->Get(str)))
			hCutIMPart[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutIMPi0b", help[i]);
		if(!(hCutIMPart[i][1]	= (TH1D*)gROOT->Get(str)))
			hCutIMPart[i][1]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutIMPi0aDiced", help[i]);
		if(!(hCutIMPartDiced[i][0]	= (TH1D*)gROOT->Get(str)))
			hCutIMPartDiced[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutIMPi0bDiced", help[i]);
		if(!(hCutIMPartDiced[i][1]	= (TH1D*)gROOT->Get(str)))
			hCutIMPartDiced[i][1]	= new TH1D(str, str, 300, 0, 300);
		if(i==0)
		{
			sprintf(str, "6G_%s_CutIMEta", help[i]);
			if(!(hCutIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutIMPart[i][2]	= new TH1D(str, str, 300, 400, 700);
			sprintf(str, "6G_%s_CutIMEtaDiced", help[i]);
			if(!(hCutIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutIMPartDiced[i][2]	= new TH1D(str, str, 300, 400, 700);
		}
		else
		{
			sprintf(str, "6G_%s_CutIMPi0c", help[i]);
			if(!(hCutIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutIMPart[i][2]	= new TH1D(str, str, 300, 0, 300);
			sprintf(str, "6G_%s_CutIMPi0cDiced", help[i]);
			if(!(hCutIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutIMPartDiced[i][2]	= new TH1D(str, str, 300, 0, 300);
		}
		sprintf(str, "6G_%s_CutIM", help[i]);
		if(!(hCutIM[i]	= (TH1D*)gROOT->Get(str)))
			hCutIM[i]	= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "6G_%s_CutIMSet", help[i]);
		if(!(hCutIMSet[i]	= (TH1D*)gROOT->Get(str)))
			hCutIMSet[i]	= new TH1D(str, str, 1600, 0, 1600);
	}
	
	for(int i=0; i<2; i++)
	{
		sprintf(str, "6G_%s_CutAllIMPi0a", help[i]);
		if(!(hCutAllIMPart[i][0]	= (TH1D*)gROOT->Get(str)))
			hCutAllIMPart[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutAllIMPi0b", help[i]);
		if(!(hCutAllIMPart[i][1]	= (TH1D*)gROOT->Get(str)))
			hCutAllIMPart[i][1]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutAllIMPi0aDiced", help[i]);
		if(!(hCutAllIMPartDiced[i][0]	= (TH1D*)gROOT->Get(str)))
			hCutAllIMPartDiced[i][0]	= new TH1D(str, str, 300, 0, 300);
		sprintf(str, "6G_%s_CutAllIMPi0bDiced", help[i]);
		if(!(hCutAllIMPartDiced[i][1]	= (TH1D*)gROOT->Get(str)))
			hCutAllIMPartDiced[i][1]	= new TH1D(str, str, 300, 0, 300);
		if(i==0)
		{
			sprintf(str, "6G_%s_CutAllIMEta", help[i]);
			if(!(hCutAllIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutAllIMPart[i][2]	= new TH1D(str, str, 300, 400, 700);
			sprintf(str, "6G_%s_CutAllIMEtaDiced", help[i]);
			if(!(hCutAllIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutAllIMPartDiced[i][2]	= new TH1D(str, str, 300, 400, 700);
		}
		else
		{
			sprintf(str, "6G_%s_CutAllIMPi0c", help[i]);
			if(!(hCutAllIMPart[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutAllIMPart[i][2]	= new TH1D(str, str, 300, 0, 300);
			sprintf(str, "6G_%s_CutAllIMPi0cDiced", help[i]);
			if(!(hCutAllIMPartDiced[i][2]	= (TH1D*)gROOT->Get(str)))
				hCutAllIMPartDiced[i][2]	= new TH1D(str, str, 300, 0, 300);
		}
		sprintf(str, "6G_%s_CutAllIM", help[i]);
		if(!(hCutAllIM[i]	= (TH1D*)gROOT->Get(str)))
			hCutAllIM[i]	= new TH1D(str, str, 1600, 0, 1600);
		sprintf(str, "6G_%s_CutAllIMSet", help[i]);
		if(!(hCutAllIMSet[i]	= (TH1D*)gROOT->Get(str)))
			hCutAllIMSet[i]	= new TH1D(str, str, 1600, 0, 1600);
	}
}
TreeAnalyse6Gamma::~TreeAnalyse6Gamma()
{
	for(int i=0; i<2; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
		if(outTreeCutAll[i])
			delete	outTreeCutAll[i];
		if(outFileCutAll[i])
			delete	outFileCutAll[i];
	}
}

void	TreeAnalyse6Gamma::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}

void	TreeAnalyse6Gamma::Clear()
{
	hCount->Reset("M");
	for(int i=0; i<2; i++)
	{
		for(int l=0; l<3; l++)
		{
			hIMPart[i][l]->Reset("M");
			hIMPartDiced[i][l]->Reset("M");
			hCutIMPart[i][l]->Reset("M");
			hCutIMPartDiced[i][l]->Reset("M");
			hCutAllIMPart[i][l]->Reset("M");
			hCutAllIMPartDiced[i][l]->Reset("M");
		}
		hIM[i]->Reset("M");
		hIMSet[i]->Reset("M");
		hCutIM[i]->Reset("M");
		hCutIMSet[i]->Reset("M");
		hCutAllIM[i]->Reset("M");
		hCutAllIMSet[i]->Reset("M");
	}
}

void	TreeAnalyse6Gamma::Open()
{
	//printf("TreeAnalyse6Gamma::Open()\n");
	TreeRead::Open();
	//printf("TreeAnalyse6Gamma::Open 2()\n");
	
	Char_t	str[128];
	sprintf(str, "%s_IMEta2Pi0.root", GetFileName());
	outFile[0]	= new TFile(str, "RECREATE");
	outTree[0]		= new TTree("tree", "tree");
	sprintf(str, "%s_IM3Pi0.root", GetFileName());
	outFile[1]	= new TFile(str, "RECREATE");
	outTree[1]		= new TTree("tree", "tree");
	
	sprintf(str, "%s_IMAllEta2Pi0.root", GetFileName());
	outFileCutAll[0]	= new TFile(str, "RECREATE");
	outTreeCutAll[0]		= new TTree("tree", "tree");
	sprintf(str, "%s_IMAll3Pi0.root", GetFileName());
	outFileCutAll[1]	= new TFile(str, "RECREATE");
	outTreeCutAll[1]		= new TTree("tree", "tree");
	
	
	for(int i=0; i<2; i++)
	{
		outTree[i]->Branch("nTagged",&nTagged,"nTagged/I");
		outTree[i]->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
		outTree[i]->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
		outTree[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTree[i]->Branch("Px", out[0], "Px[nCBHits]/D");
		outTree[i]->Branch("Py", out[1], "Py[nCBHits]/D");
		outTree[i]->Branch("Pz", out[2], "Pz[nCBHits]/D");
		outTree[i]->Branch("E", out[3], "E[nCBHits]/D");	
		outTree[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
		
		
		outTreeCutAll[i]->Branch("nTagged",&nTagged,"nTagged/I");
		outTreeCutAll[i]->Branch("BeamEnergy", TaggedEnergy, "BeamEnergy[nTagged]/D");
		outTreeCutAll[i]->Branch("BeamTime", TaggedTime, "BeamTime[nTagged]/D");
		
		outTreeCutAll[i]->Branch("nCBHits",&nCBHits,"nCBHits/I");
		outTreeCutAll[i]->Branch("Px", out[0], "Px[nCBHits]/D");
		outTreeCutAll[i]->Branch("Py", out[1], "Py[nCBHits]/D");
		outTreeCutAll[i]->Branch("Pz", out[2], "Pz[nCBHits]/D");
		outTreeCutAll[i]->Branch("E", out[3], "E[nCBHits]/D");	
		outTreeCutAll[i]->Branch("CBTime", CBTime, "CBTime[nCBHits]/D");
	}
}
bool	TreeAnalyse6Gamma::AnalyseEvent(const Int_t i)
{
	TreeRead::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(Px[0], Py[0], Pz[0], E[0]);
	vec[1].SetPxPyPzE(Px[1], Py[1], Pz[1], E[1]);
	vec[2].SetPxPyPzE(Px[2], Py[2], Pz[2], E[2]);
	vec[3].SetPxPyPzE(Px[3], Py[3], Pz[3], E[3]);
	vec[4].SetPxPyPzE(Px[4], Py[4], Pz[4], E[4]);
	vec[5].SetPxPyPzE(Px[5], Py[5], Pz[5], E[5]);
	vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
	massAll	= vecAll.M();
	
	Reconstruct();
	
	hCount->Fill(1);
	static	bool	dice		= false;
	static	int		dice3Pi0	= 0;
	
	switch(bestEta)
	{
	case 0:
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hIMPart[0][0]->Fill(mass[bestPerm][1]);
		hIMPart[0][1]->Fill(mass[bestPerm][2]);
		hIMPart[0][2]->Fill(mass[bestPerm][0]);
		if(dice)
		{		
			hIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
			dice	= false;
		}
		else
		{
			hIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
			dice	= true;
		}
		hIMPartDiced[0][2]->Fill(mass[bestPerm][0]);
		hIM[0]->Fill(massAll);
		hIMSet[0]->Fill(massSet);
		
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][0]<cutIMEta[0] || mass[bestPerm][0]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hCutIMPart[0][0]->Fill(mass[bestPerm][1]);
		hCutIMPart[0][1]->Fill(mass[bestPerm][2]);
		hCutIMPart[0][2]->Fill(mass[bestPerm][0]);
		if(dice)
		{		
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
		}
		else
		{
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
		}
		hCutIMPartDiced[0][2]->Fill(mass[bestPerm][0]);
		hCutIM[0]->Fill(massAll);
		hCutIMSet[0]->Fill(massSet);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTree[0]->Fill();
		
		if(massAll<cutIMEtaP[0] || massAll>cutIMEtaP[1])
			return false;
		
		hCount->Fill(8);
		hCutAllIMPart[0][0]->Fill(mass[bestPerm][1]);
		hCutAllIMPart[0][1]->Fill(mass[bestPerm][2]);
		hCutAllIMPart[0][2]->Fill(mass[bestPerm][0]);
		if(dice)
		{		
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
		}
		else
		{
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
		}
		hCutAllIMPartDiced[0][2]->Fill(mass[bestPerm][0]);
		hCutAllIM[0]->Fill(massAll);
		hCutAllIMSet[0]->Fill(massSet);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTreeCutAll[0]->Fill();
		
		break;
	case 1:
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_ETA);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hIMPart[0][0]->Fill(mass[bestPerm][0]);
		hIMPart[0][1]->Fill(mass[bestPerm][2]);
		hIMPart[0][2]->Fill(mass[bestPerm][1]);
		if(dice)
		{		
			hIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
			dice	= false;
		}
		else
		{
			hIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
			dice	= true;
		}
		hIMPartDiced[0][2]->Fill(mass[bestPerm][1]);
		hIM[0]->Fill(massAll);
		hIMSet[0]->Fill(massSet);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMEta[0] || mass[bestPerm][1]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hCutIMPart[0][0]->Fill(mass[bestPerm][0]);
		hCutIMPart[0][1]->Fill(mass[bestPerm][2]);
		hCutIMPart[0][2]->Fill(mass[bestPerm][1]);
		if(dice)
		{		
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
		}
		else
		{
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
		}
		hCutIMPartDiced[0][2]->Fill(mass[bestPerm][1]);
		hCutIM[0]->Fill(massAll);
		hCutIMSet[0]->Fill(massSet);
		
		out[0][0]	= vec[perm[bestPerm][2]].Px();
		out[1][0]	= vec[perm[bestPerm][2]].Py();
		out[2][0]	= vec[perm[bestPerm][2]].Pz();
		out[3][0]	= vec[perm[bestPerm][2]].E();
		out[0][1]	= vec[perm[bestPerm][3]].Px();
		out[1][1]	= vec[perm[bestPerm][3]].Py();
		out[2][1]	= vec[perm[bestPerm][3]].Pz();
		out[3][1]	= vec[perm[bestPerm][3]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][4]].Px();
		out[1][4]	= vec[perm[bestPerm][4]].Py();
		out[2][4]	= vec[perm[bestPerm][4]].Pz();
		out[3][4]	= vec[perm[bestPerm][4]].E();
		out[0][5]	= vec[perm[bestPerm][5]].Px();
		out[1][5]	= vec[perm[bestPerm][5]].Py();
		out[2][5]	= vec[perm[bestPerm][5]].Pz();
		out[3][5]	= vec[perm[bestPerm][5]].E();
		
		outTree[0]->Fill();
		
		if(massAll<cutIMEtaP[0] || massAll>cutIMEtaP[1])
			return false;
		
		hCount->Fill(8);
		hCutAllIMPart[0][0]->Fill(mass[bestPerm][0]);
		hCutAllIMPart[0][1]->Fill(mass[bestPerm][2]);
		hCutAllIMPart[0][2]->Fill(mass[bestPerm][1]);
		if(dice)
		{		
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][2]);
		}
		else
		{
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][2]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
		}
		hCutAllIMPartDiced[0][2]->Fill(mass[bestPerm][1]);
		hCutAllIM[0]->Fill(massAll);
		hCutAllIMSet[0]->Fill(massSet);
		
		out[0][0]	= vec[perm[bestPerm][2]].Px();
		out[1][0]	= vec[perm[bestPerm][2]].Py();
		out[2][0]	= vec[perm[bestPerm][2]].Pz();
		out[3][0]	= vec[perm[bestPerm][2]].E();
		out[0][1]	= vec[perm[bestPerm][3]].Px();
		out[1][1]	= vec[perm[bestPerm][3]].Py();
		out[2][1]	= vec[perm[bestPerm][3]].Pz();
		out[3][1]	= vec[perm[bestPerm][3]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][4]].Px();
		out[1][4]	= vec[perm[bestPerm][4]].Py();
		out[2][4]	= vec[perm[bestPerm][4]].Pz();
		out[3][4]	= vec[perm[bestPerm][4]].E();
		out[0][5]	= vec[perm[bestPerm][5]].Px();
		out[1][5]	= vec[perm[bestPerm][5]].Py();
		out[2][5]	= vec[perm[bestPerm][5]].Pz();
		out[3][5]	= vec[perm[bestPerm][5]].E();
		
		outTreeCutAll[0]->Fill();
		
		break;
	case 2:
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_ETA);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(2);
		hIMPart[0][0]->Fill(mass[bestPerm][0]);
		hIMPart[0][1]->Fill(mass[bestPerm][1]);
		hIMPart[0][2]->Fill(mass[bestPerm][2]);
		if(dice)
		{		
			hIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
			dice	= false;
		}
		else
		{
			hIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
			dice	= true;
		}
		hIMPartDiced[0][2]->Fill(mass[bestPerm][2]);
		hIM[0]->Fill(massAll);
		hIMSet[0]->Fill(massSet);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMEta[0] || mass[bestPerm][2]>cutIMEta[1])
			return false;
		
		hCount->Fill(5);
		hCutIMPart[0][0]->Fill(mass[bestPerm][0]);
		hCutIMPart[0][1]->Fill(mass[bestPerm][1]);
		hCutIMPart[0][2]->Fill(mass[bestPerm][2]);
		if(dice)
		{		
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
		}
		else
		{
			hCutIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hCutIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
		}
		hCutIMPartDiced[0][2]->Fill(mass[bestPerm][2]);
		hCutIM[0]->Fill(massAll);
		hCutIMSet[0]->Fill(massSet);
		
		out[0][0]	= vec[perm[bestPerm][4]].Px();
		out[1][0]	= vec[perm[bestPerm][4]].Py();
		out[2][0]	= vec[perm[bestPerm][4]].Pz();
		out[3][0]	= vec[perm[bestPerm][4]].E();
		out[0][1]	= vec[perm[bestPerm][5]].Px();
		out[1][1]	= vec[perm[bestPerm][5]].Py();
		out[2][1]	= vec[perm[bestPerm][5]].Pz();
		out[3][1]	= vec[perm[bestPerm][5]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][2]].Px();
		out[1][4]	= vec[perm[bestPerm][2]].Py();
		out[2][4]	= vec[perm[bestPerm][2]].Pz();
		out[3][4]	= vec[perm[bestPerm][2]].E();
		out[0][5]	= vec[perm[bestPerm][3]].Px();
		out[1][5]	= vec[perm[bestPerm][3]].Py();
		out[2][5]	= vec[perm[bestPerm][3]].Pz();
		out[3][5]	= vec[perm[bestPerm][3]].E();
		
		outTree[0]->Fill();
		
		if(massAll<cutIMEtaP[0] || massAll>cutIMEtaP[1])
			return false;
		
		hCount->Fill(8);
		hCutAllIMPart[0][0]->Fill(mass[bestPerm][0]);
		hCutAllIMPart[0][1]->Fill(mass[bestPerm][1]);
		hCutAllIMPart[0][2]->Fill(mass[bestPerm][2]);
		if(dice)
		{		
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][0]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][1]);
		}
		else
		{
			hCutAllIMPartDiced[0][0]->Fill(mass[bestPerm][1]);
			hCutAllIMPartDiced[0][1]->Fill(mass[bestPerm][0]);
		}
		hCutAllIMPartDiced[0][2]->Fill(mass[bestPerm][2]);
		hCutAllIM[0]->Fill(massAll);
		hCutAllIMSet[0]->Fill(massSet);
		
		out[0][0]	= vec[perm[bestPerm][4]].Px();
		out[1][0]	= vec[perm[bestPerm][4]].Py();
		out[2][0]	= vec[perm[bestPerm][4]].Pz();
		out[3][0]	= vec[perm[bestPerm][4]].E();
		out[0][1]	= vec[perm[bestPerm][5]].Px();
		out[1][1]	= vec[perm[bestPerm][5]].Py();
		out[2][1]	= vec[perm[bestPerm][5]].Pz();
		out[3][1]	= vec[perm[bestPerm][5]].E();
		out[0][2]	= vec[perm[bestPerm][0]].Px();
		out[1][2]	= vec[perm[bestPerm][0]].Py();
		out[2][2]	= vec[perm[bestPerm][0]].Pz();
		out[3][2]	= vec[perm[bestPerm][0]].E();
		out[0][3]	= vec[perm[bestPerm][1]].Px();
		out[1][3]	= vec[perm[bestPerm][1]].Py();
		out[2][3]	= vec[perm[bestPerm][1]].Pz();
		out[3][3]	= vec[perm[bestPerm][1]].E();
		out[0][4]	= vec[perm[bestPerm][2]].Px();
		out[1][4]	= vec[perm[bestPerm][2]].Py();
		out[2][4]	= vec[perm[bestPerm][2]].Pz();
		out[3][4]	= vec[perm[bestPerm][2]].E();
		out[0][5]	= vec[perm[bestPerm][3]].Px();
		out[1][5]	= vec[perm[bestPerm][3]].Py();
		out[2][5]	= vec[perm[bestPerm][3]].Pz();
		out[3][5]	= vec[perm[bestPerm][3]].E();
		
		outTreeCutAll[0]->Fill();
		
		break;
	case 3:
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount->Fill(3);
		hIMPart[1][0]->Fill(mass[bestPerm][0]);
		hIMPart[1][1]->Fill(mass[bestPerm][1]);
		hIMPart[1][2]->Fill(mass[bestPerm][2]);
		if(dice3Pi0==0)
		{		
			hIMPartDiced[1][0]->Fill(mass[bestPerm][0]);
			hIMPartDiced[1][1]->Fill(mass[bestPerm][1]);
			hIMPartDiced[1][2]->Fill(mass[bestPerm][2]);
			dice3Pi0	= 1;
		}
		else if(dice3Pi0==1)
		{		
			hIMPartDiced[1][0]->Fill(mass[bestPerm][1]);
			hIMPartDiced[1][1]->Fill(mass[bestPerm][2]);
			hIMPartDiced[1][2]->Fill(mass[bestPerm][0]);
			dice3Pi0	= 2;
		}
		else if(dice3Pi0==2)
		{		
			hIMPartDiced[1][0]->Fill(mass[bestPerm][2]);
			hIMPartDiced[1][1]->Fill(mass[bestPerm][0]);
			hIMPartDiced[1][2]->Fill(mass[bestPerm][1]);
			dice3Pi0	= 0;
		}
		hIM[1]->Fill(massAll);
		hIMSet[1]->Fill(massSet);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		
		hCount->Fill(6);
		hCutIMPart[1][0]->Fill(mass[bestPerm][0]);
		hCutIMPart[1][1]->Fill(mass[bestPerm][1]);
		hCutIMPart[1][2]->Fill(mass[bestPerm][2]);
		if(dice3Pi0==0)
		{		
			hCutIMPartDiced[1][0]->Fill(mass[bestPerm][0]);
			hCutIMPartDiced[1][1]->Fill(mass[bestPerm][1]);
			hCutIMPartDiced[1][2]->Fill(mass[bestPerm][2]);
		}
		else if(dice3Pi0==1)
		{		
			hCutIMPartDiced[1][0]->Fill(mass[bestPerm][1]);
			hCutIMPartDiced[1][1]->Fill(mass[bestPerm][2]);
			hCutIMPartDiced[1][2]->Fill(mass[bestPerm][0]);
		}
		else if(dice3Pi0==2)
		{		
			hCutIMPartDiced[1][0]->Fill(mass[bestPerm][2]);
			hCutIMPartDiced[1][1]->Fill(mass[bestPerm][0]);
			hCutIMPartDiced[1][2]->Fill(mass[bestPerm][1]);
		}
		hCutIM[1]->Fill(massAll);
		hCutIMSet[1]->Fill(massSet);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTree[1]->Fill();
		
		if(massAll<cutIMEta[0] || massAll>cutIMEta[1])
			return false;
		
		hCount->Fill(9);
		hCutAllIMPart[1][0]->Fill(mass[bestPerm][0]);
		hCutAllIMPart[1][1]->Fill(mass[bestPerm][1]);
		hCutAllIMPart[1][2]->Fill(mass[bestPerm][2]);
		if(dice3Pi0==0)
		{		
			hCutAllIMPartDiced[1][0]->Fill(mass[bestPerm][0]);
			hCutAllIMPartDiced[1][1]->Fill(mass[bestPerm][1]);
			hCutAllIMPartDiced[1][2]->Fill(mass[bestPerm][2]);
		}
		else if(dice3Pi0==1)
		{		
			hCutAllIMPartDiced[1][0]->Fill(mass[bestPerm][1]);
			hCutAllIMPartDiced[1][1]->Fill(mass[bestPerm][2]);
			hCutAllIMPartDiced[1][2]->Fill(mass[bestPerm][0]);
		}
		else if(dice3Pi0==2)
		{		
			hCutAllIMPartDiced[1][0]->Fill(mass[bestPerm][2]);
			hCutAllIMPartDiced[1][1]->Fill(mass[bestPerm][0]);
			hCutAllIMPartDiced[1][2]->Fill(mass[bestPerm][1]);
		}
		hCutAllIM[1]->Fill(massAll);
		hCutAllIMSet[1]->Fill(massSet);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTreeCutAll[1]->Fill();
		
		break;
	}
	
	return true;
}
void	TreeAnalyse6Gamma::Reconstruct()
{
	for(int i=0; i<15; i++)
	{
		part[i][0]	= vec[perm[i][0]] + vec[perm[i][1]];
		part[i][1]	= vec[perm[i][2]] + vec[perm[i][3]];
		part[i][2]	= vec[perm[i][4]] + vec[perm[i][5]];
		
		mass[i][0]	= part[i][0].M();
		mass[i][1]	= part[i][1].M();
		mass[i][2]	= part[i][2].M();
		
		
		ChiSqr[i][0]	= ((mass[i][0]-MASS_ETA)*(mass[i][0]-MASS_ETA)/(1600)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
		ChiSqr[i][1]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_ETA)*(mass[i][1]-MASS_ETA)/(1600)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
		ChiSqr[i][2]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_ETA)*(mass[i][1]-MASS_ETA)/(1600));
		ChiSqr[i][3]	= ((mass[i][0]-MASS_PI0)*(mass[i][0]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484)) + ((mass[i][1]-MASS_PI0)*(mass[i][1]-MASS_PI0)/(484));
	}
	Double_t		minChiSqr	= ChiSqr[0][0];
	bestPerm	= 0;
	bestEta		= 0;
	for(int i=0; i<15; i++)
	{
		for(int l=0; l<4; l++)
		{
			if(ChiSqr[i][l] < minChiSqr)
			{
				minChiSqr	= ChiSqr[i][l];
				bestPerm	= i;
				bestEta		= l;
			}
		}
	}
	
	massAll = vecAll.M();
	partSet[0]	= part[bestPerm][0];
	partSet[1]	= part[bestPerm][1];
	partSet[2]	= part[bestPerm][2];
	
}
void	TreeAnalyse6Gamma::Analyse(const Int_t Min, const Int_t Max)
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
	{
		if((i%1000)==0)
			printf("Analyse Event: %d\n", i);
		AnalyseEvent(i);
	}
}
bool	TreeAnalyse6Gamma::Save()
{
	
	outFile[0]->cd();
	outTree[0]->Write();
	outFile[0]->Flush();
	
	outFile[1]->cd();
	outTree[1]->Write();
	outFile[1]->Flush();
	
	outFileCutAll[0]->cd();
	outTreeCutAll[0]->Write();
	outFileCutAll[0]->Flush();
	
	outFileCutAll[1]->cd();
	outTreeCutAll[1]->Write();
	outFileCutAll[1]->Flush();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName()+5);
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCount->Write();
	for(int i=0; i<2; i++)
	{
		for(int l=0; l<3; l++)
			hIMPart[i][l]->Write();
		for(int l=0; l<3; l++)
			hIMPartDiced[i][l]->Write();
		hIM[i]->Write();
		hIMSet[i]->Write();
		
		for(int l=0; l<3; l++)
			hCutIMPart[i][l]->Write();
		for(int l=0; l<3; l++)
			hCutIMPartDiced[i][l]->Write();
		hCutIM[i]->Write();
		hCutIMSet[i]->Write();
		
		for(int l=0; l<3; l++)
			hCutAllIMPart[i][l]->Write();
		for(int l=0; l<3; l++)
			hCutAllIMPartDiced[i][l]->Write();
		hCutAllIM[i]->Write();
		hCutAllIMSet[i]->Write();
	}
	result->Close();
	delete result;
}


int		TreeAnalyse6Gamma::perm[15][6]=
{
	{0,1,2,3,4,5},
	{0,1,2,4,3,5},
	{0,1,2,5,4,3},
	
	{0,2,1,3,4,5},
	{0,2,1,4,3,5},
	{0,2,1,5,4,3},
	
	{0,3,2,1,4,5},
	{0,3,2,4,1,5},
	{0,3,2,5,4,1},
	
	{0,4,2,3,1,5},
	{0,4,2,1,3,5},
	{0,4,2,5,1,3},
	
	{0,5,2,3,4,1},
	{0,5,2,4,3,1},
	{0,5,2,1,4,3},
};

