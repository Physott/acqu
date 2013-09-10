


void	CBEnergyPerRun(const Char_t* filePath)
{
	gSystem->Load("libCaLib.so");
	
	char *s = new char[1];
	TCanvas* c = (TCanvas*)gROOT->Get("c");
	if(!c)
		c	= new TCanvas("c","histograms", 1000, 800); 
    
    //const Char_t* hName = "CaLib_CB_IM_Neut";
    const Char_t* hName = "CaLib_CB_IM_2Neut1Char";
    
    // get number of runs
    Char_t oldDir[128];
    strcpy(oldDir, gSystem->WorkingDirectory());
    //gSystem->ChangeDirectory(filePath);
    
    Int_t		entries = 0;
    Double_t* 	RunNumbers;
    Double_t* 	Pi0Masses;
    Double_t* 	Pi0MassesError;
    
    TH2D*	hist2d;
    TH1D*	hist;
    TF1*	fit;
    TFile*	file;
    
    TSystemDirectory dir(filePath, filePath);
	TList* files = dir.GetListOfFiles();
	if (files) 
	{
		RunNumbers		= new Double_t[files->GetEntries()];
		Pi0Masses		= new Double_t[files->GetEntries()];
		Pi0MassesError	= new Double_t[files->GetEntries()];
		TSystemFile *sfile;
		TString fname;
		Int_t	fnumber;
		TIter next(files);
		int	ccc = 0;
		while(sfile=(TSystemFile*)next())
		{
			fname = sfile->GetName();
			ccc++;
			//if(ccc > 10)
				//break;
			printf("%d\t%s\t", ccc, sfile->GetName());
			if (!sfile->IsDirectory() && fname.EndsWith(".root")) 
			{
				fname = strtok(fname.Data(),"abcdefghijklmnopqrstuvwxyzäöü_.,-ABCDEFGHIJKLMNOPQRSTUVWXYZÄÖÜ");
				fnumber	= fname.Atoi();
				fname = filePath;
				fname += "/";
				fname += sfile->GetName();
				
				if(file) delete file;
				file = new TFile(fname);
				if(file)
				{
					hist2d = (TH2D*)file->Get(hName);
					if(hist2d)
					{
						hist = (TH1D*) hist2d->ProjectionX("tmp", 1, -1, "e");
						if(hist)
						{
							printf("%lf\n", hist->GetEntries());
							fit = new TF1("fit", "gaus(0)+pol3(3)");
							if(fit)
							{
								fit->SetLineColor(2);
								
								// estimate peak position
								Double_t fPi0Pos = hist->GetBinCenter(hist->GetMaximumBin());
								if (fPi0Pos < 120 || fPi0Pos > 150) fPi0Pos = 135;
			
							
								fit->SetRange(fPi0Pos - 50, fPi0Pos + 50);
								//printf("%lf\t%lf\t",hist->GetMaximum(), fPi0Pos);
								fit->SetParameters(hist->GetMaximum(), fPi0Pos, 8, 1, 1, 0.1, 0.1);
								fit->SetParLimits(0, hist->GetMaximum()-25, hist->GetMaximum()+25); 
								fit->SetParLimits(1, 130, 140);  
								fit->SetParLimits(2, 3, 15);  
								fit->SetParLimits(3, 0, 100); 
				
								hist->Fit(fit, "RBQ0");
								
								RunNumbers[entries]	= fnumber;
								Pi0Masses[entries]	= fit->GetParameter(1);
								Pi0MassesError[entries]	= fit->GetParError(1);
								entries++;
								
								//fit->Draw();
								//hist->Draw("same");
								//c->Update();
							
								//gets(s);
							
								delete fit;
							}							
							delete hist;
						}
						delete hist2d;
					}
				}
			}
		}
	}

	printf("entries: %d\n", entries);
	TGraphErrors*	graph1 = new TGraphErrors(entries, RunNumbers, Pi0Masses, 0, Pi0MassesError);
	c->Divide(1,2,0.001,0.001);
	c->cd(1);
	graph1->Draw("AP");
	c->Update();
	
	char	str[128];
	strcpy(str, filePath);
	strcat(str, "/outputFile.txt");
	FILE* outputFile	= fopen(str, "w");
	
	Double_t* 	Correction		= new Double_t[entries];
	Double_t* 	CorrectionError	= new Double_t[entries];
	
	for(int i=0; i<entries; i++)
	{
		Correction[i]		= 134.9766/Pi0Masses[i];
		CorrectionError[i]	= (Pi0MassesError[i]*134.9766)/(Pi0Masses[i]*Pi0Masses[i]);
		fprintf(outputFile, "%d \t %lf \t %lf \t %lf \t %lf\n", RunNumbers[i], Correction[i], CorrectionError[i], Pi0Masses[i], Pi0MassesError[i]);
	}
	
	fflush(outputFile);
	fclose(outputFile);
	
	TGraphErrors*	graph2 = new TGraphErrors(entries, RunNumbers, Correction, 0, CorrectionError);
	c->cd(2);
	graph2->Draw("AP");
	c->Update();
}
