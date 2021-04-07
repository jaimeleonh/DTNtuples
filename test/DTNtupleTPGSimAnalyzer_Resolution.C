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

//  gSystem->Load("libRooFit");
//
//  using namespace RooFit;


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
  std::vector<std::string> whTags     = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};
  std::vector<std::string> secTags    = { "Sec.1", "Sec.2", "Sec.3", "Sec.4", "Sec.5", "Sec.6", "Sec.7", "Sec.8","Sec.9","Sec.10","Sec.11","Sec.12","Sec.13","Sec.14"};
  std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All"};

      Double_t limtanpsi   = 0.04; Double_t limphi   = 0.0015; Double_t limtime  = 50;   Double_t limx   = 0.6; Double_t limBX   = 8.5;
      //Double_t limtanpsi   = 0.1; Double_t limphi   = 0.005; Double_t limtime  = 50;   Double_t limx   = 0.2; Double_t limBX   = 8.5;
      UShort_t nbinstanpsi = 101; UShort_t nbinsphi = 101;   UShort_t nbinstime = 101; UShort_t nbinsx = 101; UShort_t nbinsBX = 17;
  
  m_plots["hQualAll"] = new TH1F("HQualAll", "All primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hQualEff"] = new TH1F("HQualEff", "Efficient primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hQualBest"] = new TH1F("HQualBest", "Best primitives quality distribution; Quality; Entries",9,0.5,9.5 );
  m_plots["hPrims"] = new TH1F("HPrims", "Total primitives per chamber; Number of primitives; Entries",31,-0.5,30.5 );

  m_plots["PhiDif"] = new TH1F("PhiDif",
					     "Difference between phi SL1 & phi center; DeltaPhi (rad); entries",
					     10000,-0.5,0.5);
  m_plots["Phi"] = new TH1F("Phi",
		 		"Phi SL1; DeltaPhi (rad); entries",
				1000,-30,+30);
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
	  m_plots["TimeRes_P2" + algo + qual + whTag + chambTag] = new TH1F(("hTimeRes" + algo + qual + "_" + whTag +"_" + chambTag +  "_P2").c_str(),
							   ("TimeRes Seg-TP distribution for " + chambTag +  "; #Delta t (ns); entries").c_str(),
							   nbinstime,-limtime,+limtime);
	  m_plots["xRes_P2" + algo + qual + whTag + chambTag] = new TH1S(("hxRes" + algo + qual + "_" + whTag +"_" + chambTag +  "_P2").c_str(),
							("xRes Seg-TP distribution for " + chambTag +  "; #Delta x (cm); entries").c_str(),
							nbinsx,-limx,+limx);
          m_plots["BX_P2" + algo + qual + whTag + chambTag] = new TH1F(("BX_P2"+ algo + qual + "_" + whTag + "_" + chambTag + "_P2").c_str(),
							   "BX total distribution; BX; entries",
							   nbinsBX,-limBX,+limBX);
        }
      }
      
      for (const auto & secTag : secTags){
	m_plots["TanPsiRes_P2" + algo + qual + secTag] = new TH1F(("hTanPsiRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							   ("TanPsiRes Seg-TP distribution for " + secTag +  "; #Delta tan(#psi) (adim.); entries").c_str(),
							   nbinstanpsi,-limtanpsi,+limtanpsi);
	m_plots["PhiRes_P2" + algo + qual + secTag] = new TH1F(("hPhiRes" + algo + qual  +"_" + secTag +  "_P2").c_str(),
							("Phi Res Seg-TP distribution for " + secTag +  "; #Delta#phi (rad); entries").c_str(),
							nbinsphi,-limphi,+limphi);
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
  std::vector<std::string> whTags    = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};
  std::vector<std::string> secTags   = { "Sec.1", "Sec.2", "Sec.3", "Sec.4", "Sec.5", "Sec.6", "Sec.7", "Sec.8","Sec.9","Sec.10","Sec.11","Sec.12","Sec.13","Sec.14"};

  for (std::size_t iGenPart = 0; iGenPart < gen_nGenParts; ++iGenPart){
    if (std::abs(gen_pdgId->at(iGenPart)) != 13 || gen_pt->at(iGenPart) < m_minMuPt) continue;

    // CB this should not be a vector ...
    std::vector<std::size_t> bestSegIndex = { 999, 999, 999, 999 };
    std::vector<Int_t> bestSegNHits       = { 0, 0, 0, 0 };


    for (std::size_t iSeg = 0; iSeg < seg_nSegments; ++iSeg){
      Int_t segSt    = seg_station->at(iSeg);
      Int_t segNHits = seg_phi_nHits->at(iSeg);
      
      Double_t muSegDPhi = std::abs(acos(cos(gen_phi->at(iGenPart) - seg_posGlb_phi->at(iSeg))));
      Double_t muSegDEta = std::abs(gen_eta->at(iGenPart) - seg_posGlb_eta->at(iSeg));
      
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
          Double_t finalHBDPhi   = mySegPhi - trigGlbPhi;
          Double_t segTrigHBDPhi = abs(acos(cos(finalHBDPhi)));
//           if (segTrigHBDPhi < m_maxSegTrigDPhi && trigHBBX == 20 &&  bestSegTrigHBDPhi > segTrigHBDPhi)
          if (segTrigHBDPhi < m_maxSegTrigDPhi && bestSegTrigHBDPhi > segTrigHBDPhi){
            bestTPHB          = iTrigHB;
            bestSegTrigHBDPhi = segTrigHBDPhi;
            bestHBDPhi        = TVector2::Phi_mpi_pi(finalHBDPhi);
          }
        }
      }

      if (bestTPHB > -1){

	bool isCorrelated =  (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 6) || (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 8) || (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 9);
	std::vector<std::string> corrList;
	corrList.push_back( "All" );
	if (isCorrelated) corrList.push_back( "Correlated" );
	else {
          corrList.push_back( "Uncorrelated" );
	  if ( (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 3 ) || (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 5 )) corrList.push_back( "3h" );
	  else if ((ph2TpgPhiEmuHb_quality->at(bestTPHB) == 4) || (ph2TpgPhiEmuHb_quality->at(bestTPHB) == 7)) corrList.push_back( "4h" );
        }


	for (const auto & corr : corrList){
	  // TanPsi
	  Double_t segLocalHBDtanpsi = (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) - TMath::TwoPi() * ph2TpgPhiEmuHb_dirLoc_phi->at(bestTPHB) / 360;
	  m_plots["TanPsiRes_P2_HB" + corr ]           ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + whTag]   ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + whTag + chambTag]   ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + secTag]  ->Fill( segLocalHBDtanpsi );
	  m_plots["TanPsiRes_P2_HB" + corr  + chambTag]->Fill( segLocalHBDtanpsi );
	  
          

 
	  // Phi
	  m_plots["PhiRes_P2_HB" + corr]           ->Fill( bestHBDPhi );
	  m_plots["PhiRes_P2_HB" + corr + whTag]   ->Fill( bestHBDPhi );
	  m_plots["PhiRes_P2_HB" + corr + whTag + chambTag]   ->Fill( bestHBDPhi );
	  m_plots["PhiRes_P2_HB" + corr + secTag]  ->Fill( bestHBDPhi );
	  m_plots["PhiRes_P2_HB" + corr + chambTag]->Fill( bestHBDPhi );
	  
	  
	  // Time
	  if (seg_phi_t0->at(iSeg) > -500){
	    Short_t segLocalHBDtime = seg_phi_t0->at(iSeg) - (ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20)*25;
	      

	    m_plots["TimeRes_P2_HB" + corr]           ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + whTag]   ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + whTag + chambTag]   ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + secTag]  ->Fill( segLocalHBDtime );
	    m_plots["TimeRes_P2_HB" + corr + chambTag]->Fill( segLocalHBDtime );
	  
          
	  // BX
	    m_plots["BX_P2_HB" + corr]           ->Fill( ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20 );
	    m_plots["BX_P2_HB" + corr + whTag]   ->Fill( ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20 );
	    m_plots["BX_P2_HB" + corr + whTag + chambTag]   ->Fill( ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20 );
	    m_plots["BX_P2_HB" + corr + secTag]  ->Fill( ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20 );
	    m_plots["BX_P2_HB" + corr + chambTag]->Fill( ph2TpgPhiEmuHb_BX->at(bestTPHB) - 20 );
	  }

	  // x
	  Double_t segLocalHBDx = 0;
	  if (isCorrelated){
	    segLocalHBDx = seg_posLoc_x_midPlane->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(bestTPHB);
	  }
	  else if (ph2TpgPhiEmuHb_superLayer->at(bestTPHB) == 1) segLocalHBDx = seg_posLoc_x_SL1->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(bestTPHB);
	  else                                                   segLocalHBDx = seg_posLoc_x_SL3->at(iSeg) - ph2TpgPhiEmuHb_posLoc_x->at(bestTPHB);
	  m_plots["xRes_P2_HB" + corr]           ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + whTag]   ->Fill( segLocalHBDx );
	  m_plots["xRes_P2_HB" + corr + whTag + chambTag]   ->Fill( segLocalHBDx );
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
          Double_t mySegPhi;
         // if (false) {
          if (ph2TpgPhiEmuAm_superLayer->at(iTrigAM)==1) {
	    mySegPhi = seg_posGlb_phi_SL1->at(iSeg);
          } else if (ph2TpgPhiEmuAm_superLayer->at(iTrigAM)==3) {
	    mySegPhi = seg_posGlb_phi_SL3->at(iSeg);
	  } else {
	    mySegPhi = seg_posGlb_phi->at(iSeg); 
	  }
          Double_t finalAMDPhi   = mySegPhi - trigGlbPhi;
          Double_t segTrigAMDPhi = abs(acos(cos(finalAMDPhi)));
	  nPrims++;        

          m_plots["hQualAll"]->Fill(ph2TpgPhiEmuAm_quality->at(iTrigAM));

          if (segTrigAMDPhi < m_maxSegTrigDPhi) m_plots["hQualEff"]->Fill(ph2TpgPhiEmuAm_quality->at(iTrigAM));
	  
//           if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (trigAMBX == 0) && (bestSegTrigAMDPhi > segTrigAMDPhi))
          if ((segTrigAMDPhi < m_maxSegTrigDPhi) && (bestSegTrigAMDPhi > segTrigAMDPhi)){
            bestTPAM          = iTrigAM;
            bestSegTrigAMDPhi = segTrigAMDPhi;
            bestAMDPhi        = TVector2::Phi_mpi_pi(finalAMDPhi);
          }
        }
      }

      m_plots["hPrims"]->Fill(nPrims);

      if (bestTPAM > -1){
        /*m_plots["PhiDif"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg));
        m_plots2["PhiDif2D"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL3->at(iSeg));
        if (segSt == 1) m_plots2["PhiDif2DPtMB1"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 2) m_plots2["PhiDif2DPtMB2"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 3) m_plots2["PhiDif2DPtMB3"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (segSt == 4) m_plots2["PhiDif2DPtMB4"]->Fill(seg_posGlb_phi->at(iSeg)- seg_posGlb_phi_SL1->at(iSeg), gen_pt->at(iGenPart));
        if (ph2TpgPhiEmuAm_superLayer->at(bestTPAM)==1) m_plots["Phi"]->Fill(seg_posGlb_phi_SL1->at(iSeg));
        */
        m_plots["hQualBest"]->Fill(ph2TpgPhiEmuAm_quality->at(bestTPAM));
        // TanPsi
	bool isCorrelated =  (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 6) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 8) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 9);
	std::vector<std::string> corrList;
	corrList.push_back( "All" );
	if (isCorrelated) corrList.push_back( "Correlated" );
	else {
          corrList.push_back( "Uncorrelated" );
	  if ((ph2TpgPhiEmuAm_quality->at(bestTPAM) == 1) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 2) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 5 )) corrList.push_back( "3h" );
	  else if ((ph2TpgPhiEmuAm_quality->at(bestTPAM) == 3) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 4) || (ph2TpgPhiEmuAm_quality->at(bestTPAM) == 7 )) corrList.push_back( "4h" );
	}	
	for (const auto & corr : corrList){
	  Double_t segLocalAMDtanpsi = (seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) - TMath::TwoPi() * ph2TpgPhiEmuAm_dirLoc_phi->at(bestTPAM) / 360;
	  m_plots["TanPsiRes_P2_AM" + corr]           ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + whTag]   ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + whTag + chambTag]   ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + secTag]  ->Fill( segLocalAMDtanpsi );
	  m_plots["TanPsiRes_P2_AM" + corr + chambTag]->Fill( segLocalAMDtanpsi );
	  
	  // Phi
	  m_plots["PhiRes_P2_AM" + corr]           ->Fill( bestAMDPhi );
	  m_plots["PhiRes_P2_AM" + corr + whTag]   ->Fill( bestAMDPhi );
	  m_plots["PhiRes_P2_AM" + corr + whTag + chambTag]   ->Fill( bestAMDPhi );
	  m_plots["PhiRes_P2_AM" + corr + secTag]  ->Fill( bestAMDPhi );
	  m_plots["PhiRes_P2_AM" + corr + chambTag]->Fill( bestAMDPhi );
	  
	  // Time
	  if (seg_phi_t0->at(iSeg) > -500){
	    //float segLocalAMDtime = seg_phi_t0->at(iSeg)  - 25*(ph2TpgPhiEmuAm_BX->at(bestTPAM)-20);
	    float segLocalAMDtime = seg_phi_t0->at(iSeg)  - ph2TpgPhiEmuAm_t0->at(bestTPAM) + 20*25;
	    
          // Time
	    m_plots["TimeRes_P2_AM" + corr]           ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + whTag]   ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + whTag + chambTag]   ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + secTag]  ->Fill( segLocalAMDtime );
	    m_plots["TimeRes_P2_AM" + corr + chambTag]->Fill( segLocalAMDtime );
	  
	
          // BX
	    m_plots["BX_P2_AM" + corr]           ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20);
	    m_plots["BX_P2_AM" + corr + whTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20);
	    m_plots["BX_P2_AM" + corr + whTag + chambTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20);
	    m_plots["BX_P2_AM" + corr + secTag]  ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20);
	    m_plots["BX_P2_AM" + corr + chambTag]->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20);
	    //m_plots["BX_P2_AM" + corr]           ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20 );
	    //m_plots["BX_P2_AM" + corr + whTag]   ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20 );
	    //m_plots["BX_P2_AM" + corr + secTag]  ->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20 );
	    //m_plots["BX_P2_AM" + corr + chambTag]->Fill( ph2TpgPhiEmuAm_BX->at(bestTPAM) - 20 );
	  }
	  // x
	  Double_t segLocalAMDx = 0;
	  if (isCorrelated){
	    segLocalAMDx = seg_posLoc_x_midPlane->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(bestTPAM);
	  }
	  else if (ph2TpgPhiEmuAm_superLayer->at(bestTPAM) == 1) segLocalAMDx = seg_posLoc_x_SL1->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(bestTPAM);
	  else                                                   segLocalAMDx = seg_posLoc_x_SL3->at(iSeg) - ph2TpgPhiEmuAm_posLoc_x->at(bestTPAM);
	  m_plots["xRes_P2_AM"+ corr]           ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + whTag]   ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + whTag + chambTag]   ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + secTag]  ->Fill( segLocalAMDx );
	  m_plots["xRes_P2_AM"+ corr + chambTag]->Fill( segLocalAMDx );
	}
      }
    }
  }
}



TH1F* DTNtupleTPGSimAnalyzer::makeHistoPer( std::string mag, std::string suffix, vector<std::string> tags, std::string algo)
{

  std::map<std::string,float> ranges;
  ranges["TimeRes"  ] = 10;
  ranges["PhiRes"   ] = 0.0005;
  ranges["TanPsiRes"] = 0.005;
  ranges["xRes"     ] = 0.05;

  TH1F* ret = new TH1F((mag+suffix+algo).c_str(),"",
            tags.size(), -0.5,-0.5+tags.size());
  for (unsigned int i = 0; i < tags.size(); ++i){
    ret->GetXaxis()->SetBinLabel(i+1, tags[i].c_str());

    if (m_plots[mag + "_P2_" + algo + tags[i]]->Integral()){
//      m_plots[mag + "_P2_" + algo + tags[i]]->Fit("gaus","SQ","",-ranges[mag],ranges[mag]);


      RooRealVar x("x","",-ranges[mag],ranges[mag]);
      RooRealVar mean("mean",",mean of Gaussian",0.,-ranges[mag],ranges[mag]);
      RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag]/4,0,ranges[mag]/2);
      RooRealVar sigma2("sigma2",",width of wide Gaussian",ranges[mag],0,10*ranges[mag]);
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
  std::vector<std::string> whTags    = { "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"};
  std::vector<std::string> secTags   = { "Sec.1", "Sec.2", "Sec.3", "Sec.4", "Sec.5", "Sec.6", "Sec.7", "Sec.8","Sec.9","Sec.10","Sec.11","Sec.12","Sec.13","Sec.14"};
  std::vector<std::string> magnitudes = { "TimeRes", "PhiRes", "TanPsiRes", "xRes"};
  std::vector<std::string> algos      = { "AM", "HB" };
  std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated", "All"};
  
  for (const auto & mag : magnitudes){
    for (const auto & algo : algos){
      for (const auto & qual : qualTags){
	m_plots[mag + "_" + algo + qual + "_Res_perChamb"] = makeHistoPer( mag, "_Res_perChamb", chambTags, algo + qual);
	m_plots[mag + "_" + algo + qual + "_Res_perWheel"] = makeHistoPer( mag, "_Res_perWheel", whTags   , algo + qual);
	m_plots[mag + "_" + algo + qual + "_Res_perSec"]   = makeHistoPer( mag, "_Res_perSec"  , secTags  , algo + qual);
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
