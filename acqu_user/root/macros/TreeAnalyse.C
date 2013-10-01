#include "TreeAnalyseMultiplicity.C"
#include "TreeAnalyse2GammaTagg.C"
#include "TreeAnalyse2GammaTaggMissMass.C"
#include "TreeAnalyse6Gamma.C"


void	TreeAnalyse(const Char_t* FileName)
{
	TreeAnalyseMultiplicity* a = new TreeAnalyseMultiplicity(FileName);
	a->Open();
	if(!a->IsOpen())
	{
		delete a;
		return;
	}
	a->SetCutCBTime(-50, 50);
	a->Analyse();
	a->Save();
	delete a;
	
	Char_t	str1[128];
	TreeRead::FindName(FileName, str1);
	Char_t	str2[128];

	{	//Analyse 2g
		sprintf(str2, "tree_%s_2g_IMEtaP.root", str1);
		printf("\t\tAnalyse 2g EtaP Tagger. File: %s\n\t\t\t", str2);
		TreeAnalyse2GammaTagg* b = new TreeAnalyse2GammaTagg(str2);
		b->Open();
		if(!b->IsOpen())
		{
			delete b;
			return;
		}
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
		if(!b->IsOpen())
		{
			delete b;
			return;
		}

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
		if(!b->IsOpen())
		{
			delete b;
			return;
		}

		b->SetCutTaggPrompt(-6, 6);
		b->SetCutTaggRand1(-20, -8);
		b->SetCutTaggRand2(8, 20);
		b->Analyse();
		b->Save();
		delete b;
	
	
		printf("\t\tAnalyse 2g EtaP_MM\n");
		sprintf(str2, "tree_%s_2g_IMEtaP_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEtaP_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEtaP_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g EtaP Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_EtaP);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		
		
		printf("\t\tAnalyse 2g Eta_MM\n");
		sprintf(str2, "tree_%s_2g_IMEta_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g Eta Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEta_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g Eta Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMEta_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g Eta Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Eta);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		
		
		printf("\t\tAnalyse 2g Pi0_MM\n");
		sprintf(str2, "tree_%s_2g_IMPi0_Prompt.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Prompt_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMPi0_Rand1.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Rand1_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
		sprintf(str2, "tree_%s_2g_IMPi0_Rand2.root", str1);
		printf("\t\t\tAnalyse 2g Pi0 Rand2_MM. File: %s\n\t\t\t\t", str2);
		TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(str2, ETA2G_Pi0);
		c->Open();
		if(!c->IsOpen())
		{
			delete c;
			return;
		}
		c->Analyse();
		c->Save();
		delete c;
	}
	
	{	//Analyse 6g
		sprintf(str2, "tree_%s_6g.root", str1);
		printf("\t\tAnalyse 6g. File: %s\n\t\t\t", str2);
		TreeAnalyse6Gamma* a6 = new TreeAnalyse6Gamma(str2);
		a6->Open();
		if(!a6->IsOpen())
		{
			delete a6;
			return;
		}
		a6->Analyse(100);
		a6->Save();
		delete a6;
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
		
		TSystem	sys;
		sys.Init();
		sys.Exec("hadd hist_Multiplicity.root hist_*_Multiplicity.root");
		sys.Exec("hadd hist_2g_IMPi0Tagg.root hist_*_2g_IMPi0Tagg.root");
		sys.Exec("hadd hist_2g_IMEtaTagg.root hist_*_2g_IMEtaTagg.root");
		sys.Exec("hadd hist_2g_IMEtaPTagg.root hist_*_2g_IMEtaPTagg.root");
		sys.Exec("hadd hist_2g_IMPi0Tagg.root hist_*_2g_IMPi0Tagg.root");
		sys.Exec("hadd hist_2g_IMEtaTagg.root hist_*_2g_IMEtaTagg.root");
		sys.Exec("hadd hist_2g_IMEtaPTagg.root hist_*_2g_IMEtaPTagg.root");
		sys.Exec("hadd hist_6g_IM.root hist_*_6g_IM.root");
	}
}
