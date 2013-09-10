


void	CBTimePerRun(const Char_t* filePath)
{
	gSystem->Load("libCaLib.so");
	
	char *s = new char[1];
	TCanvas *c = new TCanvas("c","histograms"); 
    
    const Char_t* hName = "CaLib_CB_Time_Ind";
    
    // get number of runs
    Char_t oldDir[128];
    strcpy(oldDir, gSystem->WorkingDirectory());
    //gSystem->ChangeDirectory(filePath);
    
    Int_t		entries = 0;
    Double_t* 	RunNumbers;
    Double_t* 	CBtime;
    Double_t* 	CBtimeError;
    
    TH2D*	hist2d;
    TH1D*	hist;
    TF1*	fit;
    TFile*	file;
    
    TSystemDirectory dir(filePath, filePath);
	TList* files = dir.GetListOfFiles();
	if (files) 
	{
		RunNumbers		= new Double_t[files->GetEntries()];
		CBtime		= new Double_t[files->GetEntries()];
		CBtimeError	= new Double_t[files->GetEntries()];
		TSystemFile *sfile;
		TString fname;
		Int_t	fnumber;
		TIter next(files);
		int ccc	= 0;
		while ((sfile=(TSystemFile*)next())) 
		{
			fname = sfile->GetName();
			ccc++;
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
								if (fPi0Pos < -10 || fPi0Pos > 10) fPi0Pos = 0;
			
							
								fit->SetRange(fPi0Pos - 50, fPi0Pos + 50);
								//printf("%lf\t%lf\t",hist->GetMaximum(), fPi0Pos);
								fit->SetParameters(hist->GetMaximum(), fPi0Pos, 8, 1, 1, 0.1, 0.1);
								//fit->SetParLimits(0, hist->GetMaximum()-25, hist->GetMaximum()+25); 
								fit->SetParLimits(1, -10, 10);  
								fit->SetParLimits(2, 1, 15);  
								//fit->SetParLimits(3, 0, 100); 
				
								hist->Fit(fit, "RBQ0");
								
								RunNumbers[entries]	= fnumber;
								CBtime[entries]	= fit->GetParameter(1);
								CBtimeError[entries]	= fit->GetParError(1);
								entries++;
								
								//fit->Draw();
								//hist->Draw("same");
								//c->Update();
							
								//char* s;
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
	TGraphErrors*	graph = new TGraphErrors(entries, RunNumbers, CBtime, 0, CBtimeError);
	graph->Draw("AP");
	c->Update();
    
    gSystem->ChangeDirectory(oldDir);
}
