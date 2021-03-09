#include "DTNtupleTPGSimAnalyzer.h"
#include "TVector2.h"
#include "TF1.h"
#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"

using namespace RooFit ;


DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
                                               const TString & outFileName):
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)
{

  m_minMuPt = 0;
  m_maxMuPt = 20;

  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;

  m_minSegHits = 4;

  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;

  numberOfEntries = 0; 
}



DTNtupleTPGSimAnalyzer::~DTNtupleTPGSimAnalyzer()
{

}



void DTNtupleTPGSimAnalyzer::Loop()
{

  book();

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  numberOfEntries = nentries; 
  cout << "numberOfEntries: " << numberOfEntries << endl; 

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++)
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0)
  std::cout << "[DTNtupleTPGSimAnalyzer::Loop] processed : "
      << jentry << " entries\r" << std::flush;

      fill();

    }

  std::cout << std::endl;

  endJob();
}



void DTNtupleTPGSimAnalyzer::book()
{
  m_outFile.cd();
  std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  std::vector<std::string> slTags   = { "Sl1", "Sl2", "Sl3"};
  std::vector<std::string> layerTags = { "La1", "La2", "La3", "La4"};

  m_plots["DigiDistr"] = new TH1F("DigiDistr", "Distribution of phi Hits; Number of phi hits; ", 61 ,-0.5, 60.5 );

  for (const auto & chambTag : chambTags){
    m_plots2["hits" + chambTag] = new TH2F(("hits_"  + chambTag).c_str(),
									(" Number of hits in " + chambTag + "; Sector ; Wheel").c_str(),
	  							        12, 0.5, 12.5, 5, -2.5, 2.5);
    for (const auto & wheelTag : whTags){
      for (const auto & secTag : secTags){
        m_plots["hits" + wheelTag + secTag + chambTag] = new TH1F(("hits_"  + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
									(" Number of hits in " + wheelTag + " " +  secTag + " " + chambTag + "; ; Number of hits").c_str(),
	  							        1, -0.5, 0.5);
        for (const auto & slTag : slTags){
          for (const auto & layerTag : layerTags){
            m_plots["hits" + wheelTag + secTag + chambTag + slTag + layerTag] = new TH1F(("hits_"  + wheelTag + "_" + secTag + "_" + chambTag + "_" + slTag + "_" + layerTag).c_str(),
									(" Number of hits in " + wheelTag + " " +  secTag + " " + chambTag + " " + slTag + " " + layerTag + "; ; Number of hits").c_str(),
	  							        101, -0.5, 100.5);
	  }
        }
      } 
    }
  

    for (size_t i = 0; i<whTags.size(); i++){
      m_plots2["hits" + chambTag] -> GetYaxis()->SetBinLabel(i+1, whTags.at(i).c_str());
    }
    for (size_t i = 0; i<secTags.size(); i++){
      m_plots2["hits" + chambTag] -> GetXaxis()->SetBinLabel(i+1, secTags.at(i).c_str());
    }

  }

} // book



void DTNtupleTPGSimAnalyzer::fill()
{
  int goodBX = 20; 
 
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  std::vector<std::string> slTags   = { "Sl1", "Sl2", "Sl3"};
  std::vector<std::string> layerTags = { "La1", "La2", "La3", "La4"};


      std::map <std::string, int> digis; 

      for (std::size_t iDigi = 0; iDigi < ph2Digi_nDigis; ++iDigi){
        auto mySector = ph2Digi_sector->at(iDigi); if (mySector==13) mySector=4; if (mySector==14) mySector=10;
        auto myWheel = ph2Digi_wheel->at(iDigi);
        auto myStation = ph2Digi_station->at(iDigi);
        auto myLayer = ph2Digi_layer->at(iDigi);
        auto mySuperlayer = ph2Digi_superLayer->at(iDigi);
        auto myWire = ph2Digi_wire->at(iDigi);

        m_plots2["hits" + chambTags.at(myStation-1)] -> Fill (mySector, myWheel);
        m_plots["hits" + whTags.at(myWheel+2) + secTags.at(mySector-1) + chambTags.at(myStation-1)] -> Fill (0);
        m_plots["hits" + whTags.at(myWheel+2) + secTags.at(mySector-1) + chambTags.at(myStation-1)+ slTags.at(mySuperlayer-1)+ layerTags.at(myLayer-1)] -> Fill (myWire);
        if (mySuperlayer == 2) continue; 
        if (digis[whTags.at(myWheel+2) + secTags.at(mySector-1) + chambTags.at(myStation-1)]) {
          digis[whTags.at(myWheel+2) + secTags.at(mySector-1) + chambTags.at(myStation-1)]++;
        } else {
          digis[whTags.at(myWheel+2) + secTags.at(mySector-1) + chambTags.at(myStation-1)]=1;
        }
      }

      for (auto & digi : digis) {
        m_plots["DigiDistr"] -> Fill(digi.second);
      }

}




void DTNtupleTPGSimAnalyzer::endJob()
{
  std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  std::vector<std::string> slTags   = { "Sl1", "Sl2", "Sl3"};
  std::vector<std::string> layerTags = { "La1", "La2", "La3", "La4"};
  for (unsigned int i = 0; i<chambTags.size(); i++){
    auto chambTag = chambTags.at(i);  
    m_plots2["hits" + chambTag]->Scale((1. / (double) numberOfEntries));
    for (unsigned int j = 0; j<secTags.size(); j++){
      auto secTag = secTags.at(j);  
      for (unsigned int k = 0; k<whTags.size(); k++){
        auto wheelTag = whTags.at(k); 
        m_plots["hits" + wheelTag + secTag + chambTag]->Scale((1. / (double) numberOfEntries));
	    int channels = 0; 
        for (const auto & slTag : slTags){
          for (const auto & layerTag : layerTags){
            m_plots["hits" + wheelTag + secTag + chambTag + slTag + layerTag]->Scale((1. / (double) numberOfEntries));
	        for (int bin = 100; bin>0; bin--){
              if ( m_plots["hits" + wheelTag + secTag + chambTag + slTag + layerTag]->GetBinContent(bin) !=0) {
                channels += bin;
		        break;
	          }	
	        } 
          }
        }
	    m_plots2["hits" + chambTag]->SetBinContent( j+1, k+1, m_plots2["hits" + chambTag]->GetBinContent(j+1, k+1) * 1E6 / (1000*channels));
      } // for wh
    } // for sec
  } // for chamb
  


  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();
}



Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1);
}
