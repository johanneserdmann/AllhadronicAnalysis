int includeBranchCheck(TString headerName);
int includeLibrary(TString headerName, TString libraryName);
int rm(TString fileName);

int makeClass(TString fileName, TString treeName) {

  TFile * f = TFile::Open(fileName);
  if(!f->IsOpen()) {
    printf("ERROR file %s not found\n",fileName);
    return 1;
  }

  TTree * tree = (TTree*)f->Get(treeName);
  if(!tree) {
    printf("ERROR tree %s not found\n",treeName);
    return 1;
  }

  tree->MakeClass(treeName);

  if (includeLibrary(treeName + ".h", "<vector>") != 0)
    return 1;

  if (includeBranchCheck(treeName + ".h") != 0)
    return 1;

  if (rm(treeName + ".C") != 0)
    return 1;

  return 0;
}

int includeLibrary(TString headerName, TString libraryName) {
  TString command = "echo ";
  command += headerName;
  command += " | xargs perl -p -i -e \"s/#include <TFile.h>/#include <TFile.h>\\n#include ";
  command += libraryName;
  command += "\\n\\nusing namespace std;/\"";
  gSystem->Exec(command);
  return 0;
}

int includeBranchCheck(TString headerName) {
  TString command = "scripts/insertBranchCheck.sh ";
  command += headerName;
  gSystem->Exec(command);
  return 0;
}

int rm(TString fileName) {
  TString command = "rm ";
  command += fileName;
  gSystem->Exec(command);
  return 0;
}
