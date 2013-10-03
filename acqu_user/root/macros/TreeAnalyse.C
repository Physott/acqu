#include "TreeAnalyseMultiplicity.C"
#include "TreeAnalyseTagger.C"
#include "TreeAnalyse2Gamma.C"

void	CorrectBackground()
{
	
}
void	TreeAnalyse(const Char_t* FileName)
{
	printf("Analysing Multiplicity\n");
	TreeAnalyseMultiplicity* multiplicity = new TreeAnalyseMultiplicity(FileName);
	multiplicity->Open();
	if(!multiplicity->IsOpen())
	{
		delete multiplicity;
		return;
	}
	multiplicity->SetCutCBTime(-50, 50);
	multiplicity->Analyse(10000);
	multiplicity->Save();
	delete multiplicity;
	
	Char_t	str1[128];
	TreeRead::FindName(FileName, str1);
	Char_t	str2[128];
	
	{//Tagger
	
	printf("Analysing Tagger 2G\n");
	sprintf(str2, "tree_%s_2g", str1);
	TreeAnalyseTagger* tagger = new TreeAnalyseTagger(str2);
	tagger->Open();
	if(!tagger->IsOpen())
	{
		delete tagger;
		return;
	}
	tagger->SetCutTaggerTimePrompt(-6,  6);
	tagger->SetCutTaggerTimeRand1(-20, -8);
	tagger->SetCutTaggerTimeRand2(  8, 20);
	tagger->Analyse();
	tagger->Save();
	delete tagger;
	
	printf("Analysing Tagger 6G\n");
	sprintf(str2, "tree_%s_6g", str1);
	TreeAnalyseTagger* tagger = new TreeAnalyseTagger(str2);
	tagger->Open();
	if(!tagger->IsOpen())
	{
		delete tagger;
		return;
	}
	tagger->SetCutTaggerTimePrompt(-6,  6);
	tagger->SetCutTaggerTimeRand1(-20, -8);
	tagger->SetCutTaggerTimeRand2(  8, 20);
	tagger->Analyse();
	tagger->Save();
	delete tagger;
	
	printf("Analysing Tagger 10G\n");
	sprintf(str2, "tree_%s_10g", str1);
	TreeAnalyseTagger* tagger = new TreeAnalyseTagger(str2);
	tagger->Open();
	if(!tagger->IsOpen())
	{
		delete tagger;
		return;
	}
	tagger->SetCutTaggerTimePrompt(-6,  6);
	tagger->SetCutTaggerTimeRand1(-20, -8);
	tagger->SetCutTaggerTimeRand2(  8, 20);
	tagger->Analyse();
	tagger->Save();
	delete tagger;
	}
	
	{//2g IM
	printf("Analysing 2G IM\n");
	sprintf(str2, "tree_%s_2g_Prompt", str1);
	TreeAnalyse2Gamma* an2g = new TreeAnalyse2Gamma(str2);
	an2g->Open();
	if(!an2g->IsOpen())
	{
		delete an2g;
		return;
	}
	an2g->Analyse();
	an2g->Save();
	delete an2g;
	
	sprintf(str2, "tree_%s_2g_Rand1", str1);
	TreeAnalyse2Gamma* an2g = new TreeAnalyse2Gamma(str2);
	an2g->Open();
	if(!an2g->IsOpen())
	{
		delete an2g;
		return;
	}
	an2g->Analyse();
	an2g->Save();
	delete an2g;
	
	sprintf(str2, "tree_%s_2g_Rand2", str1);
	TreeAnalyse2Gamma* an2g = new TreeAnalyse2Gamma(str2);
	an2g->Open();
	if(!an2g->IsOpen())
	{
		delete an2g;
		return;
	}
	an2g->Analyse();
	an2g->Save();
	delete an2g;
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

void	SubstractBackground(TFile* file, const Char_t* HistName)
{
	TH1D*	hist[5];
	Char_t	histName[64];
	sprintf(histName, "Prompt_%s", HistName);
	hist[0]	= (TH1D*)file->Get(histName);
	sprintf(histName, "Rand1_%s", HistName);
	hist[1]	= (TH1D*)file->Get(histName);
	sprintf(histName, "Rand2_%s", HistName);
	hist[2]	= (TH1D*)file->Get(histName);
	sprintf(histName, "BG_%s", HistName);
	hist[3] = (TH1D*)hist[1]->Clone(histName);
	hist[3]->Add(hist[2]);
	hist[3]->Scale(0.5);
	sprintf(histName, "Result_%s", HistName);
	hist[4]	= (TH1D*)hist[0]->Clone(histName);
	hist[4]->Add(hist[3], -1);
	
	file->cd();
	hist[3]->Write();
	hist[4]->Write();
	file->Flush();
}
void	SubstractBackground(TFile* filePrompt, TFile* fileRand1, TFile* fileRand2, TFile* out, const Char_t* HistName)
{
	TH1D*	hist[5];
	Char_t	histName[64];
	hist[0]	= (TH1D*)filePrompt->Get(HistName);
	hist[1]	= (TH1D*)fileRand1->Get(HistName);
	hist[2]	= (TH1D*)fileRand2->Get(HistName);
	sprintf(histName, "BG_%s", HistName);
	hist[3] = (TH1D*)hist[1]->Clone(histName);
	hist[3]->Add(hist[2]);
	hist[3]->Scale(0.5);
	sprintf(histName, "Result_%s", HistName);
	hist[4]	= (TH1D*)hist[0]->Clone(histName);
	hist[4]->Add(hist[3], -1);
	
	out->cd();
	hist[3]->Write();
	hist[4]->Write();
	out->Flush();
}
void	SubstractBackground(const Char_t* FileName)
{
	TFile*	file	= TFile::Open(FileName, "UPDATE");
	if(!file)
		return;
	
	SubstractBackground(file, "TaggedEnergy");
	SubstractBackground(file, "TaggedTime");
	SubstractBackground(file, "CBEnergyAll");
	SubstractBackground(file, "IMAll");
	SubstractBackground(file, "ThetaAll");
	SubstractBackground(file, "PhiAll");
	
	file->Close();
}
void	SubstractBackground(const Char_t* FileNamePrompt, const Char_t* FileNameRand1, const Char_t* FileNameRand2)
{
	TFile*	filePrompt	= TFile::Open("hist_2g_Prompt_IM.root");
	if(!filePrompt)
		return;
	TFile*	fileRand1	= TFile::Open("hist_2g_Rand1_IM.root");
	if(!fileRand1)
		return;
	TFile*	fileRand2	= TFile::Open("hist_2g_Rand2_IM.root");
	if(!fileRand2)
		return;
	TFile*	out			= TFile::Open("hist_2g_IM.root", "RECREATE");
	
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_TaggedEnergy");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_TaggedTime");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_CBEnergyAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_IMAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_ThetaAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Pi0_PhiAll");

	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_TaggedEnergy");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_TaggedTime");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_CBEnergyAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_IMAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_ThetaAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "Eta_PhiAll");
	
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_TaggedEnergy");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_TaggedTime");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_CBEnergyAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_IMAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_ThetaAll");
	SubstractBackground(filePrompt, fileRand1, fileRand2, out, "EtaP_PhiAll");
		
	filePrompt->Close();
	fileRand1->Close();
	fileRand2->Close();
	out->Close();
}
void	SubstractBackground()
{
	SubstractBackground("hist_Tagger.root");
	SubstractBackground("hist_2g_Tagger.root");
	SubstractBackground("hist_6g_Tagger.root");
	SubstractBackground("hist_10g_Tagger.root");
	
	SubstractBackground("hist_2g_Prompt_IM.root", "hist_2g_Rand1_IM.root", "hist_2g_Rand2_IM.root");
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
		
		AddFiles(FolderName);
		SubstractBackground();
	}
}
