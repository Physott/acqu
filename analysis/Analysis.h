#ifndef	_Analysis_h__
#define	_Analysis_h__


#include "AnalysisTagger.h"
#include "AnalysisEtaP2GammaTaggedCanvas.h"
#include "AnalysisEtaP6GammaTaggedCanvas.h"




class	Analysis	:	public AnalysisTagger
{
private:
	
	AnalysisEtaP2GammaTaggedCanvas*		result2;
	AnalysisEtaP6GammaTaggedCanvas*		result6[2];		//[EtaP,3Pi0]
	
	bool	AnalyseEvent(const int index);				// no index checking
	void	Save();
	
public:
			Analysis(const char* _treeFileName, const char* _treeName);
	virtual	~Analysis();
	
	virtual	void	Clear();
			
	virtual	void	Analyse(const int min, const int max);
	
	virtual	void	Draw();
	virtual	void	Save(const Char_t* outputFileName);
};





#endif
