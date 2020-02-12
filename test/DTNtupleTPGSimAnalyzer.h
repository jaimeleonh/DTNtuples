#ifndef DTTnPBaseAnalysis_h
#define DTTnPBaseAnalysis_h

#include "DTNtupleBaseAnalyzer.h"

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TEfficiency.h"

#include <string>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <map>

class DTNtupleTPGSimAnalyzer : public DTNtupleBaseAnalyzer 
{
  
public:
  
  DTNtupleTPGSimAnalyzer(const TString & inFileName,
			 const TString & outFileName);
  DTNtupleTPGSimAnalyzer(const TString & inFileName,
			 const TString & outFileName,
			 const bool & correctL1A);
  ~DTNtupleTPGSimAnalyzer();

  void virtual Loop() override;

protected:
  
  void book();
  void fill();
  void endJob();

private:
  
  Double_t trigPhiInRad(Double_t trigPhi, Int_t sector);
  Int_t qualityGroup(Int_t quality);
  void DisplayPh2Hits () ;  
  void DisplayPh1Segs () ;  
  void getTheStupidPlots () ;  
  TFile m_outFile;
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TEfficiency*> m_effs;
  
  Double_t m_minMuPt;
  
  Double_t m_maxMuSegDPhi;
  Double_t m_maxMuSegDEta;
  
  Int_t m_minSegHits;

  Double_t m_maxSegTrigDPhi;
  Double_t m_maxMuTrigDPhi;

  Long64_t entryNumber;  
  Long64_t totalEntries = 0;  
  bool debug;  
  bool correctL1A; 

  //struct primitive; 
};

#endif
