#include "TreeAnalyseTagger.C"
#include "TreeAnalyse2Gamma.C"
#include "TreeAnalyse6Gamma.C"
#include "TreeAnalyse2GammaMissMass.C"
#include "TreeAnalyse6GammaMissMass.C"



void	AddFiles(const Char_t* FolderName)
{
	Char_t	str1[256];
	sprintf(str1, "hadd -f %s/result_Tagger.root %s/hist_*_Tagger.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_2g_IM.root %s/hist_*_2g_IM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_2g_IMPi0_MM.root %s/hist_*_2g_IMPi0_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_2g_IMEta_MM.root %s/hist_*_2g_IMEta_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_2g_IMEtaP_MM.root %s/hist_*_2g_IMEtaP_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_2gProton_IMPi0_MM.root %s/hist_*_2gProton_IMPi0_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_2gProton_IMEta_MM.root %s/hist_*_2gProton_IMEta_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_2gProton_IMEtaP_MM.root %s/hist_*_2gProton_IMEtaP_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_6g_IM.root %s/hist_*_6g_IM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_6g_IM3Pi0_MM.root %s/hist_*_6g_IM3Pi0_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_6g_IMEtaP_MM.root %s/hist_*_6g_IMEtaP_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	
	sprintf(str1, "hadd -f %s/result_6gProton_IM3Pi0_MM.root %s/hist_*_6gProton_IM3Pi0_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
	sprintf(str1, "hadd -f %s/result_6gProton_IMEtaP_MM.root %s/hist_*_6gProton_IMEtaP_MM.root", FolderName, FolderName);
	gSystem->Exec(str1);
}


void	AnalyseFolder(const Char_t* FolderName)
{
	TreeAnalyseTagger::AnalyseFolder(FolderName);
	TreeAnalyse2Gamma::AnalyseFolder(FolderName);
	TreeAnalyse6Gamma::AnalyseFolder(FolderName);
	TreeAnalyse2GammaMissMass::AnalyseFolder(FolderName);
	TreeAnalyse6GammaMissMass::AnalyseFolder(FolderName);
}
