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
  m_minSegZHits = 4;

  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;

//  gSystem->Load("libRooFit");
//
//  using namespace RooFit;

  unique = false; 

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
  std::vector<std::string> algoTag    = {"_AM","_HB" };
  std::vector<std::string> chambTags  = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags     = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags    = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12","Sec13","Sec14"};
  std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "Legacy","Q9","Q8","Q6"};
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "min3", "min4", "Legacy", "CorrelExt"};
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All"};

     // Double_t limtanpsi   = 0.04; Double_t limphi   = 3.14; Double_t limtime  = 50;   Double_t limx   = 0.6; Double_t limBX   = 8.5;
      Double_t limtanpsi   = 0.1; Double_t limphi   = 0.0015; Double_t limphiB   = 0.03;  Double_t limtime  = 50.5;   Double_t limx   = 0.6; Double_t limBX   = 8.5;
      //Double_t limtanpsi   = 0.1; Double_t limphi   = 0.005; Double_t limtime  = 50;   Double_t limx   = 0.2; Double_t limBX   = 8.5;
      UShort_t nbinstanpsi = 500; UShort_t nbinsphi = 501; UShort_t nbinsphiB = 501;  UShort_t nbinstime = 101; UShort_t nbinsx = 501; UShort_t nbinsBX = 17;
      //UShort_t nbinstanpsi = 101; UShort_t nbinsphi = 10001;   UShort_t nbinstime = 101; UShort_t nbinsx = 101; UShort_t nbinsBX = 17;
  
  m_plots["hQualAll"] = new TH1F("HQualAll", "All primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hQualEff"] = new TH1F("HQualEff", "Efficient primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hQualBest"] = new TH1F("HQualBest", "Best primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hPrims"] = new TH1F("HPrims", "Total primitives per chamber; Number of primitives; Entries",31,-0.5,30.5 );

  m_plots["PhiDif"] = new TH1F("PhiDif",
					     "Difference between phi SL1 & phi center; DeltaPhi (rad); entries",
					     10000,-0.5,0.5);
  for (const auto & chambTag : chambTags){
    for (const auto & whTag : whTags){
      m_plots["hPhiGenSeg" + whTag + chambTag] = new TH1F(("hPhiGenSeg_" + whTag  + "_" + chambTag).c_str(),
							  ("#Delta #Phi Gen muon-segment "+ whTag + " " + chambTag +  "; #Delta#phi (rad); entries").c_str(),
							  100,0,0.3);
      m_plots["ht0SegBefore" + whTag + chambTag] = new TH1F(("ht0SegBefore_" + whTag  + "_" + chambTag).c_str(),
							  ("Segment t0 before Cuts "+ whTag + " " + chambTag +  "; Time (ns); entries").c_str(),
							  60,-120,+120);
      m_plots["ht0SegAfterDPhi" + whTag + chambTag] = new TH1F(("ht0SegAfterDPhi_" + whTag  + "_" + chambTag).c_str(),
							  ("Segment t0 after dPhi Cut "+ whTag + " " + chambTag +  "; Time (ns); entries").c_str(),
							  60,-120,+120);
      m_plots["ht0SegAfterDEta" + whTag + chambTag] = new TH1F(("ht0SegAfterDEta_" + whTag  + "_" + chambTag).c_str(),
							  ("Segment t0 after dEta Cut "+ whTag + " " + chambTag +  "; Time (ns); entries").c_str(),
							  60,-120,+120);
      m_plots["ht0SegAfterSegHits" + whTag + chambTag] = new TH1F(("ht0SegAfterSegHits_" + whTag  + "_" + chambTag).c_str(),
							  ("Segment t0 after Seg Phi Hits Cut"+ whTag + " " + chambTag +  "; Time (ns); entries").c_str(),
							  60,-120,+120);
      m_plots["ht0SegAfterSegZHits" + whTag + chambTag] = new TH1F(("ht0SegAfterSegZHits_" + whTag  + "_" + chambTag).c_str(),
							  ("Segment t0 after Seg Z Hits Cut "+ whTag + " " + chambTag +  "; Time (ns); entries").c_str(),
							  60,-120,+120);
      m_plots["hEtaGenSeg" + whTag + chambTag] = new TH1F(("hEtaGenSeg_" + whTag  + "_" + chambTag).c_str(),
							  ("#Delta #Eta Gen muon-segment "+ whTag + " " + chambTag +  "; #Delta#phi (rad); entries").c_str(),
							  100,0,+0.4);
      m_plots2["hPhiHitsGenSeg" + whTag + chambTag] = new TH2F(("hPhiHitsGenSeg_" + whTag  + "_" + chambTag).c_str(),
							  ("#Delta #Phi Gen muon-segment vs Number of Phi-hits"+ whTag + " " + chambTag +  "; #Delta#phi (rad); Number of hits").c_str(),
							  100,0,0.3,6,2.5,8.5);
      m_plots2["hEtaHitsGenSeg" + whTag + chambTag] = new TH2F(("hEtaHitsGenSeg_" + whTag  + "_" + chambTag).c_str(),
							  ("#Delta #Eta Gen muon-segment vs Number of Z-hits"+ whTag + " " + chambTag +  "; #Delta#phi (rad); Number of hits").c_str(),
							  100,0,+0.4,2,2.5,4.5);
      m_plots2["hEtaPhiGenSeg" + whTag + chambTag] = new TH2F(("hEtaPhiGenSeg_" + whTag  + "_" + chambTag).c_str(),
							  ("#Delta #Eta vs #Delta #Phi Gen muon-segment "+ whTag + " " + chambTag +  "; #Delta#eta (rad); #Delta#Phi (rad)").c_str(),
							  100,0,+0.4,100,0,+0.3);
    }
  }
  for (const auto & algo : algoTag){



    for (const auto & secTag : secTags){
      m_plots["Phi"+algo+secTag] = new TH1F(("Phi"+algo+secTag).c_str(),
		 		"Phi distribution; DeltaPhi (rad); entries",
				2000,-2,+8);
    }
  }
  m_plots2["PhiDif2D"] = new TH2F("PhiDif2D",
					     "SL1 phi dif vs SL3 phi dif; DeltaPhi SL1(rad); DeltaPhi SL3 (rad)",
					     10000,-0.5,0.5, 10000, -0.5, 0.5);
  m_plots2["PhiDif2DPtMB1"] = new TH2F("PhiDif2DPtMB1",
					     "SL1 phi dif in MB1 vs pt; DeltaPhi SL1(rad); pt (GeV)",
					     10000,-0.5,0.5, 101, -0.5, 100.5);
  m_plots2["PhiDif2DPtMB2"] = new TH2F("PhiDif2DPtMB2",
					     "SL1 phi dif in MB2 vs pt; DeltaPhi SL1(rad); pt (GeV)",
					     10000,-0.5,0.5, 101, -0.5, 100.5);
  m_plots2["PhiDif2DPtMB3"] = new TH2F("PhiDif2DPtMB3",
					     "SL1 phi dif in MB3 vs pt; DeltaPhi SL1(rad); pt (GeV)",
					     10000,-0.5,0.5, 101, -0.5, 100.5);
  m_plots2["PhiDif2DPtMB4"] = new TH2F("PhiDif2DPtMB4",
					     "SL1 phi dif in MB4 vs pt; DeltaPhi SL1(rad); pt (GeV)",
					     10000,-0.5,0.5, 101, -0.5, 100.5);


  for (const auto & algo : algoTag){
    for (const auto & qual : qualTags){
 //     Double_t limtanpsi   = 0.04; Double_t limphi   = 0.0015; Double_t limtime  = 50;   Double_t limx   = 0.6; Double_t limBX   = 8.5;
      //Double_t limtanpsi   = 0.1; Double_t limphi   = 0.005; Double_t limtime  = 50;   Double_t limx   = 0.2; Double_t limBX   = 8.5;
 //     UShort_t nbinstanpsi = 101; UShort_t nbinsphi = 101;   UShort_t nbinstime = 101; UShort_t nbinsx = 101; UShort_t nbinsBX = 17;
      //UShort_t nbinstanpsi = 1001; UShort_t nbinsphi = 1001;   UShort_t nbinstime = 101; UShort_t nbinsx = 101; UShort_t nbinsBX = 17;
      m_plots["TanPsiRes_P2" + algo + qual] = new TH1F(("hTanPsiRes_P2"+ algo + qual).c_str() ,
						"TanPsiRes Seg-TP total distribution; #Delta tan(#psi) (adim.); entries",
						nbinstanpsi,-limtanpsi,+limtanpsi);
      m_plots["PhiRes_P2" + algo + qual] = new TH1F(("hPhiRes_P2"+ algo + qual).c_str(),
					     "PhiRes Seg-TP total distribution; #Delta#phi (rad); entries",
					     nbinsphi,-limphi,+limphi);
      m_plots["PhiBRes_P2" + algo + qual] = new TH1F(("hPhiBRes_P2"+ algo + qual).c_str(),
					     "PhiBRes Seg-TP total distribution; #Delta#phiB (rad); entries",
					     nbinsphiB,-limphiB,+limphiB);
      m_plots["TimeRes_P2" + algo + qual] = new TH1F(("hTimeRes_P2"+ algo + qual).c_str(),
					      "TimeRes Seg-TP total distribution; #Delta t (ns); entries",
					      nbinstime,-limtime,+limtime);
      m_plots["xRes_P2" + algo + qual] = new TH1S(("hxRes_P2"+ algo + qual).c_str(),
					   "xRes Seg-TP total distribution; #Delta x (cm); entries",
					   nbinsx,-limx,+limx);
      m_plots["BX_P2" + algo + qual] = new TH1F(("BX_P2"+ algo + qual).c_str(),
					   "BX total distribution; BX; entries",
					   nbinsBX,-limBX,+limBX);
      
      for (const auto & whTag : whTags){
	m_plots["TanPsiRes_P2" + algo + qual + whTag] = new TH1F(("hTanPsiRes" + algo + qual +"_" + whTag +  "_P2").c_str(),
							  ("TanPsiRes Seg-TP distribution for " + whTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							  nbinstanpsi,-limtanpsi,+limtanpsi);
	m_plots["PhiRes_P2" + algo + qual + whTag] = new TH1F(("hPhiRes" + algo + qual  +"_" + whTag +  "_P2").c_str(),
						       ("PhiRes Seg-TP distribution for " + whTag +  "; #Delta#phi (rad); entries").c_str(),
						       nbinsphi,-limphi,+limphi);
	m_plots["PhiBRes_P2" + algo + qual + whTag] = new TH1F(("hPhiBRes" + algo + qual  +"_" + whTag +  "_P2").c_str(),
						       ("PhiBRes Seg-TP distribution for " + whTag +  "; #Delta#phiB (rad); entries").c_str(),
						       nbinsphiB,-limphiB,+limphiB);
	m_plots["TimeRes_P2" + algo + qual + whTag] = new TH1F(("hTimeRes" + algo + qual  +"_" + whTag +  "_P2").c_str(),
							("TimeRes Seg-TP distribution for " + whTag +  "; #Delta t (ns); entries").c_str(),
							nbinstime,-limtime,+limtime);
	m_plots["xRes_P2" + algo + qual + whTag] = new TH1S(("hxRes" + algo + qual  +"_" + whTag +  "_P2").c_str(),
						     ("xRes Seg-TP distribution for " + whTag +  "; #Delta x (cm); entries").c_str(),
						     nbinsx,-limx,+limx);
        m_plots["BX_P2" + algo + qual + whTag] = new TH1F(("BX_P2"+ algo + qual + "_" + whTag + "_P2").c_str(),
						   "BX total distribution; BX; entries",
						   nbinsBX,-limBX,+limBX);
        for (const auto & chambTag : chambTags){
	  m_plots["TanPsiRes_P2" + algo + qual + whTag + chambTag] = new TH1F(("hTanPsiRes" + algo + qual  + "_" + whTag + "_" + chambTag +  "_P2").c_str(),
							     ("TanPsiRes Seg-TP distribution for " + chambTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							     nbinstanpsi,-limtanpsi,+limtanpsi);
	  m_plots["PhiRes_P2" + algo + qual + whTag + chambTag] = new TH1F(("hPhiRes" + algo + qual + "_" + whTag  + "_" + chambTag +  "_P2").c_str(),
							  ("PhiRes Seg-TP distribution for " + chambTag +  "; #Delta#phi (rad); entries").c_str(),
							  nbinsphi,-limphi,+limphi);
	  m_plots["PhiBRes_P2" + algo + qual + whTag + chambTag] = new TH1F(("hPhiBRes" + algo + qual + "_" + whTag  + "_" + chambTag +  "_P2").c_str(),
							  ("PhiBRes Seg-TP distribution for " + chambTag +  "; #Delta#phiB (rad); entries").c_str(),
							  nbinsphiB,-limphiB,+limphiB);
	  m_plots["TimeRes_P2" + algo + qual + whTag + chambTag] = new TH1F(("hTimeRes" + algo + qual + "_" + whTag +"_" + chambTag +  "_P2").c_str(),
							   ("TimeRes Seg-TP distribution for " + chambTag +  "; #Delta t (ns); entries").c_str(),
							   nbinstime,-limtime,+limtime);
	  m_plots["xRes_P2" + algo + qual + whTag + chambTag] = new TH1S(("hxRes" + algo + qual + "_" + whTag +"_" + chambTag +  "_P2").c_str(),
							("xRes Seg-TP distribution for " + chambTag +  "; #Delta x (cm); entries").c_str(),
							nbinsx,-limx,+limx);
          m_plots["BX_P2" + algo + qual + whTag + chambTag] = new TH1F(("BX_P2"+ algo + qual + "_" + whTag + "_" + chambTag + "_P2").c_str(),
							   "BX total distribution; BX; entries",
							   nbinsBX,-limBX,+limBX);
          for (const auto & secTag : secTags){
	    m_plots["TanPsiRes_P2" + algo + qual + whTag + chambTag + secTag] = new TH1F(("hTanPsiRes" + algo + qual  + "_" + whTag + "_" + chambTag + "_" + secTag +  "_P2").c_str(),
							     ("TanPsiRes Seg-TP distribution for " + chambTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							     nbinstanpsi,-limtanpsi,+limtanpsi);
	    m_plots["PhiRes_P2" + algo + qual + whTag + chambTag + secTag] = new TH1F(("hPhiRes" + algo + qual + "_" + whTag  + "_" + chambTag+ "_" + secTag +  "_P2").c_str(),
							  ("PhiRes Seg-TP distribution for " + chambTag +  "; #Delta#phi (rad); entries").c_str(),
							  nbinsphi,-limphi,+limphi);
	    m_plots["PhiBRes_P2" + algo + qual + whTag + chambTag + secTag] = new TH1F(("hPhiBRes" + algo + qual + "_" + whTag  + "_" + chambTag+ "_" + secTag +  "_P2").c_str(),
							  ("PhiBRes Seg-TP distribution for " + chambTag +  "; #Delta#phiB (rad); entries").c_str(),
							  nbinsphiB,-limphiB,+limphiB);
  	    m_plots["TimeRes_P2" + algo + qual + whTag + chambTag + secTag] = new TH1F(("hTimeRes" + algo + qual + "_" + whTag +"_" + chambTag + "_" + secTag +  "_P2").c_str(),
							   ("TimeRes Seg-TP distribution for " + chambTag +  "; #Delta t (ns); entries").c_str(),
							   nbinstime,-limtime,+limtime);
	    m_plots["xRes_P2" + algo + qual + whTag + chambTag + secTag] = new TH1S(("hxRes" + algo + qual + "_" + whTag +"_" + chambTag + "_" + secTag +  "_P2").c_str(),
							("xRes Seg-TP distribution for " + chambTag +  "; #Delta x (cm); entries").c_str(),
							nbinsx,-limx,+limx);
            m_plots["BX_P2" + algo + qual + whTag + chambTag + secTag] = new TH1F(("BX_P2"+ algo + qual + "_" + whTag + "_" + chambTag + "_" + secTag + "_P2").c_str(),
							   "BX total distribution; BX; entries",
							   nbinsBX,-limBX,+limBX);
	  }
        }
      }
      
      for (const auto & secTag : secTags){
	m_plots["TanPsiRes_P2" + algo + qual + secTag] = new TH1F(("hTanPsiRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							   ("TanPsiRes Seg-TP distribution for " + secTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							   nbinstanpsi,-limtanpsi,+limtanpsi);
	m_plots["PhiRes_P2" + algo + qual + secTag] = new TH1F(("hPhiRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							("Phi Res Seg-TP distribution for " + secTag +  "; #Delta#phi (rad); entries").c_str(),
							nbinsphi,-limphi,+limphi);
	m_plots["PhiBRes_P2" + algo + qual + secTag] = new TH1F(("hPhiBRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							("PhiB Res Seg-TP distribution for " + secTag +  "; #Delta#phiB (rad); entries").c_str(),
							nbinsphiB,-limphiB,+limphiB);
	m_plots["TimeRes_P2" + algo + qual + secTag] = new TH1F(("hTimeRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							 ("TimeRes Seg-TP distribution for " + secTag +  "; #Delta t (ns); entries").c_str(),
							 nbinstime,-limtime,+limtime);
	m_plots["xRes_P2" + algo + qual + secTag] = new TH1S(("hxRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
						      ("xRes Seg-TP distribution for " + secTag +  "; #Delta x (cm); entries").c_str(),
						      nbinsx,-limx,+limx);
        m_plots["BX_P2" + algo + qual + secTag] = new TH1F(("BX_P2"+ algo + qual + "_" + secTag + "_P2").c_str(),
						   "BX total distribution; BX; entries",
						   nbinsBX,-limBX,+limBX);
      }

      for (const auto & chambTag : chambTags){
	m_plots["TanPsiRes_P2" + algo + qual + chambTag] = new TH1F(("hTanPsiRes" + algo + qual  +"_" + chambTag +  "_P2").c_str(),
							     ("TanPsiRes Seg-TP distribution for " + chambTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							     nbinstanpsi,-limtanpsi,+limtanpsi);
	m_plots["PhiRes_P2" + algo + qual + chambTag] = new TH1F(("hPhi Res" + algo + qual  +"_" + chambTag +  "_P2").c_str(),
							  ("PhiRes Seg-TP distribution for " + chambTag +  "; #Delta#phi (rad); entries").c_str(),
							  nbinsphi,-limphi,+limphi);
	m_plots["PhiBRes_P2" + algo + qual + chambTag] = new TH1F(("hPhiB Res" + algo + qual  +"_" + chambTag +  "_P2").c_str(),
							  ("PhiBRes Seg-TP distribution for " + chambTag +  "; #Delta#phiB (rad); entries").c_str(),
							  nbinsphiB,-limphiB,+limphiB);
	m_plots["TimeRes_P2" + algo + qual + chambTag] = new TH1F(("hTimeRes" + algo + qual  +"_" + chambTag +  "_P2").c_str(),
							   ("TimeRes Seg-TP distribution for " + chambTag +  "; #Delta t (ns); entries").c_str(),
							   nbinstime,-limtime,+limtime);
	m_plots["xRes_P2" + algo + qual + chambTag] = new TH1S(("hxRes" + algo + qual  +"_" + chambTag +  "_P2").c_str(),
							("xRes Seg-TP distribution for " + chambTag +  "; #Delta x (cm); entries").c_str(),
							nbinsx,-limx,+limx);
        m_plots["BX_P2" + algo + qual + chambTag] = new TH1F(("BX_P2"+ algo + qual + "_" + chambTag + "_P2").c_str(),
							   "BX total distribution; BX; entries",
							   nbinsBX,-limBX,+limBX);
      }
    }
  } // for algo
} // book



void DTNtupleTPGSimAnalyzer::fill()
{
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12","Sec13","Sec14"};

  for (std::size_t iGenPart = 0; iGenPart < gen_nGenParts; ++iGenPart){
    if (std::abs(gen_pdgId->at(iGenPart)) != 13 || gen_pt->at(iGenPart) < m_minMuPt) continue;

    // CB this should not be a vector ...
    std::vector<std::size_t> bestSegIndex = { 999, 999, 999, 999 };
    std::vector<Int_t> bestSegNHits       = { 0, 0, 0, 0 };


    for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg){
      Int_t segSt    = seg_station->at(iSeg);
      Int_t segWh    = seg_wheel->at(iSeg);
      Int_t segNHits = seg_phi_nHits->at(iSeg);
      Int_t segZNHits = seg_z_nHits->at(iSeg);
      float segt0 = seg_phi_t0->at(iSeg);
  
      Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
      Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
      
      m_plots["hPhiGenSeg" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(muSegDPhi);
      m_plots["hEtaGenSeg" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(muSegDEta);
      m_plots2["hEtaPhiGenSeg" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(muSegDEta,muSegDPhi);
      m_plots2["hPhiHitsGenSeg" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(muSegDPhi,segNHits);
      m_plots2["hEtaHitsGenSeg" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(muSegDEta,segZNHits);

      m_plots["ht0SegBefore" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(segt0);
      if(muSegDPhi < m_maxMuSegDPhi){
        m_plots["ht0SegAfterDPhi" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(segt0);
        if (muSegDPhi < m_maxMuSegDEta){
          m_plots["ht0SegAfterDEta" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(segt0);
          if (segNHits >= m_minSegHits){
            m_plots["ht0SegAfterSegHits" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(segt0);
            if (segZNHits >= m_minSegZHits){
              m_plots["ht0SegAfterSegZHits" + whTags.at(segWh+2) + chambTags.at(segSt-1)]->Fill(segt0);
            }
          }
        }
      }
 
     if (muSegDPhi < m_maxMuSegDPhi &&
          muSegDEta < m_maxMuSegDEta &&
          segNHits >= m_minSegHits &&
          segNHits >= bestSegNHits.at(segSt - 1)){
        bestSegNHits[segSt - 1] = segNHits;
        bestSegIndex[segSt - 1] = iSeg;
      }
    }
    

    // ==================== VARIABLES FOR THE HOUGH TRANSFORM BASED ALGORITHM
    for (const auto & iSeg : bestSegIndex){
      if (iSeg == 999) continue;
      std::vector <int> indexHB;
      std::vector <Double_t> DPhiHB;
      std::vector <int> indexAM;
      std::vector <Double_t> DPhiAM;
      indexHB.clear(); 
      indexAM.clear(); 
      DPhiHB.clear(); 
      DPhiAM.clear(); 

      Int_t segWh  = seg_wheel->at(iSeg);
      Int_t segSec = seg_sector->at(iSeg); if (segSec == 13) segSec=4; if (segSec == 14) segSec=10;
      Int_t segSt  = seg_station->at(iSeg);
      
      std::string chambTag = chambTags.at(segSt - 1);
      std::string whTag    = whTags.at(segWh+2);
      std::string secTag   = secTags.at(segSec-1);

      int bestTPHB = -1;
      Double_t bestSegTrigHBDPhi = 1000;
      Double_t bestHBDPhi = 0;

      for (std::size_t iTrigHB = 0; iTrigHB < ph2TpgPhiEmuHb_nTrigs; ++iTrigHB){
        Int_t trigHBWh  = ph2TpgPhiEmuHb_wheel->at(iTrigHB);
        Int_t trigHBSec = ph2TpgPhiEmuHb_sector->at(iTrigHB);
        Int_t trigHBSt  = ph2TpgPhiEmuHb_station->at(iTrigHB);
        Int_t trigHBBX  = ph2TpgPhiEmuHb_BX->at(iTrigHB);

        if (segWh  == trigHBWh && segSec == trigHBSec &&  segSt  == trigHBSt){
          Double_t mySegPhi;
          if (ph2TpgPhiEmuHb_superLayer->at(iTrigHB)==1) {
	    mySegPhi = seg_posGlb_phi_SL1->at(iSeg);
          } else if (ph2TpgPhiEmuHb_superLayer->at(iTrigHB)==3) {
	    mySegPhi = seg_posGlb_phi_SL3->at(iSeg);
	  } else {
	    mySegPhi = seg_posGlb_phi->at(iSeg); 
	  }
          Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuHb_phi->at(iTrigHB),trigHBSec);
          if (segWh==-2 && segSt == 1) m_plots["Phi_HB"+secTag]->Fill(trigGlbPhi);
	  //Double_t finalHBDPhi   = acos(cos(mySegPhi - trigGlbPhi));
          Double_t finalHBDPhi   = mySegPhi - trigGlbPhi;
          //Double_t segTrigHBDPhi = abs(finalHBDPhi);
          Double_t segTrigHBDPhi = abs(acos(cos(finalHBDPhi)));
          // if (segTrigHBDPhi < m_maxSegTrigDPhi && trigHBBX == 20 &&  bestSegTrigHBDPhi > segTrigHBDPhi)
          if (unique) {
            if (segTrigHBDPhi < m_maxSegTrigDPhi && bestSegTrigHBDPhi > segTrigHBDPhi){
	      bestTPHB = iTrigHB; 
	      bestSegTrigHBDPhi = segTrigHBDPhi;
	      bestHBDPhi = finalHBDPhi;
	    }
	  } else {
	    if (trigHBBX != 20) continue; 
	    DPhiHB.push_back(TVector2::Phi_mpi_pi(finalHBDPhi));
	    indexHB.push_back(iTrigHB);
          }
        }
      }
      if (unique && bestTPHB > -1) {
        DPhiHB.push_back(TVector2::Phi_mpi_pi(bestHBDPhi));
	indexHB.push_back(bestTPHB);
      }

      //if (bestTPHB > -1){
      for (unsigned int i = 0; i < indexHB.size(); i++) {
	bool isCorrelated =  (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 6) || (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 8) || (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 9);
	std::vector<std::string> corrList;
	corrList.push_back( "All" );
	if (isCorrelated) { 
	  corrList.push_back( "Correlated" );
	  if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 9) corrList.push_back("Q9");
	  if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 8) corrList.push_back("Q8");
	  if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 6) corrList.push_back("Q6");
	}
	else {
          corrList.push_back( "Uncorrelated" );
	  if ( (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 3 ) || (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 5 )) corrList.push_back( "3h" );
	  else if ((ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 4) || (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) == 7)) corrList.push_back( "4h" );
        }
	//if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) >= 6) corrList.push_back("CorrelExt");
        if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) >= 4 && ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) != 5) corrList.push_back("Legacy");
        //if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) >= 4) corrList.push_back("min4");
        //if (ph2TpgPhiEmuHb_quality->at(indexHB.at(i)) >= 3) corrList.push_back("min3");


	for (const auto & corr : corrList){
	  // TanPsi
	  Double_t segLocalHBDtanpsi = (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) - tan (TMath::TwoPi() * ph2TpgPhiEmuHb_dirLoc_phi->at(indexHB.at(i)) / 360 );
	  m_plots["TanPsiRes_P2_HB" + corr ]           ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + whTag]   ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + whTag + chambTag]   ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + whTag + chambTag + secTag]   ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + secTag]  ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + chambTag]->Fill( segLocalHBDtanpsi );
	  
          

 
	  // Phi
	  m_plots["PhiRes_P2_HB" + corr]           ->Fill( DPhiHB.at(i) );
	  m_plots["PhiRes_P2_HB" + corr + whTag]   ->Fill( DPhiHB.at(i) );
	  m_plots["PhiRes_P2_HB" + corr + whTag + chambTag]   ->Fill( DPhiHB.at(i) );
	  m_plots["PhiRes_P2_HB" + corr + whTag + chambTag + secTag]   ->Fill( DPhiHB.at(i) );
	  m_plots["PhiRes_P2_HB" + corr + secTag]  ->Fill( DPhiHB.at(i) );
	  m_plots["PhiRes_P2_HB" + corr + chambTag]->Fill( DPhiHB.at(i) );
	  
	  // PhiB
          Double_t segPhi;
          if (ph2TpgPhiEmuHb_superLayer->at(indexHB.at(i))==1) {
	    segPhi = seg_posGlb_phi_SL1->at(iSeg);
          } else if (ph2TpgPhiEmuHb_superLayer->at(indexHB.at(i))==3) {
	    segPhi = seg_posGlb_phi_SL3->at(iSeg);
	  } else {
	    segPhi = seg_posGlb_phi->at(iSeg); 
	  }
          double segPsi = atan ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg) );
	  double segPhiB = atan(tan(segPsi - (segPhi - TMath::Pi() / 6 * (segSec - 1))));
	  double DPhiBHB = -999; 
	  if (fabs(segPhiB - 1.4*ph2TpgPhiEmuHb_phiB->at(indexHB.at(i)) / 2048. ) < 0.1) {
	    //DPhiBHB = segPhiB;
	    DPhiBHB = segPhiB - 1.4*ph2TpgPhiEmuHb_phiB->at(indexHB.at(i)) / 2048.;
	  } else {
	    segPhiB = atan(tan(-segPsi - (segPhi - TMath::Pi() / 6 * (segSec - 1))));
	    //DPhiBHB = segPhiB;
	    DPhiBHB = segPhiB - 1.4*ph2TpgPhiEmuHb_phiB->at(indexHB.at(i)) / 2048.;
          }
	 
	  m_plots["PhiBRes_P2_HB" + corr]           ->Fill( DPhiBHB );
	  m_plots["PhiBRes_P2_HB" + corr + whTag]   ->Fill( DPhiBHB );
	  m_plots["PhiBRes_P2_HB" + corr + whTag + chambTag]   ->Fill( DPhiBHB );
	  m_plots["PhiBRes_P2_HB" + corr + whTag + chambTag + secTag]   ->Fill( DPhiBHB );
	  m_plots["PhiBRes_P2_HB" + corr + secTag]  ->Fill( DPhiBHB );
	  m_plots["PhiBRes_P2_HB" + corr + chambTag]->Fill( DPhiBHB );
	  
	  // Time
	  if (seg_phi_t0->at(iSeg) > -500){
	    Short_t segLocalHBDtime = seg_phi_t0->at(iSeg) - (ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20)*25;
	      

	    m_plots["TimeRes_P2_HB" + corr]           ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + whTag]   ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + whTag + chambTag]   ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + whTag + chambTag + secTag]   ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + secTag]  ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + chambTag]->Fill( segLocalHBDtime );
	  
          
	  // BX
	    m_plots["BX_P2_HB" + corr]           ->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	    m_plots["BX_P2_HB" + corr + whTag]   ->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	    m_plots["BX_P2_HB" + corr + whTag + chambTag]   ->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	    m_plots["BX_P2_HB" + corr + whTag + chambTag + secTag]   ->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	    m_plots["BX_P2_HB" + corr + secTag]  ->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	    m_plots["BX_P2_HB" + corr + chambTag]->Fill( ph2TpgPhiEmuHb_BX->at(indexHB.at(i)) - 20 );
	  }

	  // x
	  Double_t segLocalHBDx = 0;
	  if (isCorrelated){
	    segLocalHBDx = seg_posLoc_x_midPlane->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(indexHB.at(i));
	  }
	  else if (ph2TpgPhiEmuHb_superLayer->at(indexHB.at(i)) == 1) segLocalHBDx = seg_posLoc_x_SL1->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(indexHB.at(i));
	  else                                                   segLocalHBDx = seg_posLoc_x_SL3->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(indexHB.at(i));
	  m_plots["xRes_P2_HB" + corr]           ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + whTag]   ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + whTag + chambTag]   ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + whTag + chambTag + secTag]   ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + secTag]  ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + chambTag]->Fill( segLocalHBDx );
	}
      }
    

      // ==================== VARIABLES FOR THE ANALYTICAL METHOD ALGORITHM
      int bestTPAM = -1;
      Double_t bestSegTrigAMDPhi = 1000;
      Double_t bestAMDPhi = 0;
      int nPrims = 0; 
      for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM){

        Int_t trigAMWh  = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
        Int_t trigAMSec = ph2TpgPhiEmuAm_sector->at(iTrigAM);
        Int_t trigAMSt  = ph2TpgPhiEmuAm_station->at(iTrigAM);
        Int_t trigAMBX  = ph2TpgPhiEmuAm_BX->at(iTrigAM);

        if (segWh  == trigAMWh && segSec == trigAMSec &&  segSt  == trigAMSt){
          Double_t trigGlbPhi    = trigPhiInRad(ph2TpgPhiEmuAm_phi->at(iTrigAM),trigAMSec);
          if (segWh==-2 && segSt == 1) m_plots["Phi_AM"+secTag]->Fill(trigGlbPhi);
          Double_t mySegPhi;
         // if (false) {
          if (ph2TpgPhiEmuAm_superLayer->at(iTrigAM)==1) {
	    mySegPhi = seg_posGlb_phi_SL1->at(iSeg);
          } else if (ph2TpgPhiEmuAm_superLayer->at(iTrigAM)==3) {
	    mySegPhi = seg_posGlb_phi_SL3->at(iSeg);
	  } else {
	    mySegPhi = seg_posGlb_phi->at(iSeg); 
	  }
          //Double_t finalAMDPhi   = acos(cos(mySegPhi - trigGlbPhi));
          Double_t finalAMDPhi   = mySegPhi - trigGlbPhi;
          //Double_t segTrigAMDPhi = abs(finalAMDPhi);
          Double_t segTrigAMDPhi = abs(acos(cos(finalAMDPhi)));
	  nPrims++;        

          m_plots["hQualAll"]->Fill(ph2TpgPhiEmuAm_quality->at(iTrigAM));

          if (segTrigAMDPhi < m_maxSegTrigDPhi) m_plots["hQualEff"]->Fill(ph2TpgPhiEmuAm_quality->at(iTrigAM));
	  
          //if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (trigAMBX == 20) && (bestSegTrigAMDPhi > segTrigAMDPhi)) 
//          if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (bestSegTrigAMDPhi > segTrigAMDPhi)){
          if (unique) {
            if (segTrigAMDPhi < m_maxSegTrigDPhi && bestSegTrigAMDPhi > segTrigAMDPhi){
	      bestTPAM = iTrigAM; 
	      bestSegTrigAMDPhi = segTrigAMDPhi;
	      bestAMDPhi = finalAMDPhi;
	    }
	  } else { 
	    if (trigAMBX != 20 || ph2TpgPhiEmuAm_index->at(iTrigAM)!=0) continue; 
	    DPhiAM.push_back(TVector2::Phi_mpi_pi(finalAMDPhi));
	    indexAM.push_back(iTrigAM);
          }
        }
      }

      if (unique && bestTPAM > -1) {
         DPhiAM.push_back(TVector2::Phi_mpi_pi(bestAMDPhi));
         indexAM.push_back(bestTPAM);
      }

      

      m_plots["hPrims"]->Fill(nPrims);

      for (unsigned int i = 0; i < DPhiAM.size(); i++){
        /*m_plots["PhiDif"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg));
        m_plots2["PhiDif2D"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL3->at(iSeg));
        if (segSt == 1) m_plots2["PhiDif2DPtMB1"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 2) m_plots2["PhiDif2DPtMB2"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 3) m_plots2["PhiDif2DPtMB3"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 4) m_plots2["PhiDif2DPtMB4"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (ph2TpgPhiEmuAm_superLayer->at(bestTPAM)==1) m_plots["Phi"]->Fill(seg_posGlb_phi_SL1->at(iSeg));
        */
        //m_plots["hQualBest"]->Fill(ph2TpgPhiEmuAm_quality->at(bestTPAM));
        // TanPsi
	bool isCorrelated =  (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 6) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 8) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 9);
	std::vector<std::string> corrList;
	corrList.push_back( "All" );
	if (isCorrelated) { 
	  corrList.push_back( "Correlated" );
	  if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 9) corrList.push_back("Q9");
	  if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 8) corrList.push_back("Q8");
	  if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 6) corrList.push_back("Q6");
	}
	else {
          corrList.push_back( "Uncorrelated" );
	  if ((ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 1) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 2) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 5 )) corrList.push_back( "3h" );
	  else if ((ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 3) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 4) || (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) == 7 )) corrList.push_back( "4h" );
	}	
       // if (isCorrelated) corrList.push_back("CorrelExt");
        if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) >= 3 && ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) != 5) corrList.push_back("Legacy");
       // if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) >= 3 && ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) != 5) corrList.push_back("min4");
       // if (ph2TpgPhiEmuAm_quality->at(indexAM.at(i)) >= 1) corrList.push_back("min3");
	
        for (const auto & corr : corrList){
	  Double_t segLocalAMDtanpsi = (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) - tan (TMath::TwoPi() * ph2TpgPhiEmuAm_dirLoc_phi->at(indexAM.at(i)) / 360 ) ; 
	  m_plots["TanPsiRes_P2_AM" + corr]           ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + whTag]   ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + whTag + chambTag]   ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + whTag + chambTag + secTag]   ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + secTag]  ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + chambTag]->Fill( segLocalAMDtanpsi );
	  
	  // Phi
	  m_plots["PhiRes_P2_AM" + corr]           ->Fill( DPhiAM.at(i) );
	  m_plots["PhiRes_P2_AM" + corr + whTag]   ->Fill( DPhiAM.at(i) );
	  m_plots["PhiRes_P2_AM" + corr + whTag + chambTag]   ->Fill( DPhiAM.at(i) );
	  m_plots["PhiRes_P2_AM" + corr + whTag + chambTag + secTag]   ->Fill( DPhiAM.at(i) );
	  m_plots["PhiRes_P2_AM" + corr + secTag]  ->Fill( DPhiAM.at(i) );
	  m_plots["PhiRes_P2_AM" + corr + chambTag]->Fill( DPhiAM.at(i) );
	  
	  // PhiB
          Double_t segPhi;
          if (ph2TpgPhiEmuAm_superLayer->at(indexAM.at(i))==1) {
	    segPhi = seg_posGlb_phi_SL1->at(iSeg);
          } else if (ph2TpgPhiEmuAm_superLayer->at(indexAM.at(i))==3) {
	    segPhi = seg_posGlb_phi_SL3->at(iSeg);
	  } else {
	    segPhi = seg_posGlb_phi->at(iSeg); 
	  }
          double segPsi = atan ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg) );
	  double segPhiB = atan(tan(segPsi - (segPhi - TMath::Pi() / 6 * (segSec - 1))));
	  double DPhiBAM = -999; 
	  if (fabs(segPhiB - 1.4*ph2TpgPhiEmuAm_phiB->at(indexAM.at(i)) / 2048. ) < 0.1) {
	    DPhiBAM = segPhiB - 1.4*ph2TpgPhiEmuAm_phiB->at(indexAM.at(i)) / 2048.;
	  } else {
	    segPhiB = atan(tan(-segPsi - (segPhi - TMath::Pi() / 6 * (segSec - 1))));
	    DPhiBAM = segPhiB - 1.4*ph2TpgPhiEmuAm_phiB->at(indexAM.at(i)) / 2048.;
          }
	 
	  m_plots["PhiBRes_P2_AM" + corr]           ->Fill( DPhiBAM );
	  m_plots["PhiBRes_P2_AM" + corr + whTag]   ->Fill( DPhiBAM );
	  m_plots["PhiBRes_P2_AM" + corr + whTag + chambTag]   ->Fill( DPhiBAM );
	  m_plots["PhiBRes_P2_AM" + corr + whTag + chambTag + secTag]   ->Fill( DPhiBAM );
	  m_plots["PhiBRes_P2_AM" + corr + secTag]  ->Fill( DPhiBAM );
	  m_plots["PhiBRes_P2_AM" + corr + chambTag]->Fill( DPhiBAM );
	  // Time
	  if (seg_phi_t0->at(iSeg) > -500){
	    //float segLocalAMDtime = seg_phi_t0->at(iSeg)  - 25*(ph2TpgPhiEmuAm_BX->at(bestTPAM)-20);
	    float segLocalAMDtime = seg_phi_t0->at(iSeg)  - ph2TpgPhiEmuAm_t0->at(indexAM.at(i)) + 20*25;
	    
          // Time
	    m_plots["TimeRes_P2_AM" + corr]           ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + whTag]   ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + whTag + chambTag]   ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + whTag + chambTag + secTag]   ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + secTag]  ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + chambTag]->Fill( segLocalAMDtime );
	  
	
          // BX
	    m_plots["BX_P2_AM" + corr]           ->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	    m_plots["BX_P2_AM" + corr + whTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	    m_plots["BX_P2_AM" + corr + whTag + chambTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	    m_plots["BX_P2_AM" + corr + whTag + chambTag + secTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	    m_plots["BX_P2_AM" + corr + secTag]  ->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	    m_plots["BX_P2_AM" + corr + chambTag]->Fill( ph2TpgPhiEmuAm_BX->at(indexAM.at(i)) - 20);
	  }
	  // x
	  Double_t segLocalAMDx = 0;
	  if (isCorrelated){
	    segLocalAMDx = seg_posLoc_x_midPlane->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(indexAM.at(i));
	  }
	  else if (ph2TpgPhiEmuAm_superLayer->at(indexAM.at(i)) == 1) segLocalAMDx = seg_posLoc_x_SL1->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(indexAM.at(i));
	  else                                                   segLocalAMDx = seg_posLoc_x_SL3->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(indexAM.at(i));
	  m_plots["xRes_P2_AM"+ corr]           ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + whTag]   ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + whTag + chambTag]   ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + whTag + chambTag + secTag]   ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + secTag]  ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + chambTag]->Fill( segLocalAMDx );
	}
      }
    }
  }
}


TH1F* DTNtupleTPGSimAnalyzer::makeHistoPerCorrelated( std::string mag, std::string suffix, vector<std::string> tags, std::string algo)
{

  std::map<std::string,float> ranges;
  ranges["TimeRes"  ] = 10;
  ranges["PhiRes"   ] = 0.0005;
  ranges["PhiBRes"   ] = 0.05;
  ranges["TanPsiRes"] = 0.02;
  ranges["xRes"     ] = 0.2;

  std::map<std::string,float> means;
  means["TimeRes"  ] = 10;
  means["PhiRes"   ] = 0.0005;
  means["PhiBRes"  ] = 0.05;
  means["TanPsiRes"] = 0.02;
  means["xRes"     ] = 0.1;

  std::map<std::string,float> sigmas;
  sigmas["TimeRes"  ] = 10;
  sigmas["PhiRes"   ] = 0.0005;
  sigmas["PhiBRes"   ] = 0.05;
  sigmas["TanPsiRes"] = 0.1;
  sigmas["xRes"     ] = 0.1;




  TH1F* ret = new TH1F((mag+suffix+algo).c_str(),"",
            tags.size(), -0.5,-0.5+tags.size());
  for (unsigned int i = 0; i < tags.size(); ++i){
    ret->GetXaxis()->SetBinLabel(i+1, tags[i].c_str());

    if (m_plots[mag + "_P2_" + algo + tags[i]]->Integral()){
//      m_plots[mag + "_P2_" + algo + tags[i]]->Fit("gaus","SQ","",-ranges[mag],ranges[mag]);


      RooRealVar x("x","",-ranges[mag],ranges[mag]);
      RooRealVar mean("mean",",mean of Gaussian",0.,-means[mag],means[mag]);
      RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag]/2.5,0,sigmas[mag]);
      RooRealVar sigma2("sigma2",",width of wide Gaussian",ranges[mag],0,sigmas[mag]);
      RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);

      RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
      RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);

      RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2),fraction);

      RooDataHist data("data","data",x,m_plots[mag + "_P2_" + algo + tags[i]]);

      twogauss.fitTo(data,RooFit::Extended());
/*
      RooPlot* xframe=x.frame();
      data.plotOn(xframe);
      twogauss.plotOn(xframe);
      twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
      twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
*/
      ret->SetBinContent(i+1, sigma1.getVal());

//      ret->SetBinContent(i+1, m_plots[mag + "_P2_" + algo + tags[i]]->GetFunction("gaus")->GetParameter(2));
    }
  }
  return ret;
}

TH1F* DTNtupleTPGSimAnalyzer::makeHistoPer( std::string mag, std::string suffix, vector<std::string> tags, std::string algo)
{

  std::map<std::string,float> ranges;
  ranges["TimeRes"  ] = 10;
  ranges["PhiRes"   ] = 0.0005;
  ranges["PhiBRes"   ] = 0.05;
  ranges["TanPsiRes"] = 0.02;
  ranges["xRes"     ] = 0.2;

  TH1F* ret = new TH1F((mag+suffix+algo).c_str(),"",
            tags.size(), -0.5,-0.5+tags.size());
  for (unsigned int i = 0; i < tags.size(); ++i){
    ret->GetXaxis()->SetBinLabel(i+1, tags[i].c_str());

    if (m_plots[mag + "_P2_" + algo + tags[i]]->Integral()){
//      m_plots[mag + "_P2_" + algo + tags[i]]->Fit("gaus","SQ","",-ranges[mag],ranges[mag]);


      RooRealVar x("x","",-ranges[mag],ranges[mag]);
      RooRealVar mean("mean",",mean of Gaussian",0.,-ranges[mag]/2,ranges[mag]/2);
      //RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag]/2.5,0,25*ranges[mag]);
      RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag]/20,0,ranges[mag]);
      RooRealVar sigma2("sigma2",",width of wide Gaussian",ranges[mag]/2,0,3*ranges[mag]);
      RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);

      RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
      RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);

      RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2),fraction);

      RooDataHist data("data","data",x,m_plots[mag + "_P2_" + algo + tags[i]]);

      twogauss.fitTo(data,RooFit::Extended());
/*
      RooPlot* xframe=x.frame();
      data.plotOn(xframe);
      twogauss.plotOn(xframe);
      twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
      twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
*/
      ret->SetBinContent(i+1, sigma1.getVal());

//      ret->SetBinContent(i+1, m_plots[mag + "_P2_" + algo + tags[i]]->GetFunction("gaus")->GetParameter(2));
    }
  }
  return ret;
}


void DTNtupleTPGSimAnalyzer::endJob()
{
  // make the fits
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12","Sec13","Sec14"};
  std::vector<std::string> magnitudes = { "TimeRes", "PhiRes","PhiBRes", "TanPsiRes", "xRes"};
  std::vector<std::string> algos      = { "AM", "HB" };
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated", "All"};
  std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "Legacy","Q9","Q8","Q6"};
  
  for (const auto & mag : magnitudes){
    for (const auto & algo : algos){
      for (const auto & qual : qualTags){
	if (qual == "Correlated" || qual == "Q9" || qual == "Q8" || qual == "Q6" ) { 
          m_plots[mag + "_" + algo + qual + "_Res_perChamb"] = makeHistoPerCorrelated( mag, "_Res_perChamb", chambTags, algo + qual);
	  m_plots[mag + "_" + algo + qual + "_Res_perWheel"] = makeHistoPerCorrelated( mag, "_Res_perWheel", whTags   , algo + qual);
	  m_plots[mag + "_" + algo + qual + "_Res_perSec"]   = makeHistoPerCorrelated( mag, "_Res_perSec"  , secTags  , algo + qual);
        } else {
          m_plots[mag + "_" + algo + qual + "_Res_perChamb"] = makeHistoPer( mag, "_Res_perChamb", chambTags, algo + qual);
	  m_plots[mag + "_" + algo + qual + "_Res_perWheel"] = makeHistoPer( mag, "_Res_perWheel", whTags   , algo + qual);
	  m_plots[mag + "_" + algo + qual + "_Res_perSec"]   = makeHistoPer( mag, "_Res_perSec"  , secTags  , algo + qual);

	}
      }
    }
  }


  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();
}



Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1);
}
