#ifndef Analysis_h
#define Analysis_h

#include "DelphesNtup.h"

#include "TString.h"
#include "TChain.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TAxis.h"

#include <vector>

class Analysis {

 public :

  Analysis();
  ~Analysis();

  void add(TString fileName);
  void init();
  void loop(TString outputName);

 private :
  int n() { return fDelphes->fChain->GetEntries(); }

  TChain * fChain;
  DelphesNtup * fDelphes;

  bool isGoodElectron(int i);
  bool isGoodMuon(int i);
  bool isGoodFatJet(int i);

  void definehistograms();
  void setaxistitles(TH1 * h, TString x, TString y);

  void xpt(TAxis * a);

  void newTH1D(int i, TString title, int n, double a, double b, TString xtitle, TString ytitle, void (Analysis::*func)(TAxis*) = 0x0);
  void newTH2D(int i, TString title, int nx, double ax, double bx, int ny, double ay, double by, TString xtitle, TString ytitle, void (Analysis::*xfunc)(TAxis*) = 0x0, void (Analysis::*yfunc)(TAxis*) = 0x0);
  void newTProfile(int i, TString title, int n, double a, double b, TString xtitle, TString ytitle, void (Analysis::*func)(TAxis*) = 0x0);
  void newTProfile2D(int i, TString title, int nx, double ax, double bx, int ny, double ay, double by, TString xtitle, TString ytitle, void (Analysis::*xfunc)(TAxis*) = 0x0, void (Analysis::*yfunc)(TAxis*) = 0x0);

  std::vector<TH1D> vTH1D;
  std::vector<TH2D> vTH2D;
  std::vector<TProfile> vTProfile;
  std::vector<TProfile2D> vTProfile2D;

  string fClearLine;

  enum iTH1D {
    iLeadingJetPt,
    lastTH1D
  };

  enum iTH2D {
    lastTH2D
  };

  enum iTProfile2D {
    lastTProfile2D
  };

  enum iTProfile {
    lastTProfile
  };

};

#endif
