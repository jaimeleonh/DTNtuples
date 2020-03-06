#include "DTNtupleTPGSimAnalyzer.h"
#include "TVector2.h"
#include "TF1.h"

// 1,2 -> 3 hits
// 3,4 -> 4 hits
// 5 -> 3+2
// 6 -> 3+3
// 7 -> 4+2
// 8 -> 4+3
// 9 -> 4+4





DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
                                               const TString & outFileName,
                               					       const TString & quality = "",
                               					       const bool    & DM = false
					       ):
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName), quality_(quality), DM_(DM)
{

  m_minMuPt = 20;

  m_maxMuSegDPhi = 0.3;
 // m_maxMuSegDPhi = 20;
  //m_maxMuSegDPhi = 0.1;
  m_maxMuSegDEta = 0.4;
  //m_maxMuSegDEta = 20;
  //m_maxMuSegDEta = 0.15;

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
//      if (jentry != 12619) continue;
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

  std::vector<std::string> algoTag  = {"HB",      "AM", "AM+RPC"};
  std::vector<std::string> totalTag = {"matched", "total"};
  std::vector<std::string> chambTag = {"MB1",     "MB2", "MB3", "MB4"};
  std::vector<std::string> wheelTag = {"Wh.-2","Wh.-1","Wh.0","Wh.+1","Wh.+2",};

  m_plots["hSegmentPsi"] = new TH1D("hSegmentPsi",
                          "Segment Psi distribution ; Psi; Entries",
                          200, -50, +50);
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
                          200, -50, +50);
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

  //printHits();

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
      
      Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
      Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
      
      m_plots["hSegmentPsi"] -> Fill( atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi())  );
      m_plots2["hSegmentPsiVST0"] -> Fill( atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) , seg_phi_t0->at(iSeg) );
      m_plots["hGenSegDeltaPhi"] -> Fill( muSegDPhi );
      m_plots["hGenSegDeltaPhi"+whTags.at(seg_wheel->at(iSeg)+2)+chambTags.at(segSt-1)] -> Fill( muSegDPhi );
      m_plots["hGenSegDeltaEta"] -> Fill( muSegDEta );
      m_plots["hGenSegDeltaEta"+whTags.at(seg_wheel->at(iSeg)+2)+chambTags.at(segSt-1)] -> Fill( muSegDEta );

      if (abs(seg_phi_t0->at(iSeg)) > m_maxSegT0 ) continue;
// if (abs(seg_wheel->at(iSeg))==2 && segSt==4) cout << "Found! muSegDPhi=" << muSegDPhi<< " genPhi="<< gen_phi->at(iGenPart) << " seg_posGlb_phi=" << seg_posGlb_phi->at(iSeg)  << " muSegDEta=" << muSegDEta << " segNHits=" << segNHits << " segZNHits=" << segZNHits  << endl;

      if (muSegDPhi < m_maxMuSegDPhi &&
          muSegDEta < m_maxMuSegDEta &&
          segNHits >= m_minSegHits &&
          (segZNHits >= m_minZSegHits  || segSt==4) &&
          segNHits >= bestSegNHits.at(segSt - 1))
      {
        bestSegNHits[segSt - 1] = segNHits;
        bestSegIndex[segSt - 1] = iSeg;
     //   if (abs(seg_wheel->at(iSeg))==2 && segSt==4) cout << "Found!" << endl;
      }
    }

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
      minQuality = 3;
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

      Int_t segWh  = seg_wheel->at(iSeg);
      Int_t segSec = seg_sector->at(iSeg);
      if (segSec == 13) segSec = 4;
      if (segSec == 14) segSec = 10;
      Int_t segSt  = seg_station->at(iSeg);
     

      std::string chambTag = chambTags.at(segSt - 1);
      std::string whTag    = whTags.at(segWh + 2);
      std::string secTag   = secTags.at(segSec - 1);

      Int_t    bestTPHB = -1;
      Double_t bestSegTrigHBDPhi = 1000;
      Double_t bestHBDPhi = 0;
      Int_t    besttrigHBBX = 0;
      for (std::size_t iTrigHB = 0; iTrigHB < ph2TpgPhiEmuHb_nTrigs; ++iTrigHB)
      {
        Int_t trigHBWh  = ph2TpgPhiEmuHb_wheel->at(iTrigHB);
        Int_t trigHBSec = ph2TpgPhiEmuHb_sector->at(iTrigHB);
        Int_t trigHBSt  = ph2TpgPhiEmuHb_station->at(iTrigHB);
        Int_t trigHBBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB);

        if (segWh  == trigHBWh && segSec == trigHBSec &&  segSt  == trigHBSt)
        {
          Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuHb_phi->at(iTrigHB),trigHBSec);
          Double_t finalHBDPhi   = seg_posGlb_phi->at(iSeg) - trigGlbPhi;
          Double_t segTrigHBDPhi = abs(acos(cos(finalHBDPhi)));
	  
          m_plots["hPrimPsiHB"] -> Fill( ph2TpgPhiEmuHb_dirLoc_phi->at(iTrigHB) );
          m_plots["hDeltaPhiHB"] -> Fill( segTrigHBDPhi );
          m_plots2["hBXvsPrimPsiHB"] -> Fill ( trigHBBX - 20 , atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()));
          //if (ph2TpgPhiEmuHb_index->at(iTrigHB) > 3 ) continue;  	  

          //if ((segTrigHBDPhi < m_maxSegTrigDPhi)  && (bestSegTrigHBDPhi > segTrigHBDPhi) && (ph2TpgPhiEmuHb_quality->at(iTrigHB) >= minQuality))
          if ((segTrigHBDPhi < m_maxSegTrigDPhi) && (trigHBBX == 20) && (bestSegTrigHBDPhi > segTrigHBDPhi) && (ph2TpgPhiEmuHb_quality->at(iTrigHB) >= minQuality))
          {
            bestTPHB          = iTrigHB;
            besttrigHBBX      = trigHBBX;
            bestSegTrigHBDPhi = segTrigHBDPhi;
            bestHBDPhi        = TVector2::Phi_mpi_pi(finalHBDPhi);
          }
        }
      }

      if (bestTPHB > -1 && seg_phi_t0->at(iSeg) > -500)
      {
        m_plots["Eff_" + chambTag + "_HB_matched"]->Fill(segWh);
        m_plots["EffEta_" + chambTag + "_HB_matched"]->Fill(gen_eta->at(iGenPart));
        m_plots["hEffvsSlopeHB" + chambTag + whTag + "matched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
        m_plots["hEffvsSlopeHBmatched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
      }

      if (seg_phi_t0->at(iSeg) > -500)
      {
        m_plots["Eff_" + chambTag + "_HB_total"]->Fill(segWh);
        m_plots["EffEta_" + chambTag + "_HB_total"]->Fill(gen_eta->at(iGenPart));
        m_plots["hEffvsSlopeHB" + chambTag + whTag + "total"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
        m_plots["hEffvsSlopeHBtotal"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
      }


      // ==================== VARIABLES FOR THE ANALYTICAL METHOD ALGORITHM
      Int_t    bestTPAM = -1;
      Int_t    AMRPCflag= -1;
      Double_t bestSegTrigAMDPhi = 1000;
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

          m_plots["hPrimPsiAM"] -> Fill( ph2TpgPhiEmuAm_dirLoc_phi->at(iTrigAM) );
          m_plots["hDeltaPhiAM"] -> Fill( segTrigAMDPhi );
          m_plots2["hBXvsPrimPsiAM"] -> Fill ( trigAMBX - 20 , atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()));
	 
          m_plots2["hSegmentPsiVSDeltaT0AM"]->Fill(  atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) , ph2TpgPhiEmuAm_t0->at(iTrigAM) - 20*25 - seg_phi_t0->at(iSeg) );
          if (ph2TpgPhiEmuAm_index->at(iTrigAM) > maxIndex ) continue;  	  
	      if (ph2TpgPhiEmuAm_quality->at(iTrigAM) < minQuality ) continue;  	  
          if (qualityORSegs && ( (trigAMqual < 3 && trigAMqual > -1) || ( trigAMqual ==-1 && trigAMrpc!=2 ) )) continue;
          if (qualityORSegsClus &&  (trigAMqual < 3 && trigAMqual > -1)  ) continue;
          if (qualityMatched && ( (ph2TpgPhiEmuAm_quality->at(iTrigAM) < 3 && ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM)==0 ) || ( ph2TpgPhiEmuAm_quality->at(iTrigAM) ==-1 ) ) ) continue;
          if (qualityMatchedORSegs && ( ( (trigAMqual < 3 && trigAMrpc==0 ) && trigAMqual > -1) || ( trigAMqual ==-1 && trigAMrpc!=2 ) )) continue;
          if (qualityMatchedORSegsClus && ( ( (trigAMqual < 3 && trigAMrpc==0 ) && trigAMqual > -1) )) continue;
          if (qualityCorrelated && (trigAMqual < 6 || trigAMqual==7)) continue;
          if (qualityLegacy && (trigAMqual < 3 || trigAMqual==5)) continue;

          //if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (bestSegTrigAMDPhi > segTrigAMDPhi) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQuality))
          if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (trigAMBX == 20) && (bestSegTrigAMDPhi > segTrigAMDPhi) && (ph2TpgPhiEmuAm_quality->at(iTrigAM) >= minQuality))
          {
            bestTPAM          = iTrigAM;
            besttrigAMBX      = trigAMBX;
            bestSegTrigAMDPhi = segTrigAMDPhi;
            bestAMDPhi        = TVector2::Phi_mpi_pi(finalAMDPhi);
            AMRPCflag         = ph2TpgPhiEmuAm_rpcFlag->at(iTrigAM);
          }
        }
      }

      if (bestTPAM > -1 && seg_phi_t0->at(iSeg) > -500)
      {
//        cout << "Efficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) <<endl;
        //cout << "Efficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) << " Primitive iTrig " << bestTPAM << " out of " <<  ph2TpgPhiEmuAm_nTrigs  <<endl;
        //cout << "Eficiente!" << endl; 
        m_plots["Eff_" + chambTag + "_AM_matched"]->Fill(segWh);
        m_plots["EffEta_" + chambTag + "_AM_matched"]->Fill(gen_eta->at(iGenPart));
        m_plots["hEffvsSlopeAM" + chambTag + whTag + "matched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
        if (DM_) m_plots["hEffvsLxyAM" + chambTag + whTag + "matched"] -> Fill( gen_lxy->at(iGenPart) );
        if (DM_) m_plots["hEffvsLxyAMmatched"] -> Fill( gen_lxy->at(iGenPart) );
        m_plots["hEffvsSlopeAMmatched"] -> Fill(atan ( (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) * 360 / (2*TMath::Pi()) );
        if (AMRPCflag > 0) m_plots["Eff_" + chambTag + "_AM+RPC_matched"]->Fill(segWh);
      } else if (bestTPAM  < 0 && seg_phi_t0->at(iSeg) > -500) {
       // cout << "Inefficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) <<endl;
        //cout << "Inefficient event " <<  thisEntry << " in " << whTag << " " << secTag << " " << chambTag << " Segment hits: " << seg_phi_nHits->at(iSeg) << " Segment Position: " << seg_posLoc_x->at(iSeg) << " Primitive iTrig " << bestTPAM << " out of " <<  ph2TpgPhiEmuAm_nTrigs  <<endl;
      }
      if (seg_phi_t0->at(iSeg) > -500)
      {
        m_plots["Eff_" + chambTag + "_AM_total"]->Fill(segWh);
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
    cout << "Wh: " << digi_wheel->at(i) << " Se:" << digi_sector->at(i) << " St:" << digi_station->at(i) << " Sl:" << digi_superLayer->at(i) << " La:" << digi_layer->at(i) << " Wi:" << digi_wire->at(i) << " Time: " << digi_time->at(i) << endl;      
  }
}

void DTNtupleTPGSimAnalyzer::printPh2Hits () 
{
  for (unsigned int i = 0; i < ph2Digi_nDigis; i++){
    cout << "Wh: " << ph2Digi_wheel->at(i) << " Se:" << ph2Digi_sector->at(i) << " St:" << ph2Digi_station->at(i) << " Sl:" << ph2Digi_superLayer->at(i) << " La:" << ph2Digi_layer->at(i) << " Wi:" << ph2Digi_wire->at(i) << " Time: " << ph2Digi_time->at(i) << endl;      
  }
}

Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1);
}
