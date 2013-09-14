#include "ReadRootTree.h"


ReadRootTree::ReadRootTree(const char* _treeFileName, const char* _treeName)	: file(0), tree(0), isOpened(false), canvas(0)
{
	printf("ReadRootTree created. Parameters %s %s\n", _treeFileName, _treeName);
	
	strcpy(treeFileName,_treeFileName);
	strcpy(treeName,_treeName);
	
	hNTagged	= new TH1I("NTagged", "NTagged", 16, 0, 16);
	hTaggedTime	= new TH1D("TaggedTime", "TaggedTime", 1000, -250, 250);
	hNCBHits	= new TH1I("NCBHits", "NCBHits", 16, 0, 16);
	
	analysisAll[0]				= new Analysis6GammaCanvas("ETA_ALL");
	analysisAll[1]				= new Analysis6GammaCanvas("PI0_ALL");
	
	analysisDecayTagger[0][0]	= new Analysis6GammaCanvas("ETA_PROMPT");
	analysisDecayTagger[0][1]	= new Analysis6GammaCanvas("ETA_RAND1");
	analysisDecayTagger[0][2]	= new Analysis6GammaCanvas("ETA_RAND2");
	analysisDecayTagger[0][3]	= new Analysis6GammaCanvas("ETA_SINGLEPROMPT");
	analysisDecayTagger[1][0]	= new Analysis6GammaCanvas("PI0_PROMPT");
	analysisDecayTagger[1][1]	= new Analysis6GammaCanvas("PI0_RAND1");
	analysisDecayTagger[1][2]	= new Analysis6GammaCanvas("PI0_RAND2");
	analysisDecayTagger[1][3]	= new Analysis6GammaCanvas("PI0_SINGLEPROMPT");
	analysisDecayTaggerMulti[0][0]	= new Analysis6GammaCanvas("ETA_MULTIPROMPT");
	analysisDecayTaggerMulti[0][1]	= new Analysis6GammaCanvas("ETA_MULTIRAND1");
	analysisDecayTaggerMulti[0][2]	= new Analysis6GammaCanvas("ETA_MULTIRAND2");
	analysisDecayTaggerMulti[1][0]	= new Analysis6GammaCanvas("PI0_MULTIPROMPT");
	analysisDecayTaggerMulti[1][1]	= new Analysis6GammaCanvas("PI0_MULTIRAND1");
	analysisDecayTaggerMulti[1][2]	= new Analysis6GammaCanvas("PI0_MULTIRAND2");
	
	for(int i=0; i<3; i++)
	{
		cutTaggerTime[2*i]		= -100000;
		cutTaggerTime[(2*i)+1]	= 100000;
	}
	
	Clear();
}
ReadRootTree::~ReadRootTree()
{
}

void	ReadRootTree::Clear()
{
	hNTagged->Clear(); 
	hTaggedTime->Clear(); 
	hNCBHits->Clear(); 
	
	for(int i=0; i<3; i++)
	{
		countDecays[i] 						= 0;
		countTaggerTime[i]					= 0;
		countTaggerTimeMulti[i]				= 0;
	}
	countTaggerTime[3]		= 0;
	countTaggerTime[4]		= 0;
}
bool	ReadRootTree::openTree()
{
	if(isOpened)
		return true;
		
	file	= new TFile(treeFileName);
	if(!file)
	{
		printf("Could not open file %s\n", treeFileName);
		return false;
	}
	tree	= (TTree*)file->Get(treeName);
	if(!tree)
	{
		printf("Could not open tree %s in file %s\n", treeName, treeFileName);
		return false;
	}
	
	tree->SetBranchAddress("nTagged",&nTagged);
	tree->SetBranchAddress("BeamEnergy",&TaggedEnergy);
	tree->SetBranchAddress("BeamTime", &TaggedTime);
	
	tree->SetBranchAddress("nCBHits",&nCBHits);
	tree->SetBranchAddress("Px", &Px);
	tree->SetBranchAddress("Py", &Py);
	tree->SetBranchAddress("Pz", &Pz);
	tree->SetBranchAddress("E", &E);	
	tree->SetBranchAddress("Time", &Time);
	
	printf("Open file %s and load tree %s successfully\n", treeFileName, treeName);

	isOpened = true;
	return true;
}
void	ReadRootTree::AnalyseEntry(const int index)
{	
	tree->GetEntry(index);
	
	if(nCBHits == 6)
	{
		for(int i=0; i<6; i++)
		{
			vec[i].SetPxPyPzE(Px[i], Py[i], Pz[i], E[i]);
		}
		AnalyseTaggerTime();
		Reconstruct();
	}
}
void	ReadRootTree::AnalyseTaggerTime()
{
	countTaggerTime[4]++;
	nBeam[0]	= 0;
	nBeam[1]	= 0;
	nBeam[2]	= 0;
	for(int i=0; i<nTagged; i++)
	{
		if(TaggedTime[i]>cutTaggerTime[0] && TaggedTime[i]<cutTaggerTime[1])
		{
			beam[0][nBeam[0]].SetPxPyPzE(TaggedEnergy[i], 0.0, 0.0, TaggedEnergy[i]);
			nBeam[0]++;
		}
		if(TaggedTime[i]>cutTaggerTime[2] && TaggedTime[i]<cutTaggerTime[3])
		{
			beam[1][nBeam[1]].SetPxPyPzE(TaggedEnergy[i], 0.0, 0.0, TaggedEnergy[i]);
			nBeam[1]++;
		}
		if(TaggedTime[i]>cutTaggerTime[4] && TaggedTime[i]<cutTaggerTime[5])
		{
			beam[2][nBeam[2]].SetPxPyPzE(TaggedEnergy[i], 0.0, 0.0, TaggedEnergy[i]);
			nBeam[2]++;
		}
	}
}
void	ReadRootTree::Analyse(const int min, const int max)
{
	int start=min;
	if(start < 0)
		start	= 0;
	if(start >= tree->GetEntries())
		start	= tree->GetEntries()-1;
	int stop=max;
	if(stop < 0)
		stop	= tree->GetEntries();
	if(stop > tree->GetEntries())
		stop	= tree->GetEntries();
		
	for(int i=start; i<stop; i++)
	{
		hNTagged->Fill(nTagged);
		for(int t=0; t<nTagged; t++)
			hTaggedTime->Fill(TaggedTime[t]);
		hNCBHits->Fill(nCBHits);
		AnalyseEntry(i);
	}
	
	PrintCounters();
}
void	ReadRootTree::Reconstruct()
{
	countDecays[0]++;
	
	Double_t		ChiSqr[15][4];
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
	
	partAll	= part[0][0] + part[0][1] + part[0][2];
	massAll = partAll.M();
	
	switch(bestEta)
	{
	case 0:
		countDecays[1]++;
		analysisAll[0]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll);
		for(int i=0; i<3; i++)
		{
			if(nBeam[i] > 1)
			{
				countTaggerTimeMulti[i]++;
				analysisDecayTaggerMulti[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll);
			}
			else if(nBeam[i] == 1)
			{
				if(i==0 && nTagged == 1)
				{
					countTaggerTime[3]++;
					analysisDecayTagger[0][3]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll);
				}
				else
				{
					countTaggerTime[i]++;
					analysisDecayTagger[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][2], mass[bestPerm][0], massAll);
				}
			}
		}
		break;
	case 1:
		countDecays[1]++;
		analysisAll[0]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll);
		for(int i=0; i<3; i++)
		{
			if(nBeam[i] > 1)
			{
				countTaggerTimeMulti[i]++;
				analysisDecayTaggerMulti[0][i]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll);
			}
			else if(nBeam[i] == 1)
			{
				if(i==0 && nTagged == 1)
				{
					countTaggerTime[3]++;
					analysisDecayTagger[0][3]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll);
				}
				else
				{
					countTaggerTime[i]++;
					analysisDecayTagger[0][i]->Fill(mass[bestPerm][0], mass[bestPerm][2], mass[bestPerm][1], massAll);
				}
			}
		}
		break;
	case 2:
		countDecays[1]++;
		analysisAll[0]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
		for(int i=0; i<3; i++)
		{
			if(nBeam[i] > 1)
			{
				countTaggerTimeMulti[i]++;
				analysisDecayTaggerMulti[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
			}
			else if(nBeam[i] == 1)
			{
				if(i==0 && nTagged == 1)
				{
					countTaggerTime[3]++;
					analysisDecayTagger[0][3]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
				}
				else
				{
					countTaggerTime[i]++;
					analysisDecayTagger[0][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
				}
			}
		}
		break;
	case 3:
		countDecays[2]++;
		analysisAll[1]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
		for(int i=0; i<3; i++)
		{
			if(nBeam[i] > 1)
			{
				countTaggerTimeMulti[i]++;
				analysisDecayTaggerMulti[1][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
			}
			else if(nBeam[i] == 1)
			{
				if(i==0 && nTagged == 1)
				{
					countTaggerTime[3]++;
					analysisDecayTagger[1][3]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
				}
				else
				{
					countTaggerTime[i]++;
					analysisDecayTagger[1][i]->Fill(mass[bestPerm][1], mass[bestPerm][0], mass[bestPerm][2], massAll);
				}
			}
		}
		break;
	}
}

void	ReadRootTree::PrintCounters()
{
	printf("Counters:\n");
	printf("Decay: 	    %d events checked. %d reconstructed as Eta & 2Pi0 (%lf %%). %d reconstructed as 3Pi0 (%lf %%)\n", countDecays[0], countDecays[1], 100.0 * double(countDecays[1])/double(countDecays[0]), countDecays[2], 100.0*double(countDecays[2])/double(countDecays[0]));
	printf("TaggerTime: %d events checked. %d in Prompt Window (%lf %%). %d in Rand1 Window (%lf %%). %d in Rand2 Window (%lf %%)\n", countTaggerTime[4], countTaggerTime[0], 100.0 * double(countTaggerTime[0])/double(countTaggerTime[4]), countTaggerTime[1], 100.0*double(countTaggerTime[1])/double(countTaggerTime[4]), countTaggerTime[2], 100.0*double(countTaggerTime[2])/double(countTaggerTime[4]));
	printf("           MultiHits:          %d in Prompt Window (%lf %%). %d in Rand1 Window (%lf %%). %d in Rand2 Window (%lf %%)\n", countTaggerTimeMulti[0], 100.0 * double(countTaggerTimeMulti[0])/double(countTaggerTime[4]), countTaggerTimeMulti[1], 100.0*double(countTaggerTimeMulti[1])/double(countTaggerTime[4]), countTaggerTimeMulti[2], 100.0*double(countTaggerTimeMulti[2])/double(countTaggerTime[4]));
	printf("           Single Prompt Hit:  %d (%lf %%)\n", countTaggerTime[3], 100.0 * double(countTaggerTime[3])/double(countTaggerTime[4]));
	
}
void	ReadRootTree::Draw()
{
	if(!(canvas	= (TCanvas*)gROOT->Get("General")))
		canvas	= new TCanvas("General", "General", 50, 50, 1600, 800);
		
	canvas->Divide(2, 2, 0.001, 0.001);
	
	canvas->cd(1);	hNTagged->Draw();
	canvas->cd(2);	hTaggedTime->Draw();
	canvas->cd(3);	hNCBHits->Draw();
	
	analysisAll[0]->Draw();
	analysisAll[1]->Draw();
	
	for(int l=0; l<2; l++)
	{
		for(int i=0; i<3; i++)
		{
			analysisDecayTagger[l][i] ->Draw();
			analysisDecayTaggerMulti[l][i] ->Draw();
		}
		analysisDecayTagger[l][3] ->Draw();
	}

}


int		ReadRootTree::perm[15][6]=
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
