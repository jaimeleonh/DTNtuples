#include "TROOT.h"

void loadTPGSimAnalysis_Res()
{
  gROOT->ProcessLine(".L DTNtupleBaseAnalyzer.C++");
  //gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Efficiency.C++");
  gROOT->ProcessLine(".L DTNtupleTPGSimAnalyzer_Resolution.C++");
}
