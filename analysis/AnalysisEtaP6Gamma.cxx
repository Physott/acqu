#include "AnalysisEtaP6Gamma.h"




AnalysisEtaP6Gamma::AnalysisEtaP6Gamma(const char* _treeFileName, const char* _treeName)	: AnalysisTagger(_treeFileName, _treeName)
{
	canvasAll[0]			= new AnalysisEtaP6GammaCanvas("EtaP_All");
	canvasAll[1]			= new AnalysisEtaP6GammaCanvas("3Pi0_All");
	canvasUntagged[0]		= new AnalysisEtaP6GammaCanvas("EtaP_UnTag");
	canvasUntagged[1]		= new AnalysisEtaP6GammaCanvas("3Pi0_UnTag");
	canvasTagged[0][0]		= new AnalysisEtaP6GammaCanvas("EtaP_Pro");
	canvasTagged[0][1]		= new AnalysisEtaP6GammaCanvas("EtaP_Ran1");
	canvasTagged[0][2]		= new AnalysisEtaP6GammaCanvas("EtaP_Ran2");
	canvasTagged[0][3]		= new AnalysisEtaP6GammaCanvas("EtaP_SiPro");
	canvasTagged[1][0]		= new AnalysisEtaP6GammaCanvas("3Pi0_Pro");
	canvasTagged[1][1]		= new AnalysisEtaP6GammaCanvas("3Pi0_Ran1");
	canvasTagged[1][2]		= new AnalysisEtaP6GammaCanvas("3Pi0_Ran2");
	canvasTagged[1][3]		= new AnalysisEtaP6GammaCanvas("3Pi0_SiPro");
	canvasTaggedMulti[0][0]	= new AnalysisEtaP6GammaCanvas("EtaP_MulPro");
	canvasTaggedMulti[0][1]	= new AnalysisEtaP6GammaCanvas("EtaP_MulRan1");
	canvasTaggedMulti[0][2]	= new AnalysisEtaP6GammaCanvas("EtaP_MulRan2");
	canvasTaggedMulti[1][0]	= new AnalysisEtaP6GammaCanvas("3Pi0_MulPro");
	canvasTaggedMulti[1][1]	= new AnalysisEtaP6GammaCanvas("3Pi0_MulRan1");
	canvasTaggedMulti[1][2]	= new AnalysisEtaP6GammaCanvas("3Pi0_MulRan2");
	
	Clear();
}
AnalysisEtaP6Gamma::~AnalysisEtaP6Gamma()
{
}

void	AnalysisEtaP6Gamma::Clear()
{
	AnalysisTagger::Clear();
	
	for(int i=0; i<2; i++)
	{
		canvasAll[i]->Clear();
		canvasUntagged[i]->Clear();
		countDecaysAll[i] 					= 0;
		countDecaysUntagged[i]			= 0;
		for(int l=0; l<3; l++)
		{
			canvasTagged[i][l]->Clear();
			canvasTaggedMulti[i][l]->Clear();
			countDecaysTagged[i][l]			= 0;
			countDecaysTaggedMulti[i][l]	= 0;
		}
		canvasTagged[i][3]->Clear();
		countDecaysTagged[i][3]			= 0;
	}
	countDecaysAll[2]				= 0;
	countDecaysUntagged[2]			= 0;
}
void	AnalysisEtaP6Gamma::SetMass(const int index, const Double_t mass)
{
	Double_t E	= (partSet[index].E() - partSet[index].M()) + mass;
	Double_t P	= TMath::Sqrt(E*E - mass*mass);

	partSet[index].SetE(E);
	partSet[index].SetVect(partSet[index].Vect().Unit() * P);
}
bool	AnalysisEtaP6Gamma::AnalyseEvent(const int index)
{	
	if(AnalysisTagger::AnalyseEvent(index))
	{
		if(GetNCBHits()!=6)
			return false;
			
		countDecaysAll[2]++;
		
		Reconstruct();
		
		return true;
	}
	return false;
}
void	AnalysisEtaP6Gamma::Analyse(const int min, const int max)
{
	int start=min;
	if(start < 0)
		start	= 0;
	if(start >= GetNEvents())
		start	= GetNEvents()-1;
	int stop=max;
	if(stop < 0)
		stop	= GetNEvents();
	if(stop > GetNEvents())
		stop	= GetNEvents();
		
	for(int i=start; i<stop; i++)
	{
		AnalyseEvent(i);
	}
	
	PrintCounters();
}
void	AnalysisEtaP6Gamma::calcEvent()
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
	
	massAll = all.M();
	partSet[0]	= part[bestPerm][0];
	partSet[1]	= part[bestPerm][1];
	partSet[2]	= part[bestPerm][2];
	
}
void	AnalysisEtaP6Gamma::Reconstruct()
{
	countDecaysAll[2]++;
	
	calcEvent();
	
	switch(bestEta)
	{
	case 0:
		countDecaysAll[0]++;
		
		SetMass(0, MASS_ETA);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		canvasAll[0]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
		if(isUntagged())
			canvasUntagged[0]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
		else
		{
			for(int i=0; i<3; i++)
			{
				if(nBeam[i] > 1)
					canvasTaggedMulti[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
				else if(nBeam[i] == 1)
				{
					if(i==0 && isUniqueWindow())
						canvasTagged[0][3]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
					canvasTagged[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll, massSet);
				}
			}
		}
		break;
	case 1:
		countDecaysAll[0]++;
		
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_ETA);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		canvasAll[0]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
		if(isUntagged())
			canvasUntagged[0]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
		else
		{
			for(int i=0; i<3; i++)
			{
				if(nBeam[i] > 1)
					canvasTaggedMulti[0][i]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
				else if(nBeam[i] == 1)
				{
					if(i==0 && isUniqueWindow())
						canvasTagged[0][3]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
					canvasTagged[0][i]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll, massSet);
				}
			}
		}
		break;
	case 2:
		countDecaysAll[0]++;
		
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_ETA);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		canvasAll[0]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		if(isUntagged())
			canvasUntagged[0]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		else
		{
			for(int i=0; i<3; i++)
			{
				if(nBeam[i] > 1)
					canvasTaggedMulti[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
				else if(nBeam[i] == 1)
				{
					if(i==0 && isUniqueWindow())
						canvasTagged[0][3]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
					canvasTagged[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
				}
			}
		}
		break;
	case 3:
		countDecaysAll[1]++;
		
		SetMass(0, MASS_PI0);
		SetMass(1, MASS_PI0);
		SetMass(2, MASS_PI0);
		allSet	= partSet[0] + partSet[1] + partSet[2];
		massSet	= allSet.M();
		
		canvasAll[1]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		if(isUntagged())
			canvasUntagged[1]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
		else
		{
			for(int i=0; i<3; i++)
			{
				if(nBeam[i] > 1)
					canvasTaggedMulti[1][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
				else if(nBeam[i] == 1)
				{
					if(i==0 && isUniqueWindow())
						canvasTagged[1][3]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
					canvasTagged[1][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll, massSet);
				}
			}
		}
		break;
	}
}

void	AnalysisEtaP6Gamma::PrintCounters()
{
	AnalysisTagger::PrintCounters();
	
	printf("\tDecay:\t\t%d events checked.\t\t%d reconstructed as Eta & 2Pi0 (%lf %%).\t%d reconstructed as 3Pi0 (%lf %%)\n", countDecaysAll[2], countDecaysAll[0], 100.0 * double(countDecaysAll[0])/double(countDecaysAll[2]), countDecaysAll[1], 100.0*double(countDecaysAll[1])/double(countDecaysAll[2]));
}
void	AnalysisEtaP6Gamma::Draw()
{
	AnalysisTagger::Draw();
	
	for(int i=0; i<2; i++)
	{
		canvasAll[i]->Draw();
		canvasUntagged[i]->Draw();
		for(int l=0; l<3; l++)
		{
			canvasTagged[i][l]->Draw();
			canvasTaggedMulti[i][l]->Draw();
		}
		canvasTagged[i][3]->Draw();
	}
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
