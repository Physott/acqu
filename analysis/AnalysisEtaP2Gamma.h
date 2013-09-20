#ifndef	_AnalysisEtaP2Gamma_h__
#define	_AnalysisEtaP2Gamma_h__





#include "AnalysisEtaP.h"


class AnalysisEtaP2Gamma
{
private:
	
	//general	
	TCanvas*			canvas;
	
	//histograms
	TH1D*				hInvMass;
	
	//bool	CutInvariantMass(AnalysisEtaP* analysis);
	
	
protected:
	
	
public:
	AnalysisEtaP2Gamma();
	~AnalysisEtaP2Gamma();
	
	void	Clear();
	
	bool	Analyse(AnalysisEtaP* analysis);
		
	void	Draw();
	void	Save();
};



#endif
