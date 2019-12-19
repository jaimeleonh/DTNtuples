#include "TROOT.h"

void loadTPGSimAnalysis_Digis()
{
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Digis.C++");
}
