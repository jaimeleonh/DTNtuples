#include "DTNtupleTPGSimAnalyzer.h"
#include "TVector2.h"
#include "TF1.h"
// DT AM Emulator constants
#include "L1Trigger/DTTriggerPhase2/interface/constants.h"

using namespace cmsdt;







DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
 const TString & outFileName,
 const TString & quality = "",
 const bool    & DM = false
):
m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName), quality_(quality), DM_(DM)
{
 
 m_minMuPt = 20;
 
 //m_maxMuSegDPhi = 0.3;
 // m_maxMuSegDPhi = 20;
 m_maxMuSegDPhi = 0.1;
 //m_maxMuSegDEta = 0.4;
 //m_maxMuSegDEta = 20;
 m_maxMuSegDEta = 0.15;
 
 m_minSegHits = 4;
 m_minZSegHits = 4;
 
 m_maxSegTrigDPhi = 0.1;
 m_maxMuTrigDPhi  = 0.2;
 
 m_maxSegT0 = 15;
 
}



DTNtupleTPGSimAnalyzer::~DTNtupleTPGSimAnalyzer()
{
 
}



void DTNtupleTPGSimAnalyzer::Loop()
{
 book();
 
 if (fChain == 0) return;
 
 Long64_t nentries = fChain->GetEntries();
 
 Long64_t nbytes = 0, nb = 0;
 //for (Long64_t jentry = 0; jentry < 100; jentry++)
 for (Long64_t jentry = 0; jentry < nentries; jentry++)
 {
  //if (jentry >= 10000) continue;
  thisEntry = jentry;
  Long64_t ientry = LoadTree(jentry);
  if (ientry < 0) break;
  nb = fChain->GetEvent(jentry);   nbytes += nb;
  
  //cout << jentry << endl; 
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
 
 std::vector<std::string> algoTag  = {"AM", "AM+RPC"};
 std::vector<std::string> totalTag = {"matched", "total"};
 std::vector<std::string> chambTag = {"MB1",     "MB2", "MB3", "MB4"};
 std::vector<std::string> wheelTag = {"Wh.-2","Wh.-1","Wh.0","Wh.+1","Wh.+2",};
 
 m_plots["hSegmentPsi"] = new TH1D("hSegmentPsi",
  "Segment Psi distribution ; Psi; Entries",
 200, -60, +60);
 m_plots2["hSegmentPsiVST0"] = new TH2D("hSegmentPsiVST0",
  "Segment Psi distribution vs segment t0; Psi; Segment t0 (ns)",
 200, -50, +50, 200, -100, 100);
 m_plots["hGenSegDeltaPhi"] = new TH1D("hGenSegDeltaPhi",
  "Gen Muon - Segment Delta Phi distribution ; Delta Phi; Entries",
 600, 0, 10);
 m_plots["hGenSegDeltaEta"] = new TH1D("hGenSegDeltaEta",
  "Gen Muon - Segment Delta Eta distribution ; Delta Eta; Entries",
 600, 0, 3);
 for (const auto & chamb : chambTag) {
  for (const auto & wheel : wheelTag) {
   m_plots["hGenSegDeltaPhi"+wheel+chamb] = new TH1D(("hGenSegDeltaPhi"+wheel+chamb).c_str(),
    "Gen Muon - Segment Delta Phi distribution ; Delta Phi; Entries",
   600, 0, 10);
   m_plots["hGenSegDeltaEta"+wheel+chamb] = new TH1D(("hGenSegDeltaEta"+wheel+chamb).c_str(),
    "Gen Muon - Segment Delta Eta distribution ; Delta Eta; Entries",
   600, 0, 3);
  }
 }
 
 for (const auto & algo : algoTag)
 {
  m_plots2["hSegmentPsiVSDeltaT0" + algo] = new TH2D(("hSegmentPsiVSDeltaT0_" + algo).c_str(),
   "Segment Psi distribution vs Delta t0; Psi; Delta t0 (ns)",
  200, -50, +50, 100, -50, 50);
  m_plots["hPrimPsi" + algo] = new TH1D(("hPrimPsi_" + algo).c_str(),
   (algo + " Primitives Psi distribution ; Psi; Entries").c_str(),
  200, -60, +60);
  m_plots2["hBXvsPrimPsi" + algo] = new TH2D(("hBXvsPrimPsi_" + algo).c_str(),
   (algo + " BX vs Primitives Psi distribution ; BX; Psi").c_str(),
  11, -5.5, 5.5, 13 ,-65., 65.);
  m_plots["hDeltaPhi" + algo] = new TH1D(("hDeltaPhi_" + algo).c_str(),
   (algo + " Primitive - Segment Delta Phi distribution ; Delta Phi; Entries").c_str(),
  600, 0, 0.1);
  m_plots["hEffvsSlope" + algo + "matched"] = new TH1D(("hEff_" + algo + "_matched" ).c_str(),
   ("Efficiency for " + algo + "; Local Direction; Efficiency").c_str(),
  50, -50, 50);
  m_plots["hEffvsSlope" + algo + "total"] = new TH1D(("hEff_" + algo + "_total" ).c_str(),
   ("Efficiency for "  + algo + "; Local Direction; Efficiency").c_str(),
  50, -50, 50);
  m_plots["hEffvsLxy" + algo + "matched"] = new TH1D(("hEffLxy_" + algo + "_matched" ).c_str(),
   ("Efficiency for " + algo + "; Gen muon Lxy; Efficiency").c_str(),
  50, 0, 310);
  m_plots["hEffvsLxy" + algo + "total"] = new TH1D(("hEffLxy_" + algo + "_total" ).c_str(),
   ("Efficiency for "  + algo + "; Gen muon Lxy; Efficiency").c_str(),
  50, 0, 310);
  for (const auto & chamb : chambTag)
  {
   for (const auto & total : totalTag)
   {
    m_plots["Eff_" + chamb + "_" + algo + "_" + total] = new TH1D(("hEff_" + chamb + "_" + algo + "_" + total).c_str(),
     ("Efficiency for " + chamb + " " + algo + "; Sector; Efficiency").c_str(),
    5, -2.5, +2.5);
    m_plots["SegEff_" + chamb + "_" + algo + "_" + total] = new TH1D(("hSegEff_" + chamb + "_" + algo + "_" + total).c_str(),
     ("Segment efficiency for " + chamb + "; Sector; Efficiency").c_str(),
    5, -2.5, +2.5);
    m_plots["EffNoBX_" + chamb + "_" + algo + "_" + total] = new TH1D(("hEffNoBX_" + chamb + "_" + algo + "_" + total).c_str(),
     ("Efficiency for " + chamb + " " + algo + "; Sector; Efficiency").c_str(),
    5, -2.5, +2.5);
    m_plots["EffEta_" + chamb + "_" + algo + "_" + total] = new TH1D(("hEffEta_" + chamb + "_" + algo + "_" + total).c_str(),
     ("Efficiency vs Eta for " + chamb + " " + algo + "; #eta; Efficiency").c_str(),
    100, -1.5, +1.5);
    for (const auto & wheel : wheelTag)
    {
     m_plots["hEffvsSlope" + algo + chamb + wheel + total] = new TH1D(("hEff_" + wheel + "_" + chamb + "_" + algo + "_" + total ).c_str(),
      ("Efficiency for " + wheel + " " + chamb + " " + algo + "; Local Direction; Efficiency").c_str(),
     50, -50, 50);
     m_plots["hEffvsLxy" + algo + chamb + wheel + total] = new TH1D(("hEffLxy_" + wheel + "_" + chamb + "_" + algo + "_" + total ).c_str(),
      ("Efficiency for " + wheel + " " + chamb + " " + algo + "; Gen muon Lxy; Efficiency").c_str(),
     50, 0, 310);
    }
   }
  }
 }
}



void DTNtupleTPGSimAnalyzer::fill()
{
 std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
 std::vector<std::string> whTags    = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};
 std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12","Sec13","Sec14"};
 
 //  cout << "--------------------------------Hits de Ph1: ----------------------------" <<endl;
 //  printHits();
 //  cout << "--------------------------------Hits de Ph2: ----------------------------" <<endl;
 //  printPh2Hits();
 
 // bool print = true;
 for (std::size_t iGenPart = 0; iGenPart < gen_nGenParts; ++iGenPart)
 {
  if (std::abs(gen_pdgId->at(iGenPart)) != 13 || gen_pt->at(iGenPart) < m_minMuPt) continue;
  //    if (gen_lxy->at(iGenPart) > 100) continue;
  
  // CB this should not be a vector ...
  std::vector<std::size_t> bestSegIndex = { 999, 999, 999, 999 };
  std::vector<Int_t> bestSegNHits       = { 0, 0, 0, 0 };
  
  
  for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg)
  {
   Int_t segSt    = seg_station->at(iSeg);
   Int_t segNHits = seg_phi_nHits->at(iSeg);
   Int_t segZNHits = seg_z_nHits->at(iSeg);
   
   
   if (abs(seg_phi_t0->at(iSeg)) > m_maxSegT0 ) continue;
   // if (abs(seg_wheel->at(iSeg))==2 && segSt==4) cout << "Found! muSegDPhi=" << muSegDPhi<< " genPhi="<< gen_phi->at(iGenPart) << " seg_posGlb_phi=" << seg_posGlb_phi->at(iSeg)  << " muSegDEta=" << muSegDEta << " segNHits=" << segNHits << " segZNHits=" << segZNHits  << endl;
   
   Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
   Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
   
   if (muSegDPhi < m_maxMuSegDPhi &&
    //  muSegDEta < m_maxMuSegDEta &&
    segNHits >= m_minSegHits &&
    (segZNHits >= m_minZSegHits  || segSt==4) &&
   segNHits >= bestSegNHits.at(segSt - 1))
   {
    bestSegNHits[segSt - 1] = segNHits;
    bestSegIndex[segSt - 1] = iSeg;
    //   if (abs(seg_wheel->at(iSeg))==2 && segSt==4) cout << "Found!" << endl;
   }
  }
  
  // Quality Code
  // LOWQ 1 -> 3 hits
  // CLOWQ 2 -> 3+2
  // HIGHQ 3 -> 4 hits
  // CHIGHQ 4 -> 4+2
  // LOWLOWQ 6 -> 3+3
  // HIGHLOWQ 7 -> 4+3
  // HIGHHIGHQ 8 -> 4+4
  
  int minQuality = -99;
  int maxIndex = 9999;
  bool qualityMatched = false;
  bool qualityORSegs = false;
  bool qualityORSegsClus = false;
  bool qualityMatchedORSegs = false;
  bool qualityMatchedORSegsClus = false;
  bool qualityCorrelated = false;
  bool qualityLegacy = false;
  
  if (quality_ == "nothreehits"){
   minQuality = CLOWQ;
  } 
  else if (quality_ == "index0")
  maxIndex = 0;
  else if (quality_ == "index01")
  maxIndex = 1;
  else if (quality_ == "index012")
  maxIndex = 2;
  else if (quality_ == "index0123")
  maxIndex = 3;
  else if (quality_ == "withmatchedthreehits")
  qualityMatched = true;  
  else if (quality_ == "qualityORSegs")
  qualityORSegs = true;  
  else if (quality_ == "qualityORSegsClus")
  qualityORSegsClus = true;  
  else if (quality_ == "qualityMatchedORSegs")
  qualityMatchedORSegs = true;  
  else if (quality_ == "qualityMatchedORSegsClus")
  qualityMatchedORSegsClus = true;  
  else if (quality_ == "correlated")
  qualityCorrelated = true;  
  else if (quality_ == "legacy")
  qualityLegacy = true;
  else if (quality_ == "Q8" ) 
  minQuality = HIGHHIGHQ;
  else if (quality_ == "All" ) 
  minQuality = -999;
  else {
   cout << "Error: Efficiency category not found" << endl;
   //  std::exit(EXIT_FAILURE);
  }
  
  
  // ==================== VARIABLES FOR THE HOUGH TRANSFORM BASED ALGORITHM
  for (const auto & iSeg : bestSegIndex)
  {
   if (iSeg == 999) continue;
   
   Int_t segSt    = seg_station->at(iSeg);
   Int_t segNHits = seg_phi_nHits->at(iSeg);
   Int_t segZNHits = seg_z_nHits->at(iSeg);
   
   Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
   Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
   
   m_plots["hSegmentPsi"] -> Fill( atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi())  );
   m_plots2["hSegmentPsiVST0"] -> Fill( atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) , seg_phi_t0->at(iSeg) );
   m_plots["hGenSegDeltaPhi"] -> Fill( muSegDPhi );
   m_plots["hGenSegDeltaPhi"+whTags.at(seg_wheel->at(iSeg)+2)+chambTags.at(segSt-1)] -> Fill( muSegDPhi );
   m_plots["hGenSegDeltaEta"] -> Fill( muSegDEta );
   m_plots["hGenSegDeltaEta"+whTags.at(seg_wheel->at(iSeg)+2)+chambTags.at(segSt-1)] -> Fill( muSegDEta );
   
   Int_t segWh  = seg_wheel->at(iSeg);
   Int_t segSec = seg_sector->at(iSeg);
   if (segSec == 13) segSec = 4;
   if (segSec == 14) segSec = 10;
   
   //cout << segWh << " " << segSec << " " << segSt << " " << seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg) << " " << seg_phi_t0->at(iSeg) << " " << seg_posLoc_x_midPlane->at(iSeg) << " "
   // << seg_posLoc_x_SL1->at(iSeg) << " " << seg_posLoc_x_SL3->at(iSeg) << " " << segNHits << " " << segZNHits << endl; 
   
   std::string chambTag = chambTags.at(segSt - 1);
   std::string whTag    = whTags.at(segWh + 2);
   std::string secTag   = secTags.at(segSec - 1);
   
   // ==================== VARIABLES FOR THE ANALYTICAL METHOD ALGORITHM
   Int_t    bestTPAM = -1;
   Int_t    bestTPNoBXAM = -1;
   Int_t    AMRPCflag= -1;
   Double_t bestSegTrigAMDPhi = 1000;
   Double_t bestSegTrigAMDPhiNoBX = 1000;
   Double_t bestAMDPhi = 0;
   Int_t    besttrigAMBX = 0;
   for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM)
   {
    Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
    Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM);
    Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
    Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
    Int_t trigAMqual  = ph2TpgPhiEmuAm_quality->at(iTrigAM);
    Int_t trigAMrpc  = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
    
    if (segWh == trigAMWh && segSec == trigAMSec && segSt  == trigAMSt)
    {
     Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuAm_phi->at(iTrigAM),trigAMSec);
     Double_t finalAMDPhi   = seg_posGlb_phi->at(iSeg) - trigGlbPhi;
     Double_t segTrigAMDPhi = abs(acos(cos(finalAMDPhi)));
     
     // if (abs(segWh) == 2 and abs(segSt) == 1 and print) {
     // print = false;
     // cout << event_eventNumber << endl;
     // for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM) {
     // cout << ph2TpgPhiEmuAm_wheel->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_sector->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_station->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_quality->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_BX->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_t0->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_posLoc_x->at(iTrigAM) << " ";
     // cout << ph2TpgPhiEmuAm_dirLoc_phi->at(iTrigAM) << endl;
     // }
     // printPh2Hits();
     // }
     
     m_plots["hPrimPsiAM"] -> Fill( ph2TpgPhiEmuAm_dirLoc_phi->at(iTrigAM) );
     m_plots["hDeltaPhiAM"] -> Fill( segTrigAMDPhi );
     m_plots2["hBXvsPrimPsiAM"] -> Fill ( trigAMBX - 20 , atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()));
     if (segNHits == 8)
     m_plots["SegEff_" + chambTag + "_AM_matched"] -> Fill(segWh);
     m_plots["SegEff_" + chambTag + "_AM_total"] -> Fill(segWh);
     m_plots2["hSegmentPsiVSDeltaT0AM"]->Fill(  atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) , ph2TpgPhiEmuAm_t0->at(iTrigAM) - 20*25 - seg_phi_t0->at(iSeg) );
     if (ph2TpgPhiEmuAm_index->at(iTrigAM) > maxIndex ) continue;  	  
     // if (ph2TpgPhiEmuAm_quality->at(iTrigAM) < minQuality ) continue;  	  
     if (qualityORSegs && ( (trigAMqual < LOWQ && trigAMqual > -1) || ( trigAMqual ==-1 && trigAMrpc!=2 ) )) continue;
     if (qualityORSegsClus &&  (trigAMqual < LOWQ && trigAMqual > -1)  ) continue;
     if (qualityMatched && ( (ph2TpgPhiEmuAm_quality->at(iTrigAM) < LOWQ && ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM)==0 ) || ( ph2TpgPhiEmuAm_quality->at(iTrigAM) ==-1 ) ) ) continue;
     if (qualityMatchedORSegs && ( ( (trigAMqual < LOWQ && trigAMrpc==0 ) && trigAMqual > -1) || ( trigAMqual ==-1 && trigAMrpc!=2 ) )) continue;
     if (qualityMatchedORSegsClus && ( ( (trigAMqual < LOWQ && trigAMrpc==0 ) && trigAMqual > -1) )) continue;
     if (qualityCorrelated && (trigAMqual < LOWLOWQ)) continue;
     if (qualityLegacy && (trigAMqual < HIGHQ)) continue;
     
     //if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (bestSegTrigAMDPhi > segTrigAMDPhi) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQuality))
     if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (trigAMBX == 20) && (bestSegTrigAMDPhi > segTrigAMDPhi) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQuality))
     {
      bestTPAM          = iTrigAM;
      besttrigAMBX      = trigAMBX;
      bestSegTrigAMDPhi = segTrigAMDPhi;
      bestAMDPhi        = TVector2::Phi_mpi_pi(finalAMDPhi);
      AMRPCflag         = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
     }
     if ((segTrigAMDPhi< m_maxSegTrigDPhi )                 && (bestSegTrigAMDPhiNoBX > segTrigAMDPhi) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQuality))
     {
      bestTPNoBXAM          = iTrigAM;
      bestSegTrigAMDPhiNoBX = segTrigAMDPhi;
     }
    }
   }
   // cout << bestTPAM << " " << bestTPNoBXAM << endl;
   if (bestTPAM > -1 && seg_phi_t0->at(iSeg) > -500)
   {
    //cout << thisEntry << " " << 1 << " "<< segWh << " " << segSec << " " << segSt << " " << seg_phi_nHits->at(iSeg) << " " << getPh1Hits(segWh,segSec,segSt) << " " << getPh2Hits(segWh,segSec,segSt) <<endl;
    //        cout << "Efficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) <<endl;
    //cout << "Efficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) << " Primitive iTrig " << bestTPAM << " out of " <<  ph2TpgPhiEmuAm_nTrigs  << " Quality: " << ph2TpgPhiEmuAm_quality->at(bestTPAM) << " t0=" << ph2TpgPhiEmuAm_t0->at(bestTPAM)  <<endl;
    //cout << "Eficiente!" << endl; 
    m_plots["Eff_" + chambTag + "_AM_matched"]->Fill(segWh);
    m_plots["EffEta_" + chambTag + "_AM_matched"]->Fill(gen_eta->at(iGenPart));
    m_plots["hEffvsSlopeAM" + chambTag + whTag + "matched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
    if (DM_) m_plots["hEffvsLxyAM" + chambTag + whTag + "matched"] -> Fill( gen_lxy->at(iGenPart) );
    if (DM_) m_plots["hEffvsLxyAMmatched"] -> Fill( gen_lxy->at(iGenPart) );
    m_plots["hEffvsSlopeAMmatched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
    if (AMRPCflag > 0) m_plots["Eff_" + chambTag + "_AM+RPC_matched"]->Fill(segWh);
    } else if (bestTPAM  < 0 && seg_phi_t0->at(iSeg) > -500) {
    //cout << thisEntry << " " << -1 << " "<< segWh << " " << segSec << " " << segSt << " " << seg_phi_nHits->at(iSeg) << " " << getPh1Hits(segWh,segSec,segSt) << " " << getPh2Hits(segWh,segSec,segSt) <<endl;
    //cout << "Inefficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) <<endl;
    //cout << "Inefficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) << " Primitive iTrig " << bestTPAM << " out of " <<  ph2TpgPhiEmuAm_nTrigs  <<endl;
   }
   if (bestTPNoBXAM > -1 && seg_phi_t0->at(iSeg) > -500)
   {
    m_plots["EffNoBX_" + chambTag + "_AM_matched"]->Fill(segWh);
   }
   
   if (seg_phi_t0->at(iSeg) > -500)
   {
    m_plots["Eff_" + chambTag + "_AM_total"]->Fill(segWh);
    m_plots["EffNoBX_" + chambTag + "_AM_total"]->Fill(segWh);
    m_plots["EffEta_" + chambTag + "_AM_total"]->Fill(gen_eta->at(iGenPart));
    m_plots["hEffvsSlopeAM" + chambTag + whTag + "total"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
    m_plots["hEffvsSlopeAMtotal"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
    if (DM_) m_plots["hEffvsLxyAM" + chambTag + whTag + "total"] -> Fill( gen_lxy->at(iGenPart) );
    if (DM_) m_plots["hEffvsLxyAMtotal"] -> Fill( gen_lxy->at(iGenPart) );
    m_plots["Eff_" + chambTag + "_AM+RPC_total"]->Fill(segWh);
   }
   //       if (iSeg == 0)
   //       {
   //         for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM)
   //         {
   //           Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
   //           Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
   //           Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);
   //           chambTag = chambTags.at(trigAMSt - 1);
   //           if (trigAMBX > 5)
   //           {
   //             m_plots["Eff_" + chambTag + "_AM_total"]->Fill(trigAMWh);
   //             m_plots["Eff_" + chambTag + "_AM+RPC_total"]->Fill(trigAMWh);
   //           }
   //         }
   //       }
   
  }
  
  
 }
 //cout << "-1 -1 -1 -1 -1 -1 -1 -1 -1 -1" << endl; 
 //for (unsigned int iTrig = 0; iTrig < ph2TpgPhiEmuAm_nTrigs; iTrig++)  {
 //  printMPs(iTrig);
 //  cout << "-------------------------------------------------" <<endl;
//}
}



void DTNtupleTPGSimAnalyzer::endJob()
{
 m_outFile.cd();
 
 m_outFile.Write();
 m_outFile.Close();
}

void DTNtupleTPGSimAnalyzer::printSeg(int iSeg)
{
} 


void DTNtupleTPGSimAnalyzer::printMPs(int iTrig)
{
 cout << "iTrig " << iTrig
 << " Wh:" << ph2TpgPhiEmuAm_wheel->at(iTrig) 
 << " Se:" << ph2TpgPhiEmuAm_sector->at(iTrig) 
 << " St:" << ph2TpgPhiEmuAm_station->at(iTrig) << endl 
 << "Position :" << ph2TpgPhiEmuAm_posLoc_x->at(iTrig) << endl 
 << "Direction :" << ph2TpgPhiEmuAm_dirLoc_phi->at(iTrig) << endl 
 << "Time :" << ph2TpgPhiEmuAm_t0->at(iTrig) << endl 
 << "Quality :" << ph2TpgPhiEmuAm_quality->at(iTrig) << endl; 
}

void DTNtupleTPGSimAnalyzer::printHits () 
{
 for (unsigned int i = 0; i < digi_nDigis; i++){
  cout << digi_wheel->at(i) << " " << digi_sector->at(i) << " " << digi_station->at(i) << " " << digi_superLayer->at(i)-1 << " " << digi_layer->at(i)-1 << " " << digi_wire->at(i)-1 << " " << (int)(round(digi_time->at(i))) << endl;      
 }
 cout << "-1 -1 -1 -1 -1 -1 -1" << endl; 
}

void DTNtupleTPGSimAnalyzer::printPh2Hits () 
{
 for (unsigned int i = 0; i < ph2Digi_nDigis; i++){
  cout << ph2Digi_wheel->at(i) << " ";
  cout << ph2Digi_sector->at(i) << " ";
  cout << ph2Digi_station->at(i) << " ";
  cout << ph2Digi_superLayer->at(i) << " ";
  cout << ph2Digi_layer->at(i) << " " ;
  cout << ph2Digi_wire->at(i) << " ";
  cout << ph2Digi_time->at(i) << endl;      
 }
}

Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
 return trigPhi / PHIRES_CONV + TMath::Pi() / 6 * (sector - 1);
}

int DTNtupleTPGSimAnalyzer::getPh1Hits (int wh, int se, int st) 
{
 int sum = 0;
 for (unsigned int i = 0; i < digi_nDigis; i++){
  if ( digi_wheel->at(i) == wh &&
   digi_sector->at(i) == se &&
  digi_station->at(i) == st ) sum++;
 } 
 return sum; 
}

int DTNtupleTPGSimAnalyzer::getPh2Hits (int wh, int se, int st) 
{
 int sum = 0;
 for (unsigned int i = 0; i < ph2Digi_nDigis; i++){
  if ( ph2Digi_wheel->at(i) == wh &&
   ph2Digi_sector->at(i) == se &&
  ph2Digi_station->at(i) == st ) sum++;
 } 
 return sum; 
}
