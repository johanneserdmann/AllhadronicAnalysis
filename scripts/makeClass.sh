#!/bin/bash

INFILE=$1
if [[ ! $INFILE ]]; then
  echo "$0 : no input file specified -> exiting"
  exit 1
fi

echo "gROOT->LoadMacro(\"scripts/makeClass.C\"); makeClass(\"$INFILE\", \"DelphesNtup\")" | root.exe -l -b >> /dev/null

exit 0
