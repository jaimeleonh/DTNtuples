#include "DTNtupleTPGSimAnalyzer.h"

#include"TMath.h"

struct primitive {
  short station; 
  short sector; 	
  short wheel; 
  short quality; 
  short superLayer;
  int phi; 
  int phiB; 
  float position; 
  float direction; 
  int chi2; 
  int BX; 
  int t0; 
};

typedef std::vector <primitive> primitives;


DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
						 const TString & outFileName) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  m_minMuPt = 20;

  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;
  
  m_minSegHits = 4;
  
  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;

  entryNumber = -1; 
  debug = false;   
  correctL1A = false; 
}
DTNtupleTPGSimAnalyzer::DTNtupleTPGSimAnalyzer(const TString & inFileName,
					       const TString & outFileName,
					       const bool & correct) :
  m_outFile(outFileName,"RECREATE"), DTNtupleBaseAnalyzer(inFileName)  
{ 

  m_minMuPt = 20;

  m_maxMuSegDPhi = 0.2;
  m_maxMuSegDEta = 0.3;
  
  m_minSegHits = 4;
  
  m_maxSegTrigDPhi = 0.1;
  m_maxMuTrigDPhi  = 0.2;

  entryNumber = -1; 
  debug = false;  ////// 
  correctL1A = correct; 
}

DTNtupleTPGSimAnalyzer::~DTNtupleTPGSimAnalyzer() 
{ 

}


void DTNtupleTPGSimAnalyzer::Loop()
{

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();
  totalEntries = nentries;
  book();
  Long64_t nbytes = 0, nb = 0;
 // for (Long64_t jentry=0; jentry<50000;jentry++) 
  for (Long64_t jentry=0; jentry<nentries;jentry++)
     
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;
      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleTPGSimAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

        entryNumber = jentry; 
	fill();

    }

  std::cout << std::endl; 

  endJob();

}

void DTNtupleTPGSimAnalyzer::book()
{

  m_outFile.cd();

  std::vector<std::string> chambTags = {"MB1","MB2","MB3", "MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  //std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q8","Q9"};
  //std::vector<std::string> quTags = {"Q1","Q2","Q3","Q4","Q5","Q6","Q7","Q8","Q9"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
 
  int nbinPosEmuFW = 101; double minPosEmuFW = 0.01 * (0.5 + nbinPosEmuFW / 2); 
  int nbinPsiEmuFW = 101; double minPsiEmuFW = 0.01 * (0.5 + nbinPsiEmuFW / 2); 
  int nbinTimeEmuFW = 101; double minTimeEmuFW = 1 * (0.5 + nbinTimeEmuFW / 2); 

  int nbinPhiTM = 1001; double minPhiTM = 1600;  
  int nbinPhiFW = 1001; double minPhiFW = 30000; 
  int nbinPhiBTM = 1001; double minPhiBTM = 600;  
  int nbinPhiBFW = 1001; double minPhiBFW = 2500; 
 

      m_effs["hEffCorAM"] = new TEfficiency("hEffCorAM",
					    "Primitive percentage that appear in AM",
					    10,-0.5,0.5); 
      m_effs["hEffCorHW"] = new TEfficiency("hEffCorHW",
					    "Primitive percentage that appear in HW",
					    10,-0.5,0.5); 
      m_effs["hEffCor"] = new TEfficiency("hEffCor",
					    "Common correlated primitives percentage",
					    10,-0.5,0.5); 
      m_plots["hQualityHW"] = new TH1F("hQualityHW",
					    "Distribution of HW qualities",
					    9,0.5,9.5); 
      m_plots["hQualityAM"] = new TH1F("hQualityAM",
					    "Distribution of Emul qualities",
					    9,0.5,9.5); 
      m_plots["hBXDif"] = new TH1F("hBXDif",
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
      m_plots["hBXtotal"] = new TH1F("hBXtotal",
					    "Distribution of BX; BX; Entries",
					    3564,0,3564); 

      
      for (const auto & chambTag : chambTags) {
	
        m_effs["hEffHWvsSegXGoodBX"+ chambTag] = new TEfficiency(("hEffHWvsSegXGoodBX_"+ chambTag).c_str(),
					    "HW Eff in Good BX vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffAMvsSegXGoodBX"+ chambTag] = new TEfficiency(("hEffAMvsSegXGoodBX_"+ chambTag).c_str(),
					    "AM Eff in Good BX vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffTMvsSegXGoodBX"+ chambTag] = new TEfficiency(("hEffTMvsSegXGoodBX_"+ chambTag).c_str(),
					    "TM Eff in Good BX vs Seg X",
					    50,-251.5,250.5); 
        m_effs["hEffHWvsSegX"+ chambTag] = new TEfficiency(("hEffHWvsSegX_"+ chambTag).c_str(),
					    "HW Eff vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffAMvsSegX"+ chambTag] = new TEfficiency(("hEffAMvsSegX_"+ chambTag).c_str(),
					    "AM Eff vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffTMvsSegX"+ chambTag] = new TEfficiency(("hEffTMvsSegX_"+ chambTag).c_str(),
					    "TM Eff vs Seg X",
					    50,-251.5,250.5);
        m_plots["hQualityHW"+chambTag] = new TH1F(("hQualityHW_" +chambTag).c_str(),
					    "Distribution of HW qualities; HW Qualities; Entries",
					    9,0.5,9.5); 
        m_plots["hQualityAM"+chambTag] = new TH1F(("hQualityAM_" +chambTag).c_str(),
					    "Distribution of Emul qualities; Emul Qualities; Entries",
					    9,0.5,9.5); 
        m_plots["hQualityTM"+chambTag] = new TH1F(("hQualityTM_" +chambTag).c_str(),
					    "Distribution of TM qualities; TM Qualities; Entries",
					    6,1.5,7.5); 
        m_plots2["hQualityVsBXHW"+chambTag] = new TH2F(("hQualityVsBXHW_" +chambTag).c_str(),
					    "Distribution of Quality vs BX for HW; HW Primitive Quality; HW Primitive BX - Offset",
					    9,0.5,9.5,21,-10.5,10.5); 
        m_plots2["hQualityVsBXAM"+chambTag] = new TH2F(("hQualityVsBXAM_" +chambTag).c_str(),
					    "Distribution of Quality vs BX for Emul; Emul Primitive Quality; Emul Primitive BX - Offset",
					    9,0.5,9.5,21,-10.5,10.5); 
        m_plots["hBXTM"+chambTag] = new TH1F(("hBXTM_" +chambTag).c_str(),
					    "Distribution of BX Twinmux; BX Twinmux; Entries",
					    21,-10.5,10.5); 
        
        
        m_plots2["hHits"+chambTag] = new TH2F(("hHits_" +chambTag).c_str(),
					    "Distribution of Hits; Cell Number; ",
               100,0.5,100.5,14,0,14.5); 
        std::vector<std::string> tagsHits = {"SL1 La1", "SL1 La2", "SL1 La3","SL1 La4", " ", "SL2 La1", "SL2 La2", "SL2 La3","SL2 La4", " ", "SL3 La1", "SL3 La2", "SL3 La3","SL3 La4"};

        for (unsigned int i = 0; i < tagsHits.size(); i++){
          m_plots2["hHits" + chambTag]->GetYaxis()->SetBinLabel(i+1, tagsHits[i].c_str());
        }



	/*****************************************************************************************************************************
 	*				         		DATA - EMULATOR
 	*****************************************************************************************************************************/

        m_plots2["hPrimNumber" + chambTag] = new TH2F(("hPrimNumber_" + chambTag).c_str(),
					    "Distribution of number of primitives; Number of AM_HW; Number of AM_Emul",
					    11,-0.5,10.5,11,-0.5,10.5); 
        m_plots2["hPrimTypeVsPos"+ chambTag] = new TH2F(("hPrimTypeVsPos_"+ chambTag).c_str(),
					    "Number of each type of primitive vs Position; Position (cm); Type",
					    50,-251.5,250.5, 3,-0.5,2.5); 
        m_plots2["hCorPrimNumber" + chambTag] = new TH2F(("hCorPrimNumber_" + chambTag).c_str(),
					    "Distribution of number of correlated primitives; Number of Cor AM_HW; Number of Cor AM_Emul",
					    11,-0.5,10.5,11,-0.5,10.5); 
        m_plots2["hAMQuality" + chambTag] = new TH2F(("hAMQuality_" + chambTag).c_str(),
					    "; ; EntryNumber",
					    5,-0.5,4.5,totalEntries,0,totalEntries); 
        m_plots2["hHWQuality" + chambTag] = new TH2F(("hHWQuality_" + chambTag).c_str(),
					    "; ; EntryNumber",
					    5,-0.5,4.5,totalEntries,0,totalEntries); 
        m_plots2["hAMQualityWhenHWcor" + chambTag] = new TH2F(("hAMQualityWhenHWcor_" + chambTag).c_str(),
					    "; ; EntryNumber",
					    5,-0.5,4.5,totalEntries,0,totalEntries); 
        m_plots2["hHWQualityWhenAMcor" + chambTag] = new TH2F(("hHWQualityWhenAMcor_" + chambTag).c_str(),
					    "; ; EntryNumber",
					    5,-0.5,4.5,totalEntries,0,totalEntries); 
        m_plots2["hEventQuality" + chambTag] = new TH2F(("hEventQuality_" + chambTag).c_str(),
					    "; AM_FW; AM_Emul",
					    5,-0.5,4.5,5,-0.5,4.5); 
        std::vector<std::string> tagsEvent = {"Correlated", "SL1&SL3", "Only SL1","Only SL3","None"};
        std::vector<std::string> tagsSL = {"Correlated", "SL1","SL3"};
        for (unsigned int i = 0; i < tagsSL.size(); i++){
	  m_plots2["hPrimTypeVsPos"+ chambTag]->GetYaxis()->SetBinLabel(i+1, tagsSL[i].c_str());
	}
        for (unsigned int i = 0; i < tagsEvent.size(); i++){
          m_plots2["hAMQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
          m_plots2["hHWQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
          m_plots2["hAMQualityWhenHWcor" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
          m_plots2["hHWQualityWhenAMcor" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
          m_plots2["hEventQuality" + chambTag]->GetXaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
          m_plots2["hEventQuality" + chambTag]->GetYaxis()->SetBinLabel(i+1, tagsEvent[i].c_str());
        }
     


	/****************************************************************************************************************************/


        m_plots["hSLHW" + chambTag] = new TH1F(("hSLHW_" + chambTag).c_str(),
					    "Distribution of HW SL; SL; Entries",
					    4,-0.5,3.5); 
        m_plots["ht0" + chambTag] = new TH1F(("ht0_" + chambTag).c_str(),
					    "Distribution of t0; t0 (ns); Entries",
					    9000,-90000,90000); 
        m_plots["hBXAll" + chambTag] = new TH1F(("hBXAll_" + chambTag).c_str(),
					    "Distribution of All BXs; BX; Entries",
					    3564,0,3564); 
        m_plots["hBXNextOrbit" + chambTag] = new TH1F(("hBXNextOrbit_" + chambTag).c_str(),
					    "Distribution of BX from Next Orbit Primx; BX; Entries",
					    3564,0,3564); 
        m_plots["hdift0" + chambTag] = new TH1F(("hdift0_" + chambTag).c_str(),
					    "Distribution of #Delta t0; #Delta t0 (ns); Entries",
					    90000,-90000,90000); 
        m_plots["hSLAM" + chambTag] = new TH1F(("hSLAM_" + chambTag).c_str(),
					    "Distribution of AM SL; SL; Entries",
					    4,-0.5,3.5); 
        m_plots["hPrimsSegs" + chambTag] = new TH1F(("hPrimsSegs_" + chambTag).c_str(),
					    "Number of primitives (Qu>=8) and 8-hit-Phase-1 segments; ; Entries",
					    4,-0.5,3.5); 
        std::vector<std::string> tags = {"AM_FW", "AM_Emul","Phase-1 Segments","TwinMux In"};
        for (unsigned int i = 0; i < tags.size(); i++){
          m_plots["hPrimsSegs" + chambTag]->GetXaxis()->SetBinLabel(i+1, tags[i].c_str());
        }
        m_plots2["h2DHwQualSegNHits"+chambTag]=new TH2F(("h2DHwQualSegNHits_" + chambTag).c_str(),"NHitsSeg vs HWQual; Hits seg; HWQual",7,2.5,9.5,9,0.5,9.5);
        m_plots2["h2DEmuQualSegNHits"+chambTag]=new TH2F(("h2DEmuQualSegNHits_" + chambTag).c_str(),"NHitsSeg vs EmuQual; Hits seg; Emulator Quality",7,2.5,9.5,9,0.5,9.5);
        m_plots2["h2DTMQualSegNHits"+chambTag]=new TH2F(("h2DTMQualSegNHits_" + chambTag).c_str(),"NHitsSeg vs TMQual; Hits seg; TwinMux Quality",7,2.5,9.5,9,0.5,9.5);

        for (const auto & labelTag : labelTags) {
          m_plots["hBX" + chambTag + labelTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
					    //201,3199.5,3400.5); 
          m_plots["hBXDif" + chambTag + labelTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7128,-3564,3564); 
          m_plots["hBXEmul" + chambTag + labelTag] = new TH1F(("hBXEmul_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
          m_plots["hBXDifEmul" + chambTag + labelTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + labelTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
          m_plots["hBXfromT0" + chambTag + labelTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    7128,-3564,3564); 
					    //201,3199.5,3400.5); 
          m_plots["hChi2Emul" +  chambTag + labelTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
          m_plots["hChi2FW" +  chambTag + labelTag] = new TH1F(("hChi2FW_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
          m_plots["hPsiHW" + chambTag + labelTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware #Psi Distribution; Firmware #Psi ( #circ); Entries",
					    160,-80,80); 
          m_plots["hPsiEmul" + chambTag + labelTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + labelTag).c_str(),
					    "Emulator #Psi Distribution; Emulator #Psi ( #circ); Entries",
					    160,-80,80); 
          m_plots2["hPsi2D" + chambTag + labelTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hTime2D"+ chambTag + labelTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    400,-200,200,400,-200,200); 
          m_plots2["hPos2D"+ chambTag + labelTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots["hPsi"+ chambTag + labelTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
          m_plots["hTime"+ chambTag + labelTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
          m_plots["hPos"+ chambTag + labelTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    nbinPosEmuFW,-minTimeEmuFW,minTimeEmuFW); 
          m_plots2["hPsi2DSeg" + chambTag + labelTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Segment #Psi (#circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hTime2DSeg"+ chambTag + labelTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
					    //200,-100,100,200,-100,100); 
          m_plots2["hPos2DSeg"+ chambTag + labelTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots2["hPos2DTM"+ chambTag + labelTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Twinmux position; Firmware position (cm); Twinmux position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots2["hPsi2DTM" + chambTag + labelTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Twinmux #Psi; Firmware #Psi (#circ); Twinmux #Psi (#circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hPhi2DTM"+ chambTag + labelTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Twinmux phi; Firmware phi; Twinmux phi",
					    nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
          m_plots2["hPhiB2DTM"+ chambTag + labelTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Twinmux phiB; Firmware phiB; Twinmux phiB",
					    nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
          m_plots["hPsiSeg"+ chambTag + labelTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment #Psi; Firmware - Segment #Psi (#circ); Entries",
					    40,-2,2); 
          m_plots["hTimeSeg"+ chambTag + labelTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment time; Firmware - Segment time (ns); Entries",
					    200,-100,100); 
          m_plots["hPosSeg"+ chambTag + labelTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment position; Firmware - Segment position (cm); Entries",
					    100,-10,10); 
          m_plots2["hTimeSegvsPos"+ chambTag + labelTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment time vs Segment position; Segment position (cm); Firmware - Segment time (ns)",
					    600,-300,300,200,-100,100); 
          m_plots2["hTimeSegvsPsi"+ chambTag + labelTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    160,-80,80,200,-100,100); 
       	  if (labelTag == "All" || labelTag == "Correlated") continue;
	        for (const auto & slTag : slTags) {
            m_plots2["hPhi2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Twinmux phi; Firmware phi; Twinmux phi",
					    nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
            m_plots2["hPhiB2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
			        "Firmware vs Twinmux phiB; Firmware phiB; Twinmux phiB",
					    nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
            m_plots2["hPos2DTM"+ chambTag + labelTag + slTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + labelTag + "_" + slTag ).c_str(),
					    "Firmware vs Twinmux position; Firmware position (cm); Twinmux position (cm)",
					    600,-300,300,600,-300,300); 
            m_plots2["hPsi2DTM" + chambTag + labelTag + slTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Twinmux #Psi; Firmware #Psi (#circ); Twinmux #Psi (#circ)",
					    160,-80,80,160,-80,80); 
            m_plots["hBX" + chambTag + labelTag + slTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
	   				    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
            m_plots["hBXDif" + chambTag + labelTag + slTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7128,-3564,3564); 
            m_plots["hBXEmul" + chambTag + labelTag + slTag] = new TH1F(("hBXEmul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
            m_plots["hBXDifEmul" + chambTag + labelTag + slTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
            m_plots["hBXfromT0" + chambTag + labelTag + slTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    201,3199.5,3400.5); 
            m_plots["hChi2Emul" +  chambTag + labelTag + slTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
            m_plots["hChi2FW" +  chambTag + labelTag + slTag] = new TH1F(("hChi2FW_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
            m_plots["hPsiHW" + chambTag + labelTag + slTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + labelTag+ "_" + slTag).c_str(),
					    "Firmware #Psi Distribution; Firmware #Psi ( #circ); Entries",
					    160,-80,80); 
            m_plots["hPsiEmul" + chambTag + labelTag + slTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + labelTag+ "_" + slTag).c_str(),
					    "Emulator #Psi Distribution; Emulator #Psi ( #circ); Entries",
					    160,-80,80); 
            m_plots2["hPsi2D" + chambTag + labelTag + slTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    160,-80,80,160,-80,80); 
            m_plots2["hTime2D"+ chambTag + labelTag + slTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    200,-100,100,200,-100,100); 
            m_plots2["hPos2D"+ chambTag + labelTag + slTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    300,0,300,300,0,300); 
            m_plots["hPsi"+ chambTag + labelTag + slTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
            m_plots["hTime"+ chambTag + labelTag + slTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
            m_plots["hPos"+ chambTag + labelTag + slTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    nbinPosEmuFW,-minTimeEmuFW,minTimeEmuFW); 
            m_plots2["hPsi2DSeg" + chambTag + labelTag + slTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Segment #Psi (#circ)",
					    160,-80,80,160,-80,80); 
            m_plots2["hTime2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
            m_plots2["hPos2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    600,-300,300,600,-300,300); 
            m_plots["hPsiSeg"+ chambTag + labelTag + slTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment #Psi; Firmware - Segment #Psi (#circ); Entries",
					    40,-2,2); 
            m_plots["hTimeSeg"+ chambTag + labelTag + slTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment time; Firmware - Segment time (ns); Entries",
					    200,-100,100); 
            m_plots["hPosSeg"+ chambTag + labelTag + slTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment position; Firmware - Segment position (cm); Entries",
					    100,-10,10); 
            m_plots2["hTimeSegvsPos"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment time vs Segment position; Segment position (cm); Firmware - Segment time (ns)",
					    600,-300,300,200,-100,100); 
            m_plots2["hTimeSegvsPsi"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    120,-60,60,200,-100,100); 
          }
        }
	for (const auto & quTag : quTags) {
          m_plots["hBX" + chambTag + quTag] = new TH1F(("hBX_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
          m_plots["hBXDif" + chambTag + quTag] = new TH1F(("hBXDif_" + chambTag + "_" + quTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
          m_plots["hBXEmul" + chambTag + quTag] = new TH1F(("hBXEmul_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    21,-10.5,10.5); 
          m_plots["hBXDifEmul" + chambTag + quTag] = new TH1F(("hBXDifEmul_" + chambTag + "_" + quTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
          m_plots["hBXfromT0" + chambTag + quTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    201,3199.5,3400.5); 
          m_plots["hChi2Emul" +  chambTag + quTag] = new TH1F(("hChi2Emul_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
          m_plots["hChi2FW" +  chambTag + quTag] = new TH1F(("hChi2FW_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    10000,-0.5,99999.5); 
          m_plots["hPsiHW" + chambTag + quTag] = new TH1F(("hPsiHW_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware #Psi Distribution; Firmware #Psi ( #circ); Entries",
					    160,-80,80); 
          m_plots["hPsiEmul" + chambTag + quTag] = new TH1F(("hPsiEmul_"+ chambTag + "_" + quTag).c_str(),
					    "Emulator #Psi Distribution; Emulator #Psi ( #circ); Entries",
					    160,-80,80); 
          m_plots2["hPsi2D" + chambTag + quTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hTime2D"+ chambTag + quTag] = new TH2F(("hTime2D_" + chambTag + "_"+ quTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    400,-200,200,400,-200,200); 
          m_plots2["hPos2D"+ chambTag + quTag] = new TH2F(("hPos2D_" + chambTag + "_" + quTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots["hPsi"+ chambTag + quTag] = new TH1F(("hPsi_"+ chambTag + "_" +quTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    nbinPsiEmuFW,-minPsiEmuFW,minPsiEmuFW); 
          m_plots["hTime"+ chambTag + quTag] = new TH1F(("hTime_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    nbinTimeEmuFW,-minTimeEmuFW,minTimeEmuFW); 
          m_plots["hPos"+ chambTag + quTag] = new TH1F(("hPos_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    nbinPosEmuFW,-minPosEmuFW,minPosEmuFW); 
          m_plots2["hPsi2DSeg" + chambTag + quTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Segment #Psi (#circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hTime2DSeg"+ chambTag + quTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
          m_plots2["hPos2DSeg"+ chambTag + quTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots2["hPos2DTM"+ chambTag + quTag] = new TH2F(("hPos2DTM_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Twinmux position; Firmware position (cm); Twinmux position (cm)",
					    600,-300,300,600,-300,300); 
          m_plots2["hPsi2DTM" + chambTag + quTag] = new TH2F(("hPsi2DTM_" + chambTag + "_" + quTag).c_str(),
					    "Firmware vs Twinmux #Psi; Firmware #Psi (#circ); Twinmux #Psi (#circ)",
					    160,-80,80,160,-80,80); 
          m_plots2["hPhi2DTM"+ chambTag + quTag] = new TH2F(("hPhi2DTM_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Twinmux phi; Firmware phi; Twinmux phi",
					    nbinPhiFW,-minPhiFW,minPhiFW,nbinPhiTM,-minPhiTM,minPhiTM); 
          m_plots2["hPhiB2DTM"+ chambTag + quTag] = new TH2F(("hPhiB2DTM_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Twinmux phiB; Firmware phiB; Twinmux phiB",
					    nbinPhiBFW,-minPhiBFW,minPhiBFW,nbinPhiBTM,-minPhiBTM,minPhiBTM); 
          m_plots["hPsiSeg"+ chambTag + quTag] = new TH1F(("hPsiSeg_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment #Psi; Firmware - Segment #Psi (#circ); Entries",
					    40,-2,2); 
          m_plots["hTimeSeg"+ chambTag + quTag] = new TH1F(("hTimeSeg_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment time; Firmware - Segment time (ns); Entries",
					    200,-100,100); 
          m_plots["hPosSeg"+ chambTag + quTag] = new TH1F(("hPosSeg_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment position; Firmware - Segment position (cm); Entries",
					    100,-10,10); 
          m_plots2["hTimeSegvsPos"+ chambTag + quTag] = new TH2F(("hTimeSegvsPos_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment time vs Segment position; Segment position (cm); Firmware - Segment time (ns)",
					    600,-300,300,200,-100,100); 
          m_plots2["hTimeSegvsPsi"+ chambTag + quTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    160,-80,80,200,-100,100); 

        }
      }

  
}

void DTNtupleTPGSimAnalyzer::fill()
{
     std::vector<std::string> chambTags = {"MB1", "MB2", "MB3","MB4"};
     std::vector<std::string> slTags = { "SL1", "SL3"};
     //std::vector<std::string> quTags  = {"Q1","Q2","Q3","Q4","Q5","Q6","Q7","Q8","Q9"};
     std::vector<std::string> quTags = {"3h","4h","Q6","Q8","Q9"};
     std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
     //int eventoBX; // = ph2TpgPhiHw_bx; //848 
     int eventoBX = event_bunchCrossing; //848 
     int offset[4];

     primitives primitivesHW [4];
     primitives primitivesAM [4];
     for (int i = 0; i < 4; i++) {
	primitivesHW[i].clear();
	primitivesAM[i].clear();
     }



     offset[0] = -1; //FIXME
     offset[1] = -195; //FIXME
     offset[2] = -195; //FIXME
     offset[3] = -195; //FIXME


     /* HIT PLOTS */

     for (unsigned int iHit = 0; iHit < ph2Digi_nDigis; iHit++) {
       m_plots2["hHits" + chambTags.at(ph2Digi_station -> at(iHit) - 1)]  -> Fill(ph2Digi_wire->at(iHit) , ph2Digi_layer->at(iHit) + ( ph2Digi_superLayer->at(iHit) - 1 ) * 5 );      
     } 



















     bool titPrint = false; 

     //int offset = -270; //FIXME
     m_plots["hBXtotal"]->Fill(eventoBX);
     //int eventoBX = 3365; //848

     int bestI[6]; short bestQu[6];
     for (int i = 0; i<6; i++) {
       bestI[i] = -1; 
       bestQu[i] = -1; 
     }   
 
     //bool debug = false; 
     if (debug && ph2TpgPhiHw_nTrigs!=0 && !titPrint) { 
       cout << "====================Entry " << entryNumber << " =================="<< endl; 
       cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
       titPrint = true; 
     }
     if (debug && ph2TpgPhiHw_nTrigs!=0) cout << "####################### HARDWARE PRIMITIVES ############################" << endl;  
      int bestTrigHW[chambTags.size()][quTags.size()];  
      int bestTimeHW[chambTags.size()][quTags.size()];

      for (unsigned int i = 0; i<chambTags.size(); i++){
        for (unsigned int j = 0; j<quTags.size(); j++){
          bestTrigHW[i][j] = -1; 
	  bestTimeHW[i][j] = 999999;
        }
      }
      int bestQualTrigHW[4];int IbestQualTrigHW[4] ; // 4 stations MB1 to MB4 
      int bestQualTrigBXHW[4];int IbestQualTrigBXHW[4] ; // 4 stations MB1 to MB4 
      for(unsigned indstat=0;indstat<4; indstat++){
	bestQualTrigHW[indstat]=-1;IbestQualTrigHW[indstat]=-1;
	bestQualTrigBXHW[indstat]=-1;IbestQualTrigBXHW[indstat]=-1;
      }
     
      for (std::size_t iTrigHW = 0; iTrigHW < ph2TpgPhiHw_nTrigs; ++iTrigHW)
        {
		
        short myStationHW = ph2TpgPhiHw_station->at(iTrigHW);
        short mySectorHW = ph2TpgPhiHw_sector->at(iTrigHW);
        short myWheelHW = ph2TpgPhiHw_wheel->at(iTrigHW);
        short myQualityHW = ph2TpgPhiHw_quality->at(iTrigHW);
        short mySLHW = ph2TpgPhiHw_superLayer->at(iTrigHW);
        int myChiHW =  ph2TpgPhiHw_chi2->at(iTrigHW);
        int myPhiHW = ph2TpgPhiHw_phi->at(iTrigHW);
        int myPhiBHW =   ph2TpgPhiHw_phiB->at(iTrigHW);
        float myPosHW =  ph2TpgPhiHw_posLoc_x->at(iTrigHW);
        float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(iTrigHW);
        int myChi2HW = ph2TpgPhiHw_chi2->at(iTrigHW); 
        int myBXHW = ph2TpgPhiHw_BX->at(iTrigHW); //eventoBX = myBXHW; 
        int myt0HW = ph2TpgPhiHw_t0->at(iTrigHW);
        //m_plots["ht0" + chambTags.at(myStationHW/2-1)]->Fill(myt0HW);
        m_plots["ht0" + chambTags.at(myStationHW-1)]->Fill(myt0HW - eventoBX*25);

 	int indstat = myStationHW - 1; 	
	if(myQualityHW>bestQualTrigHW[indstat]){
	  bestQualTrigHW[indstat]=myQualityHW;
	  IbestQualTrigHW[indstat]=iTrigHW ;
	}

	if (myQualityHW >=6 && myQualityHW != 7) {
	//  if (!printeado && debug) {cout << "----------------------------------------" << endl; printeado = true;}
	  if (debug) cout << "Correlated HW con quality " << myQualityHW << endl;
	}

        //myt0HW = myt0HW - eventoBX*25 + 3564*25;	
	if (correctL1A) {
	  myt0HW = myt0HW - eventoBX*25;
	  myBXHW = myBXHW - eventoBX;
	}
        m_plots["hBXAll" + chambTags.at(myStationHW-1)]->Fill(eventoBX);
	if (myt0HW > 0) { 
          m_plots["hBXNextOrbit" + chambTags.at(myStationHW-1)]->Fill(eventoBX);
	}
	
	while (myt0HW > 0) { 
	  myt0HW = myt0HW - 3564*25; 
	  myBXHW = myBXHW - 3564   ; 
	}




	if(myQualityHW>bestQualTrigBXHW[indstat] && (myBXHW - offset[myStationHW - 1] == 0)){
	  bestQualTrigBXHW[indstat]=myQualityHW;
	  IbestQualTrigBXHW[indstat]=iTrigHW ;
	}
	//if (myt0HW < 0) myt0HW += 3564*25;	

	if (myQualityHW <= 4){
		if (iTrigHW + 1 <  ph2TpgPhiHw_nTrigs) {
			if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+1)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+1)!=7 ))) continue;
		}
		if (iTrigHW + 2 <  ph2TpgPhiHw_nTrigs) {
			if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+2)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+2)!=7 ))) continue;
		}
	}	
        primitivesHW[myStationHW-1].push_back(primitive({myStationHW, mySectorHW, myWheelHW, myQualityHW, mySLHW, myPhiHW, myPhiBHW, myPosHW, myDirHW, myChi2HW, myBXHW, myt0HW}));

//	if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+1)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+1)!=7 ) || (ph2TpgPhiHw_quality->at(iTrigHW+2)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+2)!=7 ) )) continue;

	m_plots["hBXDif"]->Fill(myBXHW);
//        m_plots["ht0" + chambTags.at(myStationHW/2-1)]->Fill(myt0HW - eventoBX*25);
        if (myQualityHW >= 9){ m_plots["hPrimsSegs" + chambTags.at(myStationHW-1)] -> Fill(0); } // cout << "Habemus primitiva" << endl;  }
	
        /*if (myt0HW - eventoBX*25 < bestTimeHW[myStationHW/2-1][myQualityHW-1]){
	  bestTrigHW[myStationHW/2-1][myQualityHW-1] = iTrigHW; 
	  bestTimeHW[myStationHW/2-1][myQualityHW-1] = myt0HW - eventoBX*25;
	} */
        m_plots["hdift0" + chambTags.at(myStationHW-1)]->Fill(myt0HW - offset[myStationHW -1]*25);
        if (abs(myt0HW - offset[myStationHW-1]*25) < bestTimeHW[myStationHW-1][qualityGroup(myQualityHW)]){
	  bestTrigHW[myStationHW-1][qualityGroup(myQualityHW)] = iTrigHW; 
	  bestTimeHW[myStationHW-1][qualityGroup(myQualityHW)] = abs(myt0HW - offset[myStationHW -1]*25);
	}

	
	int index = mySLHW; 
	if (mySLHW==3) index = 2; 
	//if (debug) cout << "Index HW " << index <<  endl; 
	if (bestQu[index] < myQualityHW) { bestQu[index] = myQualityHW; bestI[index] = iTrigHW;}

	//if (debug) cout << "bestQu[index] " << bestQu[index] <<  endl; 


        if (debug) {
          cout << "Wh:" << myWheelHW << " Se:" <<  mySectorHW << " St:" << myStationHW << endl;	
          cout << "Quality " << myQualityHW << endl;	
          cout << "SL " << mySLHW << endl;	
          cout << "Chi2 " << myChiHW << endl;	
          cout << "Phi " << myPhiHW << endl;	
          cout << "PhiB " << myPhiBHW << endl;	
          cout << "Position " << myPosHW << endl;	
          cout << "Direction " << myDirHW << endl;	
          cout << "BX " << myBXHW << endl;	
          cout << "t0 " << myt0HW << endl; //myt0HW = myt0HW - eventoBX + 3564*25;	
          cout << "-------------------------------------------------------------------------" << endl;
        }
	
        m_plots["hSLHW" + chambTags.at(myStationHW-1)]->Fill(mySLHW);
  	m_plots["hPsiHW"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myDirHW);
	m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myBXHW - offset[myStationHW-1]);
	m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(round(myt0HW/25));
	m_plots["hChi2FW"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(1.E6*myChi2HW / (1024. * 100) );

	if (myQualityHW == 6 || myQualityHW == 8 || myQualityHW == 9){	
  	  m_plots["hPsiHW"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myDirHW);
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myBXHW - offset[myStationHW-1]);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(round(myt0HW/25));
	  m_plots["hChi2FW"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(1.E6*myChi2HW / (1024. * 100) );
	} else {
  	  m_plots["hPsiHW"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myDirHW);
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myBXHW - offset[myStationHW-1]);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(round(myt0HW/25));
	  m_plots["hChi2FW"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(1.E6*myChi2HW/ (1024. * 100) );
	  
  	  m_plots["hPsiHW"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myDirHW);
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myBXHW - offset[myStationHW-1]);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(round(myt0HW/25));
	  m_plots["hChi2FW"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(1.E6*myChi2HW/ (1024. * 100) );
	}

/*	m_plots["hBX"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(round(myt0HW/25));
	m_plots["hBXDif"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(round(myt0HW/25) - 32*myBXHW/25);
_plots["hQualityHW"]->Fill(myQualityHW);
	m_plots["hChi2"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(myChi2HW);
 */
  	m_plots["hPsiHW"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myDirHW);
	m_plots["hBX"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myBXHW - offset[myStationHW-1]);
	m_plots["hBXDif"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(round(myt0HW/25));
//	m_plots["hBXDif"+chambTags.at(myStationHW/2-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(round(myt0HW/25) - 32*myBXHW/25);
	m_plots["hQualityHW"]->Fill(myQualityHW);
	m_plots["hQualityHW"+ chambTags.at(myStationHW-1)]->Fill(myQualityHW);
	m_plots2["hQualityVsBXHW"+ chambTags.at(myStationHW-1)]->Fill(myQualityHW, myBXHW-offset[myStationHW-1]);
	m_plots["hChi2FW"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(1.E6*myChi2HW/ (1024. * 100) );
	

     } // end HW
   
     if (debug && ph2TpgPhiEmuAm_nTrigs!=0 && !titPrint) { 
      std::cout << "====================Entry " << entryNumber << " =================="<< std::endl; 
       cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
       titPrint = true; 
     }
     
     if (debug && ph2TpgPhiEmuAm_nTrigs!=0) cout << "####################### EMULATOR PRIMITIVES ############################" << endl;  
      int bestTrigAM[chambTags.size()][quTags.size()];  
      int bestTimeAM[chambTags.size()][quTags.size()];  

      for (unsigned int i = 0; i<chambTags.size(); i++){
        for (unsigned int j = 0; j<quTags.size(); j++){
          bestTrigAM[i][j] = -1; 
 	  bestTimeAM[i][j] = 999999;
	}
      }

      int bestQualTrigAM[4];int IbestQualTrigAM[4] ; // 4 stations MB1 to MB4 
      int bestQualTrigBXAM[4];int IbestQualTrigBXAM[4] ; // 4 stations MB1 to MB4 
      for(unsigned indstat=0;indstat<4; indstat++){
	bestQualTrigAM[indstat]=-1;IbestQualTrigAM[indstat]=-1;
	bestQualTrigBXAM[indstat]=-1;IbestQualTrigBXAM[indstat]=-1;
      }

      for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM) {
		
        short myStationAM = ph2TpgPhiEmuAm_station->at(iTrigAM);
        short mySectorAM = ph2TpgPhiEmuAm_sector->at(iTrigAM);
        short myWheelAM = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
        short myQualityAM = ph2TpgPhiEmuAm_quality->at(iTrigAM);
        short mySLAM = ph2TpgPhiEmuAm_superLayer->at(iTrigAM);
        int myPhiAM = ph2TpgPhiEmuAm_phi->at(iTrigAM);
        int myPhiBAM =   ph2TpgPhiEmuAm_phiB->at(iTrigAM);
        int myChi2AM =  ph2TpgPhiEmuAm_chi2->at(iTrigAM);
        float myPosAM =  ph2TpgPhiEmuAm_posLoc_x->at(iTrigAM);
        float myDirAM =  ph2TpgPhiEmuAm_dirLoc_phi->at(iTrigAM);
        int myBXAM = ph2TpgPhiEmuAm_BX->at(iTrigAM);
        int myt0AM = ph2TpgPhiEmuAm_t0->at(iTrigAM);//  myt0AM = myt0AM - eventoBX*25 + 3564*25;	
	
        int indstat;
	indstat=myStationAM-1;
	if(myQualityAM>bestQualTrigAM[indstat]){
	  bestQualTrigAM[indstat]=myQualityAM;
	  IbestQualTrigAM[indstat]=iTrigAM ;
	}

	if (correctL1A) {
	  myt0AM = myt0AM - eventoBX*25;
	  myBXAM = myBXAM - eventoBX   ;
	}
	while (myt0AM > 0) 
	{
	  myt0AM = myt0AM - 3564*25; 
	  myBXAM = myBXAM - 3564   ; 
	}

        primitivesAM[myStationAM-1].push_back(primitive({myStationAM, mySectorAM, myWheelAM, myQualityAM, mySLAM, myPhiAM, myPhiBAM, myPosAM, myDirAM, myChi2AM, myBXAM, myt0AM}));




	if(myQualityAM>bestQualTrigBXAM[indstat] && (myBXAM - offset[myStationAM -1] == 0)){
	  bestQualTrigBXAM[indstat]=myQualityAM;
	  IbestQualTrigBXAM[indstat]=iTrigAM ;
	}
	 //     if (myt0AM < 0) myt0AM += 3564*25;	

	if (myQualityAM >=6 && myQualityAM != 7) {
//	  if (!printeado && debug) {cout << "----------------------------------------" << endl; printeado = true;}
	  if (debug) cout << "Correlated AM con quality " << myQualityAM << endl;
	}

        if (myQualityAM >= 8){ m_plots["hPrimsSegs" + chambTags.at(myStationAM-1)] -> Fill(1); } // cout << "Habemus primitiva" << endl;  }
	
	/*if (myt0AM - eventoBX*25 < bestTimeAM[myStationAM/2-1][myQualityAM-1]){
	  bestTrigAM[myStationAM/2-1][myQualityAM-1] = iTrigAM; 
	  bestTimeAM[myStationAM/2-1][myQualityAM-1] = myt0AM - eventoBX*25;
	}*/
	if (abs(myt0AM - offset[myStationAM - 1]*25) < bestTimeAM[myStationAM-1][qualityGroup(myQualityAM)]){
	  bestTrigAM[myStationAM-1][qualityGroup(myQualityAM)] = iTrigAM; 
	  bestTimeAM[myStationAM-1][qualityGroup(myQualityAM)] = abs(myt0AM - offset[myStationAM - 1]*25);
	}
	

        int index = mySLAM+3; 
	if (mySLAM==3) index = 5; 
	//if (debug) cout << "Index AM " << index <<  endl; 
	if (bestQu[index] < myQualityAM) { bestQu[index] = myQualityAM; bestI[index] = iTrigAM;}
	//if (debug) cout << "bestQu[index] " << bestQu[index] <<  endl; 
        
        if (debug) {
          cout << "Wh:" << myWheelAM << " Se:" <<  mySectorAM << " St:" << myStationAM << endl;	
          cout << "Quality " << myQualityAM << endl;	
          cout << "SL " << mySLAM << endl;	
          cout << "Chi2 " << myChi2AM << endl;	
          cout << "Phi " << myPhiAM << endl;	
          cout << "PhiB " << myPhiBAM << endl;	
          cout << "Position " << myPosAM << endl;	
          cout << "Direction " << myDirAM << endl;	
          cout << "BX " << myBXAM << endl;	
          cout << "t0 " << myt0AM << endl;	
          cout << "-------------------------------------------------------------------------" << endl;
        }	
  	m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+labelTags.at(0)]->Fill(myDirAM);
	m_plots["hBXEmul"+chambTags.at(myStationAM-1)+labelTags.at(0)]->Fill(myBXAM - offset[myStationAM-1]);
	m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+labelTags.at(0)]->Fill(myBXAM);
	m_plots["hQualityAM"]->Fill(myQualityAM);
	m_plots["hQualityAM"+ chambTags.at(myStationAM-1)]->Fill(myQualityAM);
	m_plots2["hQualityVsBXAM"+ chambTags.at(myStationAM-1)]->Fill(myQualityAM, myBXAM-offset[myStationAM-1]);
        m_plots["hSLAM" + chambTags.at(myStationAM-1)]->Fill(mySLAM);
	m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+labelTags.at(0)]->Fill(myChi2AM);
	if (myQualityAM == 6 || myQualityAM == 8 || myQualityAM == 9){	
  	  m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+labelTags.at(1)]->Fill(myDirAM);
          m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+labelTags.at(1)]->Fill(myChi2AM);  
	  m_plots["hBXEmul"+chambTags.at(myStationAM-1)+labelTags.at(1)]->Fill(myBXAM - offset[myStationAM-1]);
	  m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+labelTags.at(1)]->Fill(myBXAM);
        } else {
  	  m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)]->Fill(myDirAM);
  	  m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)+slTags.at(mySLAM/2)]->Fill(myDirAM);
          m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+labelTags.at(2)]->Fill(myChi2AM);  
	  m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+labelTags.at(2)+slTags.at(mySLAM/2)]->Fill(myChi2AM);  
	  m_plots["hBXEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)]->Fill(myBXAM - offset[myStationAM-1]);
	  m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)]->Fill(myBXAM);
	  m_plots["hBXEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)+slTags.at(mySLAM/2)]->Fill(myBXAM - offset[myStationAM-1]);
	  m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+labelTags.at(2)+slTags.at(mySLAM/2)]->Fill(myBXAM);
	}
	m_plots["hChi2Emul"+chambTags.at(myStationAM-1)+quTags.at(qualityGroup(myQualityAM))]->Fill(myChi2AM);
  	m_plots["hPsiEmul"+chambTags.at(myStationAM-1)+quTags.at(qualityGroup(myQualityAM))]->Fill(myDirAM);
	m_plots["hBXEmul"+chambTags.at(myStationAM-1)+quTags.at(qualityGroup(myQualityAM))]->Fill(myBXAM - offset[myStationAM-1]);
	m_plots["hBXDifEmul"+chambTags.at(myStationAM-1)+quTags.at(qualityGroup(myQualityAM))]->Fill(myBXAM);
      } // end AM

      for (unsigned int i = 0; i<chambTags.size(); i++){
        for (unsigned int j = 0; j<quTags.size(); j++){
//	    cout << "i " << i << " j " << j << " " <<  bestTrigHW[i][j] << " " << bestTrigAM[i][j] << endl; 
          if (bestTrigHW[i][j] != -1 && bestTrigAM[i][j] != -1){

	    //eventoBX = ph2TpgPhiHw_BX ->at(bestTrigHW[i][j]);
            int myt0HW = ph2TpgPhiHw_t0->at(bestTrigHW[i][j]); //myt0HW = myt0HW - eventoBX*25 + 3564*25;	
	      if (correctL1A) myt0HW = myt0HW - eventoBX*25;
	      while (myt0HW < 0) myt0HW += 3564*25;	
            int myt0AM = ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]); // myt0AM = myt0AM - eventoBX*25 + 3564*25;	
	      if (correctL1A) myt0AM = myt0AM - eventoBX*25;
	      while (myt0AM < 0) myt0AM += 3564*25;	
	    	 

  	    m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
	    //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	    m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	    m_plots["hPsi"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots["hTime"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset[i]*25 - myt0AM + offset[i]*25);
	    //m_plots["hTime"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	    m_plots["hPos"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   

	    if (j == 2 || j == 3 || j == 4) {
	    //if (j+1 == 6 || j+1 == 8 || j+1 == 9) {
  	      m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
	      //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	      m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	      m_plots["hPsi"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots["hTime"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset[i]*25 - (myt0AM - offset[i]*25));
	      //m_plots["hTime"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	      m_plots["hPos"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   
	    } else {	   

              short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	
              short myAMSL = ph2TpgPhiEmuAm_superLayer->at(bestTrigAM[i][j]);	
	      if ( myHWSL != myAMSL ) continue; 

  	      m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
	      //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	      m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	      m_plots["hPsi"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots["hTime"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset[i]*25 - (myt0AM - offset[i]*25));
	      //m_plots["hTime"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	      m_plots["hPos"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));  


  	      m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
	      m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	      m_plots["hPsi"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots["hTime"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset[i]*25 - (myt0AM - offset[i]*25));
   	      m_plots["hPos"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));  

 
	    }	   
 

  	    m_plots2["hPsi2D"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots2["hTime2D"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset[i]*25, myt0AM - offset[i]*25);
	    //m_plots2["hTime2D"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	    m_plots2["hPos2D"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	    m_plots["hPsi"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots["hTime"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset[i]*25 - (myt0AM - offset[i]*25));
	    //m_plots["hTime"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	    m_plots["hPos"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   
          }
        }
      }
      bool printTwin = false; 

      int bestQualTrigTM[4];int IbestQualTrigTM[4] ; // 4 stations MB1 to MB4 
      int bestQualTrigBXTM[4];int IbestQualTrigBXTM[4] ; // 4 stations MB1 to MB4 
      for(unsigned indstat=0;indstat<4; indstat++){
        bestQualTrigTM[indstat]=-1;IbestQualTrigTM[indstat]=-1;
        bestQualTrigBXTM[indstat]=-1;IbestQualTrigBXTM[indstat]=-1;
      }
      for (std::size_t iTwin = 0; iTwin <  ltTwinMuxIn_nTrigs; ++iTwin) {
        //break; 
        short myStationTwin = ltTwinMuxIn_station->at(iTwin);
        short mySectorTwin = ltTwinMuxIn_sector->at(iTwin);
        short myWheelTwin = ltTwinMuxIn_wheel->at(iTwin);
        short myQualityTwin = ltTwinMuxIn_quality->at(iTwin);
        int myPhiTwin = ltTwinMuxIn_phi->at(iTwin);
        int myPhiBTwin =   ltTwinMuxIn_phiB->at(iTwin);
        float myPosTwin =  ltTwinMuxIn_posLoc_x->at(iTwin);
        float myDirTwin =  ltTwinMuxIn_dirLoc_phi->at(iTwin);
        int myBXTwin = ltTwinMuxIn_BX->at(iTwin);

        if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 2){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl; 
        if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 4){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl;  
        if (myWheelTwin == 2 && mySectorTwin == 12) {
          if (debug && !titPrint) { 
            std::cout << "====================Entry " << entryNumber << " =================="<< std::endl; 
            cout << "####################### L1A BX = " << eventoBX << " ############################" << endl;  
            titPrint = true; 
          }
          if (debug && !printTwin) { 
            cout << "####################### TwimMux PRIMITIVES ############################" << endl;
            printTwin = true;
          }
           if (debug) {
            cout << "Wh:" << myWheelTwin << " Se:" <<  mySectorTwin << " St:" << myStationTwin << endl;	
            cout << "Quality " << myQualityTwin << endl;	
            cout << "Phi " << myPhiTwin << endl;	
            cout << "PhiB " << myPhiBTwin << endl;	
            cout << "Position " << myPosTwin << endl;	
            cout << "Direction " << myDirTwin << endl;	
            cout << "BX " << myBXTwin << endl;	
            cout << "-------------------------------------------------------------------------" << endl;
          }	
	      }
      	int indstat = myStationTwin - 1;  
      	if(myQualityTwin>bestQualTrigTM[indstat]){
	        bestQualTrigTM[indstat]=myQualityTwin;
	        IbestQualTrigTM[indstat]=iTwin;
      	}
	      if(myQualityTwin>bestQualTrigBXTM[indstat] && myBXTwin == 1){
	        bestQualTrigBXTM[indstat]=myQualityTwin;
	        IbestQualTrigBXTM[indstat]=iTwin;
      	}

        m_plots["hBXTM"+ chambTags.at(indstat)] -> Fill(myBXTwin);
        m_plots["hQualityTM"+ chambTags.at(indstat)] -> Fill(myQualityTwin);
        // FILL TM PLOTS

	if (myWheelTwin != 2 || mySectorTwin != 12) continue; 
        //cout << "before if " << myStationTwin << " " << myQualityTwin-2 << endl; 
        if (bestTrigHW[myStationTwin-1][myQualityTwin-2] != -1){
          //cout << "after if " << myStationTwin << " " << myQualityTwin-2 << endl; 

            int i = myStationTwin-1; 
            int j = myQualityTwin-2; 


	    short myHwWheel = ph2TpgPhiHw_wheel->at(bestTrigHW[i][j]);
	    short myHwSector = ph2TpgPhiHw_sector->at(bestTrigHW[i][j]);
	    short myHwStation = ph2TpgPhiHw_station->at(bestTrigHW[i][j]);
	    short myHwSuperlayer = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);
            float myPosHW =  ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]);
            float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]);
	      	      
	    m_plots2["hPsi2DTM"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),myDirTwin);
	    m_plots2["hPos2DTM"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),myPosTwin);
	    m_plots2["hPhi2DTM"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_phi->at(bestTrigHW[i][j]),myPhiTwin);
	    m_plots2["hPhiB2DTM"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_phiB->at(bestTrigHW[i][j]),myPhiBTwin);

	    if (j == 2 || j == 3 || j == 4) {
	      m_plots2["hPsi2DTM"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),myDirTwin);
	      m_plots2["hPos2DTM"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),myPosTwin);
	      m_plots2["hPhi2DTM"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_phi->at(bestTrigHW[i][j]),myPhiTwin);
	      m_plots2["hPhiB2DTM"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_phiB->at(bestTrigHW[i][j]),myPhiBTwin);
	    } else {	   
	      m_plots2["hPsi2DTM"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),myDirTwin);
	      m_plots2["hPos2DTM"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),myPosTwin);
	      m_plots2["hPhi2DTM"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_phi->at(bestTrigHW[i][j]),myPhiTwin);
	      m_plots2["hPhiB2DTM"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_phiB->at(bestTrigHW[i][j]),myPhiBTwin);

        short indexSL = myHwSuperlayer / 2; 

	      m_plots2["hPsi2DTM" +chambTags.at(i) + labelTags.at(2) + slTags.at(indexSL)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]), myDirTwin );
	      m_plots2["hPos2DTM" +chambTags.at(i) + labelTags.at(2) + slTags.at(indexSL)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j])  , myPosTwin );
	      m_plots2["hPhi2DTM" +chambTags.at(i) + labelTags.at(2) + slTags.at(indexSL)]->Fill(ph2TpgPhiHw_phi->at(bestTrigHW[i][j])       , myPhiTwin );
	      m_plots2["hPhiB2DTM"+chambTags.at(i) + labelTags.at(2) + slTags.at(indexSL)]->Fill(ph2TpgPhiHw_phiB->at(bestTrigHW[i][j])      , myPhiBTwin);



	    }	   
	    m_plots2["hPsi2DTM"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),myDirTwin);
	    m_plots2["hPos2DTM"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),myPosTwin);
	    m_plots2["hPhi2DTM"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_phi->at(bestTrigHW[i][j]),myPhiTwin);
	    m_plots2["hPhiB2DTM"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_phiB->at(bestTrigHW[i][j]),myPhiBTwin);
          
         }

	
      }
      /***********************************************************************************************************************************************
      *
      * 							  FIRMWARE - EMULATOR
      * 
      ************************************************************************************************************************************************/
     
      int counterHW [4][2]; 
      int counterAM [4][2]; 
      for (int i = 0; i < 4; i++) {
	bool corHW = false, corAM = false, SL1HW = false, SL1AM = false, SL3HW = false, SL3AM = false; 
	for (int j = 0; j < 2; j++) {
	  counterHW[i][j] = 0; 
	  counterAM[i][j] = 0; 
	}
	if (i == 0) continue; //FIXME when MB1 starts working
	for (auto & primitiveHW : primitivesHW[i]) {
	  counterHW[i][0]++; 
	  if (qualityGroup (primitiveHW.quality) > 1){
	    counterHW[i][1]++;
            corHW = true; 
              m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position,0.);
	  } else {
	    if (primitiveHW.superLayer == 1){
	      SL1HW = true;
	      m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position,1.);
	    } 
	    else if (primitiveHW.superLayer == 3){
	      SL3HW = true;
	      m_plots2["hPrimTypeVsPos"+ chambTags.at(i)]->Fill(primitiveHW.position,2.);
	    } 
	  } 
	}
	for (auto & primitiveAM : primitivesAM[i]) {
	  counterAM[i][0]++;
	  if (qualityGroup (primitiveAM.quality) > 1){ 
	    counterAM[i][1]++;
            corAM = true; 
	  } else {
	    if (primitiveAM.superLayer == 1) SL1AM = true; 
	    else if (primitiveAM.superLayer == 3) SL3AM = true; 
	  } 
	}
        if (counterHW[i][0] != 0 || counterAM[i][0] != 0)m_plots2["hPrimNumber" + chambTags.at(i)] -> Fill(counterHW[i][0], counterAM[i][0]);	
        if (counterHW[i][1] != 0 || counterAM[i][1] != 0)m_plots2["hCorPrimNumber" + chambTags.at(i)] -> Fill(counterHW[i][1], counterAM[i][1]);	
	
	if (corHW) {
	  m_plots2["hHWQuality" + chambTags.at(i)]->Fill(0.,entryNumber);
	  if (corAM) { 
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0.,0.);
	  } else if (SL1AM && SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0.,1);
	  } else if (SL3AM && !SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2,entryNumber);
	    m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(2,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0.,2);
	  } else if (SL3AM && !SL1AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0.,3);
	  } else {
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4,entryNumber);
	    m_plots2["hAMQualityWhenHWcor"+chambTags.at(i)]->Fill(4,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(0.,4);
	  } 
	} else if (SL1HW && SL3HW) {
	  m_plots2["hHWQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	  if (corAM) { 
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(1.,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1,0.);
	  } else if (SL1AM && SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1,1);
	  } else if (SL1AM && !SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1,2);
	  } else if (SL3AM && !SL1AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1,3);
	  } else {
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(1,4);
	  }
	} else if (SL1HW && !SL3HW) {
	  m_plots2["hHWQuality"+chambTags.at(i)]->Fill(2,entryNumber);
	  if (corAM) { 
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(2.,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2,0.);
	  } else if (SL1AM && SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2,1);
	  } else if (SL1AM && !SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2,2);
	  } else if (SL3AM && !SL1AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2,3);
	  } else {
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(2,4);
	  }
	} else if (SL3HW && !SL1HW) {
	  //m_plots2["hHWQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	  if (corAM) { 
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(3.,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3,0.);
	  } else if (SL1AM && SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3,1);
	  } else if (SL1AM && !SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3,2);
	  } else if (SL3AM && !SL1AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3,3);
	  } else {
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(4,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(3,4);
	  } 
	} else if (corAM || SL1AM || SL3AM ){ 
	  m_plots2["hHWQuality"+chambTags.at(i)]->Fill(4,entryNumber);
	  if (corAM) { 
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(0.,entryNumber);
	    m_plots2["hHWQualityWhenAMcor"+chambTags.at(i)]->Fill(4.,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4,0.);
	  } else if (SL1AM && SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(1,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4,1);
	  } else if (SL1AM && !SL3AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(2,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4,2);
	  } else if (SL3AM && !SL1AM){
	    m_plots2["hAMQuality"+chambTags.at(i)]->Fill(3,entryNumber);
	    m_plots2["hEventQuality"+chambTags.at(i)]->Fill(4,3);
	  }  
	}


      }   


      /***********************************************************************************************************************************************
      *
      * 							COMPARING WITH SEGMENTS
      * 
      ************************************************************************************************************************************************/

      bool entro = false; 
      for (std::size_t iSeg = 0; iSeg <  seg_nSegments; ++iSeg) {


   	float mySegt0 = seg_phi_t0->at(iSeg);
   	short mySegStation = seg_station->at(iSeg);
   	short mySegWheel = seg_wheel->at(iSeg);
   	short mySegSector = seg_sector->at(iSeg);
	float mySegPos = seg_posLoc_x->at(iSeg);
   	float mySegPosSL1 = seg_posLoc_x_SL1->at(iSeg);
        float mySegPosSL3 = seg_posLoc_x_SL3->at(iSeg);
        float mySegPosMid = seg_posLoc_x_midPlane->at(iSeg);
	float mySegPsi = 360*TMath::ATan ( ( seg_dirLoc_x->at(iSeg) / seg_dirLoc_z->at(iSeg)) ) / (2*TMath::Pi());
	//cout <<"Wh:" << mySegWheel << " Se:" << mySegSector << " St:" << mySegStation << " Hits:" << seg_phi_nHits ->at(iSeg) << endl;

        if (mySegSector == 12 && mySegWheel == 2 && seg_phi_nHits->at(iSeg) == 8){ m_plots["hPrimsSegs" + chambTags.at(mySegStation/2 -1)] -> Fill(2);
	
	entro = true; 
	if (debug) cout <<"Segment in Wh:" << mySegWheel << " Se:" << mySegSector << " St:" << mySegStation << " Hits:" << seg_phi_nHits ->at(iSeg) << endl;

	}

	if(mySegWheel==2 && mySegSector==12){
	  int indstat = mySegStation-1;
	  m_plots2["h2DHwQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg),bestQualTrigHW[indstat]);
	  m_plots2["h2DEmuQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg),bestQualTrigAM[indstat]);
	  m_plots2["h2DTMQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg),bestQualTrigTM[indstat]);

	  m_effs["hEffHWvsSegX"+chambTags.at(indstat)]->Fill(IbestQualTrigHW[indstat]!=-1,mySegPos);
	  m_effs["hEffAMvsSegX"+chambTags.at(indstat)]->Fill(IbestQualTrigAM[indstat]!=-1,mySegPos);
	  m_effs["hEffTMvsSegX"+chambTags.at(indstat)]->Fill(IbestQualTrigTM[indstat]!=-1,mySegPos);
	  m_effs["hEffHWvsSegXGoodBX"+chambTags.at(indstat)]->Fill(IbestQualTrigBXHW[indstat]!=-1,mySegPos);
	  m_effs["hEffAMvsSegXGoodBX"+chambTags.at(indstat)]->Fill(IbestQualTrigBXAM[indstat]!=-1,mySegPos);
	  m_effs["hEffTMvsSegXGoodBX"+chambTags.at(indstat)]->Fill(IbestQualTrigBXTM[indstat]!=-1,mySegPos);
	}
	
	for (unsigned int i = 0; i<chambTags.size(); i++){
          for (unsigned int j = 0; j<quTags.size(); j++){
            if (bestTrigHW[i][j] != -1 && mySegt0 > -500){
	      
	      short myHwWheel = ph2TpgPhiHw_wheel->at(bestTrigHW[i][j]);
	      short myHwSector = ph2TpgPhiHw_sector->at(bestTrigHW[i][j]);
	      short myHwStation = ph2TpgPhiHw_station->at(bestTrigHW[i][j]);
              float myPosHW =  ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]);
              float myDirHW =  ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]);
	
	      if (mySegWheel != myHwWheel || mySegSector != myHwSector || mySegStation != myHwStation ) continue; 

  	       
	      float mySegPos; 
 	      if ((j != 2 && j !=3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 1)) mySegPos = mySegPosSL1;
 	      else if ((j != 2 && j != 3 && j != 4 ) && (ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]) == 3)) mySegPos = mySegPosSL3;
 	      else mySegPos = mySegPosMid; 
  	      
	      //eventoBX = ph2TpgPhiHw_BX ->at(bestTrigHW[i][j]);              

              int myt0HW = ph2TpgPhiHw_t0->at(bestTrigHW[i][j]); 
	      if (correctL1A) myt0HW = myt0HW - eventoBX*25;
	      //if (myt0HW < 0) myt0HW += 3564*25;	
	      	      
	      m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	      m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset[i]*25,mySegt0);
	      m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	      m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	      m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset[i]*25 - mySegt0);
   	      m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
              m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(0)]->Fill(mySegPos,myt0HW - offset[i]*25 - mySegt0);
              m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(0)]->Fill(mySegPsi,myt0HW - offset[i]*25 - mySegt0);

	      if (j == 2 || j == 3 || j == 4) {
	      //if (j+1 == 6 || j+1 == 8 || j+1 == 9) {
	        m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset[i]*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset[i]*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(1)]->Fill(mySegPos,myt0HW - offset[i]*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(1)]->Fill(mySegPsi,myt0HW - offset[i]*25 - mySegt0);
	      } else {	   
	        m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset[i]*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset[i]*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)]->Fill(mySegPos,myt0HW - offset[i]*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)]->Fill(mySegPsi,myt0HW - offset[i]*25 - mySegt0);
	       
		short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	

		 
		m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset[i]*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset[i]*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myPosHW,myt0HW - offset[i]*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myDirHW,myt0HW - offset[i]*25 - mySegt0);
                //m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(mySegPos,myt0HW - eventoBX*25 - mySegt0);
                //m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(mySegPsi,myt0HW - eventoBX*25 - mySegt0);
	      }	   

 
	      m_plots2["hPsi2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	      m_plots2["hTime2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset[i],mySegt0);
	      m_plots2["hPos2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	      m_plots["hPsiSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	      m_plots["hTimeSeg"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset[i]*25 - mySegt0);
   	      m_plots["hPosSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
              m_plots2["hTimeSegvsPos"+ chambTags.at(i) + quTags.at(j)]->Fill(mySegPos,myt0HW - offset[i]*25 - mySegt0);
              m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + quTags.at(j)]->Fill(mySegPsi,myt0HW - offset[i]*25 - mySegt0);
            }
           }
	}		
      } //for segments



      // if (entro) cout << "------------------------------------------------------" << endl; 

      if (bestI[0] != -1 && bestI[3] != -1) {
        m_effs["hEffCor"]->Fill(true,0);
        m_effs["hEffCorAM"]->Fill(true,0);
        m_effs["hEffCorHW"]->Fill(true,0);
      } else if (bestI[0] == -1 && bestI[3] != -1) {
        m_effs["hEffCor"]->Fill(false,0);
        m_effs["hEffCorAM"]->Fill(true,0);
        m_effs["hEffCorHW"]->Fill(false,0);
      } else if (bestI[3] == -1 && bestI[0] != -1) {
        m_effs["hEffCor"]->Fill(false,0);
        m_effs["hEffCorHW"]->Fill(true,0);
        m_effs["hEffCorAM"]->Fill(false,0);
      }

       
}

void DTNtupleTPGSimAnalyzer::endJob()
{

  m_outFile.cd();

  m_outFile.Write();
  m_outFile.Close();

}

Double_t DTNtupleTPGSimAnalyzer::trigPhiInRad(Double_t trigPhi, Int_t sector)
{
  
  return trigPhi / 65536. * 0.8 + TMath::Pi() / 6 * (sector - 1); 
  
}

Int_t DTNtupleTPGSimAnalyzer::qualityGroup(Int_t quality)
{
  
  if (quality == 1 || quality == 2 || quality == 5) return 0; 
  if (quality == 3 || quality == 4 || quality == 7) return 1; 
  if (quality == 6) return 2; 
  if (quality == 8) return 3; 
  if (quality == 9) return 4; 
 
}
