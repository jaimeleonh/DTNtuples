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
  std::vector<std::string> algoTag    = {"AM","HB" };
  std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=3","GoodBX+qu>=3"};

  for (const auto & wheelTag : whTags){
    for (const auto & secTag : secTags){
      for (const auto & algo : algoTag){
        for (const auto & chambTag : chambTags){
          m_plots2["outputPrims" + algo + wheelTag + secTag + chambTag] = new TH2F(("outputPrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
									(algo + " Output primitive number in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Number of primtives").c_str(),
								        8, 0.5, 8.5, 21, -0.5, 20.5);
          m_plots["ratePrim" + algo + wheelTag + secTag + chambTag] = new TH1F(("ratePrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
									(algo + " Primitive rate in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Rate").c_str(),
									8, 0.5, 8.5);
          for (unsigned int i = 0; i < quTags.size(); ++i){
            m_plots["ratePrim" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
            m_plots2["outputPrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
          }
        }
	m_plots["rateAllBXPrim" + algo + wheelTag + secTag] = new TH1F(("rateAllBXPrims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " All BX Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXPrim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXPrims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXIndex0Prim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXIndex0Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX Index 0 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXIndex01Prim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXIndex01Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX Index 01 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXIndex012Prim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXIndex012Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX Index 012 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXIndex0123Prim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXIndex0123Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX Index 0123 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateAllBXqu>=3Prim" + algo + wheelTag + secTag] = new TH1F(("rateAllBXqu>=3Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " All BX qu>=3 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
	m_plots["rateGoodBXqu>=3Prim" + algo + wheelTag + secTag] = new TH1F(("rateGoodBXqu>=3Prims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
									(algo + " Good BX qu>=3 Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
                                                                        4, 0.5, 4.5);
        for (unsigned int i = 0; i < chambTags.size(); ++i){
          m_plots["rateAllBXPrim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXPrim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXIndex0Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXIndex01Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXIndex012Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXIndex0123Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateAllBXqu>=3Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
          m_plots["rateGoodBXqu>=3Prim" + algo + wheelTag + secTag]->GetXaxis()->SetBinLabel(i+1, chambTags[i].c_str());
        }
        
      } 
    }
  }
} // book



void DTNtupleTPGSimAnalyzer::fill()
{
  int goodBX = 20; 
 
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=3","GoodBX+qu>=3"};

  short numberOfAMTotalPrimitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXPrimitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXIndex0Primitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXIndex01Primitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXIndex012Primitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXIndex0123Primitives[5][12][4][quTags.size()]; 
  short numberOfAMTotalQualitygr2Primitives[5][12][4][quTags.size()]; 
  short numberOfAMGoodBXQualitygr2Primitives[5][12][4][quTags.size()]; 
  
  short numberOfHBTotalPrimitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXPrimitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXIndex0Primitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXIndex01Primitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXIndex012Primitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXIndex0123Primitives[5][12][4][quTags.size()]; 
  short numberOfHBTotalQualitygr2Primitives[5][12][4][quTags.size()]; 
  short numberOfHBGoodBXQualitygr2Primitives[5][12][4][quTags.size()]; 
  
  bool AMprimitive[quTags.size()][5][12][4]; 
  bool HBprimitive[quTags.size()][5][12][4]; 

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 12; j++) {
      for (int k = 0; k < 4; k++) {
        for (unsigned int q = 0; q < quTags.size(); q++) {
          numberOfAMTotalPrimitives[i][j][k][q]=0; 
          numberOfAMGoodBXPrimitives[i][j][k][q]=0; 
          numberOfAMGoodBXIndex0Primitives[i][j][k][q]=0; 
          numberOfAMGoodBXIndex01Primitives[i][j][k][q]=0; 
          numberOfAMGoodBXIndex012Primitives[i][j][k][q]=0; 
          numberOfAMGoodBXIndex0123Primitives[i][j][k][q]=0; 
          numberOfAMTotalQualitygr2Primitives[i][j][k][q]=0; 
          numberOfAMGoodBXQualitygr2Primitives[i][j][k][q]=0; 
          
          numberOfHBTotalPrimitives[i][j][k][q]=0; 
          numberOfHBGoodBXPrimitives[i][j][k][q]=0; 
          numberOfHBGoodBXIndex0Primitives[i][j][k][q]=0; 
          numberOfHBGoodBXIndex01Primitives[i][j][k][q]=0; 
          numberOfHBGoodBXIndex012Primitives[i][j][k][q]=0; 
          numberOfHBGoodBXIndex0123Primitives[i][j][k][q]=0; 
          numberOfHBTotalQualitygr2Primitives[i][j][k][q]=0; 
          numberOfHBGoodBXQualitygr2Primitives[i][j][k][q]=0; 
        
          AMprimitive[q][i][j][k]=false; 
          HBprimitive[q][i][j][k]=false; 
        }
      }
    }
  }

      for (std::size_t iTrigHB = 0; iTrigHB < ph2TpgPhiEmuHb_nTrigs; ++iTrigHB){
        Int_t trigHBWh  = ph2TpgPhiEmuHb_wheel->at(iTrigHB);
        Int_t trigHBSec = ph2TpgPhiEmuHb_sector->at(iTrigHB);  if (trigHBSec==13) trigHBSec=4;  if (trigHBSec==14) trigHBSec=10;
        Int_t trigHBSt  = ph2TpgPhiEmuHb_station->at(iTrigHB);
        Int_t trigHBBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB);
        Int_t trigHBindex  = ph2TpgPhiEmuHb_index->at(iTrigHB);
        Int_t trigHBquality  = ph2TpgPhiEmuHb_quality->at(iTrigHB);
        
        numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][0]++;
	HBprimitive[0][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;
	if (trigHBquality >= 4) {HBprimitive[6][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true; numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][6]++;}
        if (trigHBBX == goodBX) {
          numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][1]++;
	  HBprimitive[1][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;  
	  if (trigHBindex <= 1){ HBprimitive[2][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;  numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][2]++;}
	  if (trigHBindex <= 2){ HBprimitive[3][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;  numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][3]++;}
	  if (trigHBindex <= 3){ HBprimitive[4][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;  numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][4]++;}
	  if (trigHBindex <= 4){ HBprimitive[5][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;  numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][5]++;}
	  if (trigHBquality >= 4){ HBprimitive[7][trigHBWh+2][trigHBSec-1][trigHBSt-1]=true;   numberOfHBTotalPrimitives[trigHBWh+2][trigHBSec-1][trigHBSt-1][7]++;}
	}
      }

      for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM){
        Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
        Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM); if (trigAMSec==13) trigAMSec=4;  if (trigAMSec==14) trigAMSec=10;
        Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
        Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
        Int_t trigAMindex  = ph2TpgPhiEmuAm_index->at(iTrigAM);
        Int_t trigAMquality  = ph2TpgPhiEmuAm_quality->at(iTrigAM);

        numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][0]++;
	AMprimitive[0][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;
	if (trigAMquality >= 3) {AMprimitive[6][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true; numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][6]++;}
        if (trigAMBX == goodBX) {
          numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][1]++;
	  AMprimitive[1][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  
	  if (trigAMindex <= 0){ AMprimitive[2][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][2]++;}
	  if (trigAMindex <= 1){ AMprimitive[3][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][3]++;}
	  if (trigAMindex <= 2){ AMprimitive[4][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][4]++;}
	  if (trigAMindex <= 3){ AMprimitive[5][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][5]++;}
	  if (trigAMquality >= 3){ AMprimitive[7][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][7]++;}
	
	}
      }

  for (unsigned int i = 0; i<chambTags.size(); i++){
    auto chambTag = chambTags.at(i);  
    for (unsigned int j = 0; j<secTags.size(); j++){
      auto secTag = secTags.at(j);  
      for (unsigned int k = 0; k<whTags.size(); k++){
        auto wheelTag = whTags.at(k);
        for (unsigned int q = 0; q<quTags.size(); q++){
 
	    if (HBprimitive[q][k][j][i]) 
            { 
              m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->Fill(q+1);
              m_plots2["outputPrimsHB" + wheelTag + secTag + chambTag]->Fill(q+1,numberOfHBTotalPrimitives[k][j][i][q]);
            }
	    if (AMprimitive[q][k][j][i]) 
            { 
              m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->Fill(q+1);
              m_plots2["outputPrimsAM" + wheelTag + secTag + chambTag]->Fill(q+1,numberOfAMTotalPrimitives[k][j][i][q]);
            }
        }
      } // for wh
    } // for sec
  } // for chamb




}




void DTNtupleTPGSimAnalyzer::endJob()
{
  std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
  for (unsigned int j = 0; j<secTags.size(); j++){
    auto secTag = secTags.at(j);  
    for (unsigned int k = 0; k<whTags.size(); k++){
      auto wheelTag = whTags.at(k); 
        for (unsigned int i = 0; i<chambTags.size(); i++){
          auto chambTag = chambTags.at(i);  
          m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->Scale((1. / (double) numberOfEntries) * 2760 * 11246);
          m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->Scale((1. / (double) numberOfEntries) * 2760 * 11246);  
          m_plots["rateAllBXPrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(1)); 
          m_plots["rateGoodBXPrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(2)); 
          m_plots["rateGoodBXIndex0PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(3)); 
          m_plots["rateGoodBXIndex01PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(4)); 
          m_plots["rateGoodBXIndex012PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(5)); 
          m_plots["rateGoodBXIndex0123PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(6)); 
          m_plots["rateAllBXqu>=3PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(7)); 
          m_plots["rateGoodBXqu>=3PrimHB" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimHB" + wheelTag + secTag + chambTag]->GetBinContent(8)); 
          m_plots["rateAllBXPrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(1)); 
          m_plots["rateGoodBXPrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(2)); 
          m_plots["rateGoodBXIndex0PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(3)); 
          m_plots["rateGoodBXIndex01PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(4)); 
          m_plots["rateGoodBXIndex012PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(5)); 
          m_plots["rateGoodBXIndex0123PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(6)); 
          m_plots["rateAllBXqu>=3PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(7)); 
          m_plots["rateGoodBXqu>=3PrimAM" + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->GetBinContent(8)); 
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
