#include "TaggedHist.C"
#include "TaggedHist2D.C"


#define MASS_PROTON	938.27203

class	TreeAnalyse3GammaFindProton
{
private:
	TLorentzVector	vecPart[3];				//[Perm0,Perm1,Perm2]
	Double_t		massPart[3];
	TLorentzVector	beam[8];
	TLorentzVector	vecMiss[3][8];
	Double_t		thetaDiff[3][8];
	Double_t		phiDiff[3][8];
	Int_t			foundIM[3];
	Int_t			foundAngle;
	Int_t			foundAll[3];
	
	Double_t		cutIMPi0[2];
	Double_t		cutIMEta[2];
	Double_t		cutIMEtaP[2];
	
	bool			foundIM[3];
	bool			foundAngle;
	
	TH1I*			hCount;
	TaggedHist*		hIMParticle[3];			//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleDiced[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonThetaDiff[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonPhiDiff[3];		//[Perm0,Perm1,Perm2]
	TaggedHist2D*	hProtonAngleDiff[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleCutIM[3][3];		//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleDicedCutIM[3][3];//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonThetaDiffCutIM[3][3];//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonPhiDiffCutIM[3][3];	//[Perm0,Perm1,Perm2]
	TaggedHist2D*	hProtonAngleDiffCutIM[3][3];//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleCutAngle[3];			//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleDicedCutAngle[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonThetaDiffCutAngle[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonPhiDiffCutAngle[3];		//[Perm0,Perm1,Perm2]
	TaggedHist2D*	hProtonAngleDiffCutAngle[3];	//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleCutAll[3][3];		//[Perm0,Perm1,Perm2]
	TaggedHist*		hIMParticleDicedCutAll[3][3];//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonThetaDiffCutAll[3][3];//[Perm0,Perm1,Perm2]
	TaggedHist*		hProtonPhiDiffCutAll[3][3];	//[Perm0,Perm1,Perm2]
	TaggedHist2D*	hProtonAngleDiffCutAll[3][3];//[Perm0,Perm1,Perm2]
	
protected:

public:
	TreeAnalyse3GammaFindProton(const Char_t* FileName);
	~TreeAnalyse3GammaFindProton();
	
	void	Clear();
	Int_t	Analyse(const TLorentzVector* vec, const Int_t nTagged, const Double_t* TaggedEnergy);
	bool	Save();
	
	const	Double_t*	GetCutIMPi0()		const	{return cutIMPi0;}
	const	Double_t	GetCutIMPi0Min()	const	{return cutIMPi0[0];}
	const	Double_t	GetCutIMPi0Max()	const	{return cutIMPi0[1];}
	const	Double_t*	GetCutIMEta()		const	{return cutIMEta;}
	const	Double_t	GetCutIMEtaMin()	const	{return cutIMEta[0];}
	const	Double_t	GetCutIMEtaMax()	const	{return cutIMEta[1];}
	const	Double_t*	GetCutIMEtaP()		const	{return cutIMEtaP;}
	const	Double_t	GetCutIMEtaPMin()	const	{return cutIMEtaP[0];}
	const	Double_t	GetCutIMEtaPMax()	const	{return cutIMEtaP[1];}
	
	
			void		SetCutIMPi0(const Double_t Min, const Double_t Max)		{cutIMPi0[0] = Min ; cutIMPi0[1] = Max;}
			void		SetCutIMEta(const Double_t Min, const Double_t Max)		{cutIMEta[0] = Min ; cutIMEta[1] = Max;}
			void		SetCutIMEtaP(const Double_t Min, const Double_t Max)	{cutIMEtaP[0] = Min; cutIMEtaP[1] = Max;}
			
	static	TreeAnalyse3GammaFindProton*	test()
	{
		printf("Creating\n");
		TreeAnalyse3GammaFindProton* c = new TreeAnalyse3GammaFindProton("tree_TTreeOutput_41941_3g.root");
		printf("Opening\n");
		c->Open();
		printf("Analysing\n");
		c->Analyse();
		printf("Saving\n");
		c->Save();
		printf("End\n");
		return c;
	}
};


TreeAnalyse3GammaFindProton::TreeAnalyse3GammaFindProton(const Char_t* FileName)	: TreeReadTagged(FileName)
{
	cutIMPi0[0]		= 100;
	cutIMPi0[1]		= 170;
	cutIMEta[0]		= 487;
	cutIMEta[1]		= 607;
	cutIMEtaP[0]	= 850;
	cutIMEtaP[1]	= 1050;
	
	if((hCount	= (TH1I*)gROOT->Get("hCount")))
		hCount->Delete();
	hCount		= new TH1I("hCount", "1:All/2,3,4:PassedIM(Pi0,Eta,EtaP)/6:PassedAngle/8,9,10:PassedAll(Pi0,Eta,EtaP)", 12, 0, 12);
	if(!hCount)
	{
		cout << "Could not create histogram " << "hCount" << ". Exiting!" << endl;
		exit(1);
	}
	
	hIMParticle[0]		= new TaggedHist("IMParticle_0", 1600, 0, 1600);
	hIMParticle[1]		= new TaggedHist("IMParticle_1", 1600, 0, 1600);
	hIMParticle[2]		= new TaggedHist("IMParticle_2", 1600, 0, 1600);
	hIMParticleDiced[0]	= new TaggedHist("IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDiced[1]	= new TaggedHist("IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDiced[2]	= new TaggedHist("IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiff[0]	= new TaggedHist("ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiff[1]	= new TaggedHist("ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiff[2]	= new TaggedHist("ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiff[0]	= new TaggedHist("ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiff[1]	= new TaggedHist("ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiff[2]	= new TaggedHist("ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiff[0]	= new TaggedHist2D("ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiff[1]	= new TaggedHist2D("ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiff[2]	= new TaggedHist2D("ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	
	hIMParticleCutIM[0][0]		= new TaggedHist("CutIMPi0_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutIM[1][0]		= new TaggedHist("CutIMPi0_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutIM[2][0]		= new TaggedHist("CutIMPi0_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutIM[0][0]	= new TaggedHist("CutIMPi0_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutIM[1][0]	= new TaggedHist("CutIMPi0_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutIM[2][0]	= new TaggedHist("CutIMPi0_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutIM[0][0]	= new TaggedHist("CutIMPi0_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutIM[1][0]	= new TaggedHist("CutIMPi0_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutIM[2][0]	= new TaggedHist("CutIMPi0_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutIM[0][0]	= new TaggedHist("CutIMPi0_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutIM[1][0]	= new TaggedHist("CutIMPi0_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutIM[2][0]	= new TaggedHist("CutIMPi0_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutIM[0][0]	= new TaggedHist2D("CutIMPi0_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[1][0]	= new TaggedHist2D("CutIMPi0_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[2][0]	= new TaggedHist2D("CutIMPi0_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	hIMParticleCutIM[0][1]		= new TaggedHist("CutIMEta_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutIM[1][1]		= new TaggedHist("CutIMEta_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutIM[2][1]		= new TaggedHist("CutIMEta_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutIM[0][1]	= new TaggedHist("CutIMEta_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutIM[1][1]	= new TaggedHist("CutIMEta_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutIM[2][1]	= new TaggedHist("CutIMEta_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutIM[0][1]	= new TaggedHist("CutIMEta_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutIM[1][1]	= new TaggedHist("CutIMEta_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutIM[2][1]	= new TaggedHist("CutIMEta_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutIM[0][1]	= new TaggedHist("CutIMEta_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutIM[1][1]	= new TaggedHist("CutIMEta_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutIM[2][1]	= new TaggedHist("CutIMEta_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutIM[0][1]	= new TaggedHist2D("CutIMEta_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[1][1]	= new TaggedHist2D("CutIMEta_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[2][1]	= new TaggedHist2D("CutIMEta_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	hIMParticleCutIM[0][2]		= new TaggedHist("CutIMEtaP_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutIM[1][2]		= new TaggedHist("CutIMEtaP_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutIM[2][2]		= new TaggedHist("CutIMEtaP_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutIM[0][2]	= new TaggedHist("CutIMEtaP_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutIM[1][2]	= new TaggedHist("CutIMEtaP_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutIM[2][2]	= new TaggedHist("CutIMEtaP_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutIM[0][2]	= new TaggedHist("CutIMEtaP_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutIM[1][2]	= new TaggedHist("CutIMEtaP_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutIM[2][2]	= new TaggedHist("CutIMEtaP_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutIM[0][2]	= new TaggedHist("CutIMEtaP_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutIM[1][2]	= new TaggedHist("CutIMEtaP_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutIM[2][2]	= new TaggedHist("CutIMEtaP_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutIM[0][2]	= new TaggedHist2D("CutIMEtaP_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[1][2]	= new TaggedHist2D("CutIMEtaP_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutIM[2][2]	= new TaggedHist2D("CutIMEtaP_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	
	hIMParticleCutAngle[0]			= new TaggedHist("CutAngle_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutAngle[1]			= new TaggedHist("CutAngle_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutAngle[2]			= new TaggedHist("CutAngle_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutAngle[0]	= new TaggedHist("CutAngle_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutAngle[1]	= new TaggedHist("CutAngle_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutAngle[2]	= new TaggedHist("CutAngle_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutAngle[0]	= new TaggedHist("CutAngle_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutAngle[1]	= new TaggedHist("CutAngle_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutAngle[2]	= new TaggedHist("CutAngle_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutAngle[0]		= new TaggedHist("CutAngle_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutAngle[1]		= new TaggedHist("CutAngle_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutAngle[2]		= new TaggedHist("CutAngle_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutAngle[0]	= new TaggedHist2D("CutAngle_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAngle[1]	= new TaggedHist2D("CutAngle_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAngle[2]	= new TaggedHist2D("CutAngle_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	
	hIMParticleCutAll[0][0]			= new TaggedHist("CutAllPi0_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutAll[1][0]			= new TaggedHist("CutAllPi0_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutAll[2][0]			= new TaggedHist("CutAllPi0_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutAll[0][0]	= new TaggedHist("CutAllPi0_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutAll[1][0]	= new TaggedHist("CutAllPi0_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutAll[2][0]	= new TaggedHist("CutAllPi0_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutAll[0][0]	= new TaggedHist("CutAllPi0_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutAll[1][0]	= new TaggedHist("CutAllPi0_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutAll[2][0]	= new TaggedHist("CutAllPi0_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutAll[0][0]		= new TaggedHist("CutAllPi0_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutAll[1][0]		= new TaggedHist("CutAllPi0_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutAll[2][0]		= new TaggedHist("CutAllPi0_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutAll[0][0]	= new TaggedHist2D("CutAllPi0_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[1][0]	= new TaggedHist2D("CutAllPi0_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[2][0]	= new TaggedHist2D("CutAllPi0_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	hIMParticleCutAll[0][1]			= new TaggedHist("CutAllEta_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutAll[1][1]			= new TaggedHist("CutAllEta_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutAll[2][1]			= new TaggedHist("CutAllEta_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutAll[0][1]	= new TaggedHist("CutAllEta_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutAll[1][1]	= new TaggedHist("CutAllEta_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutAll[2][1]	= new TaggedHist("CutAllEta_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutAll[0][1]	= new TaggedHist("CutAllEta_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutAll[1][1]	= new TaggedHist("CutAllEta_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutAll[2][1]	= new TaggedHist("CutAllEta_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutAll[0][1]		= new TaggedHist("CutAllEta_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutAll[1][1]		= new TaggedHist("CutAllEta_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutAll[2][1]		= new TaggedHist("CutAllEta_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutAll[0][1]	= new TaggedHist2D("CutAllEta_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[1][1]	= new TaggedHist2D("CutAllEta_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[2][1]	= new TaggedHist2D("CutAllEta_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
	
	hIMParticleCutAll[0][2]			= new TaggedHist("CutAllEtaP_IMParticle_0", 1600, 0, 1600);
	hIMParticleCutAll[1][2]			= new TaggedHist("CutAllEtaP_IMParticle_1", 1600, 0, 1600);
	hIMParticleCutAll[2][2]			= new TaggedHist("CutAllEtaP_IMParticle_2", 1600, 0, 1600);
	hIMParticleDicedCutAll[0][2]	= new TaggedHist("CutAllEtaP_IMParticleDiced_0", 1600, 0, 1600);
	hIMParticleDicedCutAll[1][2]	= new TaggedHist("CutAllEtaP_IMParticleDiced_1", 1600, 0, 1600);
	hIMParticleDicedCutAll[2][2]	= new TaggedHist("CutAllEtaP_IMParticleDiced_2", 1600, 0, 1600);
	hProtonThetaDiffCutAll[0][2]	= new TaggedHist("CutAllEtaP_ProtonThetaDiff_0", 720, -360, 360);
	hProtonThetaDiffCutAll[1][2]	= new TaggedHist("CutAllEtaP_ProtonThetaDiff_1", 720, -360, 360);
	hProtonThetaDiffCutAll[2][2]	= new TaggedHist("CutAllEtaP_ProtonThetaDiff_2", 720, -360, 360);
	hProtonPhiDiffCutAll[0][2]		= new TaggedHist("CutAllEtaP_ProtonPhiDiff_0", 720, -360, 360);
	hProtonPhiDiffCutAll[1][2]		= new TaggedHist("CutAllEtaP_ProtonPhiDiff_1", 720, -360, 360);
	hProtonPhiDiffCutAll[2][2]		= new TaggedHist("CutAllEtaP_ProtonPhiDiff_2", 720, -360, 360);
	hProtonAngleDiffCutAll[0][2]	= new TaggedHist2D("CutAllEtaP_ProtonAngleDiff_0", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[1][2]	= new TaggedHist2D("CutAllEtaP_ProtonAngleDiff_1", 720, -360, 360, 720, -360, 360);
	hProtonAngleDiffCutAll[2][2]	= new TaggedHist2D("CutAllEtaP_ProtonAngleDiff_2", 720, -360, 360, 720, -360, 360);
}

TreeAnalyse3GammaFindProton::~TreeAnalyse3GammaFindProton()
{
	for(int i=0; i<3; i++)
	{
		if(outTree[i])
			delete	outTree[i];
		if(outFile[i])
			delete	outFile[i];
	}
}


inline	void	TreeAnalyse3GammaFindProton::Clear()
{
	hCount->Reset("M");
	
	for(int i=0; i<3; i++)
	{
		hIMParticle[i].Clear();
		hIMParticleDiced[i].Clear();
		hProtonThetaDiff[i].Clear();
		hProtonPhiDiff[i].Clear();
		hProtonAngleDiff[i].Clear();
		
		for(int l=0; l<3; l++)
		{
			hIMParticleCutIM[i][l].Clear();
			hIMParticleDicedCutIM[i][l].Clear();
			hProtonThetaDiffCutIM[i][l].Clear();
			hProtonPhiDiffCutIM[i][l].Clear();
			hProtonAngleDiffCutIM[i][l].Clear();
		}
		
		hIMParticleCutAngle[i].Clear();
		hIMParticleDicedCutAngle[i].Clear();
		hProtonThetaDiffCutAngle[i].Clear();
		hProtonPhiDiffCutAngle[i].Clear();
		hProtonAngleDiffCutAngle[i].Clear();
		
		for(int l=0; l<3; l++)
		{
			hIMParticleCutAll[i][l].Clear();
			hIMParticleDicedCutAll[i][l].Clear();
			hProtonThetaDiffCutAll[i][l].Clear();
			hProtonPhiDiffCutAll[i][l].Clear();
			hProtonAngleDiffCutAll[i][l].Clear();
		}
	}
}
Int_t	TreeAnalyse3GammaFindProton::Analyse(const TLorentzVector* vec, const Int_t nTagged, const Double_t* TaggedEnergy)
{
	TreeReadTagged::AnalyseEvent(i);
	
	hCount[0]->Fill(1);
	
	vecPart[0]	= vec[1] + vec[2];
	vecPart[1]	= vec[0] + vec[2];
	vecPart[2]	= vec[0] + vec[1];
	massPart[0]	= vecPart[0].M();
	massPart[1]	= vecPart[1].M();
	massPart[2]	= vecPart[2].M();
	
	hIMParticle[0]->Fill(nTagged, massPart[0]);
	hIMParticle[1]->Fill(nTagged, massPart[1]);
	hIMParticle[2]->Fill(nTagged, massPart[2]);
	
	static	Int_t	dice1	= 0;
	if(dice1 == 0)
	{
		hIMParticleDiced[0]->Fill(nTagged, massPart[0]);
		hIMParticleDiced[1]->Fill(nTagged, massPart[1]);
		hIMParticleDiced[2]->Fill(nTagged, massPart[2]);
		dice1	= 1;
	}
	else if(dice1 == 1)
	{
		hIMParticleDiced[0]->Fill(nTagged, massPart[1]);
		hIMParticleDiced[1]->Fill(nTagged, massPart[2]);
		hIMParticleDiced[2]->Fill(nTagged, massPart[0]);
		dice1	= 2;
	}
	else
	{
		hIMParticleDiced[0]->Fill(nTagged, massPart[2]);
		hIMParticleDiced[1]->Fill(nTagged, massPart[0]);
		hIMParticleDiced[2]->Fill(nTagged, massPart[1]);
		dice1	= 0;
	}
	
	for(int l=0; l<nTagged; l++)
	{
		beam[l].SetPxPyPzE(TaggedEnergy[l], 0.0, 0.0, TaggedEnergy[l] + MASS_PROTON);
		for(int i=0; i<3; i++)
		{
			vecMiss[i][l]	= beam[l] - vecPart[i];
			thetaDiff[i][l]	= TMath::RadToDeg()*(vecPart[i].Theta() - vecMiss[i][l].Theta());
			phiDiff[i][l]	= TMath::RadToDeg()*(vecPart[i].Phi() - vecMiss[i][l].Phi());
			if(massPart[m] >= cutIMPi0[0] && massPart[m] <= cutIMPi0[1])
			{
			}
		}
	}
	
	
	
	
	
	for(int l=0; l<3; l++)
	{
		if(nTagged[l] == 1)
		{
			found	= true;
			beam.SetPxPyPzE(TaggedEnergy[l][0], 0.0, 0.0, TaggedEnergy[l][0] + MASS_PROTON);
			hCount[1]->Fill(1);
			hCount[2]->Fill(1);
			hCount[3]->Fill(1);
			hCount[4]->Fill(1);
			for(int m=0; m<3; m++)
			{
				vecMiss		= beam - vecPart[m];
				thetaDiff	= TMath::RadToDeg()*(vecPart[m].Theta() - vecMiss.Theta());
				phiDiff		= TMath::RadToDeg()*(vecPart[m].Phi() - vecMiss.Phi());
				hProtonThetaDiff[m]->Fill(l, thetaDiff);
				hProtonPhiDiff[m]->Fill(l, phiDiff);
				hProtonAngleDiff[m]->Fill(l, thetaDiff, phiDiff);
				if(massPart[m] >= cutIMPi0[0] && massPart[m] <= cutIMPi0[1])
				{
					hCount[0]->Fill(2);
					hCount[1]->Fill(l+2);
					hCount[5]->Fill(1);
			
					hIMParticleCutIM[m][0]->Fill(l, massPart[m]);
					
					static	Int_t	dice2	= 0;
					if(dice2 == 0)
					{
						hIMParticleDicedCutIM[0][0]->Fill(l, massPart[m]);
						dice2	= 1;
					}
					else if(dice2 == 1)
					{
						hIMParticleDicedCutIM[1][0]->Fill(l, massPart[m]);
						dice2	= 2;
					}
					else
					{
						hIMParticleDicedCutIM[2][0]->Fill(l, massPart[m]);
						dice2	= 0;
					}
					
					hProtonThetaDiffCutIM[m][0]->Fill(l, thetaDiff);
					hProtonPhiDiffCutIM[m][0]->Fill(l, phiDiff);
					hProtonAngleDiffCutIM[m][0]->Fill(l, thetaDiff, phiDiff);
					
					if(thetaDiff[m] >= -10 && thetaDiff[m] <= 10 && phiDiff[m] >= -10 && phiDiff[m] <= 10)
					{
						hCount[0]->Fill(8);
						hCount[5]->Fill(l+2);
	
						hIMParticleCutAll[m][0]->Fill(l, massPart[m]);
						
						static	Int_t	dice5	= 0;
						if(dice5 == 0)
						{
							hIMParticleDicedCutAll[0][0]->Fill(l, massPart[m]);
							dice5	= 1;
						}
						else if(dice5 == 1)
						{
							hIMParticleDicedCutAll[1][0]->Fill(l, massPart[m]);
							dice5	= 2;
						}
						else
						{
							hIMParticleDicedCutAll[2][0]->Fill(l, massPart[m]);
							dice5	= 0;
						}
						
						hProtonThetaDiffCutAll[m][0]->Fill(l, thetaDiff);
						hProtonPhiDiffCutAll[m][0]->Fill(l, phiDiff);
						hProtonAngleDiffCutAll[m][0]->Fill(l, thetaDiff, phiDiff);
						
						outTree[0]->Fill();
					}
				}
				else if(massPart[m] >= cutIMEta[0] && massPart[m] <= cutIMEta[1])
				{
					hCount[0]->Fill(3);
					hCount[2]->Fill(l+2);
					hCount[6]->Fill(1);
	
					hIMParticleCutIM[m][1]->Fill(l, massPart[m]);
					
					static	Int_t	dice3	= 0;
					if(dice3 == 0)
					{
						hIMParticleDicedCutIM[0][1]->Fill(l, massPart[m]);
						dice3	= 1;
					}
					else if(dice3 == 1)
					{
						hIMParticleDicedCutIM[1][1]->Fill(l, massPart[m]);
						dice3	= 2;
					}
					else
					{
						hIMParticleDicedCutIM[2][1]->Fill(l, massPart[m]);
						dice3	= 0;
					}
					
					hProtonThetaDiffCutIM[m][1]->Fill(l, thetaDiff);
					hProtonPhiDiffCutIM[m][1]->Fill(l, phiDiff);
					hProtonAngleDiffCutIM[m][1]->Fill(l, thetaDiff, phiDiff);
					
					if(thetaDiff[m] >= -10 && thetaDiff[m] <= 10 && phiDiff[m] >= -10 && phiDiff[m] <= 10)
					{
						hCount[0]->Fill(9);
						hCount[6]->Fill(l+2);
	
						hIMParticleCutAll[m][1]->Fill(l, massPart[m]);
						
						static	Int_t	dice6	= 0;
						if(dice6 == 0)
						{
							hIMParticleDicedCutAll[0][1]->Fill(l, massPart[m]);
							dice6	= 1;
						}
						else if(dice6 == 1)
						{
							hIMParticleDicedCutAll[1][1]->Fill(l, massPart[m]);
							dice6	= 2;
						}
						else
						{
							hIMParticleDicedCutAll[2][1]->Fill(l, massPart[m]);
							dice6	= 0;
						}
						
						hProtonThetaDiffCutAll[m][1]->Fill(l, thetaDiff);
						hProtonPhiDiffCutAll[m][1]->Fill(l, phiDiff);
						hProtonAngleDiffCutAll[m][1]->Fill(l, thetaDiff, phiDiff);
						
						outTree[1]->Fill();
					}
				}
				else if(massPart[m] >= cutIMEtaP[0] && massPart[m] <= cutIMEtaP[1])
				{
					hCount[0]->Fill(4);
					hCount[3]->Fill(l+2);
					hCount[7]->Fill(1);
	
					hIMParticleCutIM[m][2]->Fill(l, massPart[m]);
					
					static	Int_t	dice4	= 0;
					if(dice4 == 0)
					{
						hIMParticleDicedCutIM[0][2]->Fill(l, massPart[m]);
						dice4	= 1;
					}
					else if(dice4 == 1)
					{
						hIMParticleDicedCutIM[1][2]->Fill(l, massPart[m]);
						dice4	= 2;
					}
					else
					{
						hIMParticleDicedCutIM[2][2]->Fill(l, massPart[m]);
						dice4	= 0;
					}
					
					hProtonThetaDiffCutIM[m][2]->Fill(l, thetaDiff);
					hProtonPhiDiffCutIM[m][2]->Fill(l, phiDiff);
					hProtonAngleDiffCutIM[m][2]->Fill(l, thetaDiff, phiDiff);
					
					if(thetaDiff[m] >= -10 && thetaDiff[m] <= 10 && phiDiff[m] >= -10 && phiDiff[m] <= 10)
					{
						hCount[0]->Fill(10);
						hCount[7]->Fill(l+2);
	
						hIMParticleCutAll[m][2]->Fill(l, massPart[m]);
						
						static	Int_t	dice7	= 0;
						if(dice7 == 0)
						{
							hIMParticleDicedCutAll[0][2]->Fill(l, massPart[m]);
							dice7	= 1;
						}
						else if(dice7 == 1)
						{
							hIMParticleDicedCutAll[1][2]->Fill(l, massPart[m]);
							dice7	= 2;
						}
						else
						{
							hIMParticleDicedCutAll[2][2]->Fill(l, massPart[m]);
							dice7	= 0;
						}
						
						hProtonThetaDiffCutAll[m][2]->Fill(l, thetaDiff);
						hProtonPhiDiffCutAll[m][2]->Fill(l, phiDiff);
						hProtonAngleDiffCutAll[m][2]->Fill(l, thetaDiff, phiDiff);
						
						outTree[2]->Fill();
					}
				}
				
				if(thetaDiff[m] >= -10 && thetaDiff[m] <= 10 && phiDiff[m] >= -10 && phiDiff[m] <= 10)
				{
					hCount[0]->Fill(6);
					hCount[4]->Fill(l+2);
	
					hIMParticleCutAngle[m]->Fill(l, massPart[m]);
					
					static	Int_t	dice4	= 0;
					if(dice4 == 0)
					{
						hIMParticleDicedCutAngle[0]->Fill(l, massPart[m]);
						dice4	= 1;
					}
					else if(dice4 == 1)
					{
						hIMParticleDicedCutAngle[1]->Fill(l, massPart[m]);
						dice4	= 2;
					}
					else
					{
						hIMParticleDicedCutAngle[2]->Fill(l, massPart[m]);
						dice4	= 0;
					}
					
					hProtonThetaDiffCutAngle[m]->Fill(l, thetaDiff);
					hProtonPhiDiffCutAngle[m]->Fill(l, phiDiff);
					hProtonAngleDiffCutAngle[m]->Fill(l, thetaDiff, phiDiff);
				}
			}
		}
	}
}

void	TreeAnalyse3GammaFindProton::Save()
{
	for(int i=0; i<3; i++)
	{
		outFile[i]->cd();
		outTree[i]->Write();
		outFile[i]->Flush();
	}

	Char_t	str[128];
	sprintf(str, "hist_%s_FindProton.root", GetFileName());
	TFile*	result	= new TFile(str, "RECREATE");
	if(!result)
		return false;
		
	result->cd();
	hCount[0]->Write();
	hIMParticle[0]->SaveResult();
	hIMParticle[1]->SaveResult();
	hIMParticle[2]->SaveResult();
	hIMParticleDiced[0]->SaveResult();
	hIMParticleDiced[1]->SaveResult();
	hIMParticleDiced[2]->SaveResult();
	hProtonThetaDiff[0]->SaveResult();
	hProtonThetaDiff[1]->SaveResult();
	hProtonThetaDiff[2]->SaveResult();
	hProtonPhiDiff[0]->SaveResult();
	hProtonPhiDiff[1]->SaveResult();
	hProtonPhiDiff[2]->SaveResult();
	hProtonAngleDiff[0]->SaveResult();
	hProtonAngleDiff[1]->SaveResult();
	hProtonAngleDiff[2]->SaveResult();
	
	hIMParticle[0]->SaveSubs();
	hIMParticle[1]->SaveSubs();
	hIMParticle[2]->SaveSubs();
	hIMParticleDiced[0]->SaveSubs();
	hIMParticleDiced[1]->SaveSubs();
	hIMParticleDiced[2]->SaveSubs();
	hProtonThetaDiff[0]->SaveSubs();
	hProtonThetaDiff[1]->SaveSubs();
	hProtonThetaDiff[2]->SaveSubs();
	hProtonPhiDiff[0]->SaveSubs();
	hProtonPhiDiff[1]->SaveSubs();
	hProtonPhiDiff[2]->SaveSubs();
	hProtonAngleDiff[0]->SaveSubs();
	hProtonAngleDiff[1]->SaveSubs();
	hProtonAngleDiff[2]->SaveSubs();
	
	result->cd();
	result->mkdir("CutIMPi0");
	result->cd("CutIMPi0");
	hCount[1]->Write();
	hIMParticleCutIM[0][0]->SaveResult();
	hIMParticleCutIM[1][0]->SaveResult();
	hIMParticleCutIM[2][0]->SaveResult();
	hIMParticleDicedCutIM[0][0]->SaveResult();
	hIMParticleDicedCutIM[1][0]->SaveResult();
	hIMParticleDicedCutIM[2][0]->SaveResult();
	hProtonThetaDiffCutIM[0][0]->SaveResult();
	hProtonThetaDiffCutIM[1][0]->SaveResult();
	hProtonThetaDiffCutIM[2][0]->SaveResult();
	hProtonPhiDiffCutIM[0][0]->SaveResult();
	hProtonPhiDiffCutIM[1][0]->SaveResult();
	hProtonPhiDiffCutIM[2][0]->SaveResult();
	hProtonAngleDiffCutIM[0][0]->SaveResult();
	hProtonAngleDiffCutIM[1][0]->SaveResult();
	hProtonAngleDiffCutIM[2][0]->SaveResult();
	
	hIMParticleCutIM[0][0]->SaveSubs();
	hIMParticleCutIM[1][0]->SaveSubs();
	hIMParticleCutIM[2][0]->SaveSubs();
	hIMParticleDicedCutIM[0][0]->SaveSubs();
	hIMParticleDicedCutIM[1][0]->SaveSubs();
	hIMParticleDicedCutIM[2][0]->SaveSubs();
	hProtonThetaDiffCutIM[0][0]->SaveSubs();
	hProtonThetaDiffCutIM[1][0]->SaveSubs();
	hProtonThetaDiffCutIM[2][0]->SaveSubs();
	hProtonPhiDiffCutIM[0][0]->SaveSubs();
	hProtonPhiDiffCutIM[1][0]->SaveSubs();
	hProtonPhiDiffCutIM[2][0]->SaveSubs();
	hProtonAngleDiffCutIM[0][0]->SaveSubs();
	hProtonAngleDiffCutIM[1][0]->SaveSubs();
	hProtonAngleDiffCutIM[2][0]->SaveSubs();
	
	result->cd();
	result->mkdir("CutIMEta");
	result->cd("CutIMEta");
	hCount[2]->Write();
	hIMParticleCutIM[0][1]->SaveResult();
	hIMParticleCutIM[1][1]->SaveResult();
	hIMParticleCutIM[2][1]->SaveResult();
	hIMParticleDicedCutIM[0][1]->SaveResult();
	hIMParticleDicedCutIM[1][1]->SaveResult();
	hIMParticleDicedCutIM[2][1]->SaveResult();
	hProtonThetaDiffCutIM[0][1]->SaveResult();
	hProtonThetaDiffCutIM[1][1]->SaveResult();
	hProtonThetaDiffCutIM[2][1]->SaveResult();
	hProtonPhiDiffCutIM[0][1]->SaveResult();
	hProtonPhiDiffCutIM[1][1]->SaveResult();
	hProtonPhiDiffCutIM[2][1]->SaveResult();
	hProtonAngleDiffCutIM[0][1]->SaveResult();
	hProtonAngleDiffCutIM[1][1]->SaveResult();
	hProtonAngleDiffCutIM[2][1]->SaveResult();
	
	hIMParticleCutIM[0][1]->SaveSubs();
	hIMParticleCutIM[1][1]->SaveSubs();
	hIMParticleCutIM[2][1]->SaveSubs();
	hIMParticleDicedCutIM[0][1]->SaveSubs();
	hIMParticleDicedCutIM[1][1]->SaveSubs();
	hIMParticleDicedCutIM[2][1]->SaveSubs();
	hProtonThetaDiffCutIM[0][1]->SaveSubs();
	hProtonThetaDiffCutIM[1][1]->SaveSubs();
	hProtonThetaDiffCutIM[2][1]->SaveSubs();
	hProtonPhiDiffCutIM[0][1]->SaveSubs();
	hProtonPhiDiffCutIM[1][1]->SaveSubs();
	hProtonPhiDiffCutIM[2][1]->SaveSubs();
	hProtonAngleDiffCutIM[0][1]->SaveSubs();
	hProtonAngleDiffCutIM[1][1]->SaveSubs();
	hProtonAngleDiffCutIM[2][1]->SaveSubs();
	
	result->cd();
	result->mkdir("CutIMEtaP");
	result->cd("CutIMEtaP");
	hCount[3]->Write();
	hIMParticleCutIM[0][2]->SaveResult();
	hIMParticleCutIM[1][2]->SaveResult();
	hIMParticleCutIM[2][2]->SaveResult();
	hIMParticleDicedCutIM[0][2]->SaveResult();
	hIMParticleDicedCutIM[1][2]->SaveResult();
	hIMParticleDicedCutIM[2][2]->SaveResult();
	hProtonThetaDiffCutIM[0][2]->SaveResult();
	hProtonThetaDiffCutIM[1][2]->SaveResult();
	hProtonThetaDiffCutIM[2][2]->SaveResult();
	hProtonPhiDiffCutIM[0][2]->SaveResult();
	hProtonPhiDiffCutIM[1][2]->SaveResult();
	hProtonPhiDiffCutIM[2][2]->SaveResult();
	hProtonAngleDiffCutIM[0][2]->SaveResult();
	hProtonAngleDiffCutIM[1][2]->SaveResult();
	hProtonAngleDiffCutIM[2][2]->SaveResult();
	
	hIMParticleCutIM[0][2]->SaveSubs();
	hIMParticleCutIM[1][2]->SaveSubs();
	hIMParticleCutIM[2][2]->SaveSubs();
	hIMParticleDicedCutIM[0][2]->SaveSubs();
	hIMParticleDicedCutIM[1][2]->SaveSubs();
	hIMParticleDicedCutIM[2][2]->SaveSubs();
	hProtonThetaDiffCutIM[0][2]->SaveSubs();
	hProtonThetaDiffCutIM[1][2]->SaveSubs();
	hProtonThetaDiffCutIM[2][2]->SaveSubs();
	hProtonPhiDiffCutIM[0][2]->SaveSubs();
	hProtonPhiDiffCutIM[1][2]->SaveSubs();
	hProtonPhiDiffCutIM[2][2]->SaveSubs();
	hProtonAngleDiffCutIM[0][2]->SaveSubs();
	hProtonAngleDiffCutIM[1][2]->SaveSubs();
	hProtonAngleDiffCutIM[2][2]->SaveSubs();
	
	result->cd();
	result->mkdir("CutAngle");
	result->cd("CutAngle");
	hCount[4]->Write();
	hIMParticleCutAngle[0]->SaveResult();
	hIMParticleCutAngle[1]->SaveResult();
	hIMParticleCutAngle[2]->SaveResult();
	hIMParticleDicedCutAngle[0]->SaveResult();
	hIMParticleDicedCutAngle[1]->SaveResult();
	hIMParticleDicedCutAngle[2]->SaveResult();
	hProtonThetaDiffCutAngle[0]->SaveResult();
	hProtonThetaDiffCutAngle[1]->SaveResult();
	hProtonThetaDiffCutAngle[2]->SaveResult();
	hProtonPhiDiffCutAngle[0]->SaveResult();
	hProtonPhiDiffCutAngle[1]->SaveResult();
	hProtonPhiDiffCutAngle[2]->SaveResult();
	hProtonAngleDiffCutAngle[0]->SaveResult();
	hProtonAngleDiffCutAngle[1]->SaveResult();
	hProtonAngleDiffCutAngle[2]->SaveResult();
	
	hIMParticleCutAngle[0]->SaveSubs();
	hIMParticleCutAngle[1]->SaveSubs();
	hIMParticleCutAngle[2]->SaveSubs();
	hIMParticleDicedCutAngle[0]->SaveSubs();
	hIMParticleDicedCutAngle[1]->SaveSubs();
	hIMParticleDicedCutAngle[2]->SaveSubs();
	hProtonThetaDiffCutAngle[0]->SaveSubs();
	hProtonThetaDiffCutAngle[1]->SaveSubs();
	hProtonThetaDiffCutAngle[2]->SaveSubs();
	hProtonPhiDiffCutAngle[0]->SaveSubs();
	hProtonPhiDiffCutAngle[1]->SaveSubs();
	hProtonPhiDiffCutAngle[2]->SaveSubs();
	hProtonAngleDiffCutAngle[0]->SaveSubs();
	hProtonAngleDiffCutAngle[1]->SaveSubs();
	hProtonAngleDiffCutAngle[2]->SaveSubs();
	
	result->cd();
	result->mkdir("CutAllPi0");
	result->cd("CutAllPi0");
	hCount[5]->Write();
	hIMParticleCutAll[0][0]->SaveResult();
	hIMParticleCutAll[1][0]->SaveResult();
	hIMParticleCutAll[2][0]->SaveResult();
	hIMParticleDicedCutAll[0][0]->SaveResult();
	hIMParticleDicedCutAll[1][0]->SaveResult();
	hIMParticleDicedCutAll[2][0]->SaveResult();
	hProtonThetaDiffCutAll[0][0]->SaveResult();
	hProtonThetaDiffCutAll[1][0]->SaveResult();
	hProtonThetaDiffCutAll[2][0]->SaveResult();
	hProtonPhiDiffCutAll[0][0]->SaveResult();
	hProtonPhiDiffCutAll[1][0]->SaveResult();
	hProtonPhiDiffCutAll[2][0]->SaveResult();
	hProtonAngleDiffCutAll[0][0]->SaveResult();
	hProtonAngleDiffCutAll[1][0]->SaveResult();
	hProtonAngleDiffCutAll[2][0]->SaveResult();
	
	hIMParticleCutAll[0][0]->SaveSubs();
	hIMParticleCutAll[1][0]->SaveSubs();
	hIMParticleCutAll[2][0]->SaveSubs();
	hIMParticleDicedCutAll[0][0]->SaveSubs();
	hIMParticleDicedCutAll[1][0]->SaveSubs();
	hIMParticleDicedCutAll[2][0]->SaveSubs();
	hProtonThetaDiffCutAll[0][0]->SaveSubs();
	hProtonThetaDiffCutAll[1][0]->SaveSubs();
	hProtonThetaDiffCutAll[2][0]->SaveSubs();
	hProtonPhiDiffCutAll[0][0]->SaveSubs();
	hProtonPhiDiffCutAll[1][0]->SaveSubs();
	hProtonPhiDiffCutAll[2][0]->SaveSubs();
	hProtonAngleDiffCutAll[0][0]->SaveSubs();
	hProtonAngleDiffCutAll[1][0]->SaveSubs();
	hProtonAngleDiffCutAll[2][0]->SaveSubs();
	
	result->cd();
	result->mkdir("CutAllEta");
	result->cd("CutAllEta");
	hCount[6]->Write();
	hIMParticleCutAll[0][1]->SaveResult();
	hIMParticleCutAll[1][1]->SaveResult();
	hIMParticleCutAll[2][1]->SaveResult();
	hIMParticleDicedCutAll[0][1]->SaveResult();
	hIMParticleDicedCutAll[1][1]->SaveResult();
	hIMParticleDicedCutAll[2][1]->SaveResult();
	hProtonThetaDiffCutAll[0][1]->SaveResult();
	hProtonThetaDiffCutAll[1][1]->SaveResult();
	hProtonThetaDiffCutAll[2][1]->SaveResult();
	hProtonPhiDiffCutAll[0][1]->SaveResult();
	hProtonPhiDiffCutAll[1][1]->SaveResult();
	hProtonPhiDiffCutAll[2][1]->SaveResult();
	hProtonAngleDiffCutAll[0][1]->SaveResult();
	hProtonAngleDiffCutAll[1][1]->SaveResult();
	hProtonAngleDiffCutAll[2][1]->SaveResult();
	
	hIMParticleCutAll[0][1]->SaveSubs();
	hIMParticleCutAll[1][1]->SaveSubs();
	hIMParticleCutAll[2][1]->SaveSubs();
	hIMParticleDicedCutAll[0][1]->SaveSubs();
	hIMParticleDicedCutAll[1][1]->SaveSubs();
	hIMParticleDicedCutAll[2][1]->SaveSubs();
	hProtonThetaDiffCutAll[0][1]->SaveSubs();
	hProtonThetaDiffCutAll[1][1]->SaveSubs();
	hProtonThetaDiffCutAll[2][1]->SaveSubs();
	hProtonPhiDiffCutAll[0][1]->SaveSubs();
	hProtonPhiDiffCutAll[1][1]->SaveSubs();
	hProtonPhiDiffCutAll[2][1]->SaveSubs();
	hProtonAngleDiffCutAll[0][1]->SaveSubs();
	hProtonAngleDiffCutAll[1][1]->SaveSubs();
	hProtonAngleDiffCutAll[2][1]->SaveSubs();
	
	result->cd();
	result->mkdir("CutAllEtaP");
	result->cd("CutAllEtaP");
	hCount[7]->Write();
	hIMParticleCutAll[0][2]->SaveResult();
	hIMParticleCutAll[1][2]->SaveResult();
	hIMParticleCutAll[2][2]->SaveResult();
	hIMParticleDicedCutAll[0][2]->SaveResult();
	hIMParticleDicedCutAll[1][2]->SaveResult();
	hIMParticleDicedCutAll[2][2]->SaveResult();
	hProtonThetaDiffCutAll[0][2]->SaveResult();
	hProtonThetaDiffCutAll[1][2]->SaveResult();
	hProtonThetaDiffCutAll[2][2]->SaveResult();
	hProtonPhiDiffCutAll[0][2]->SaveResult();
	hProtonPhiDiffCutAll[1][2]->SaveResult();
	hProtonPhiDiffCutAll[2][2]->SaveResult();
	hProtonAngleDiffCutAll[0][2]->SaveResult();
	hProtonAngleDiffCutAll[1][2]->SaveResult();
	hProtonAngleDiffCutAll[2][2]->SaveResult();
	
	hIMParticleCutAll[0][2]->SaveSubs();
	hIMParticleCutAll[1][2]->SaveSubs();
	hIMParticleCutAll[2][2]->SaveSubs();
	hIMParticleDicedCutAll[0][2]->SaveSubs();
	hIMParticleDicedCutAll[1][2]->SaveSubs();
	hIMParticleDicedCutAll[2][2]->SaveSubs();
	hProtonThetaDiffCutAll[0][2]->SaveSubs();
	hProtonThetaDiffCutAll[1][2]->SaveSubs();
	hProtonThetaDiffCutAll[2][2]->SaveSubs();
	hProtonPhiDiffCutAll[0][2]->SaveSubs();
	hProtonPhiDiffCutAll[1][2]->SaveSubs();
	hProtonPhiDiffCutAll[2][2]->SaveSubs();
	hProtonAngleDiffCutAll[0][2]->SaveSubs();
	hProtonAngleDiffCutAll[1][2]->SaveSubs();
	hProtonAngleDiffCutAll[2][2]->SaveSubs();

	result->Close();
	delete result;
}
