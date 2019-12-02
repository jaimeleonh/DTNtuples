#include "TROOT.h"

void loadTPGSimAnalysis_Rates()
{
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Rates.C++");
}
