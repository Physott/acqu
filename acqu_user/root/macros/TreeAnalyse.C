#include "TreeAnalyseTagger.C"
#include "TreeAnalyse2Gamma.C"
#include "TreeAnalyse6Gamma.C"
#include "TreeAnalyse2GammaMissMass.C"
#include "TreeAnalyse6GammaMissMass.C"

void	CorrectBackground()
{
	
}
void	TreeAnalyse(const Char_t* FileName)
{
	TreeAnalyseTagger* Tagger;
	TreeAnalyse2Gamma* analysis2g;
	TreeAnalyse6Gamma* analysis6g;
	TreeAnalyse2GammaMissMass* missMass2g;
	TreeAnalyse6GammaMissMass* missMass6g;
	
	printf("Analysing Tagger\n");
	Tagger = new TreeAnalyseTagger(FileName);
	Tagger->Open();
	if(!Tagger->IsOpen())
	{
		delete Tagger;
		return;
	}
	Tagger->SetCutCBTime(-50, 50);
	Tagger->SetCutTaggerTimePrompt(-6,  6);
	Tagger->SetCutTaggerTimeRand1(-20, -8);
	Tagger->SetCutTaggerTimeRand2(  8, 20);
	Tagger->Analyse();
	Tagger->Save();
	delete Tagger;
	
	Char_t	str1[128];
	TreeRead::FindName(FileName, str1);
	Char_t	str2[128];
	
	{//2g
	printf("Analysing 2g IM\n");
	sprintf(str2, "tree_%s_2g", str1);
	analysis2g = new TreeAnalyse2Gamma(str2);
	analysis2g->Open();
	if(!analysis2g->IsOpen())
	{
		delete analysis2g;
		return;
	}
	analysis2g->Analyse();
	analysis2g->Save();
	delete analysis2g;
	
	{//mm
	printf("Analysing 2g IM Pi0 MM\n");
	sprintf(str2, "tree_%s_2g_IMPi0", str1);
	missMass2g = new TreeAnalyse2GammaMissMass(str2);
	missMass2g->Open();
	if(!missMass2g->IsOpen())
	{
		delete missMass2g;
		return;
	}
	missMass2g->Analyse();
	missMass2g->Save();
	delete missMass2g;
	
	printf("Analysing 2g IM Eta MM\n");
	sprintf(str2, "tree_%s_2g_IMEta", str1);
	missMass2g = new TreeAnalyse2GammaMissMass(str2);
	missMass2g->Open();
	if(!missMass2g->IsOpen())
	{
		delete missMass2g;
		return;
	}
	missMass2g->Analyse();
	missMass2g->Save();
	delete missMass2g;
	
	printf("Analysing 2g IM EtaP MM\n");
	sprintf(str2, "tree_%s_2g_IMEtaP", str1);
	missMass2g = new TreeAnalyse2GammaMissMass(str2);
	missMass2g->Open();
	if(!missMass2g->IsOpen())
	{
		delete missMass2g;
		return;
	}
	missMass2g->Analyse();
	missMass2g->Save();
	delete missMass2g;
	}
	
	}
	
	{//6g
	printf("Analysing 6g IM\n");
	sprintf(str2, "tree_%s_6g", str1);
	analysis6g = new TreeAnalyse6Gamma(str2);
	analysis6g->Open();
	if(!analysis6g->IsOpen())
	{
		delete analysis6g;
		return;
	}
	analysis6g->Analyse();
	analysis6g->Save();
	delete analysis6g;
	
	{//mm
	printf("Analysing 6g IM EtaP MM\n");
	sprintf(str2, "tree_%s_6g_IMEtaP", str1);
	missMass6g = new TreeAnalyse6GammaMissMass(str2);
	missMass6g->Open();
	if(!missMass6g->IsOpen())
	{
		delete missMass6g;
		return;
	}
	missMass6g->Analyse();
	missMass6g->Save();
	delete missMass6g;
	
	printf("Analysing 6g IM 3Pi0 MM\n");
	sprintf(str2, "tree_%s_6g_IM3Pi0", str1);
	missMass6g = new TreeAnalyse6GammaMissMass(str2);
	missMass6g->Open();
	if(!missMass6g->IsOpen())
	{
		delete missMass6g;
		return;
	}
	missMass6g->Analyse();
	missMass6g->Save();
	delete missMass6g;
	
	}
	
	}
	
}

void	AddFiles(const Char_t* FolderName)
{
	Char_t	str1[256];
	sprintf(str1, "hadd -f %s/hist_Multiplicity.root %s/hist_*_Multiplicity.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/hist_2g_Tagger.root %s/hist_*_2g_Tagger.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/hist_6g_Tagger.root %s/hist_*_6g_Tagger.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/hist_10g_Tagger.root %s/hist_*_10g_Tagger.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/hist_Tagger.root %s/hist_2g_Tagger.root %s/hist_6g_Tagger.root %s/hist_10g_Tagger.root", FolderName, FolderName, FolderName, FolderName);
	gSystem->Exec(str1);
	
	
	sprintf(str1, "hadd -f %s/hist_2g_Prompt_IM.root %s/hist_*_2g_Prompt_IM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/hist_2g_Rand1_IM.root %s/hist_*_2g_Rand1_IM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/hist_2g_Rand2_IM.root %s/hist_*_2g_Rand2_IM.root", FolderName, FolderName);
	gSystem->Exec(str1);
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
				//gROOT->ls();
			}
			else
				printf("\n");
		}
		
	}
}
