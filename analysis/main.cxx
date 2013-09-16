

#include <stdio.h>

#include "AnalysisEtaP6Gamma.h"


int	main( int argc, const char* argv[] )
{
	//printf("OK\n");
	
	AnalysisEtaP6Gamma*		c = new AnalysisEtaP6Gamma("cutTree_6.root", "cutTree_6");
	
	c->SetCutTaggerTime(171,178,151,158,191,198);
	//printf("vor\n");
	c->Analyse(0,1000);
	//printf("nach\n");
	c->Draw();
	
	
}
