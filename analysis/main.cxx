

#include <stdio.h>

#include "ReadRootTree.h"

#include "TSystemDirectory.h"
//#include "TLorentzVector.h"


int	main( int argc, const char* argv[] )
{
	//printf("OK\n");
	
	ReadRootTree*		c = new ReadRootTree("TTreeOutput_41948_CUT_2_6_10.root", "TTreeOutput_41948_CUT_2_6_10");
	
	//c->SetCutCBTime(-13, 40);
	//c->SetCutTaggerTime(-5,5,-18,-8,8,18);
	//c->SetCutMissMass(738,1138);
	c->Analyse(0,50001);
	c->Draw();
	//c->Save("test.root");
	
}



/*void	AnalyseFolder(const char* FolderName)
{
	TSystemDirectory dir(FolderName, FolderName);
	TList* files = dir.GetListOfFiles();
	if (files) 
	{
		TSystemFile *sfile;
		TString fname;
		TString tname;
		TString	outputFileName;
		TIter next(files);
		int	ccc=0;
		while((sfile=(TSystemFile*)next()))
		{
			fname = sfile->GetName();
			ccc++;
			printf("%d\t%s\t", ccc, fname.Data());
			if (!sfile->IsDirectory() && fname.EndsWith(".root")) 
			{
				tname = fname.Strip(TString::kTrailing,'t');
				tname.Remove(TString::kTrailing,'o');
				tname.Remove(TString::kTrailing,'o');
				tname.Remove(TString::kTrailing,'r');
				tname.Remove(TString::kTrailing,'.');
				printf("%s\n", tname.Data());
				
				AnalysisEtaP6Gamma*		c = new AnalysisEtaP6Gamma(fname, tname);
				if(c->IsOpened())
				{
					c->SetCutTaggerTime(-4,4,-14,-6,6,14);
					c->Analyse(0,-1);
					outputFileName = tname;
					outputFileName.Prepend("result_");
					outputFileName.Append(".root");
					c->Save(outputFileName);
				}
				delete c;
			}
		}
	}
}*/
