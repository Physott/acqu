#include "TA2GoAT.h"

ClassImp(TA2GoAT)

TA2GoAT::TA2GoAT(const char* Name, TA2Analysis* Analysis) : TA2AccessSQL(Name, Analysis),
															file(0),
                                                            treeRawEvent(0),
															treeTagger(0),
															treeTrigger(0),
                                                                    treeDetectorHits(0),
                                                                    treeScaler(0),
                                                                    nParticles(0),
                                                                    Ek(0),
																	Theta(0),
																	Phi(0),
                                                                    time(0),
                                                                    clusterSize(0),
                                                                    Apparatus(0),
                                                                    d_E(0),
                                                                    WC0_E(0),
                                                                    WC1_E(0),
                                                                    WC_Vertex_X(0),
                                                                    WC_Vertex_Y(0),
                                                                    WC_Vertex_Z(0),
                                                                    nTagged(0),
                                                                    photonbeam_E(0),
                                                                    tagged_ch(0),
                                                                    tagged_t(0),
                                                                    nNaI_Hits(0),
                                                                    NaI_Hits(0),
                                                                    nPID_Hits(0),
                                                                    PID_Hits(0),
                                                                    nWC_Hits(0),
																	WC_Hits(0),
																	nBaF2_PbWO4_Hits(0),
																	BaF2_PbWO4_Hits(0),
                                                                    nVeto_Hits(0),
                                                                    Veto_Hits(0),
                                                                    ESum(0),
                                                                    Mult(0),
                                                                    eventNumber(0),
                                                                    eventID(0)														
{
    	strcpy(outputFolder,"~");
    	strcpy(fileName,"RootTree");

    	AddCmdList(RootTreeConfigKeys);
}


TA2GoAT::~TA2GoAT()
{
	if(treeRawEvent)
		delete treeRawEvent;
	if(treeTagger)
		delete treeTagger;
	if(treeTrigger)
		delete treeTrigger;
	if(treeDetectorHits)
		delete treeDetectorHits;
	if(treeScaler)
		delete treeScaler;
    if(file)
		delete file;
}

void    TA2GoAT::LoadVariable()
{
	// Including histogram output for testing purposes (quick check of variables)
   	TA2AccessSQL::LoadVariable();

	TA2DataManager::LoadVariable("nParticles", 	&nParticles,EISingleX);
	TA2DataManager::LoadVariable("Ek", 			Ek,			EDMultiX);
	TA2DataManager::LoadVariable("Theta", 		Theta,		EDMultiX);   
	TA2DataManager::LoadVariable("Phi", 		Phi,		EDMultiX);     	 	
	TA2DataManager::LoadVariable("time", 		time,		EDMultiX);

	TA2DataManager::LoadVariable("nTagged", 	&nTagged,	EISingleX);
	TA2DataManager::LoadVariable("photonbeam_E",photonbeam_E,EDMultiX);
	TA2DataManager::LoadVariable("taggedCh", 	tagged_ch,	EIMultiX);
	TA2DataManager::LoadVariable("taggedT", 	tagged_t,	EDMultiX);    

	TA2DataManager::LoadVariable("dE", 			d_E,		EDMultiX);
	TA2DataManager::LoadVariable("WC0E", 		WC0_E,		EDMultiX);   
	TA2DataManager::LoadVariable("WC1E", 		WC1_E,		EDMultiX);

	TA2DataManager::LoadVariable("ESum",		&ESum, 		EDSingleX);
 
    return;
    
}

void    TA2GoAT::SetConfig(Char_t* line, Int_t key)
{
    	switch(key)
    	{
    	case EG_OUTPUT_FOLDER:
     	   strcpy(outputFolder,line);
     	   while(outputFolder[strlen(outputFolder)-1]=='\n' 
						|| outputFolder[strlen(outputFolder)-1]=='\r')
			outputFolder[strlen(outputFolder)-1]='\0';
        return;
    	case EG_FILE_NAME:
        	strcpy(fileName,line);
        	while(fileName[strlen(fileName)-1]=='\n' 
						|| fileName[strlen(fileName)-1]=='\r')
			fileName[strlen(fileName)-1]='\0';
        return;
    	default:
        	TA2AccessSQL::SetConfig(line, key);
    	}
}

void    TA2GoAT::PostInit()
{

   	Ek		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	Theta		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	Phi		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	time		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	clusterSize = new UChar_t[TA2GoAT_MAX_PARTICLE];
    
   	photonbeam_E= new Double_t[TA2GoAT_MAX_TAGGER];
   	tagged_ch	= new Int_t[TA2GoAT_MAX_TAGGER];
   	tagged_t	= new Double_t[TA2GoAT_MAX_TAGGER];
    
   	Apparatus	= new UChar_t[TA2GoAT_MAX_PARTICLE];
   	d_E			= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC0_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC1_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
    
   	WC_Vertex_X 	= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC_Vertex_Y 	= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC_Vertex_Z 	= new Double_t[TA2GoAT_MAX_PARTICLE];
    
   	NaI_Hits	= new Int_t[TA2GoAT_MAX_HITS];  
   	PID_Hits	= new Int_t[TA2GoAT_MAX_HITS];
   	WC_Hits		= new Int_t[TA2GoAT_MAX_HITS];
   	BaF2_PbWO4_Hits	= new Int_t[TA2GoAT_MAX_HITS];
   	Veto_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    
   	printf("---------\n");
   	printf("Init Tree\n");
   	printf("---------\n");
    
   	// Append input filename to output tree name.
   	TString fullName;
   	if(gAR->GetProcessType() == EMCProcess) fullName = gAR->GetTreeFileList(0);        
   	else  fullName = gAR->GetFileName();
		
	int length = fullName.Length();
	int last = 0;
	for (int i = 0; i < length; i++)
	{
		int index = fullName.Index("/"); 
		fullName.Remove(0,index+1);	
		if (index == -1) break;
		last += index+1; 
	}
	Char_t inFile[256], str[256];
    	if(gAR->GetProcessType() == EMCProcess) 
		sscanf( gAR->GetTreeFileList(0)+last, "%[^.].root\n", inFile);       
    	else    
		sscanf( gAR->GetFileName()+last, "%[^.].dat\n", inFile);	

    	sprintf(str, "%s/%s_%s.root", outputFolder, fileName, inFile);        
   	printf("Root file saved to %s", str);  

   	file		= new TFile(str,"RECREATE");
	treeRawEvent	= new TTree("treeRawEvent", "treeRawEvent");
	treeTagger	= new TTree("treeTagger","treeTagger");
	treeTrigger	= new TTree("treeTrigger","treeTrigger");
	treeDetectorHits = new TTree("treeDetectorHits", "treeDetectorHits");
	treeScaler	= new TTree("treeScaler", "treeScaler");
	
	treeRawEvent->Branch("nParticles",&nParticles,"nParticles/I");
	treeRawEvent->Branch("Ek",  Ek,  "Ek[nParticles]/D");	
	treeRawEvent->Branch("Theta",  Theta,  "Theta[nParticles]/D");	
	treeRawEvent->Branch("Phi",  Phi,  "Phi[nParticles]/D");	
	treeRawEvent->Branch("time", time, "time[nParticles]/D");
	treeRawEvent->Branch("clusterSize", clusterSize, "clusterSize[nParticles]/b");
	treeRawEvent->Branch("Apparatus", Apparatus, "Apparatus[nParticles]/b");
	treeRawEvent->Branch("d_E", d_E, "d_E[nParticles]/D");	
	treeRawEvent->Branch("WC0_E", WC0_E, "WC0_E[nParticles]/D");	
	treeRawEvent->Branch("WC1_E", WC1_E, "WC1_E[nParticles]/D");
	treeRawEvent->Branch("WC_Vertex_X", WC_Vertex_X, "WC_Vertex_X[nParticles]/D");	
	treeRawEvent->Branch("WC_Vertex_Y", WC_Vertex_Y, "WC_Vertex_Y[nParticles]/D");	
	treeRawEvent->Branch("WC_Vertex_Z", WC_Vertex_Z, "WC_Vertex_Z[nParticles]/D");
	
	treeTagger->Branch("nTagged", &nTagged,"nTagged/I");
	treeTagger->Branch("photonbeam_E", photonbeam_E, "photonbeam_E[nTagged]/D");
	treeTagger->Branch("tagged_ch", tagged_ch, "tagged_ch[nTagged]/I");
	treeTagger->Branch("tagged_t", tagged_t, "tagged_t[nTagged]/D");

	treeTrigger->Branch("ESum", &ESum, "ESum/D");
	treeTrigger->Branch("Mult", &Mult, "Mult/I");

	treeDetectorHits->Branch("nNaI_Hits", &nNaI_Hits, "nNaI_Hits/I");
	treeDetectorHits->Branch("NaI_Hits", NaI_Hits, "NaI_Hits[nNaI_Hits]/I");
	treeDetectorHits->Branch("nPID_Hits", &nPID_Hits, "nPID_Hits/I");
	treeDetectorHits->Branch("PID_Hits", PID_Hits, "PID_Hits[nPID_Hits]/I");
	treeDetectorHits->Branch("nWC_Hits", &nWC_Hits, "nWC_Hits/I");
	treeDetectorHits->Branch("WC_Hits", WC_Hits, "WC_Hits[nWC_Hits]/I");	
	treeDetectorHits->Branch("nBaF2_PbWO4_Hits", &nBaF2_PbWO4_Hits, "nBaF2_PbWO4_Hits/I");
	treeDetectorHits->Branch("BaF2_PbWO4_Hits", BaF2_PbWO4_Hits, "BaF2_PbWO4_Hits[nBaF2_PbWO4_Hits]/I");
	treeDetectorHits->Branch("nVeto_Hits", &nVeto_Hits, "nVeto_Hits/I");
	treeDetectorHits->Branch("Veto_Hits", Veto_Hits, "Veto_Hits[nVeto_Hits]/I");
	
	treeScaler->Branch("eventNumber", &eventNumber, "eventNumber/I");
	treeScaler->Branch("eventID", &eventID, "eventID/I");
	printf("GetMaxScaler: %d\n", GetMaxScaler());
	sprintf(str, "Scaler[%d]/i", GetMaxScaler());
	treeScaler->Branch("Scaler", fScaler, str);
	
	gROOT->cd();
	
	eventNumber	= 0;
	
	// Default SQL-physics initialisation
    	TA2AccessSQL::PostInit();	

}

void    TA2GoAT::Reconstruct()
{

	// Output scaler info on scaler read events
	if(gAR->IsScalerRead())
	{
		eventID	= gAN->GetNDAQEvent();
		treeScaler->Fill();		
	}

	if(fTagger && fLadder)
	{
        // Get the conversion of tagger channel to photon energy
		Double_t electron_E = fTagger->GetBeamEnergy();
        const Double_t* ChToE = fLadder->GetECalibration();		
		
		// Collect Tagger M0 Hits
		nTagged	= fLadder->GetNhits();
		for(int i=0; i<nTagged; i++)
		{
			tagged_ch[i]	= fLadder->GetHits(i);
			tagged_t[i]		= (fLadder->GetTimeOR())[i];
			photonbeam_E[i] = electron_E - ChToE[tagged_ch[i]];
		}
	
		// Collect Tagger M+ Hits
		for(int m=1; m<fLadder->GetNMultihit(); m++)
		{
			for(int i=0; i<fLadder->GetNhitsM(m); i++)
			{
				tagged_ch[nTagged+i] 	= (fLadder->GetHitsM(m))[i];
				tagged_t[nTagged+i]	 	= (fLadder->GetTimeORM(m))[i];
				photonbeam_E[nTagged+i] = electron_E - ChToE[tagged_ch[nTagged+i]];
			}
			nTagged	+= fLadder->GetNhitsM(m);
		}
	}
	else nTagged = 0;
	
	// Gather particle information
	nParticles = 0;
	if(fCB)
	{
		// Collect CB Hits
    	nParticles	= fCB->GetNParticle();      
		for(int i=0; i<nParticles; i++)
		{
			TA2Particle part = fCB->GetParticles(i);
			
			part.SetParticleID(kRootino); // Set mass to 0 (rootino)
			part.SetMass(0.0);
			
			Apparatus[i]	= (UChar_t)EAppCB;			
			Ek[i]			= part.GetT();
			Theta[i]		= part.GetThetaDg();
			Phi[i]			= part.GetPhiDg();			
			time[i]			= part.GetTime();	
			clusterSize[i]  = (UChar_t)part.GetClusterSize();
			d_E[i]			= part.GetVetoEnergy();
			WC0_E[i]		= part.GetEnergyMwpc0();
		 	WC1_E[i]		= part.GetEnergyMwpc1();
		 	WC_Vertex_X[i]  = part.GetPsVertex().X();
	 		WC_Vertex_Y[i]  = part.GetPsVertex().Y();
		 	WC_Vertex_Z[i]  = part.GetPsVertex().Z();
		}
	}

	if(fTAPS)
	{
		// Collect TAPS Hits
		for(int i=0; i<fTAPS->GetNParticle(); i++)
		{
			TA2Particle part = fTAPS->GetParticles(i);
			
			part.SetParticleID(kRootino); // Set mass to 0 (rootino)
			part.SetMass(0.0);				
			
			Apparatus[nParticles+i]		= (UChar_t)EAppTAPS;		
			Ek[nParticles+i]			= part.GetT();	
			Theta[nParticles+i]			= part.GetThetaDg();
			Phi[nParticles+i]			= part.GetPhiDg();					
			time[nParticles+i]			= part.GetTime();
			clusterSize[nParticles+i]  	= (UChar_t)part.GetClusterSize();
			d_E[nParticles+i]			= part.GetVetoEnergy();
			WC0_E[nParticles+i]			= ENullFloat; 
	 		WC1_E[nParticles+i]    		= ENullFloat; 
		 	WC_Vertex_X[nParticles+i]  	= ENullFloat; 
		 	WC_Vertex_Y[nParticles+i]  	= ENullFloat; 
	 		WC_Vertex_Z[nParticles+i]  	= ENullFloat; 
		}
		nParticles += fTAPS->GetNParticle(); // update number of particles
	}

	// Get Detector Hits
	if(fNaI)
	{
		nNaI_Hits = fNaI->GetNhits();
		for(int i=0; i<nNaI_Hits; i++)   
			{ NaI_Hits[i] = fNaI->GetHits(i); }
	}

	if(fPID)
	{
		nPID_Hits = fPID->GetNhits();
		for(int i=0; i<nPID_Hits; i++)   
			{ PID_Hits[i] = fPID->GetHits(i); }
	}

	if(fMWPC)
	{
		nWC_Hits = fMWPC->GetNhits();
		for(int i=0; i<nWC_Hits; i++)    
			{ WC_Hits[i] = fMWPC->GetHits(i); }
	}

	if(fBaF2PWO)
	{
		nBaF2_PbWO4_Hits = fBaF2PWO->GetNhits();
		for(int i=0; i<nBaF2_PbWO4_Hits; i++)
			{ BaF2_PbWO4_Hits[i] = fBaF2PWO->GetHits(i); }
	}

	if(fVeto)
	{
		nVeto_Hits = fVeto->GetNhits();
		for(int i=0; i<nVeto_Hits; i++) 
			{ Veto_Hits[i] = fVeto->GetHits(i);}
	}
	
	// Get Trigger information
	if(fNaI) ESum = fNaI->GetTotalEnergy();
	if(gAR->GetProcessType() == EMCProcess) MultiplicityMC();
	else MultiplicityHW();

	//Apply EndBuffer
    	Ek[nParticles] 		= EBufferEnd;
    	Theta[nParticles]	= EBufferEnd;
    	Phi[nParticles]		= EBufferEnd;
    	time[nParticles] 	= EBufferEnd;
    	WC0_E[nParticles] 	= EBufferEnd;
    	WC1_E[nParticles] 	= EBufferEnd;
    	WC_Vertex_X[nParticles] = EBufferEnd;  
    	WC_Vertex_Y[nParticles] = EBufferEnd;    
	WC_Vertex_Z[nParticles] = EBufferEnd;    
	d_E[nParticles] 	= EBufferEnd;    
    tagged_ch[nTagged] 	= EBufferEnd;
    tagged_t[nTagged] 	= EBufferEnd;	
	
	//Fill Trees
	treeRawEvent->Fill();
	treeTagger->Fill();
	treeTrigger->Fill();
	treeDetectorHits->Fill();

	//increment event number
	eventNumber++;	
}

void    TA2GoAT::Finish()
{
	printf("------------------\n");
	printf("Write Tree to file\n");
	printf("------------------\n");
	
	file->cd();
	
	if(treeRawEvent) 
	{
		treeRawEvent->Write();	// Write	
		delete treeRawEvent; 	// Close and delete in memory
	}
	if(treeTagger) 
	{
		treeTagger->Write();	// Write	
		delete treeTagger; 	// Close and delete in memory
	}	
	if(treeTrigger) 
	{
		treeTrigger->Write();	// Write	
		delete treeTrigger; 	// Close and delete in memory
	}		
	if(treeDetectorHits) 
	{
		treeDetectorHits->Write();// Write	
		delete treeDetectorHits;  // Close and delete in memory
	}		
	if(treeScaler) 
	{
		treeScaler->Write();	// Write	
		delete treeScaler; 	// Close and delete in memory
    	}
    	if(file) 
		delete file;		// Close and delete in memory

	
	TA2AccessSQL::Finish();
}

void    TA2GoAT::ParseMisc(char* line)
{
	TA2AccessSQL::ParseMisc(line);
}

void 	TA2GoAT::MultiplicityMC() 
{
	// really rough, just the basic idea 
	// A good example is done in TA2BasePhysics but requires some extra work
	// Also need some flag for new and old 
	// Set some basic discriminator thresh for now
	Double_t DiscTh = 5.0; 
	Mult = 0;
		
	if(fNaI) 
	{
		for (Int_t i = 0; i < 45; i++) 
		{ 
			Bool_t flag = kFALSE;
			for (Int_t j = 0; j < 16; j++) 
			{
				if ((fNaI->GetEnergyAll(i*16 + j)) >= DiscTh) flag = kTRUE;
			}
			if (flag == kTRUE) Mult++;
		}
	}
		
	if (fTAPS) {
		for (Int_t i = 0; i < 6; i++) { 
			Bool_t flag = kFALSE;
			for (Int_t j = 12; j < 71; j++) {  
			// really add some check of how many crystals are used, skip PbWO4s
				if ((fBaF2PWO->GetEnergyAll(i*71 + j)) >= DiscTh) flag = kTRUE;
			}
			if (flag == kTRUE) Mult++; 		
		}
	}
		
	if (Mult > 4) Mult = 4;
}
	
void 	TA2GoAT::MultiplicityHW() 
{
	Mult = 0;
	
	if(fADC) {L2Pattern = fADC[1] & 0xFF;} 
	if (L2Pattern & 0x10) 	Mult++; // Doesn't work
	if (L2Pattern & 0x20) 	Mult++;
	if (L2Pattern & 0x40) 	Mult++;
 	if (L2Pattern & 0x80) 	Mult++;
 	
}

