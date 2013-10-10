#include "TreeReadTagged.C"
#include "TaggedHistSet6g.C"

#define MASS_PI0	135
#define MASS_ETA	547
#define MASS_PROTON	938.27203

class	TreeAnalyse6Gamma	: public TreeReadTagged
{
private:
	TFile*	outFile[2][2];
	TTree*	outTree[2][2];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	Double_t	cutAngleDiff;
	
	TLorentzVector	vec[6];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	static	int		perm[15][6];
	Double_t		ChiSqr[15][4];
	int				bestPerm;
	int				bestEta;
	TLorentzVector	part[15][3];
	Double_t		mass[15][3];
	Double_t		massPart[3];
	Double_t		massAll;
	TLorentzVector	partSet[3];
	TLorentzVector	allSet;
	Double_t		massSet;
	
	Double_t		out[4][6];
	
	TH1I*				hCount[2];			//[2g,withProton]
	TaggedHistSet6g*	hist[2][2][2];		//[Etap,3Pi0][2g,withProton][All,IM]
	TH1I*				hCutAngleCount[3];	//[Prompt,Rand1,Rand2]
	TaggedHist*			hAngleDiff;
	TaggedHist*			hCutAngleDiffCheck;
	
	void	SetMass(const int index, const Double_t mass);
	void	Reconstruct();

protected:
	bool	AnalyseEvent(const Int_t i);
	bool	Analyse6g(const Int_t index);

public:
	TreeAnalyse6Gamma(const Char_t* FileName);
	~TreeAnalyse6Gamma();
	
			void	Clear();
	virtual	bool	Open();
			void	Analyse(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	static	void	Analyse(const Char_t* FileName);
	static	void	AnalyseFolder(const Char_t* FolderName);
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
	const	Double_t*	GetCutAngleDiff()	const	{return cutAngleDiff;}
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			void		SetCutAngleDiff(const Double_t Max)						{cutAngleDiff = Max;}
			
	static	TreeAnalyse6Gamma*	test()
	{
		TreeAnalyse6Gamma* c = new TreeAnalyse6Gamma("tree_TTreeOutput_41941_6g.root");
		c->Open();
		c->Analyse(100);
		c->Save();
		return c;
	}
};



void	TreeAnalyse6Gamma::Analyse(const Char_t* FileName)
{
	TreeAnalyse6Gamma* c = new TreeAnalyse6Gamma(FileName);
		c->Open();
		if(!c->IsOpen())
			return;
		c->Analyse();
		c->Save();
		delete c;
}
void	TreeAnalyse6Gamma::AnalyseFolder(const Char_t* FolderName)
{
	cout << "***********************************************************************" << endl;
	cout << "************************TreeAnalyse6Gamma******************************" << endl;
	cout << "***********************************************************************" << endl;
	TSystemDirectory dir(FolderName, FolderName);
	TList* files = dir.GetListOfFiles()->Clone("MyInputFiles");
	if (files) 
	{
		TSystemFile *sfile;
		TString fname;
		TIter next(files);
		int	ccc=0;
		while(sfile=(TSystemFile*)next())
		{
			fname = sfile->GetName();
			ccc++;
			printf("%d\t%s\t", ccc, fname.Data());
			if (!sfile->IsDirectory() && fname.EndsWith("6g.root") && fname.BeginsWith("tree_") && !fname.BeginsWith("hist_")) 
			{
				TreeAnalyse6Gamma::Analyse(fname.Data());
			}
			else
				printf("\n");
		}
		
	}
}


TreeAnalyse6Gamma::TreeAnalyse6Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
{
	outTree[0][0]	= 0;
	outTree[1][0]	= 0;
	outFile[0][1]	= 0;
	outFile[1][1]	= 0;
	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	cutAngleDiff	= 8;
	
	if((hCount[0]	= (TH1I*)gROOT->Get("ReconstructCount")))
		hCount[0]->Delete();
	hCount[0]		= new TH1I("ReconstructCount", "1:All/2,3:(EtaP,3Pi0)/5,6:(EtaP,3Pi0)Passed IM", 8, 0, 8);
	if(!hCount[0])
	{
		cout << "Could not create histogram " << "ReconstructCount" << ". Exiting!" << endl;
		exit(1);
	}
	if((hCount[1]	= (TH1I*)gROOT->Get("WithProton_ReconstructCount")))
		hCount[1]->Delete();
	hCount[1]		= new TH1I("WithProton_ReconstructCount", "1:All/2,3:(EtaP,3Pi0)/5,6:(EtaP,3Pi0)Passed IM", 8, 0, 8);
	if(!hCount[1])
	{
		cout << "Could not create histogram " << "WithProton_ReconstructCount" << ". Exiting!" << endl;
		exit(1);
	}
		
	
	hist[0][0][0]	= new TaggedHistSet6g("Etap", true);
	hist[0][0][1]	= new TaggedHistSet6g("Etap_IM", true);
	hist[1][0][0]	= new TaggedHistSet6g("3Pi0");
	hist[1][0][1]	= new TaggedHistSet6g("3Pi0_IM");
	
	if((hCutAngleCount[0]	= (TH1I*)gROOT->Get("WithProton_AngleCutCount_Prompt")))
		hCutAngleCount[0]->Delete();
	hCutAngleCount[0]		= new TH1I("WithProton_AngleCutCount_Prompt", "1:All/2:Passed", 4, 0, 4);
	if(!hCutAngleCount[0])
	{
		cout << "Could not create histogram " << "WithProton_AngleCutCount_Prompt" << ". Exiting!" << endl;
		exit(1);
	}
	if((hCutAngleCount[1]	= (TH1I*)gROOT->Get("WithProton_AngleCutCount_Rand1")))
		hCutAngleCount[1]->Delete();
	hCutAngleCount[1]		= new TH1I("WithProton_AngleCutCount_Rand1", "1:All/2:Passed", 4, 0, 4);
	if(!hCutAngleCount[1])
	{
		cout << "Could not create histogram " << "WithProton_AngleCutCount_Rand1" << ". Exiting!" << endl;
		exit(1);
	}
	if((hCutAngleCount[2]	= (TH1I*)gROOT->Get("WithProton_AngleCutCount_Rand2")))
		hCutAngleCount[2]->Delete();
	hCutAngleCount[2]		= new TH1I("WithProton_AngleCutCount_Rand2", "1:All/2:Passed", 4, 0, 4);
	if(!hCutAngleCount[2])
	{
		cout << "Could not create histogram " << "WithProton_AngleCutCount_Rand2" << ". Exiting!" << endl;
		exit(1);
	}
	hAngleDiff			= new TaggedHist("WithProton_AngleDiff", 360, 0, 180);
	hCutAngleDiffCheck	= new TaggedHist("WithProton_CutAngleDiffCheck", 90, 0, 45);
	
	hist[0][1][0]	= new TaggedHistSet6g("WithProton_Etap", true);
	hist[0][1][1]	= new TaggedHistSet6g("WithProton_Etap_IM", true);
	hist[1][1][0]	= new TaggedHistSet6g("WithProton_3Pi0");
	hist[1][1][1]	= new TaggedHistSet6g("WithProton_3Pi0_IM");
}
TreeAnalyse6Gamma::~TreeAnalyse6Gamma()
{
	for(int i=0; i<2; i++)
	{
		if(outTree[i][0])
			delete	outTree[i][0];
		if(outFile[i][0])
			delete	outFile[i][0];
		if(outTree[i][1])
			delete	outTree[i][1];
		if(outFile[i][1])
			delete	outFile[i][1];
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
	for(int i=0; i<2; i++)
	{
		hCount[i]->Reset("M");
		for(int l=0; l<2; l++)
		{
			hist[i][0][l]->Clear();
			hist[i][1][l]->Clear();
		}
		hCutAngleCount[i]->Reset("M");
	}
	hCutAngleCount[2]->Reset("M");
	hAngleDiff->Clear();
	hCutAngleDiffCheck->Clear();
}

void	TreeAnalyse6Gamma::Open()
{
	//printf("TreeAnalyse6Gamma::Open()\n");
	TreeReadTagged::Open();
	//printf("TreeAnalyse6Gamma::Open 2()\n");
	
	Char_t	str[128];	
	
	for(int i=0; i<2; i++)
	{
		if(i==0)
			sprintf(str, "tree_%s_IMEtaP.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%s_IM3Pi0.root", GetFileName());
			
		outFile[i][0]	= new TFile(str, "RECREATE");
		outTree[i][0]		= new TTree("tree", "tree");
		
		outTree[i][0]->Branch("nPrompt",&nTagged[0],"nPrompt/I");
		outTree[i][0]->Branch("PromptEnergy", TaggedEnergy[0], "PromptEnergy[nPrompt]/D");
		outTree[i][0]->Branch("PromptTime", TaggedTime[0], "PromptTime[nPrompt]/D");
		outTree[i][0]->Branch("nRand1",&nTagged[1],"nRand1/I");
		outTree[i][0]->Branch("Rand1Energy", TaggedEnergy[1], "Rand1Energy[nRand1]/D");
		outTree[i][0]->Branch("Rand1Time", TaggedTime[1], "Rand1Time[nRand1]/D");
		outTree[i][0]->Branch("nRand2",&nTagged[2],"nRand2/I");
		outTree[i][0]->Branch("Rand2Energy", TaggedEnergy[2], "Rand2Energy[nRand2]/D");
		outTree[i][0]->Branch("Rand2Time", TaggedTime[2], "Rand2Time[nRand2]/D");
		
		outTree[i][0]->Branch("nCB_Hits",&nCB_Hits,"nCB_Hits/I");
		outTree[i][0]->Branch("CB_Px", out[0], "CB_Px[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_Py", out[1], "CB_Py[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_Pz", out[2], "CB_Pz[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_E", out[3], "CB_E[nCB_Hits]/D");	
		outTree[i][0]->Branch("CB_Time", CB_Time, "CB_Time[nCB_Hits]/D");
		
		outTree[i][0]->Branch("nTAPS_Hits",&nTAPS_Hits,"nTAPS_Hits/I");
		outTree[i][0]->Branch("TAPS_Px", TAPS_Px, "TAPS_Px[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_Py", TAPS_Py, "TAPS_Py[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_Pz", TAPS_Pz, "TAPS_Pz[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_E", TAPS_E, "TAPS_E[nTAPS_Hits]/D");	
		outTree[i][0]->Branch("TAPS_Time", TAPS_Time, "TAPS_Time[nTAPS_Hits]/D");
	}
	for(int i=0; i<2; i++)
	{
		if(i==0)
			sprintf(str, "tree_%sProton_IMEtaP.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%sProton_IM3Pi0.root", GetFileName());
			
		outFile[i][1]	= new TFile(str, "RECREATE");
		outTree[i][1]		= new TTree("tree", "tree");
		
		outTree[i][1]->Branch("nPrompt",&nTagged[0],"nPrompt/I");
		outTree[i][1]->Branch("PromptEnergy", TaggedEnergy[0], "PromptEnergy[nPrompt]/D");
		outTree[i][1]->Branch("PromptTime", TaggedTime[0], "PromptTime[nPrompt]/D");
		outTree[i][1]->Branch("nRand1",&nTagged[1],"nRand1/I");
		outTree[i][1]->Branch("Rand1Energy", TaggedEnergy[1], "Rand1Energy[nRand1]/D");
		outTree[i][1]->Branch("Rand1Time", TaggedTime[1], "Rand1Time[nRand1]/D");
		outTree[i][1]->Branch("nRand2",&nTagged[2],"nRand2/I");
		outTree[i][1]->Branch("Rand2Energy", TaggedEnergy[2], "Rand2Energy[nRand2]/D");
		outTree[i][1]->Branch("Rand2Time", TaggedTime[2], "Rand2Time[nRand2]/D");
		
		outTree[i][1]->Branch("nCB_Hits",&nCB_Hits,"nCB_Hits/I");
		outTree[i][1]->Branch("CB_Px", out[0], "CB_Px[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_Py", out[1], "CB_Py[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_Pz", out[2], "CB_Pz[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_E", out[3], "CB_E[nCB_Hits]/D");	
		outTree[i][1]->Branch("CB_Time", CB_Time, "CB_Time[nCB_Hits]/D");
		
		outTree[i][1]->Branch("nTAPS_Hits",&nTAPS_Hits,"nTAPS_Hits/I");
		outTree[i][1]->Branch("TAPS_Px", TAPS_Px, "TAPS_Px[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_Py", TAPS_Py, "TAPS_Py[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_Pz", TAPS_Pz, "TAPS_Pz[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_E", TAPS_E, "TAPS_E[nTAPS_Hits]/D");	
		outTree[i][1]->Branch("TAPS_Time", TAPS_Time, "TAPS_Time[nTAPS_Hits]/D");
	}
}
bool	TreeAnalyse6Gamma::Analyse6g(const Int_t index)
{
	Reconstruct();
	
	hCount[index]->Fill(1);
	
	switch(bestEta)
	{
	case 0:
		massPart[0]	= mass[bestPerm][1];
		massPart[1]	= mass[bestPerm][2];
		massPart[2]	= mass[bestPerm][0];
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount[index]->Fill(2);
		hist[0][index][0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][0]<cutIMEta[0] || mass[bestPerm][0]>cutIMEta[1])
			return false;
		
		hCount[index]->Fill(5);
		hist[0][index][1]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		outTree[0][index]->Fill();
		
		break;
	case 1:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][2];
		massPart[2]	= mass[bestPerm][1];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_ETA);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount[index]->Fill(2);
		hist[0][index][0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMEta[0] || mass[bestPerm][1]>cutIMEta[1])
			return false;
		
		hCount[index]->Fill(5);
		hist[0][index][1]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
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
		
		outTree[0][index]->Fill();
		
		break;
	case 2:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][1];
		massPart[2]	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_ETA);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount[index]->Fill(2);
		hist[0][index][0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMEta[0] || mass[bestPerm][2]>cutIMEta[1])
			return false;
		
		hCount[index]->Fill(5);
		hist[0][index][1]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
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
		
		outTree[0][index]->Fill();
		
		break;
	case 3:
		massPart[0]	= mass[bestPerm][0];
		massPart[1]	= mass[bestPerm][1];
		massPart[2]	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		hCount[index]->Fill(3);
		hist[1][index][0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		if(mass[bestPerm][0]<cutIMPi0[0] || mass[bestPerm][0]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][1]<cutIMPi0[0] || mass[bestPerm][1]>cutIMPi0[1])
			return false;
		if(mass[bestPerm][2]<cutIMPi0[0] || mass[bestPerm][2]>cutIMPi0[1])
			return false;
		
		hCount[index]->Fill(6);
		hist[1][index][1]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massPart, massAll, massSet, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		
		for(int i=0; i<6; i++)
		{
			out[0][i]	= vec[perm[bestPerm][i]].Px();
			out[1][i]	= vec[perm[bestPerm][i]].Py();
			out[2][i]	= vec[perm[bestPerm][i]].Pz();
			out[3][i]	= vec[perm[bestPerm][i]].E();
		}
		
		outTree[1][index]->Fill();
		
		break;
	}
	
	return true;
}
bool	TreeAnalyse6Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(CB_Px[0], CB_Py[0], CB_Pz[0], CB_E[0]);
	vec[1].SetPxPyPzE(CB_Px[1], CB_Py[1], CB_Pz[1], CB_E[1]);
	vec[2].SetPxPyPzE(CB_Px[2], CB_Py[2], CB_Pz[2], CB_E[2]);
	vec[3].SetPxPyPzE(CB_Px[3], CB_Py[3], CB_Pz[3], CB_E[3]);
	vec[4].SetPxPyPzE(CB_Px[4], CB_Py[4], CB_Pz[4], CB_E[4]);
	vec[5].SetPxPyPzE(CB_Px[5], CB_Py[5], CB_Pz[5], CB_E[5]);
	vecAll	= vec[0] + vec[1] + vec[2] + vec[3] + vec[4] + vec[5];
	massAll	= vecAll.M();
	
	if(nTAPS_Hits == 0)
		return	Analyse6g(0);
		
	if(nTAPS_Hits != 1)
		return false;
	
	TLorentzVector	beam;
	TLorentzVector	miss;
	TVector3		proton(TAPS_Px[0], TAPS_Py[0], TAPS_Pz[0]);
	Double_t		angleDiff;
	bool			found	= false;
	
	for(int l=0; l<3; l++)
	{
		hCutAngleCount[l]->Fill(1);
		for(int i=0; i<nTagged[l]; i++)
		{
			beam.SetPxPyPzE(0.0, 0.0, TaggedEnergy[l][i], TaggedEnergy[l][i] + MASS_PROTON);
			//printf("%lf\t%lf\t%lf\t%lf\n", beam.Px(), beam.Py(), beam.Pz(), beam.E());
			//printf("%lf\t%lf\t%lf\t%lf\n", vecAll.Px(), vecAll.Py(), vecAll.Pz(), vecAll.E());
			miss	= beam - vecAll;
			//printf("%lf\t%lf\t%lf\t%lf\n", miss.Px(), miss.Py(), miss.Pz(), miss.E());
			//printf("%lf\t\t%lf\t\t%lf\n", proton.Theta(), miss.Theta(), TaggedEnergy[l][i]);
			angleDiff	= TMath::RadToDeg()*(proton.Angle(miss.Vect()));
			hAngleDiff->Fill(l, angleDiff);
			if(angleDiff <= cutAngleDiff)
			{	
				hCutAngleDiffCheck->Fill(l, angleDiff);
				found	= true;
			}
		}
		if(found)
			hCutAngleCount[l]->Fill(2);
	}
	if(!found)
		return false;
		
	return	Analyse6g(1);
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
	for(int i=0; i<2; i++)
	{
		outFile[i][0]->cd();
		outTree[i][0]->Write();
		outFile[i][0]->Flush();
		
		outFile[i][1]->cd();
		outTree[i][1]->Write();
		outFile[i][1]->Flush();
	}
	
	for(int i=0; i<2; i++)
	{
		hist[0][i][0]->SubstractBackground();
		hist[0][i][1]->SubstractBackground();
		hist[1][i][0]->SubstractBackground();
		hist[1][i][1]->SubstractBackground();
	}
	
	
	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCount[0]->Write();
	
	result->cd();
	result->mkdir("EtaP");
	result->cd("EtaP");
	hist[0][0][0]->Save();
	
	gDirectory->mkdir("IM");
	result->cd("EtaP/IM");
	hist[0][0][1]->Save();
	
	result->cd();
	result->mkdir("3Pi0");
	result->cd("3Pi0");
	hist[1][0][0]->Save();
	
	gDirectory->mkdir("IM");
	result->cd("3Pi0/IM");
	hist[1][0][1]->Save();
	
	result->cd();
	result->mkdir("WithProton");
	result->cd("WithProton");
	
	hCutAngleCount[0]->Write();
	hCutAngleCount[1]->Write();
	hCutAngleCount[2]->Write();
	hAngleDiff->Save();
	hCutAngleDiffCheck->Save();
	hCount[1]->Write();
	
	gDirectory->mkdir("EtaP");
	result->cd("WithProton/EtaP");
	hist[0][1][0]->Save();
	
	gDirectory->mkdir("IM");
	result->cd("WithProton/EtaP/IM");
	hist[0][1][1]->Save();
	
	result->cd("WithProton");
	gDirectory->mkdir("3Pi0");
	result->cd("WithProton/3Pi0");
	hist[1][1][0]->Save();
	
	gDirectory->mkdir("IM");
	result->cd("WithProton/3Pi0/IM");
	hist[1][1][1]->Save();
	
	result->Close();
	delete result;
	
	return true;
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

