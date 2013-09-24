#ifndef	_AnalysisEtaP2Gamma_h__
#define	_AnalysisEtaP2Gamma_h__




#include "AnalysisDecay.h"
#include "Cut.h"


class AnalysisEtaP2Gamma	: public AnalysisDecay
{
private:
	
	//general	
	TCanvas*			canvas;
	Cut*				cutInvMass;
	
	//histograms
	TH1D*				hInvMass;
	
protected:
	
public:
			AnalysisEtaP2Gamma(const char* Name, const char* Title);
	virtual	~AnalysisEtaP2Gamma();
	
	virtual	bool	IsEtaP()							{return false;}
	
	virtual	void	Clear()								{hInvMass->Reset("M");}
	
	virtual	bool	Analyse(AnalysisEtaP* analysis);
		
	virtual	void	Draw();
	virtual	void	Save(TFile* outFile);
};



#endif
