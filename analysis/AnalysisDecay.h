#ifndef	_AnalysisDecay_h__
#define	_AnalysisDecay_h__



#include "AnalysisEtaP.h"


class	AnalysisDecay
{
private:
	
protected:
	Char_t	 	name[128];
	Char_t	 	title[512];

public:
			AnalysisDecay(const char* Name, const char* Title);
	virtual	~AnalysisDecay();

	virtual	bool	IsEtaP() = 0;

	virtual	void	Clear()	= 0;
			
	virtual	bool	Analyse(AnalysisEtaP* analysis) = 0;
	
	virtual	void	Draw()	= 0;
	virtual	void	Save(TFile* outFile)	= 0;
};




#endif
