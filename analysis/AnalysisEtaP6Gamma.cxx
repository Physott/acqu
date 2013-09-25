#include "AnalysisEtaP6Gamma.h"




AnalysisEtaP6Gamma::AnalysisEtaP6Gamma()
{
	if(!(hCountDecay	= (TH1I*)gROOT->Get("6G_CountDecay")))
		hCountDecay		= new TH1I("6G_CountDecay", "1:All / 2:EtaP / 3:3Pi0", 5, 0, 5);
	
	result[0]			= new AnalysisEtaP6GammaCanvas("6G_EtaP", true);
	result[1]			= new AnalysisEtaP6GammaCanvas("6G_3Pi0", false);
	
	rawHist[0]			= new ReadRootTreeHist("6G");
	rawHist[1]			= new ReadRootTreeHist("6G_EtaP");
	rawHist[2]			= new ReadRootTreeHist("6G_3Pi0");
	
	cutEta		= new Cut1Value("6G_EtaP_CutIMEta", 300, 400, 700);
	cutEta->SetCut(517, 577);
	cut2Pi0		= new CutNValues("6G_EtaP_CutIM2Pi0", 2, 300, 0, 300);
	cut2Pi0->SetCut(120, 150);
	cut3Pi0		= new CutNValues("6G_3Pi0_CutIM", 3, 300, 0, 300);
	cut3Pi0->SetCut(120, 150);
	
	cutResult[0]		= new AnalysisEtaP6GammaCanvas("6G_EtaP_CutIM", true);
	cutResult[1]		= new AnalysisEtaP6GammaCanvas("6G_3Pi0_CutIM", false);
	
	cutRawHist[0]		= new ReadRootTreeHist("6G_EtaP_CutIM");
	cutRawHist[1]		= new ReadRootTreeHist("6G_3Pi0_CutIM");
	
	Clear();
}
AnalysisEtaP6Gamma::~AnalysisEtaP6Gamma()
{
	if(result[0])
		delete result[0];
	if(result[1])
		delete result[1];
	if(rawHist[0])
		delete rawHist[0];
	if(rawHist[1])
		delete rawHist[1];
	if(rawHist[2])
		delete rawHist[2];
		
	if(cutEta)
		delete cutEta;
	if(cut2Pi0)
		delete cut2Pi0;
	if(cut3Pi0)
		delete cut3Pi0;
		
	if(cutResult[0])
		delete cutResult[0];
	if(cutResult[1])
		delete cutResult[1];
	if(cutRawHist[0])
		delete cutRawHist[0];
	if(cutRawHist[1])
		delete cutRawHist[1];
}


void	AnalysisEtaP6Gamma::Fill(AnalysisEtaP6GammaCanvas* can)
{
	switch(bestEta)
	{
	case 0:
		can->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
		break;
	case 1:
		can->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
		break;
	case 2:
	case 3:
		can->Fill(mass[bestPerm][0], mass[bestPerm][1], mass[bestPerm][2], massAll, massSet);
		break;
	}
}
	
	

void	AnalysisEtaP6Gamma::Clear()
{
	for(int i=0; i<2; i++)
	{
		result[i]->Clear();
		rawHist[i]->Clear();
		cutResult[i]->Clear();
		cutRawHist[i]->Clear();
	}
	rawHist[2]->Clear();
	cutEta->Clear();
	cut2Pi0->Clear();
	cut3Pi0->Clear();
}
void	AnalysisEtaP6Gamma::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}


void	AnalysisEtaP6Gamma::calcEvent(AnalysisEtaP* analysis)
{
	for(int i=0; i<15; i++)
	{
		part[i][0]	= analysis->vec[perm[i][0]] + analysis->vec[perm[i][1]];
		part[i][1]	= analysis->vec[perm[i][2]] + analysis->vec[perm[i][3]];
		part[i][2]	= analysis->vec[perm[i][4]] + analysis->vec[perm[i][5]];
		
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
	
	massAll = analysis->vecAll.M();
	partSet[0]	= part[bestPerm][0];
	partSet[1]	= part[bestPerm][1];
	partSet[2]	= part[bestPerm][2];
	
}
bool	AnalysisEtaP6Gamma::Analyse(AnalysisEtaP* analysis)
{	
	calcEvent(analysis);
	
	hCountDecay->Fill(1);
	
	switch(bestEta)
	{
	case 0:
		massPi0[0]	= mass[bestPerm][1];
		massPi0[1]	= mass[bestPerm][2];
		massPi0Eta	= mass[bestPerm][0];
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		hCountDecay->Fill(2);
		analysis->Fill(rawHist[0]);
		analysis->Fill(rawHist[1]);
		result[0]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
		break;
	case 1:
		massPi0[0]	= mass[bestPerm][0];
		massPi0[1]	= mass[bestPerm][2];
		massPi0Eta	= mass[bestPerm][1];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_ETA);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		hCountDecay->Fill(2);
		analysis->Fill(rawHist[0]);
		analysis->Fill(rawHist[1]);
		result[0]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
		break;
	case 2:
		massPi0[0]	= mass[bestPerm][0];
		massPi0[1]	= mass[bestPerm][1];
		massPi0Eta	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_ETA);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		hCountDecay->Fill(2);
		analysis->Fill(rawHist[0]);
		analysis->Fill(rawHist[1]);
		result[0]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		break;
	case 3:
		massPi0[0]	= mass[bestPerm][0];
		massPi0[1]	= mass[bestPerm][1];
		massPi0Eta	= mass[bestPerm][2];
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		hCountDecay->Fill(3);
		analysis->Fill(rawHist[0]);
		analysis->Fill(rawHist[2]);
		result[1]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		break;
	}
	
	if(IsEtaP())
	{
		if(!cutEta->Analyse(massPi0Eta))
			return false;
		if(!cut2Pi0->Analyse(massPi0[0], massPi0[1]))
			return false;
		analysis->Fill(cutRawHist[0]);
		cutResult[0]->Fill(massPi0[0], massPi0[1], massPi0Eta, massAll, massSet);
	}
	else
	{
		if(!cut3Pi0->Analyse(massPi0[0], massPi0[1], massPi0Eta))
			return false;
		analysis->Fill(cutRawHist[1]);
		cutResult[1]->Fill(massPi0[0], massPi0[1], massPi0Eta, massAll, massSet);
	}
	
	return true;
}


void	AnalysisEtaP6Gamma::Draw()
{	
	if(!(canvas[0]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G")))
		canvas[0]	= new TCanvas("6G", "6G", 50, 50, 1600, 800);
	canvas[0]->Clear();
	canvas[0]->Divide(3, 3, 0.001, 0.001);
	
	canvas[0]->cd(1);	hCountDecay->Draw();
	rawHist[0]->Draw(canvas[0], 4, 5, 6, 7, 8, 9);
	
	
	if(!(canvas[1]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_EtaP")))
		canvas[1]	= new TCanvas("6G_EtaP", "6G_EtaP", 50, 50, 1600, 800);
	canvas[1]->Clear();
	canvas[1]->Divide(3, 5, 0.001, 0.001);
	
	result[0]->Draw(canvas[1], 1, 2, 3, 4, 5, 6, 7, 8);
	rawHist[1]->Draw(canvas[1], 10, 11, 12, 13, 14, 15);
	
	
	if(!(canvas[2]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_3Pi0")))
		canvas[2]	= new TCanvas("6G_3Pi0", "6G_3Pi0", 50, 50, 1600, 800);
	canvas[2]->Clear();
	canvas[2]->Divide(3, 5, 0.001, 0.001);
	
	result[1]->Draw(canvas[2], 1, 2, 3, 4, 5, 6, 7, 8);
	rawHist[2]->Draw(canvas[2], 10, 11, 12, 13, 14, 15);
	
	DrawCut();
}
void	AnalysisEtaP6Gamma::DrawCut()
{	
	if(!(cutCanvas[0]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_EtaP_CutIM")))
		cutCanvas[0]	= new TCanvas("6G_EtaP_CutIM", "6G_EtaP_CutIM", 50, 50, 1600, 800);
	cutCanvas[0]->Clear();
	cutCanvas[0]->Divide(9, 4, 0.001, 0.001);
	
	cutEta->Draw(cutCanvas[0], 1, 2, 3);
	cut2Pi0->Draw(cutCanvas[0], 4, 5, 6, 7, 8, 9);
	cutResult[0]->Draw(cutCanvas[0], 10, 11, 12, 13, 14, 15, 16, 17);
	cutRawHist[0]->Draw(cutCanvas[0], 19, 20, 21, 22, 23, 24);
	
	
	if(!(cutCanvas[1]	= (TCanvas*)gROOT->GetListOfCanvases()->FindObject("6G_3Pi0_CutIM")))
		cutCanvas[1]	= new TCanvas("6G_3Pi0_CutIM", "6G_3Pi0_CutIM", 50, 50, 1600, 800);
	cutCanvas[1]->Clear();
	cutCanvas[1]->Divide(9, 3, 0.001, 0.001);
	
	cut3Pi0->Draw(cutCanvas[1], 1, 2, 3, 4, 5, 6, 7, 8, 9);
	cutResult[1]->Draw(cutCanvas[1], 10, 11, 12, 13, 14, 15, 16, 17);
	cutRawHist[1]->Draw(cutCanvas[1], 19, 20, 21, 22, 23, 24);
}
void	AnalysisEtaP6Gamma::Save(TFile* outFile, const Char_t* nameParent)
{	
	hCountDecay->Write();
	rawHist[0]->Save();
	
	
	Char_t	str[64];
	sprintf(str,"%s/EtaP", nameParent);
	outFile->mkdir(str);
	outFile->cd(str);
	
	result[0]->Save();
	rawHist[1]->Save();
	
	
	sprintf(str,"%s/3Pi0", nameParent);
	outFile->mkdir(str);
	outFile->cd(str);
	
	result[1]->Save();
	rawHist[2]->Save();
	
	
	sprintf(str,"%s/EtaP/CutIM", nameParent);
	outFile->mkdir(str);
	outFile->cd(str);
	
	cutEta->Save();
	cut2Pi0->Save();
	cutResult[0]->Save();
	cutRawHist[0]->Save();
	
	
	sprintf(str,"%s/3Pi0/CutIM", nameParent);
	outFile->mkdir(str);
	outFile->cd(str);
	
	cut3Pi0->Save();
	cutResult[1]->Save();
	cutRawHist[1]->Save();
}

	

int		AnalysisEtaP6Gamma::perm[15][6]=
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
