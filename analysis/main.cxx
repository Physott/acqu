

#include <stdio.h>

#include "ReadRootTree.h"


int	main( int argc, const char* argv[] )
{
	printf("OK\n");
	
	ReadRootTree		c("cutTree_6.root", "cutTree_6");
	
	c.SetCutTaggerTime(171,178,151,158,191,198);
	c.openTree();
	c.Analyse(0,1000);
	c.Draw();
}
