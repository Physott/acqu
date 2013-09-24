#ifndef	_AnalysisEtaP_h__
#define	_AnalysisEtaP_h__



#include "ReadRootTree.h"


#define		MASS_PI0	134.9766
#define		MASS_ETA	547.51
#define		MASS_ETAP	957.78


class AnalysisEtaP2Gamma;
class AnalysisEtaP6Gamma;


class	AnalysisEtaP	: public ReadRootTree
{
private:
	//general
	//TCanvas*				canvas;
	
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
	
	const	AnalysisEtaP2Gamma*	GetAnalysis2Gamma()	const		{return analysis2;}
	const	AnalysisEtaP6Gamma*	GetAnalysis6Gamma()	const		{return analysis6;}
	
	friend class AnalysisEtaP2Gamma;
	friend class AnalysisEtaP6Gamma;
};




#endif
