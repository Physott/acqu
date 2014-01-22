//*************************************************************************
//* Author: Patrik Ott
//*************************************************************************

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2GoAT                                                              //
//                                                                      //
// This class is organising the output to ROOT files                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2GoAT.h"

ClassImp(TA2GoAT)





TA2GoAT::TA2GoAT(const char* Name, TA2Analysis* Analysis) : TA2AccessSQL(Name, Analysis),
                                                                    file(0),
                                                                    treeEvent(0),
                                                                    treeScaler(0),
                                                                    nParticles(0),
                                                                    Px(0),
                                                                    Py(0),
                                                                    Pz(0),
                                                                    E(0),
                                                                    time(0),
                                                                    clusterSize(0),
                                                                    nTagged(0),
                                                                    tagged_ch(0),
                                                                    tagged_t(0),
                                                                    Apparatus(0),
                                                                    d_E(0),
                                                                    WC1_E(0),
                                                                    WC2_E(0),
                                                                    WC_Vertex_X(0),
                                                                    WC_Vertex_Y(0),
                                                                    WC_Vertex_Z(0),
                                                                    nNaI_Hits(0),
                                                                    NaI_Hits(0),
                                                                    nBaF2_Hits(0),
                                                                    BaF2_Hits(0),
                                                                    nPbWO4_Hits(0),
                                                                    PbWO4_Hits(0),
                                                                    nPID_Hits(0),
                                                                    PID_Hits(0),
                                                                    nVeto_Hits(0),
                                                                    Veto_Hits(0),
                                                                    nWC1_Hits(0),
                                                                    WC1_Hits(0),
                                                                    nWC2_Hits(0),
                                                                    WC2_Hits(0),
                                                                    ESum(0),
                                                                    CBMult(0),
                                                                    TAPSMult(0),
                                                                    eventNumber(0),
                                                                    eventID(0)
{
    strcpy(outputFolder,"~");
    strcpy(fileName,"RootTree");

    AddCmdList(RootTreeConfigKeys);
}


TA2GoAT::~TA2GoAT()
{
	if(treeEvent)
		delete treeEvent;
	if(treeScaler)
		delete treeScaler;
    if(file)
		delete file;
}


void    TA2GoAT::LoadVariable()
{
    TA2AccessSQL::LoadVariable();
}


void    TA2GoAT::SetConfig(Char_t* line, Int_t key)
{
    switch(key)
    {
    case EG_OUTPUT_FOLDER:
        strcpy(outputFolder,line);
        while(outputFolder[strlen(outputFolder)-1]=='\n' || outputFolder[strlen(outputFolder)-1]=='\r')
			outputFolder[strlen(outputFolder)-1]='\0';
        return;
    case EG_FILE_NAME:
        strcpy(fileName,line);
        while(fileName[strlen(fileName)-1]=='\n' || fileName[strlen(fileName)-1]=='\r')
			fileName[strlen(fileName)-1]='\0';
        return;
    default:
        TA2AccessSQL::SetConfig(line, key);
    }
}

void    TA2GoAT::PostInit()
{
    TA2AccessSQL::PostInit();
    
    Px			= new Double_t[TA2GoAT_MAX_PARTICLE];
    Py			= new Double_t[TA2GoAT_MAX_PARTICLE];
    Pz			= new Double_t[TA2GoAT_MAX_PARTICLE];
    E			= new Double_t[TA2GoAT_MAX_PARTICLE];
    time		= new Double_t[TA2GoAT_MAX_PARTICLE];
    clusterSize	= new UChar_t[TA2GoAT_MAX_PARTICLE];
    
    tagged_ch	= new Int_t[TA2GoAT_MAX_TAGGER];
    tagged_t	= new Double_t[TA2GoAT_MAX_TAGGER];
    
    Apparatus	= new UChar_t[TA2GoAT_MAX_PARTICLE];
    d_E			= new Double_t[TA2GoAT_MAX_PARTICLE];
    WC1_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
    WC2_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
    
    NaI_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    BaF2_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    PbWO4_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    PID_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    Veto_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    WC1_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    WC2_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    
    printf("---------\n");
    printf("Init Tree\n");
    printf("---------\n");
    
    Char_t	str[256];
    sprintf(str, "%s/%s_%d.root", outputFolder, fileName, GetRunNumber());
    file		= new TFile(str,"RECREATE");
	treeEvent	= new TTree("treeEvent", "treeEvent");
	treeScaler	= new TTree("treeScaler", "treeScaler");
	
	treeEvent->Branch("nParticles",&nParticles,"nParticles/I");
	treeEvent->Branch("Px", Px, "Px[nParticles]/D");
	treeEvent->Branch("Py", Py, "Py[nParticles]/D");
	treeEvent->Branch("Pz", Pz, "Pz[nParticles]/D");
	treeEvent->Branch("E",  E,  "E[nParticles]/D");	
	treeEvent->Branch("time", time, "time[nParticles]/D");
	treeEvent->Branch("clusterSize", clusterSize, "clusterSize[nParticles]/D");
    
	treeEvent->Branch("nTagged", &nTagged,"nTagged/I");
	treeEvent->Branch("tagged_ch", tagged_ch, "tagged_ch[nTagged]/I");
	treeEvent->Branch("tagged_t", tagged_t, "tagged_t[nTagged]/D");
    
	treeEvent->Branch("Apparatus", Apparatus, "Apparatus[nParticles]/b");
	treeEvent->Branch("d_E", d_E, "d_E[nParticles]/D");	
	treeEvent->Branch("WC1_E", WC1_E, "WC1_E[nParticles]/D");	
	treeEvent->Branch("WC2_E", WC2_E, "WC2_E[nParticles]/D");
	treeEvent->Branch("WC_Vertex_X", WC_Vertex_X, "WC_Vertex_X[nParticles]/D");	
	treeEvent->Branch("WC_Vertex_Y", WC_Vertex_Y, "WC_Vertex_Y[nParticles]/D");	
	treeEvent->Branch("WC_Vertex_Z", WC_Vertex_Z, "WC_Vertex_Z[nParticles]/D");	
	
	treeEvent->Branch("nNaI_Hits", &nNaI_Hits, "nNaI_Hits/I");
	treeEvent->Branch("NaI_Hits", NaI_Hits, "NaI_Hits[nNaI_Hits]/I");
	treeEvent->Branch("nBaF2_Hits", &nBaF2_Hits, "nBaF2_Hits/I");
	treeEvent->Branch("BaF2_Hits", BaF2_Hits, "BaF2_Hits[nBaF2_Hits]/I");
	treeEvent->Branch("nPbWO4_Hits", &nPbWO4_Hits, "nPbWO4_Hits/I");
	treeEvent->Branch("PbWO4_Hits", PbWO4_Hits, "PbWO4_Hits[nPbWO4_Hits]/I");
	treeEvent->Branch("nPID_Hits", &nPID_Hits, "nPID_Hits/I");
	treeEvent->Branch("PID_Hits", PID_Hits, "PID_Hits[nPID_Hits]/I");
	treeEvent->Branch("nVeto_Hits", &nVeto_Hits, "nVeto_Hits/I");
	treeEvent->Branch("Veto_Hits", Veto_Hits, "Veto_Hits[nVeto_Hits]/I");
	treeEvent->Branch("nWC1_Hits", &nWC1_Hits, "nWC1_Hits/I");
	treeEvent->Branch("WC1_Hits", WC1_Hits, "WC1_Hits[nWC1_Hits]/I");
	treeEvent->Branch("nWC2_Hits", &nWC2_Hits, "nWC2_Hits/I");
	treeEvent->Branch("WC2_Hits", WC2_Hits, "WC2_Hits[nWC2_Hits]/I");
	
	treeEvent->Branch("ESum", &ESum, "ESum/D");
	treeEvent->Branch("CBMult", &CBMult, "CBMult/I");
	treeEvent->Branch("TAPSMult", &TAPSMult, "TAPSMult/I");
	
	treeScaler->Branch("eventNumber", &eventNumber, "eventNumber/I");
	treeScaler->Branch("eventID", &eventID, "eventID/I");
	sprintf(str, "Scaler[%d]/I", gAR->GetMaxScaler());
	treeScaler->Branch("Scaler", gAR->GetScaler(), str);
	
	eventNumber	= 0;
}


void    TA2GoAT::Reconstruct()
{
	//Is Scaler Read
	if(gAR->IsScalerRead())
	{
		eventID	= gAN->GetNDAQEvent();
		
		treeScaler->Fill();		
		return;
	}
	
	// Collect Tagger M0 Hits
	nTagged	= fLadder->GetNhits();
	for(int i=0; i<nTagged; i++)
	{
		tagged_ch[i]	= fLadder->GetHits(i);
		tagged_t[i]		= (fLadder->GetTime())[i];
	}
	
	// Collect Tagger M0 Hits
	for(int m=1; m<fLadder->GetNMultihit(); m++)
	{
		for(int i=0; i<fLadder->GetNhitsM(m); i++)
		{
			tagged_ch[nTagged+i]	= (fLadder->GetHitsM(m))[i];
			tagged_t[nTagged+i]		= (fLadder->GetTimeM()[m])[i];
		}
		nTagged	+= fLadder->GetNhitsM(m);
	}
	
	// Collect CB Hits
    nParticles	= fCB->GetNParticle();      
	for(int i=0; i<nParticles; i++)
	{
		Px[i]			= fCB->GetParticles(i).GetPx();
		Py[i]			= fCB->GetParticles(i).GetPy();
		Pz[i]			= fCB->GetParticles(i).GetPz();
		E[i]			= fCB->GetParticles(i).GetE();
		time[i]			= fCB->GetParticles(i).GetTime();
		
//		clusterSize[i]  = // Will be included

//		Apparatus 		= // Mark as CB
//    	d_E[i]			= // PID Energy		
//   	WC1_E[i]		= // Will be included
//	 	WC2_E[i]    	= // Will be included
//	 	WC_Vertex_X[i]  = // Will be included
//	 	WC_Vertex_Y[i]  = // Will be included
//	 	WC_Vertex_Z[i]  = // Will be included

	}
	// Collect TAPS Hits
	for(int i=0; i<fTAPS->GetNParticle(); i++)
	{
		Px[nParticles+i]		= fTAPS->GetParticles(i).GetPx();
		Py[nParticles+i]		= fTAPS->GetParticles(i).GetPy();
		Pz[nParticles+i]		= fTAPS->GetParticles(i).GetPz();
		E[nParticles+i]			= fTAPS->GetParticles(i).GetE();
		time[nParticles+i]		= fTAPS->GetParticles(i).GetTime();

//		clusterSize[nParticles+i]   = // Will be included

//		Apparatus[nParticles+i]		= // Mark as TAPS
//    	d_E[nParticles+i]			= // VETO Energy		
//   	WC1_E[nParticles+i]			= 0.0; // Will be included
//	 	WC2_E[nParticles+i]    		= 0.0; // Will be included
//	 	WC_Vertex_X[nParticles+i]  	= 0.0; // Will be included
//	 	WC_Vertex_Y[nParticles+i]  	= 0.0; // Will be included
//	 	WC_Vertex_Z[nParticles+i]  	= 0.0; // Will be included    			
	}
	nParticles += fTAPS->GetNParticle(); // update number of particles
	
	
	
	//increment event number
	eventNumber++;
	
	//treeEvent->Fill();
	
	
}

void    TA2GoAT::Finish()
{
	printf("------------------\n");
	printf("Write Tree to file\n");
	printf("------------------\n");
	
	file->cd();
	
	if(treeEvent) 
	{
		treeEvent->Write();	// Write	
		delete treeEvent; 	// Close and delete in memory
	}	
	if(treeScaler) 
	{
		treeScaler->Write();// Write	
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
