// SimpleTimeShowerCustomized.h is a part of the PYTHIA event generator.
// Copyright (C) 2023 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Header file for the original simple timelike final-state showers.
// TimeDipoleEnd: data on a radiating dipole end in FSR.
// SimpleTimeShowerCustomized: handles the showering description.

#ifndef Pythia8_SimpleTimeShowerCustomized_H
#define Pythia8_SimpleTimeShowerCustomized_H

#include "Pythia8/ShowerModel.h"
#include "Pythia8/TimeShower.h"
#include "Pythia8/SimpleTimeShower.h"
#include "Pythia8/SimpleWeakShowerMEs.h"

namespace Pythia8 {

// Forward declaration of SplitOnia;
class SplitOnia;

//==========================================================================

// Data on radiating dipole ends; only used inside SimpleTimeShowerCustomized class.

//==========================================================================

// The SimpleTimeShowerCustomized class does timelike showers.

class SimpleTimeShowerCustomized : public TimeShower {

public:

  static void addParameters(Settings& settings);
  
  // Constructor.
  SimpleTimeShowerCustomized() : hasWeaklyRadiated(), iSysSel(), pTmaxFudge(),
    pTLastBranch(), doQCDshower(), doQEDshowerByQ(), doQEDshowerByL(),
    doQEDshowerByOther(), doQEDshowerByGamma(), doWeakShower(),
    doMEcorrections(), doMEextended(), doMEafterFirst(), doPhiPolAsym(),
    doPhiPolAsymHard(), doInterleave(), doInterleaveResDec(),
    allowBeamRecoil(), dampenBeamRecoil(), useFixedFacScale(),
    allowRescatter(), canVetoEmission(), doHVshower(), brokenHVsym(),
    setLambdaHV(), globalRecoil(), useLocalRecoilNow(), doSecondHard(),
    hasUserHooks(), singleWeakEmission(), alphaSuseCMW(), vetoWeakJets(),
    allowMPIdipole(), weakExternal(), recoilDeadCone(), doDipoleRecoil(),
    doPartonVertex(), pTmaxMatch(), pTdampMatch(), alphaSorder(),
    alphaSnfmax(), nGluonToQuark(), weightGluonToQuark(), recoilStrategyRF(),
    alphaEMorder(), nGammaToQuark(), nGammaToLepton(), nCHV(), nFlavHV(),
    idHV(), alphaHVorder(), nMaxGlobalRecoil(), weakMode(), pTdampFudge(),
    mc(), mb(), m2c(), m2b(), renormMultFac(), factorMultFac(),
    fixedFacScale2(), alphaSvalue(), alphaS2pi(), Lambda3flav(),
    Lambda4flav(), Lambda5flav(), Lambda3flav2(), Lambda4flav2(),
    Lambda5flav2(), scaleGluonToQuark(), extraGluonToQuark(), pTcolCutMin(),
    pTcolCut(), pT2colCut(), pTchgQCut(), pT2chgQCut(), pTchgLCut(),
    pT2chgLCut(), pTweakCut(), pT2weakCut(), mMaxGamma(), m2MaxGamma(),
    mZ(), gammaZ(), thetaWRat(),
    mW(), gammaW(), CFHV(), alphaHVfix(), alphaHVref(), LambdaHV(),
    pThvCut(), pT2hvCut(), mHV(), pTmaxFudgeMPI(), weakEnhancement(),
    vetoWeakDeltaR2(), twoHard(), dopTlimit1(), dopTlimit2(), dopTdamp(),
    pT2damp(), kRad(), kEmt(), pdfScale2(), doTrialNow(), canEnhanceEmission(),
    canEnhanceTrial(), canEnhanceET(), doUncertaintiesNow(), dipSel(),
    iDipSel(), nHard(), nFinalBorn(), nMaxGlobalBranch(), nGlobal(),
    globalRecoilMode(), limitMUQ(), weakHardSize() { beamOffset = 0;
    pdfMode = 0; useSystems = true; }

  // Destructor.
  virtual ~SimpleTimeShowerCustomized() override {}

  // Initialize alphaStrong and related pTmin parameters.
  virtual void init( BeamParticle* beamAPtrIn = 0,
    BeamParticle* beamBPtrIn = 0) override;

  // Find whether to limit maximum scale of emissions, and whether to dampen.
  virtual bool limitPTmax( Event& event, double Q2Fac = 0.,
    double Q2Ren = 0.) override;

  // Top-level routine to do a full time-like shower in resonance decay.
  virtual int shower( int iBeg, int iEnd, Event& event, double pTmax,
    int nBranchMax = 0) override;

  // Top-level routine for QED radiation in hadronic decay to two leptons.
  virtual int showerQED( int i1, int i2, Event& event, double pTmax) override;

  // Prepare process-level event for shower + interleaved resonance decays.
  // Usage: prepareProcess( process, event, iPos).
  // iPos provides mapping from process to event entries (before showering).
  virtual void prepareProcess( Event& process, Event&, vector<int>&) override;

  // Global recoil: reset counters and store locations of outgoing partons.
  virtual void prepareGlobal( Event& event) override;

  // Prepare system for evolution after each new interaction; identify ME.
  virtual void prepare( int iSys, Event& event, bool limitPTmaxIn = true)
    override;

  // Update dipole list after a multiparton interactions rescattering.
  virtual void rescatterUpdate( int iSys, Event& event) override;

  // Update dipole list after each ISR emission.
  virtual void update( int iSys, Event& event, bool hasWeakRad = false)
    override;

  // Select next pT for FSR in downwards evolution.
  virtual double pTnext( Event& event, double pTbegAll, double pTendAll,
    bool isFirstTrial = false, bool doTrialIn = false) override;
  virtual double pTnext_custom(Event &event, double pTbegAll, double pTendAll,
                        bool isFirstTrial = false, bool doTrialIn = false);

  // Select next pT for interleaved resonance decays.
  virtual double pTnextResDec() override;

  // ME corrections and kinematics that may give failure.
  virtual bool branch( Event& event, bool isInterleaved = false) override;

  // Do a resonance decay + resonance shower (including any nested decays).
  // May be called recursively for nested decays.
  // Usage: resonanceShower( process, event, iPos, pTmerge), where iPos
  // maps process to event entries, and pTmerge is the scale at which this
  // system should be merged into its parent system.
  virtual bool resonanceShower(Event& process, Event& event,
    vector<int>& iPos, double qRestart) override;

  // Print dipole list; for debug mainly.
  virtual void list() const override;

  // Initialize data members for calculation of uncertainty bands.
  virtual bool initUncertainties() override;

  // Initialize data members for application of enhancements.
  virtual bool initEnhancements() override;

  // Tell whether FSR has done a weak emission.
  virtual bool getHasWeaklyRadiated() override {return hasWeaklyRadiated;}

  // Tell which system was the last processed one.
  virtual int system() const override {return iSysSel;}

  // Potential enhancement factor of pTmax scale for hardest emission.
  virtual double enhancePTmax() override {return pTmaxFudge;}

  // Provide the pT scale of the last branching in the above shower.
  virtual double pTLastInShower() override {return pTLastBranch;}

  // Functions to directly extract the probability of no emission between two
  // scales. These functions are not used in the Pythia core code, but can be
  // used by external programs to interface with the shower directly.
  double noEmissionProbability( double pTbegAll, double pTendAll, double m2dip,
    int id, int type, double s = -1., double x = -1.) override;
  double pTnext( vector<TimeDipoleEnd> dipEnds, Event event, double pTbegAll,
    double pTendAll, double m2dip, int id, int type, double s = -1.,
    double x = -1.);
  int pdfMode;
  bool useSystems;

private:

  // Constants: could only be changed in the code itself.
  static const double MCMIN, MBMIN, SIMPLIFYROOT, XMARGIN, XMARGINCOMB,
         WTRATIOMAX,TINYPDF, LARGEM2, THRESHM2, LAMBDA3MARGIN1ORD,
         LAMBDA3MARGIN2ORD,WEAKPSWEIGHT, WG2QEXTRA, REJECTFACTOR,
         PROBLIMIT;
  static const int NLOOPMAX;
  // Rescatter: try to fix up recoil between systems
  static const bool   FIXRESCATTER, VETONEGENERGY;
  static const double MAXVIRTUALITYFRACTION, MAXNEGENERGYFRACTION;

  // Store properties to be returned by methods.
  bool   hasWeaklyRadiated;
  int    iSysSel;
  double pTmaxFudge, pTLastBranch;

  // Initialization data, normally only set once.
  bool   doQCDshower, doQEDshowerByQ, doQEDshowerByL, doQEDshowerByOther,
         doQEDshowerByGamma, doWeakShower, doMEcorrections, doMEextended,
         doMEafterFirst, doPhiPolAsym, doPhiPolAsymHard, doInterleave,
         doInterleaveResDec, allowBeamRecoil, dampenBeamRecoil,
         useFixedFacScale, allowRescatter, canVetoEmission, doHVshower,
         brokenHVsym, setLambdaHV, globalRecoil, useLocalRecoilNow,
         doSecondHard, hasUserHooks, singleWeakEmission, alphaSuseCMW,
         vetoWeakJets, allowMPIdipole, weakExternal, recoilDeadCone,
         doDipoleRecoil, doPartonVertex;
  int    pdfModeSave;
  int    pTmaxMatch, pTdampMatch, alphaSorder, alphaSnfmax, nGluonToQuark,
         weightGluonToQuark, recoilStrategyRF, alphaEMorder, nGammaToQuark,
         nGammaToLepton, nCHV, nFlavHV, idHV, alphaHVorder, nMaxGlobalRecoil,
         weakMode;
  double pTdampFudge, mc, mb, m2c, m2b, renormMultFac, factorMultFac,
         fixedFacScale2, alphaSvalue, alphaS2pi, Lambda3flav, Lambda4flav,
         Lambda5flav, Lambda3flav2, Lambda4flav2, Lambda5flav2,
         scaleGluonToQuark, extraGluonToQuark, pTcolCutMin, pTcolCut,
         pT2colCut, pTchgQCut, pT2chgQCut, pTchgLCut, pT2chgLCut,
         pTweakCut, pT2weakCut, mMaxGamma, m2MaxGamma,
         mZ, gammaZ, thetaWRat, mW, gammaW, CFHV,
         alphaHVfix, alphaHVref, LambdaHV, pThvCut, pT2hvCut, mHV,
         pTmaxFudgeMPI, weakEnhancement, vetoWeakDeltaR2;

  // alphaStrong, alphaEM and alpha_HV calculations.
  AlphaStrong alphaS;
  AlphaEM     alphaEM;
  AlphaSUN    alphaHV;

  // Weak matrix elements used for corrections both of ISR and FSR.
  SimpleWeakShowerMEs  simpleWeakShowerMEs;

  // Some current values.
  bool   twoHard, dopTlimit1, dopTlimit2, dopTdamp;
  double pT2damp, kRad, kEmt, pdfScale2;

  // Bookkeeping of enhanced  actual or trial emissions (see EPJC (2013) 73).
  bool doTrialNow, canEnhanceEmission, canEnhanceTrial, canEnhanceET,
       doUncertaintiesNow;
  string splittingNameNow, splittingNameSel;
  map< double, pair<string,double> > enhanceFactors;
  void storeEnhanceFactor(double pT2, string name, double enhanceFactorIn)
    { enhanceFactors.insert(make_pair(pT2,make_pair(name,enhanceFactorIn)));}

  // All dipole ends and a pointer to the selected hardest dipole end.
  vector<TimeDipoleEnd> dipEnd;
  TimeDipoleEnd* dipSel;
  int iDipSel;

  // Setup a dipole end, either QCD, QED/photon, weak or Hidden Valley one.
  void setupQCDdip( int iSys, int i, int colTag,  int colSign, Event& event,
    bool isOctetOnium = false, bool limitPTmaxIn = true);
  void setupQEDdip( int iSys, int i, int chgType, int gamType, Event& event,
    bool limitPTmaxIn = true);
  void setupWeakdip( int iSys, int i,int weakType, Event& event,
    bool limitPTmaxIn = true);
  // Special setup for weak dipoles if already specified in info ptr.
  void setupWeakdipExternal(Event& event, bool limitPTmaxIn = true);
  void setupHVdip( int iSys, int i, int colvType, Event& event,
    bool limitPTmaxIn = true);

  // Special setup for onium.
  void regenerateOniumDipoles(Event & event);

  // Evolve a QCD dipole end.
  void pT2nextQCD( double pT2begDip, double pT2sel, TimeDipoleEnd& dip,
    Event& event);

  // Evolve a QED dipole end, either charged or photon.
  void pT2nextQED( double pT2begDip, double pT2sel, TimeDipoleEnd& dip,
    Event& event);

  // Evolve a weak dipole end.
  void pT2nextWeak( double pT2begDip, double pT2sel, TimeDipoleEnd& dip,
    Event& event);

  // Evolve a Hidden Valley dipole end.
  void pT2nextHV( double pT2begDip, double pT2sel, TimeDipoleEnd& dip,
    Event& );

  // Evolve an onium dipole end.
  void pT2nextOnium( double pT2begDip, double pT2sel, TimeDipoleEnd& dip,
    Event& );

  // Find kind of QCD ME correction.
  void findMEtype( Event& event, TimeDipoleEnd& dip);

  // Find type of particle; used by findMEtype.
  int findMEparticle( int id, bool isHiddenColour = false);

  // Find mixture of V and A in gamma/Z: energy- and flavour-dependent.
  double gammaZmix( Event& event, int iRes, int iDau1, int iDau2);

  // Set up to calculate QCD ME correction with calcMEcorr.
  double findMEcorr(TimeDipoleEnd* dip, Particle& rad, Particle& partner,
   Particle& emt, bool cutEdge = true);

  // Set up to calculate weak ME corrections for t-channel processes.
  double findMEcorrWeak(TimeDipoleEnd* dip, Vec4 rad, Vec4 rec,
   Vec4 emt, Vec4 p3, Vec4 p4, Vec4 radBef, Vec4 recBef);

  // Calculate value of QCD ME correction.
  double calcMEcorr( int kind, int combiIn, double mixIn, double x1,
    double x2, double r1, double r2, double r3 = 0., bool cutEdge = true);

  // Find coefficient of azimuthal asymmetry from gluon polarization.
  void findAsymPol( Event& event, TimeDipoleEnd* dip);

  // Compute scale for interleaved resonance decays
  double calcPTresDec(Particle& res);

  // Rescatter: propagate dipole recoil to internal lines connecting systems.
  bool rescatterPropagateRecoil( Event& event, Vec4& pNew);

  // Properties stored for (some) global recoil schemes.
  // Vectors of event indices defining the hard process.
  vector<int> hardPartons;
  // Number of partons in current hard event, number of partons in Born-type
  // hard event (to distinguish between S and H), maximally allowed number of
  // global recoil branchings.
  int nHard, nFinalBorn, nMaxGlobalBranch;
  // Number of proposed splittings in hard scattering systems.
  map<int,int> nProposed;
  // Number of splittings with global recoil (currently only 1).
  int nGlobal, globalRecoilMode;
  // Switch to constrain recoiling system.
  bool limitMUQ;

  // Calculate uncertainty-band weights for accepted/rejected trial branching.
  void calcUncertainties( bool accept, double pAccept,
    double enhance, TimeDipoleEnd* dip, Particle* radPtr,
    Particle* emtPtr, Particle* recPtr );

  // 2 -> 2 information needed for the external weak setup.
  vector<Vec4> weakMomenta;
  vector<int> weak2to2lines;
  int weakHardSize;

  // Settings and member variables for interleaved resonance decays.
  bool doFSRinResonances{};
  int resDecScaleChoice{-1}, iHardResDecSav{}, nRecurseResDec{};
  vector<int> idResDecSav;
  vector<double> pTresDecSav;

  // Onium emissions.
  bool doOniumShower{false};
  vector<SplitOniaPtr> oniumEmissions;
  set<double> oniumThresholds;

  // 🌟 New Configurable Parameters
  double zBias = 0.0;
  double coherenceFactor = 0.0;
  double azimuthalBias = 0.0;
  double recoilWeight = 0.0;
  double angleBias = 0.0;
  double alphaModifier = 0.0;

  // New Custom Methods
  void loadCustomSettings(); // ✅ Explicitly declared here
  double getSplittingZ();
  double computeSudakov(double theta, double z);
};

//==========================================================================

} // end namespace Pythia8

#endif // Pythia8_SimpleTimeShowerCustomized_H
