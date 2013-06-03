#include "Analysis.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

Analysis::Analysis() {
  fChain = new TChain("DelphesNtup");
  fClearLine = "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b";
}

Analysis::~Analysis() {
  delete fChain;
  delete fDelphes;
}

void Analysis::add(TString fileName) {
  fChain->AddFile(fileName);
}

void Analysis::init() {
  fDelphes = new DelphesNtup(fChain);
  fDelphes->fChain->SetBranchStatus("*", true);
}

void Analysis::xpt(TAxis * a) {
  double x[11] = { 150., 250., 350., 450., 600., 800., 1000., 1250., 1500., 2000., 3000. };
  a->Set(10, x);
}

void Analysis::setaxistitles(TH1 * h, TString x, TString y) {
  h->GetXaxis()->SetTitle(x);
  h->GetYaxis()->SetTitle(y);
}

void Analysis::newTH1D(int i, TString title, int n, double a, double b, TString xtitle, TString ytitle, void (Analysis::*func)(TAxis*)) {
  TH1D h(title, title, n, a, b);
  setaxistitles(&h, xtitle, ytitle);
  if (func != 0x0)
    (this->*func)(h.GetXaxis());    
  vTH1D[i] = h;
}

void Analysis::newTH2D(int i, TString title, int nx, double ax, double bx, int ny, double ay, double by, TString xtitle, TString ytitle, void (Analysis::*xfunc)(TAxis*), void (Analysis::*yfunc)(TAxis*)) {
  TH2D h(title, title, nx, ax, bx, ny, ay, by);
  setaxistitles(&h, xtitle, ytitle);
  if (xfunc != 0x0)
    (this->*xfunc)(h.GetXaxis());    
  if (yfunc != 0x0)
    (this->*yfunc)(h.GetYaxis());    
  vTH2D[i] = h;
}

void Analysis::newTProfile(int i, TString title, int n, double a, double b, TString xtitle, TString ytitle, void (Analysis::*func)(TAxis*)) {
  TProfile h(title, title, n, a, b);
  setaxistitles(&h, xtitle, ytitle);
  if (func != 0x0)
    (this->*func)(h.GetXaxis());    
  vTProfile[i] = h;
}

void Analysis::newTProfile2D(int i, TString title, int nx, double ax, double bx, int ny, double ay, double by, TString xtitle, TString ytitle, void (Analysis::*xfunc)(TAxis*), void (Analysis::*yfunc)(TAxis*)) {
  TProfile2D h(title, title, nx, ax, bx, ny, ay, by);
  setaxistitles(&h, xtitle, ytitle);
  if (xfunc != 0x0)
    (this->*xfunc)(h.GetXaxis());    
  if (yfunc != 0x0)
    (this->*yfunc)(h.GetYaxis());    
  vTProfile2D[i] = h;
}

void Analysis::definehistograms() {
  vTH1D.clear();
  vTH2D.clear();
  vTProfile.clear();
  vTProfile2D.clear();

  vTH1D.resize(lastTH1D);
  vTH2D.resize(lastTH2D);
  vTProfile.resize(lastTProfile);
  vTProfile2D.resize(lastTProfile2D);

  // TH1D

  newTH1D(iLeadingJetPt, "LeadingJetPt", 100, 0., 3000., "p_{T} [GeV]", "# events / bin", &Analysis::xpt);

  // TH2D

  // TProfile

  // TProfile2D

}

bool Analysis::isGoodElectron(int i) {
  double pt = (*fDelphes->el_pt)[i];
  double eta = (*fDelphes->el_eta)[i];
  if (pt < 25.) return false;
  if (fabs(eta) > 2.5) return false;
  return true;
}

bool Analysis::isGoodMuon(int i) {
  double pt = (*fDelphes->mu_pt)[i];
  double eta = (*fDelphes->mu_eta)[i];
  if (pt < 25.) return false;
  if (fabs(eta) > 2.5) return false;
  return true;
}

bool Analysis::isGoodFatJet(int i) {
  double pt = (*fDelphes->fatjet_pt)[i];
  double eta = (*fDelphes->fatjet_eta)[i];
  if (pt < 350.) return false;
  if (fabs(eta) > 2.0) return false;
  return true;
}

void Analysis::loop(TString outputName) {

  // initialize histograms
  definehistograms();

  // the event loop
  cout << "running on " << n() << " events" << endl;
  for (int iEntry(0), nEntries(n()); iEntry < nEntries; ++iEntry) {

    
    if ((iEntry+1)%10000 == 0)
      cout << fClearLine << setw(8) << iEntry+1 << flush;

    // get tree entries
    fDelphes->GetEntry(iEntry);

    //////////////////////////////////////////////////////////////////////////////////
    // here goes all the important stuff (event selection, variable calculations etc.)
    //////////////////////////////////////////////////////////////////////////////////

    // event selection : at least two fat jets, no electron, no muon

    int nGoodFatJets(0);
    for (int iFatJet(0), nFatJets(fDelphes->fatjet_n); iFatJet < nFatJets; ++iFatJet) {
      if (isGoodFatJet(iFatJet))
        ++nGoodFatJets;
    }
    if (nGoodFatJets < 2)
      continue;

    int nGoodElectrons(0);
    for (int iElectron(0), nElectrons(fDelphes->el_n); iElectron < nElectrons; ++iElectron) {
      if (isGoodElectron(iElectron))
        ++nGoodElectrons;
    }
    if (nGoodElectrons != 0)
      continue;

    int nGoodMuons(0);
    for (int iMuon(0), nMuons(fDelphes->mu_n); iMuon < nMuons; ++iMuon) {
      if (isGoodMuon(iMuon))
        ++nGoodMuons;
    }
    if (nGoodMuons != 0)
      continue;

    double pt1 = (*fDelphes->fatjet_pt)[0];

    // fill histograms

    vTH1D[iLeadingJetPt].Fill(pt1);

  } // event loop

  // save histograms to one file
  TFile * f = new TFile(outputName, "RECREATE");
  for (unsigned int i(0), n(vTH1D.size()); i < n; ++i)
    vTH1D[i].Write();
  for (unsigned int i(0), n(vTH2D.size()); i < n; ++i)
    vTH2D[i].Write();
  for (unsigned int i(0), n(vTProfile.size()); i < n; ++i)
    vTProfile[i].Write();
  for (unsigned int i(0), n(vTProfile2D.size()); i < n; ++i)
    vTProfile2D[i].Write();
  delete f;
  cout << fClearLine << setw(8) << "saved file " << outputName << endl;
}
