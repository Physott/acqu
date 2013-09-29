#include "TreeAnalyseEtaP2Gamma.C"
#include "TreeAnalyseEtaP2GammaTagg.C"
#include "TreeAnalyseEtaP2GammaTaggMissMass.C"


void	TreeAnalyse(const Char_t* FileName)
{
	TreeAnalyseEtaP2Gamma* a = new TreeAnalyseEtaP2Gamma(FileName);
	a->Open();
	a->SetCutCBTime(-50, 50);
	a->Analyse();
	a->Save();
	delete a;
	
	TreeAnalyseEtaP2GammaTagg* b = new TreeAnalyseEtaP2GammaTagg(FileName);
	b->Open();
	b->SetCutTaggPrompt(-6, 6);
	b->SetCutTaggRand1(-20, -8);
	b->SetCutTaggRand2(8, 20);
	b->Analyse();
	b->Save();
	delete b;
	
	TreeAnalyseEtaP2GammaTaggMissMass* c = new TreeAnalyseEtaP2GammaTaggMissMass(FileName);
	c->Open();
	c->Analyse();
	c->Save();
	delete c;
}
