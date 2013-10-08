#include "TreeReadTagged.C"
#include "TaggedHistSet.C"

#define MASS_PROTON	938.27203

class	TreeAnalyse2Gamma	: public TreeReadTagged
{
private:	
	TFile*		outFile[3][2];
	TTree*		outTree[3][2];
	
	Double_t	cutIMPi0[2];
	Double_t	cutIMEta[2];
	Double_t	cutIMEtaP[2];
	Double_t	cutAngleDiff;
	
	TLorentzVector	vec[2];
	TLorentzVector	vecAll;
	Double_t		massAll;
	
	TH1I*			hCutIMCount[2];
	TaggedHistSet*	hAll[2];
	TaggedHistSet*	h[3][2];			//[Pi0, Eta, EtaP][noProton,WithProton]
	TH1I*			hCutAngleCount[3];	//[Prompt,Rand1,Rand2]
	TaggedHist*		hAngleDiff;
	TaggedHist*		hCutAngleDiffCheck;
	
	
protected:
	bool	AnalyseEvent(const Int_t i);
	bool	Analyse2g(const Int_t index);

public:
	TreeAnalyse2Gamma(const Char_t* FileName);
	~TreeAnalyse2Gamma();
	
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
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			void		SetCutAngleDiff(const Double_t Max)						{cutAngleDiff = Max;}
			
	
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

void	TreeAnalyse2Gamma::Analyse(const Char_t* FileName)
{
	TreeAnalyse2Gamma* c = new TreeAnalyse2Gamma(FileName);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
}
void	TreeAnalyse2Gamma::AnalyseFolder(const Char_t* FolderName)
{
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
			if (!sfile->IsDirectory() && fname.EndsWith("2g.root") && fname.BeginsWith("tree_") && !fname.BeginsWith("hist_")) 
			{
				TreeAnalyse2Gamma::Analyse(fname.Data());
			}
			else
				printf("\n");
		}
		
	}
}



TreeAnalyse2Gamma::TreeAnalyse2Gamma(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	cutAngleDiff	= 5;
	
	if((hCutIMCount[0]	= (TH1I*)gROOT->Get("IMCutCount")))
		hCutIMCount[0]->Delete();
	hCutIMCount[0]		= new TH1I("IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
	if(!hCutIMCount[0])
	{
		cout << "Could not create histogram " << "IMCutCount" << ". Exiting!" << endl;
		exit(1);
	}
	
	if((hCutIMCount[1]	= (TH1I*)gROOT->Get("Proton_IMCutCount")))
		hCutIMCount[1]->Delete();
	hCutIMCount[1]		= new TH1I("Proton_IMCutCount", "1:All/2,3,4:(Pi0,Eta,EtaP)", 6, 0, 6);
	if(!hCutIMCount[1])
	{
		cout << "Could not create histogram " << "Proton_IMCutCount" << ". Exiting!" << endl;
		exit(1);
	}
	
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
			
	hAll[0] = new TaggedHistSet("");
	h[0][0] = new TaggedHistSet("Pi0");
	h[1][0] = new TaggedHistSet("Eta");
	h[2][0] = new TaggedHistSet("EtaP");
	
	hAngleDiff			= new TaggedHist("WithProton_AngleDiff", 360, 0, 180);
	hCutAngleDiffCheck	= new TaggedHist("WithProton_CutAngleDiffCheck", 90, 0, 45);
	
	hAll[1] = new TaggedHistSet("WithProton");
	h[0][1] = new TaggedHistSet("WithProton_Pi0");
	h[1][1] = new TaggedHistSet("WithProton_Eta");
	h[2][1] = new TaggedHistSet("WithProton_EtaP");
		
	Clear();
}
TreeAnalyse2Gamma::~TreeAnalyse2Gamma()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i][0])
			delete	outTree[i][0];
		if(outTree[i][1])
			delete	outTree[i][1];
		if(outFile[i][0])
			delete	outFile[i][0];
		if(outFile[i][1])
			delete	outFile[i][1];
	}
}

inline	void	TreeAnalyse2Gamma::Clear()	
{
	hCutIMCount[0]->Reset("M");
	hCutIMCount[1]->Reset("M");
	hAll[0]->Clear();
	hAll[1]->Clear();
	for(int i=0; i<3; i++)
	{
		h[i][0]->Clear();
		h[i][1]->Clear();
		hCutAngleCount[i]->Reset("M");
	}
	hAngleDiff->Clear();
	hCutAngleDiffCheck->Clear();
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
			
		outFile[i][0]		= new TFile(str, "RECREATE");
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
		outTree[i][0]->Branch("CB_Px", CB_Px, "CB_Px[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_Py", CB_Py, "CB_Py[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_Pz", CB_Pz, "CB_Pz[nCB_Hits]/D");
		outTree[i][0]->Branch("CB_E", CB_E, "CB_E[nCB_Hits]/D");	
		outTree[i][0]->Branch("CB_Time", CB_Time, "CB_Time[nCB_Hits]/D");
		
		outTree[i][0]->Branch("nTAPS_Hits",&nTAPS_Hits,"nTAPS_Hits/I");
		outTree[i][0]->Branch("TAPS_Px", TAPS_Px, "TAPS_Px[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_Py", TAPS_Py, "TAPS_Py[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_Pz", TAPS_Pz, "TAPS_Pz[nTAPS_Hits]/D");
		outTree[i][0]->Branch("TAPS_E", TAPS_E, "TAPS_E[nTAPS_Hits]/D");	
		outTree[i][0]->Branch("TAPS_Time", TAPS_Time, "TAPS_Time[nTAPS_Hits]/D");
	}
	for(int i=0; i<3; i++)
	{
		if(i==0)
			sprintf(str, "tree_%sProton_IMPi0.root", GetFileName());
		else if(i==1)
			sprintf(str, "tree_%sProton_IMEta.root", GetFileName());
		else if(i==2)
			sprintf(str, "tree_%sProton_IMEtaP.root", GetFileName());
			
		outFile[i][1]		= new TFile(str, "RECREATE");
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
		outTree[i][1]->Branch("CB_Px", CB_Px, "CB_Px[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_Py", CB_Py, "CB_Py[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_Pz", CB_Pz, "CB_Pz[nCB_Hits]/D");
		outTree[i][1]->Branch("CB_E", CB_E, "CB_E[nCB_Hits]/D");	
		outTree[i][1]->Branch("CB_Time", CB_Time, "CB_Time[nCB_Hits]/D");
		
		outTree[i][1]->Branch("nTAPS_Hits",&nTAPS_Hits,"nTAPS_Hits/I");
		outTree[i][1]->Branch("TAPS_Px", TAPS_Px, "TAPS_Px[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_Py", TAPS_Py, "TAPS_Py[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_Pz", TAPS_Pz, "TAPS_Pz[nTAPS_Hits]/D");
		outTree[i][1]->Branch("TAPS_E", TAPS_E, "TAPS_E[nTAPS_Hits]/D");	
		outTree[i][1]->Branch("TAPS_Time", TAPS_Time, "TAPS_Time[nTAPS_Hits]/D");
	}
		
	//printf("TreeAnalyseMultiplicity::Open 2()\n");
}

bool	TreeAnalyse2Gamma::Analyse2g(const Int_t index)
{
	hCutIMCount[index]->Fill(1);
	hAll[index]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massAll, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
	if(massAll >= cutIMPi0[0] && massAll <= cutIMPi0[1])
	{
		hCutIMCount[index]->Fill(2);
		h[0][index]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massAll, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		outTree[0][index]->Fill();
		return true;
	}
	else if(massAll >= cutIMEta[0] && massAll <= cutIMEta[1])
	{
		hCutIMCount[index]->Fill(3);
		h[1][index]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massAll, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		outTree[1][index]->Fill();
		return true;
	}
	else if(massAll >= cutIMEtaP[0] && massAll <= cutIMEtaP[1])
	{
		hCutIMCount[index]->Fill(4);
		h[2][index]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), massAll, vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		outTree[2][index]->Fill();
		return true;
	}
	
	return false;
}

bool	TreeAnalyse2Gamma::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(CB_Px[0], CB_Py[0], CB_Pz[0], CB_E[0]);
	vec[1].SetPxPyPzE(CB_Px[1], CB_Py[1], CB_Pz[1], CB_E[1]);
	vecAll	= vec[0] + vec[1];
	massAll	= vecAll.M();
	
	if(nTAPS_Hits == 0)
		return	Analyse2g(0);
		
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
		
	return	Analyse2g(1);
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
		outFile[i][0]->cd();
		outTree[i][0]->Write();
		outFile[i][0]->Flush();
		
		outFile[i][1]->cd();
		outTree[i][1]->Write();
		outFile[i][1]->Flush();
	}
	
	hAll[0]->SubstractBackground();
	h[0][0]->SubstractBackground();
	h[1][0]->SubstractBackground();
	h[2][0]->SubstractBackground();
	
	hAll[1]->SubstractBackground();
	h[0][1]->SubstractBackground();
	h[1][1]->SubstractBackground();
	h[2][1]->SubstractBackground();

	Char_t	str[128];
	sprintf(str, "hist_%s_IM.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCutIMCount[0]->Write();
	hAll[0]->Save();
	
	result->cd();
	result->mkdir("Pi0");
	result->cd("Pi0");
	h[0][0]->Save();
	
	result->cd();
	result->mkdir("Eta");
	result->cd("Eta");
	h[1][0]->Save();
	
	result->cd();
	result->mkdir("EtaP");
	result->cd("EtaP");
	h[2][0]->Save();
	
	result->cd();
	hCutAngleCount[0]->Write();
	hCutAngleCount[1]->Write();
	hCutAngleCount[2]->Write();
	hAngleDiff->Save();
	hCutAngleDiffCheck->Save();
	hCutIMCount[1]->Write();
	hAll[1]->Save();
	
	result->cd();
	result->mkdir("WithProton_Pi0");
	result->cd("WithProton_Pi0");
	h[0][1]->Save();
	
	result->cd();
	result->mkdir("Proton_Eta");
	result->cd("Proton_Eta");
	h[1][1]->Save();
	
	result->cd();
	result->mkdir("WithProton_EtaP");
	result->cd("WithProton_EtaP");
	h[2][1]->Save();
	
	result->Close();
	delete result;
}

