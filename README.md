AllhadronicAnalysis
===================
# make makeClass.sh script executable
source setup.sh

# use file with most recent tree format to remake header file DelphesNtup.h
./scripts/makeClass.sh <example root file>

# compile
make

# run analysis code
# (for example : ./run.exe txt/test.txt testoutput.root
#  note        : will not work if input/test.root does not have the most recent format of the tree)
./run.exe <input text file> <output root file>
