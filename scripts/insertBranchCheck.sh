#!/bin/bash

file=$1

perl -p -i -e "s/fChain->SetBranchAddress\(\"(.*)\"/if \(fChain->GetListOfBranches\(\)->Contains\(\"\1\"\)\) fChain->SetBranchAddress\(\"\1\"/" $file

exit 0
