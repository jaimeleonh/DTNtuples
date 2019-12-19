#include "TROOT.h"

void loadTPGSimAnalysis_Effs()
{
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Efficiency.C++");
}
