#ifndef	_AnalysisEtaP_h__
#define	_AnalysisEtaP_h__



#include "ReadRootTree.h"


#define		MASS_PI0	134.9766
#define		MASS_ETA	547.51
#define		MASS_ETAP	957.78


class AnalysisEtaP2Gamma;
class AnalysisEtaP6Gamma;



enum	EAE_Multiplicity
{
	MULTIPLICITY_2	= 0,
	MULTIPLICITY_6,
	MULTIPLICITY_10
};


class	AnalysisEtaP	: public ReadRootTree
{
private:
	//general
	TCanvas*				canvas;
	
	EAE_Multiplicity		multiplicity;	
	
	//histograms
	TH1D*			hCheckInvMassCutPi0;
	TH1D*			hCheckInvMassCutEta;
	
protected:

	AnalysisEtaP2Gamma*		analysis2;
	AnalysisEtaP6Gamma*		analysis6;

	bool	AnalyseEvent(const int index);				// no index checking
	void	Save();

public:
			AnalysisEtaP(const char* _treeFileName, const char* _treeName);
	virtual	~AnalysisEtaP();

	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
	
	const	EAE_Multiplicity	GetMultiplicity()	const		{return multiplicity;}
	const	AnalysisEtaP2Gamma*	GetAnalysis2Gamma()	const		{return analysis2;}
	const	AnalysisEtaP6Gamma*	GetAnalysis6Gamma()	const		{return analysis6;}
	
	friend class AnalysisEtaP2Gamma;
	friend class AnalysisEtaP6Gamma;
};




#endif
