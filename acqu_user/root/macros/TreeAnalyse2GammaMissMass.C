#include "TreeReadTagged.C"
#include "TaggedHistSet.C"


#define MASS_PROTON	938.27203

class	TreeAnalyse2GammaMissMass	: public TreeReadTagged
{
private:
	Double_t	cut[2];
	
	TLorentzVector	vec[10];
	TLorentzVector	vecAll;
	Int_t			nBeam[3];
	TLorentzVector	beam[3][8];
	Double_t		beamEnergy[3][8];
	TLorentzVector	miss[3][8];
	Double_t		missMass[3][8];
	
	TH1I*					hCutCount;
	TaggedHist*		hMissP[5][2];	//[x,y,z,E,Mag]
	TaggedHist*		hMissMass;
	TaggedHist*		hMissMassSq;
	TaggedHist*		hCutCheck;
	TaggedHistSet*	hist[2];

protected:
	bool	AnalyseEvent(const Int_t i);

public:
	TreeAnalyse2GammaMissMass(const Char_t* FileName);
	~TreeAnalyse2GammaMissMass();
	
			void	Clear();
			void	AnalyseEvent(const Int_t i);
			void	Analyse(const Int_t Min, const Int_t Max);
	virtual	void	Analyse(const Int_t Max = -1)							{Analyse(0, Max);}
	virtual	bool	Save();
	
	const	Double_t*	GetCut()	const	{return cut;}
	const	Double_t	GetCutMin()	const	{return cut[0];}
	const	Double_t	GetCutMax()	const	{return cut[1];}	
	
			void		SetCut(const Double_t Min, const Double_t Max)	{cut[0] = Min; cut[1] = Max;}
	

	static	TreeAnalyse2GammaMissMass*	test()
	{
		printf("Creating\n");
		TreeAnalyse2GammaMissMass* c = new TreeAnalyse2GammaMissMass("tree_TTreeOutput_41941_2g_IMPi0.root");
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








TreeAnalyse2GammaMissMass::TreeAnalyse2GammaMissMass(const Char_t* FileName)	: TreeReadTagged(FileName)
{	
	cut[0]	= 750;
	cut[1]	= 1100;
	
	if((hCutCount	= (TH1I*)gROOT->Get("CutCount")))
		hCutCount->Delete();
	hCutCount	= new TH1I("CutCount", "1:All/2,3,4:(Prompt,Rand1,Rand2)/5:Untagged/7,8,9:MultiFilled(Prompt,Rand1,Rand2)", 11, 0, 11);
	if(!hCutCount)
	{
		cout << "Could not create histogram " << str.Data() << ". Exiting!" << endl;
		exit(1);
	}
		
	hMissP[0][0]	= new TaggedHist("MissPx", 4000, -2000, 2000);
	hMissP[1][0]	= new TaggedHist("MissPy", 4000, -2000, 2000);
	hMissP[2][0]	= new TaggedHist("MissPz", 4000, -2000, 2000);
	hMissP[3][0]	= new TaggedHist("MissE", 3000, 0, 3000);
	hMissP[4][0]	= new TaggedHist("MissP", 3000, 0, 3000);
	hMissP[0][1]	= new TaggedHist("CutMM_MissPx", 4000, -2000, 2000);
	hMissP[1][1]	= new TaggedHist("CutMM_MissPy", 4000, -2000, 2000);
	hMissP[2][1]	= new TaggedHist("CutMM_MissPz", 4000, -2000, 2000);
	hMissP[3][1]	= new TaggedHist("CutMM_MissE", 3000, 0, 3000);
	hMissP[4][1]	= new TaggedHist("CutMM_MissP", 3000, 0, 3000);
	
	
	hMissMass	= new TaggedHist("MissMass", 4000, -2000, 2000);
	hMissMassSq	= new TaggedHist("MissMassSq", 4000, -4000000, 4000000);
	hCutCheck	= new TaggedHist("CutCheck", 4000, -2000, 2000);
	
	hist[0]	= new TaggedHistSet("");
	hist[1]	= new TaggedHistSet("CutMM");
	
	
	Clear();
}
TreeAnalyse2GammaMissMass::~TreeAnalyse2GammaMissMass()
{
	
}

inline	void	TreeAnalyse2GammaMissMass::Clear()
{
	hCutCount->Reset("M");
	for(int i=0; i<5; i++)
	{
		hMissP[i][0]->Clear();
		hMissP[i][1]->Clear();
	}
	hMissMass->Clear();
	hMissMassSq->Clear();
	hCutCheck->Clear();
	hist[0]->Clear();
	hist[1]->Clear();
}

bool	TreeAnalyse2GammaMissMass::AnalyseEvent(const Int_t i)
{
	TreeReadTagged::AnalyseEvent(i);
	
	vec[0].SetPxPyPzE(CB_Px[0], CB_Py[0], CB_Pz[0], CB_E[0]);
	vec[1].SetPxPyPzE(CB_Px[1], CB_Py[1], CB_Pz[1], CB_E[1]);
	vecAll	= vec[0] + vec[1];
	
	hCutCount->Fill(1);
	hist[0]->Fill(nTagged, TaggedEnergy[0], TaggedEnergy[1], TaggedEnergy[2], nCB_Hits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
	for(int l=0; l<3; l++)
	{
		nBeam[l]	= 0;
		for(int i=0; i<nTagged[l]; i++)
		{
			beamEnergy[l][nBeam[l]]	= TaggedEnergy[l][i];
			beam[l][nBeam[l]].SetPxPyPzE(0.0, 0.0, TaggedEnergy[l][i], TaggedEnergy[l][i] + MASS_PROTON);
			miss[l][nBeam[l]]		= beam[l][nBeam[l]] - vecAll;
			missMass[l][nBeam[l]]	= miss[l][nBeam[l]].M();
			hMissMass->Fill(l, missMass[l][nBeam[l]]);
			hMissMassSq->Fill(l, miss[l][nBeam[l]].M2());
			hMissP[0][0]->Fill(l, miss[l][nBeam[l]].Px());
			hMissP[1][0]->Fill(l, miss[l][nBeam[l]].Py());
			hMissP[2][0]->Fill(l, miss[l][nBeam[l]].Pz());
			hMissP[3][0]->Fill(l, miss[l][nBeam[l]].E());
			hMissP[4][0]->Fill(l, miss[l][nBeam[l]].P());
			if(missMass[l][nBeam[l]] >= cut[0] && missMass[l][nBeam[l]] <= cut[1])
			{
				hCutCount->Fill(l+6);
				hCutCheck->Fill(l, missMass[l][nBeam[l]]);
				nBeam[l]++;
			}
		}
	}
	
	bool	ret = false;
	for(int l=0; l<3; l++)
	{
		if(nBeam[l] > 0)
		{
			hCutCount->Fill(l+2);
			for(int i=0; i<nBeam[l]; i++)
			{
				hMissP[0][1]->Fill(l, miss[l][i].Px());
				hMissP[1][1]->Fill(l, miss[l][i].Py());
				hMissP[2][1]->Fill(l, miss[l][i].Pz());
				hMissP[3][1]->Fill(l, miss[l][i].E());
				hMissP[4][1]->Fill(l, miss[l][i].P());
			}
			ret	= true;
		}
	}
	if(ret)
	{
		hist[1]->Fill(nBeam, beamEnergy[0], beamEnergy[1], beamEnergy[2], nCB_Hits, vecAll.E(), vecAll.M(), vecAll.Theta(), vecAll.Phi(), nTAPS_Hits);
		return true;
	}
	
	return false;

}
void	TreeAnalyse2GammaMissMass::Analyse(const Int_t Min, const Int_t Max)
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
bool	TreeAnalyse2GammaMissMass::Save()
{
	TString	BaseName[10];
	TString	Name[10];
	
	hMissP[0][0]->SubstractBackground();
	hMissP[1][0]->SubstractBackground();
	hMissP[2][0]->SubstractBackground();
	hMissP[3][0]->SubstractBackground();
	hMissP[4][0]->SubstractBackground();
	hMissP[0][1]->SubstractBackground();
	hMissP[1][1]->SubstractBackground();
	hMissP[2][1]->SubstractBackground();
	hMissP[3][1]->SubstractBackground();
	hMissP[4][1]->SubstractBackground();
	
	hMissMass->SubstractBackground();
	hMissMassSq->SubstractBackground();
	hCutCheck->SubstractBackground();
	
	hist[0]->SubstractBackground();
	hist[1]->SubstractBackground();
	
	Char_t	str[128];
	sprintf(str, "hist_%s_MM.root", GetFileName());
		
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCutCount->Write();
	hist[0]->Save();
	
	result->cd();
	result->mkdir("Missing");
	result->cd("Missing");
	hMissP[0][0]->SaveResult();
	hMissP[1][0]->SaveResult();
	hMissP[2][0]->SaveResult();
	hMissP[3][0]->SaveResult();
	hMissP[4][0]->SaveResult();
	hMissMass->SaveResult();
	hMissMassSq->SaveResult();
	
	gDirectory->mkdir("TaggerWindows");
	result->cd("Missing/TaggerWindows");
	hMissP[0][0]->SaveSubs();
	hMissP[1][0]->SaveSubs();
	hMissP[2][0]->SaveSubs();
	hMissP[3][0]->SaveSubs();
	hMissP[4][0]->SaveSubs();
	hMissMass->SaveSubs();
	hMissMassSq->SaveSubs();
	
	result->cd();
	result->mkdir("CutMM");
	result->cd("CutMM");
	hist[1]->Save();
	hMissP[0][1]->SaveResult();
	hMissP[1][1]->SaveResult();
	hMissP[2][1]->SaveResult();
	hMissP[3][1]->SaveResult();
	hMissP[4][1]->SaveResult();
	hCutCheck->Save();
	hMissP[0][1]->SaveSubs();
	hMissP[1][1]->SaveSubs();
	hMissP[2][1]->SaveSubs();
	hMissP[3][1]->SaveSubs();
	hMissP[4][1]->SaveSubs();
	
	result->Close();
	delete result;
}

