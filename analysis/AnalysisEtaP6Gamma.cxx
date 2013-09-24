#include "AnalysisEtaP6Gamma.h"




AnalysisEtaP6Gamma::AnalysisEtaP6Gamma(const char* Name, const char* Title)	: AnalysisDecay(Name, Title)
{
	result[0]			= new AnalysisEtaP6GammaCanvas("6G_EtaP_All", true);
	result[1]			= new AnalysisEtaP6GammaCanvas("6G_3Pi0_All", false);
	
	Clear();
	
	cutInvMassPi0[0]	= 120;
	cutInvMassPi0[1]	= 150;
	cutInvMassEta[0]	= 517;
	cutInvMassEta[1]	= 577;
}
AnalysisEtaP6Gamma::~AnalysisEtaP6Gamma()
{
}

bool	AnalysisEtaP6Gamma::CutInvariantMass(AnalysisEtaP* analysis)
{
	if(IsEtaP())
	{
		for(int i=0; i<3; i++)
		{
			if(i==bestEta)
			{
				if(mass[bestPerm][i]<cutInvMassEta[0] || mass[bestPerm][i]>cutInvMassEta[1])
					return false;
			}
			else
			{
				if(mass[bestPerm][i]<cutInvMassPi0[0] || mass[bestPerm][i]>cutInvMassPi0[1])
					return false;
			}
		}
	}
	else
	{
		if(mass[bestPerm][0]<cutInvMassPi0[0] || mass[bestPerm][0]>cutInvMassPi0[1])
			return false;
		if(mass[bestPerm][1]<cutInvMassPi0[0] || mass[bestPerm][1]>cutInvMassPi0[1])
			return false;
		if(mass[bestPerm][2]<cutInvMassPi0[0] || mass[bestPerm][2]>cutInvMassPi0[1])
			return false;
	}
	
	
	return true;
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
	}
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
		
		result[1]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		break;
	}
	
	/*if(CutInvariantMass(analysis))
	{
		if(Is3Pi0())
			result[1]->Fill(massAll, massSet);
		else
			result[0]->Fill(massAll, massSet);	
			
		return true;
	}*/
	
	return true;
}


void	AnalysisEtaP6Gamma::Draw()
{	
	for(int i=0; i<2; i++)
	{
		result[i]->Draw();
	}
}
void	AnalysisEtaP6Gamma::Save(TFile* outFile)
{	
	outFile->cd();	
	outFile->mkdir("6G");
	outFile->cd("6G");
	
	for(int i=0; i<2; i++)
	{
		result[i]->Save();
	}
	
	outFile->cd();	
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
