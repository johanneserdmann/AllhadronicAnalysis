#include "Analysis.h"

#include "Riostream.h"
#include "TString.h"
#include "TFile.h"

#include <iostream>

using namespace std;

int main(int argc, char ** argv) {

  if (argc != 2+1) {
    cout << "wrong number of arguments - usage :" << endl;
    cout << argv[0] << " <input txt file> <output file name>" << endl;
    return 1;
  }

  TString txtFile(argv[1]);
  TString outputFileName(argv[2]);
  cout << "input txt file   = " << txtFile << endl;
  cout << "output file name = " << outputFileName << endl;

  Analysis analysis;

  ifstream in;
  in.open(txtFile);
  while (true) {
    TString fileName;
    in >> fileName;
    if (!in.good()) break;

    TFile * f = TFile::Open(fileName);
    if(!f->IsOpen()) {
      printf("ERROR file %s not found\n", fileName.Data());
      return 1;
    }
    f->Close();

    analysis.add(fileName);
  }
  in.close();

  analysis.init();
  analysis.loop(outputFileName);

  return 0;
}
