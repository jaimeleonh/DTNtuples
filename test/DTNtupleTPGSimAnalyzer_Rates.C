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
 
 m_maxMuSegDPhi = 0.1;
 m_maxMuSegDEta = 0.15;
 
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
 std::vector<std::string> algoTag    = {"AM"};
 std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
 std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
 std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
 std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=2","GoodBX+qu>=2", "GoodBX+matchedqu<2", "GodBX+qu>=2+RPCseg", "GoodBX+qu>=2+RPCseg+clus","GoodBX+matchedqu<2+RPCseg", "GoodBX+matchedqu<2+RPCseg+clus"};
 
 m_plots["segmentT0"] = new TH1F("segmentT0","segmentT0; segmentT0; entries", 400,-200,200);
 
 for (const auto & wheelTag : whTags){
  for (const auto & secTag : secTags){
   for (const auto & algo : algoTag){
    m_plots["ratePrim" + algo + wheelTag + secTag] = new TH1F(("ratePrims_" + algo + "_" + wheelTag + "_" + secTag).c_str(),
     (algo + " Primitive rate in " + wheelTag + " " +  secTag + "; ; Rate").c_str(),
    quTags.size(), 0.5, (double) (quTags.size()) + 0.5 );
    for (const auto & chambTag : chambTags){
     m_plots2["outputPrims" + algo + wheelTag + secTag + chambTag] = new TH2F(("outputPrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
      (algo + " Output primitive number in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Number of primtives").c_str(),
     quTags.size(), 0.5, (double) (quTags.size()) + 0.5, 21, -0.5, 20.5);
     m_plots["bandwidth" + algo + wheelTag + secTag + chambTag] = new TH1F(("bandwidth_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
      (algo + " Bandwidth in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Bandwidth").c_str(),
     quTags.size(), 0.5, (double) (quTags.size()) + 0.5 );
     m_plots["ratePrim" + algo + wheelTag + secTag + chambTag] = new TH1F(("ratePrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
      (algo + " Primitive rate in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Rate").c_str(),
     quTags.size(), 0.5, (double) (quTags.size()) + 0.5 );
     m_plots2["ratePrimVsBX" + algo + wheelTag + secTag + chambTag] = new TH2F(("ratePrimsVsBX_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
      (algo + " Primitive rate in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Rate").c_str(),
     quTags.size(), 0.5, (double) (quTags.size()) + 0.5, 7,-3.5,3.5);
     for (unsigned int i = 0; i < quTags.size(); ++i){
      m_plots["ratePrim" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      m_plots["bandwidth" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      m_plots2["outputPrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      m_plots2["ratePrimVsBX" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
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
 
 for (auto & segmentT0 : *seg_phi_t0) m_plots["segmentT0"]->Fill(segmentT0);
 
 
 
 std::vector<std::string> algoTags  = { "AM" };
 std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
 std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
 std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
 //std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=3","GoodBX+qu>=3"};
 std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=2","GoodBX+qu>=2", "GoodBX+matchedqu<2", "GodBX+qu>=2+RPCseg", "GoodBX+qu>=2+RPCseg+clus","GoodBX+matchedqu<2+RPCseg", "GoodBX+matchedqu<2+RPCseg+clus"};
 
 short numberOfAMTotalPrimitives[5][12][4][quTags.size()];
 
 bool AMprimitive[quTags.size()][5][12][4]; 
 bool AMprimitiveBX[quTags.size()][5][12][4][7];
 
 for (int i = 0; i < 5; i++) {
  for (int j = 0; j < 12; j++) {
   for (int k = 0; k < 4; k++) {
    for (unsigned int q = 0; q < quTags.size(); q++) {
     numberOfAMTotalPrimitives[i][j][k][q]=0;
     
     AMprimitive[q][i][j][k]=false;
     //for (unsigned int bx = 0; bx<7) 
    }
   }
  }
 }
 
 for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM){
  Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
  Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM); if (trigAMSec==13) trigAMSec=4;  if (trigAMSec==14) trigAMSec=10;
  Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
  Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
  Int_t trigAMindex  = ph2TpgPhiEmuAm_index->at(iTrigAM);
  Int_t trigAMquality  = ph2TpgPhiEmuAm_quality->at(iTrigAM);
  Int_t trigAMrpc  = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
  
  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][0]++;
  AMprimitive[0][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;
  m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(1);
  if (trigAMquality >= 2) {AMprimitive[6][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true; numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][6]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(7);}
  if (trigAMBX == goodBX) {
   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][1]++;
   AMprimitive[1][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true; 
   m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(2); 
   if (trigAMindex <= 0) { AMprimitive[2][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][2]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(3);}
   if (trigAMindex <= 1) { AMprimitive[3][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][3]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(4);}
   if (trigAMindex <= 2) { AMprimitive[4][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][4]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(5);}
   if (trigAMindex <= 3) { AMprimitive[5][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;  numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][5]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(6);}
   if (trigAMquality >= 2) { AMprimitive[7][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][7]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(8);}
   if (trigAMquality >= 2 || ( trigAMquality < 2 && trigAMquality>0 && trigAMrpc!=0 )){ AMprimitive[8][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][8]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(9);}
   if (trigAMquality >= 2 ||  (trigAMquality == -1 && trigAMrpc==2) ){ AMprimitive[9][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][9]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(10);}
   if (trigAMquality >= 2 ||  (trigAMquality == -1 && ( trigAMrpc==2 || trigAMrpc==3)) ){ AMprimitive[10][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][10]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(11);}
   if (trigAMquality >= 2 || ( trigAMquality < 2 && trigAMquality>0 && trigAMrpc!=0 ) ||  (trigAMquality == -1 && trigAMrpc==2) ){ AMprimitive[11][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][11]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(12);}
   if (trigAMquality >= 2 || ( trigAMquality < 2 && trigAMquality>0 && trigAMrpc!=0 ) ||  (trigAMquality == -1 && ( trigAMrpc==2 || trigAMrpc==3)) ){ AMprimitive[12][trigAMWh+2][trigAMSec-1][trigAMSt-1]=true;   numberOfAMTotalPrimitives[trigAMWh+2][trigAMSec-1][trigAMSt-1][12]++;m_plots["bandwidthAM" + whTags.at(trigAMWh+2) + secTags.at(trigAMSec-1) + chambTags.at(trigAMSt-1)]->Fill(13);}
   
  }
 }
 
 for (unsigned int j = 0; j<secTags.size(); j++){
  auto secTag = secTags.at(j);  
  for (unsigned int k = 0; k<whTags.size(); k++){
   auto wheelTag = whTags.at(k);
   
   bool haveIt[algoTags.size()][quTags.size()];
   for (int algo = 0; algo < 2; algo++) {
    for (unsigned int qu = 0; qu < quTags.size(); qu++) {
     haveIt[algo][qu] = false;
    }
   }
   
   for (unsigned int i = 0; i<chambTags.size(); i++){
    auto chambTag = chambTags.at(i);  
    for (unsigned int q = 0; q<quTags.size(); q++){
     if (AMprimitive[q][k][j][i]) 
     { 
      haveIt[1][q]=true;
      m_plots["ratePrimAM" + wheelTag + secTag + chambTag]->Fill(q+1);
      m_plots2["outputPrimsAM" + wheelTag + secTag + chambTag]->Fill(q+1,numberOfAMTotalPrimitives[k][j][i][q]);
     }
    }
   } // for ch
   for (unsigned int q = 0; q<quTags.size(); q++){
    for (unsigned int a = 0; a<algoTags.size(); a++){
     auto algo = algoTags.at(a);
     if (haveIt[a][q]) m_plots["ratePrim" + algo + wheelTag + secTag]->Fill(q+1);
    }
   }
  } // for wh
 } // for sec
 
 
 
 
}




void DTNtupleTPGSimAnalyzer::endJob()
{
 std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
 std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
 std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};
 //std::vector<std::string> quTags    = {"AllBX", "GoodBX", "GoodBXIndex0","GoodBXIndex01","GoodBXIndex012","GoodBXIndex0123", "AllBXqu>=3","GoodBXqu>=3"} ;
 std::vector<std::string> quTags    = { "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=2","GoodBX+qu>=2", "GoodBX+matchedqu<2", "GodBX+qu>=2+RPCseg", "GoodBX+qu>=2+RPCseg+clus","GoodBX+matchedqu<2+RPCseg", "GoodBX+matchedqu<2+RPCseg+clus"};
 std::vector<std::string> algoTags    = {"AM"} ;
 for (auto & algo : algoTags) {          
  for (unsigned int j = 0; j<secTags.size(); j++){
   auto secTag = secTags.at(j);  
   for (unsigned int k = 0; k<whTags.size(); k++){
    auto wheelTag = whTags.at(k); 
    m_plots["ratePrim"+ algo + wheelTag + secTag]->Scale((1. / (double) numberOfEntries) * 2760 * 11246);  
    for (unsigned int i = 0; i<chambTags.size(); i++){
     auto chambTag = chambTags.at(i);  
     m_plots["ratePrim"+ algo + wheelTag + secTag + chambTag]->Scale((1. / (double) numberOfEntries) * 2760 * 11246);  
     for (unsigned int bin = 1; bin <= quTags.size(); bin++) {
      m_plots["bandwidth"+algo + wheelTag + secTag + chambTag]->SetBinContent(bin ,m_plots["bandwidth" + algo + wheelTag + secTag + chambTag]->GetBinContent(bin)* 2760. * 11246. * 64. * (1. / (double) numberOfEntries) );  
      //m_plots["bandwidth"+algo + wheelTag + secTag + chambTag]->SetBinContent(bin ,m_plots["bandwidth" + algo + wheelTag + secTag + chambTag]->GetBinContent(bin)*m_plots["ratePrim" + algo + wheelTag + secTag + chambTag]->GetBinContent(bin)*64.*(1. / (double) numberOfEntries) );  
     }          
     for (unsigned int q = 0; q < quTags.size(); q++) {
      auto quTag = quTags.at(q);
      //   m_plots["rate" + quTag + "Prim" + algo + wheelTag + secTag]->SetBinContent(i+1, m_plots["ratePrim" + algo + wheelTag + secTag + chambTag]->GetBinContent(q+1)); 
     }
    } // for st
   } // for wh
  } // for sec
 } // for algo
 
 
 
 m_outFile.cd();
 
 m_outFile.Write();
 m_outFile.Close();
}



Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1);
}
