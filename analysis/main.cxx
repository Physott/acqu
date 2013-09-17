

#include <stdio.h>

#include "AnalysisEtaP6Gamma.h"


int	main( int argc, const char* argv[] )
{
	//printf("OK\n");
	
	AnalysisEtaP6Gamma*		c = new AnalysisEtaP6Gamma("TTreeOutput_41948_CUT_2_6_10.root", "TTreeOutput_41948_CUT_2_6_10");
	
	c->SetCutTaggerTime(-4,4,-14,-6,6,14);
	//printf("vor\n");
	c->Analyse(0,10000);
	//printf("nach\n");
	c->Draw();
	
	
	
}
