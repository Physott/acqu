#include "TreeAnalyseMultiplicity.C"
#include "TreeAnalyse2GammaTagg.C"
#include "TreeAnalyse2GammaTaggMissMass.C"


void	TreeAnalyse(const Char_t* FileName)
{
	TreeAnalyseMultiplicity* a = new TreeAnalyseMultiplicity(FileName);
	a->Open();
	a->SetCutCBTime(-50, 50);
	a->Analyse();
	a->Save();
	delete a;
	
	Char_t	str1[128];
	TreeRead::FindName(FileName, str1);
	Char_t	str2[128];
	
	sprintf(str2, "tree_%s_2g_IMEtaP.root", str1);
	printf("\t\tAnalyse 2g EtaP Tagger. File: %s\n\t\t\t", str2);
	TreeAnalyse2GammaTagg* b = new TreeAnalyse2GammaTagg(str2);
	b->Open();
	b->SetCutTaggPrompt(-6, 6);
	b->SetCutTaggRand1(-20, -8);
	b->SetCutTaggRand2(8, 20);
	b->Analyse();
	b->Save();
	delete b;
	sprintf(str2, "tree_%s_2g_IMEta.root", str1);
	printf("\t\tAnalyse 2g Eta Tagger. File: %s\n\t\t\t", str2);
	b = new TreeAnalyse2GammaTagg(str2, ETA2G_Eta);
	b->Open();
	b->SetCutTaggPrompt(-6, 6);
	b->SetCutTaggRand1(-20, -8);
	b->SetCutTaggRand2(8, 20);
	b->Analyse();
	b->Save();
	delete b;
	sprintf(str2, "tree_%s_2g_IMPi0.root", str1);
	printf("\t\tAnalyse 2g Pi0 Tagger. File: %s\n\t\t\t", str2);
	b = new TreeAnalyse2GammaTagg(str2, ETA2G_Pi0);
	b->Open();
	b->SetCutTaggPrompt(-6, 6);
	b->SetCutTaggRand1(-20, -8);
	b->SetCutTaggRand2(8, 20);
	b->Analyse();
	b->Save();
	delete b;
	
	{	//Analyse 2g
		printf("\t\tAnalyse 2g EtaP_MM\n");
		sprintf(str2, "tree_%s_2g_IMEtaP_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEtaP_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEtaP_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		
		
		printf("\t\tAnalyse 2g Eta_MM\n");
		sprintf(str2, "tree_%s_2g_IMEta_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g Eta Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEta_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g Eta Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEta_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g Eta Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		
		
		printf("\t\tAnalyse 2g Pi0_MM\n");
		sprintf(str2, "tree_%s_2g_IMPi0_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMPi0_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMPi0_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		c->Analyse();
		c->Save();
		delete c;
	}
}

void	TreeAnalyseFolder(const Char_t* FolderName)
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
			if (!sfile->IsDirectory() && fname.EndsWith(".root") && !fname.BeginsWith("tree_") && !fname.BeginsWith("hist_")) 
			{
				TreeAnalyse(fname.Data());
			}
			else
				printf("\n");
		}
	}
}
