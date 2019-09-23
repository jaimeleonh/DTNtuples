#include "DTNtupleTPGSimAnalyzer.h"

#include"TMath.h"

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

  debug = false;   
  correctL1A = correct; 
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
 // for (Long64_t jentry=0; jentry<50000;jentry++) 
  for (Long64_t jentry=0; jentry<nentries;jentry++) 
    {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEvent(jentry);   nbytes += nb;

      if(jentry % 100 == 0) 
	std::cout << "[DTNtupleTPGSimAnalyzer::Loop] processed : " 
		  << jentry << " entries\r" << std::flush;

    if (debug)  std::cout << "====================Entry " << jentry << " =================="<< std::endl; 
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
					    "Distribution of AM qualities",
					    9,0.5,9.5); 
      m_plots["hBXDif"] = new TH1F("hBXDif",
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
      m_plots["hBXtotal"] = new TH1F("hBXtotal",
					    "Distribution of BX; BX; Entries",
					    3564,0,3564); 

      
      for (const auto & chambTag : chambTags) {
	
        m_effs["hEffHWvsSegX"+ chambTag] = new TEfficiency(("hEffHWvsSegX_"+ chambTag).c_str(),
					    "HW Eff vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffAMvsSegX"+ chambTag] = new TEfficiency(("hEffAMvsSegX_"+ chambTag).c_str(),
					    "AM Eff vs Seg X",
					    50,-251.5,250.5); 
	m_effs["hEffTMvsSegX"+ chambTag] = new TEfficiency(("hEffTMvsSegX_"+ chambTag).c_str(),
					    "TM Eff vs Seg X",
					    50,-251.5,250.5); 

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
        m_plots2["h2DHwQualSegNHits"+chambTag]=new TH2F(("h2DHwQualSegNHits_" + chambTag).c_str(),"NHitsSeg vs HWQual",9,0.5,9.5,9,0.5,9.5);
        m_plots2["h2DEmuQualSegNHits"+chambTag]=new TH2F(("h2DEmuQualSegNHits_" + chambTag).c_str(),"NHitsSeg vs EmuQual",9,0.5,9.5,9,0.5,9.5);

        for (const auto & labelTag : labelTags) {
          m_plots["hBX" + chambTag + labelTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    3564,0,3564); 
					    //201,3199.5,3400.5); 
          m_plots["hBXDif" + chambTag + labelTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
          m_plots["hBXfromT0" + chambTag + labelTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    2000,-3564,3564); 
					    //201,3199.5,3400.5); 
          m_plots["hChi2" +  chambTag + labelTag] = new TH1F(("hChi2_" + chambTag + "_" + labelTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    32,-0.5,31.5); 
          m_plots2["hPsi2D" + chambTag + labelTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    120,-60,60,120,-60,60); 
          m_plots2["hTime2D"+ chambTag + labelTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    400,-200,200,400,-200,200); 
          m_plots2["hPos2D"+ chambTag + labelTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    300,0,300,300,0,300); 
          m_plots["hPsi"+ chambTag + labelTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    40,-2,2); 
          m_plots["hTime"+ chambTag + labelTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    200,-100,100); 
          m_plots["hPos"+ chambTag + labelTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    100,-10,10); 
          m_plots2["hPsi2DSeg" + chambTag + labelTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Emulator #Psi (#circ)",
					    120,-60,60,120,-60,60); 
          m_plots2["hTime2DSeg"+ chambTag + labelTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
					    //200,-100,100,200,-100,100); 
          m_plots2["hPos2DSeg"+ chambTag + labelTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    300,0,300,300,0,300); 
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
					    300,0,300,200,-100,100); 
          m_plots2["hTimeSegvsPsi"+ chambTag + labelTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    120,-60,60,200,-100,100); 
       	  if (labelTag == "All" || labelTag == "Correlated") continue;
	  for (const auto & slTag : slTags) {
            m_plots["hBX" + chambTag + labelTag + slTag] = new TH1F(("hBX_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
	   				    "Distribution of BX; BX; Entries",
					    3564,0,3564); 
            m_plots["hBXDif" + chambTag + labelTag + slTag] = new TH1F(("hBXDif_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
            m_plots["hBXfromT0" + chambTag + labelTag + slTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    201,3199.5,3400.5); 
            m_plots["hChi2" +  chambTag + labelTag + slTag] = new TH1F(("hChi2_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    32,-0.5,31.5); 
       /*     m_plots2["hPsi2D" + chambTag + labelTag + slTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    120,-60,60,120,-60,60); 
            m_plots2["hTime2D"+ chambTag + labelTag + slTag] = new TH2F(("hTime2D_" + chambTag + "_"+ labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    200,-100,100,200,-100,100); 
            m_plots2["hPos2D"+ chambTag + labelTag + slTag] = new TH2F(("hPos2D_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    300,0,300,300,0,300); 
            m_plots["hPsi"+ chambTag + labelTag + slTag] = new TH1F(("hPsi_"+ chambTag + "_" +labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    40,-2,2); 
            m_plots["hTime"+ chambTag + labelTag + slTag] = new TH1F(("hTime_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    200,-100,100); 
            m_plots["hPos"+ chambTag + labelTag + slTag] = new TH1F(("hPos_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    100,-10,10); 
         */   m_plots2["hPsi2DSeg" + chambTag + labelTag + slTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Emulator #Psi (#circ)",
					    120,-60,60,120,-60,60); 
            m_plots2["hTime2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
            m_plots2["hPos2DSeg"+ chambTag + labelTag + slTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + labelTag + "_" + slTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    300,0,300,300,0,300); 
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
					    300,0,300,200,-100,100); 
            m_plots2["hTimeSegvsPsi"+ chambTag + labelTag + slTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+labelTag + "_" + slTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    120,-60,60,200,-100,100); 
          }
        }
	for (const auto & quTag : quTags) {
          m_plots["hBX" + chambTag + quTag] = new TH1F(("hBX_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of BX; BX; Entries",
					    3564,0,3564); 
          m_plots["hBXDif" + chambTag + quTag] = new TH1F(("hBXDif_" + chambTag + "_" + quTag).c_str(),
					    "BX difference ; BX difference; Entries",
					    //41,-220.5,-179.5); 
					    7000,-3564,3564); 
          m_plots["hBXfromT0" + chambTag + quTag] = new TH1F(("hBXfromT0_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of BX got from T0; BX; Entries",
					    201,3199.5,3400.5); 
          m_plots["hChi2" +  chambTag + quTag] = new TH1F(("hChi2_" + chambTag + "_" + quTag).c_str(),
					    "Distribution of #chi^2; #chi^2; Entries",
					    32,-0.5,31.5); 
          m_plots2["hPsi2D" + chambTag + quTag] = new TH2F(("hPsi2D_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Emulator #Psi; Firmware #Psi ( #circ); Emulator #Psi ( #circ)",
					    120,-60,60,120,-60,60); 
          m_plots2["hTime2D"+ chambTag + quTag] = new TH2F(("hTime2D_" + chambTag + "_"+ quTag).c_str(),
					    "Firmware vs Emulator Time; Firmware time (ns); Emulator time (ns)",
					    400,-200,200,400,-200,200); 
          m_plots2["hPos2D"+ chambTag + quTag] = new TH2F(("hPos2D_" + chambTag + "_" + quTag).c_str(),
					    "Firmware vs Emulator Position; Firmware position (cm); Emulator position (cm)",
					    300,0,300,300,0,300); 
          m_plots["hPsi"+ chambTag + quTag] = new TH1F(("hPsi_"+ chambTag + "_" +quTag).c_str(),
					    "Firmware - Emulator #Psi; Firmware - Emulator #Psi (#circ); Entries",
					    40,-2,2); 
          m_plots["hTime"+ chambTag + quTag] = new TH1F(("hTime_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Emulator time; Firmware - Emulator Time (ns); Entries",
					    200,-100,100); 
          m_plots["hPos"+ chambTag + quTag] = new TH1F(("hPos_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Emulator position; Firmware - Emulator Position (cm); Entries",
					    100,-10,10); 
          m_plots2["hPsi2DSeg" + chambTag + quTag] = new TH2F(("hPsi2DSeg_" + chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment #Psi; Firmware #Psi (#circ); Emulator #Psi (#circ)",
					    120,-60,60,120,-60,60); 
          m_plots2["hTime2DSeg"+ chambTag + quTag] = new TH2F(("hTime2DSeg_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment time; Firmware time (ns); Segment time (ns)",
					    400,-200,200,200,-100,100); 
          m_plots2["hPos2DSeg"+ chambTag + quTag] = new TH2F(("hPos2DSeg_"+ chambTag + "_" + quTag).c_str(),
					    "Firmware vs Segment position; Firmware position (cm); Segment position (cm)",
					    300,0,300,300,0,300); 
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
					    300,0,300,200,-100,100); 
          m_plots2["hTimeSegvsPsi"+ chambTag + quTag] = new TH2F(("hTimeSegvsPsi_"+ chambTag + "_"+quTag).c_str(),
					    "Firmware - Segment time vs Segment #Psi; Segment #Psi (#circ); Firmware - Segment time (ns)",
					    120,-60,60,200,-100,100); 

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
     int offset = -200; //FIXME
     //int offset = -270; //FIXME
     m_plots["hBXtotal"]->Fill(eventoBX);
     //int eventoBX = 3365; //848

     int bestI[6]; short bestQu[6];
     for (int i = 0; i<6; i++) {
       bestI[i] = -1; 
       bestQu[i] = -1; 
     }   
 
     //bool debug = false; 
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
      for(unsigned indstat=0;indstat<4; indstat++){
	bestQualTrigHW[indstat]=-1;IbestQualTrigHW[indstat]=-1;
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

 	int indstat = myQualityHW - 1; 	
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
	//if (myt0HW < 0) myt0HW += 3564*25;	

	if (myQualityHW <= 4){
		if (iTrigHW + 1 <  ph2TpgPhiHw_nTrigs) {
			if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+1)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+1)!=7 ))) continue;
		}
		if (iTrigHW + 2 <  ph2TpgPhiHw_nTrigs) {
			if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+2)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+2)!=7 ))) continue;
		}
	}	

//	if (myQualityHW <= 4 && ((ph2TpgPhiHw_quality->at(iTrigHW+1)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+1)!=7 ) || (ph2TpgPhiHw_quality->at(iTrigHW+2)>=6 && ph2TpgPhiHw_quality->at(iTrigHW+2)!=7 ) )) continue;

	m_plots["hBXDif"]->Fill(myBXHW);
//        m_plots["ht0" + chambTags.at(myStationHW/2-1)]->Fill(myt0HW - eventoBX*25);
        if (myQualityHW >= 9){ m_plots["hPrimsSegs" + chambTags.at(myStationHW-1)] -> Fill(0); } // cout << "Habemus primitiva" << endl;  }
	
        /*if (myt0HW - eventoBX*25 < bestTimeHW[myStationHW/2-1][myQualityHW-1]){
	  bestTrigHW[myStationHW/2-1][myQualityHW-1] = iTrigHW; 
	  bestTimeHW[myStationHW/2-1][myQualityHW-1] = myt0HW - eventoBX*25;
	} */
        m_plots["hdift0" + chambTags.at(myStationHW-1)]->Fill(myt0HW - offset*25);
        if (abs(myt0HW - offset*25) < bestTimeHW[myStationHW-1][qualityGroup(myQualityHW)]){
	  bestTrigHW[myStationHW-1][qualityGroup(myQualityHW)] = iTrigHW; 
	  bestTimeHW[myStationHW-1][qualityGroup(myQualityHW)] = abs(myt0HW - offset*25);
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
	m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myBXHW);
	m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(round(myt0HW/25));
	m_plots["hChi2"+chambTags.at(myStationHW-1)+labelTags.at(0)]->Fill(myChi2HW);

	if (myQualityHW == 6 || myQualityHW == 8 || myQualityHW == 9){	
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myBXHW);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(round(myt0HW/25));
	  m_plots["hChi2"+chambTags.at(myStationHW-1)+labelTags.at(1)]->Fill(myChi2HW);
	} else {
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myBXHW);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(round(myt0HW/25));
	  m_plots["hChi2"+chambTags.at(myStationHW-1)+labelTags.at(2)]->Fill(myChi2HW);
	  
	  m_plots["hBX"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myBXHW);
	  m_plots["hBXDif"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myBXHW);
	  m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(round(myt0HW/25));
	  m_plots["hChi2"+chambTags.at(myStationHW-1)+labelTags.at(2)+slTags.at(mySLHW/2)]->Fill(myChi2HW);
	}

/*	m_plots["hBX"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(round(myt0HW/25));
	m_plots["hBXDif"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(round(myt0HW/25) - 32*myBXHW/25);
_plots["hQualityHW"]->Fill(myQualityHW);
	m_plots["hChi2"+chambTags.at(myStationHW/2-1)+quTags.at(myQualityHW-1)]->Fill(myChi2HW);
 */
	m_plots["hBX"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myBXHW);
	m_plots["hBXDif"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myBXHW);
	m_plots["hBXfromT0"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(round(myt0HW/25));
//	m_plots["hBXDif"+chambTags.at(myStationHW/2-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(round(myt0HW/25) - 32*myBXHW/25);
	m_plots["hQualityHW"]->Fill(myQualityHW);
	m_plots["hChi2"+chambTags.at(myStationHW-1)+quTags.at(qualityGroup(myQualityHW))]->Fill(myChi2HW);
	

     } // end HW
     
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
      for(unsigned indstat=0;indstat<4; indstat++){
	bestQualTrigAM[indstat]=-1;IbestQualTrigAM[indstat]=-1;
      }

 
      for (std::size_t iTrigAM = 0; iTrigAM < ph2TpgPhiEmuAm_nTrigs; ++iTrigAM) {
		
        short myStationAM = ph2TpgPhiEmuAm_station->at(iTrigAM);
        short mySectorAM = ph2TpgPhiEmuAm_sector->at(iTrigAM);
        short myWheelAM = ph2TpgPhiEmuAm_wheel->at(iTrigAM);
        short myQualityAM = ph2TpgPhiEmuAm_quality->at(iTrigAM);
        short mySLAM = ph2TpgPhiEmuAm_superLayer->at(iTrigAM);
        int myChiAM =  ph2TpgPhiEmuAm_chi2->at(iTrigAM);
        int myPhiAM = ph2TpgPhiEmuAm_phi->at(iTrigAM);
        int myPhiBAM =   ph2TpgPhiEmuAm_phiB->at(iTrigAM);
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
	  myBXAM = myt0AM - 3564   ; 
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
	if (abs(myt0AM - offset*25) < bestTimeAM[myStationAM-1][qualityGroup(myQualityAM)]){
	  bestTrigAM[myStationAM-1][qualityGroup(myQualityAM)] = iTrigAM; 
	  bestTimeAM[myStationAM-1][qualityGroup(myQualityAM)] = abs(myt0AM - offset*25);
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
          cout << "Chi2 " << myChiAM << endl;	
          cout << "Phi " << myPhiAM << endl;	
          cout << "PhiB " << myPhiBAM << endl;	
          cout << "Position " << myPosAM << endl;	
          cout << "Direction " << myDirAM << endl;	
          cout << "BX " << myBXAM << endl;	
          cout << "t0 " << myt0AM << endl;	
          cout << "-------------------------------------------------------------------------" << endl;
        }	
	m_plots["hQualityAM"]->Fill(myQualityAM);
        m_plots["hSLAM" + chambTags.at(myStationAM-1)]->Fill(mySLAM);
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
	    m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset*25, myt0AM - offset*25);
	    //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	    m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	    m_plots["hPsi"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots["hTime"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset*25 - myt0AM + offset*25);
	    //m_plots["hTime"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	    m_plots["hPos"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   

	    if (j == 2 || j == 3 || j == 4) {
	    //if (j+1 == 6 || j+1 == 8 || j+1 == 9) {
  	      m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset*25, myt0AM - offset*25);
	      //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	      m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	      m_plots["hPsi"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots["hTime"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset*25 - (myt0AM - offset*25));
	      //m_plots["hTime"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	      m_plots["hPos"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   
	    } else {	   
  	      m_plots2["hPsi2D"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset*25, myt0AM - offset*25);
	      //m_plots2["hTime2D"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	      m_plots2["hPos2D"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	      m_plots["hPsi"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	      m_plots["hTime"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset*25 - (myt0AM - offset*25));
	      //m_plots["hTime"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	      m_plots["hPos"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   
	    }	   
 

  	    m_plots2["hPsi2D"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots2["hTime2D"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset*25, myt0AM - offset*25);
	    //m_plots2["hTime2D"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - eventoBX*25,ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25);
	    m_plots2["hPos2D"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));
	    m_plots["hPsi"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_dirLoc_phi->at(bestTrigAM[i][j]));
	    m_plots["hTime"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset*25 - (myt0AM - offset*25));
	    //m_plots["hTime"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - eventoBX*25 - (ph2TpgPhiEmuAm_t0->at(bestTrigAM[i][j]) - eventoBX*25));
   	    m_plots["hPos"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - ph2TpgPhiEmuAm_posLoc_x->at(bestTrigAM[i][j]));   
          }
        }
      }

     if (debug && ltTwinMuxIn_nTrigs!=0) cout << "####################### TwimMux PRIMITIVES ############################" << endl;  
        int bestQualTrigTM[4];int IbestQualTrigTM[4] ; // 4 stations MB1 to MB4 
        for(unsigned indstat=0;indstat<4; indstat++){
       	  bestQualTrigTM[indstat]=-1;IbestQualTrigTM[indstat]=-1;
        }
  
      for (std::size_t iTwin = 0; iTwin <  ltTwinMuxIn_nTrigs; ++iTwin) {

        short myStationTwin = ltTwinMuxIn_station->at(iTwin);
        short mySectorTwin = ltTwinMuxIn_sector->at(iTwin);
        short myWheelTwin = ltTwinMuxIn_wheel->at(iTwin);
        short myQualityTwin = ltTwinMuxIn_quality->at(iTwin);
        int myPhiTwin = ltTwinMuxIn_phi->at(iTwin);
        int myPhiBTwin =   ltTwinMuxIn_phiB->at(iTwin);
        float myPosTwin =  ltTwinMuxIn_posLoc_x->at(iTwin);
        float myDirTwin =  ltTwinMuxIn_dirLoc_phi->at(iTwin);
        int myBXTwin = ltTwinMuxIn_BX->at(iTwin);

        if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 2){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl;  }
        if (myQualityTwin >= 5 && myWheelTwin == 2 && mySectorTwin == 12 && myStationTwin == 4){ m_plots["hPrimsSegs" + chambTags.at(myStationTwin/2-1)] -> Fill(3); } // cout << "Habemus primitiva" << endl;  }

	int indstat = myStationTwin - 1;  
	if(myQualityTwin>bestQualTrigTM[indstat]){
	  bestQualTrigTM[indstat]=myQualityTwin;
	  IbestQualTrigTM[indstat]=iTwin;
	}

	
      }

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

	bool IhaveHW=true; 
	bool IhaveAM=true; 
	bool IhaveTM=true; 
	if(mySegWheel==2 && mySegSector==12){
	  int indstat = mySegStation-1;
	  m_plots2["h2DHwQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg),bestQualTrigHW[indstat]);
	  m_plots2["h2DEmuQualSegNHits"+chambTags.at(indstat)]->Fill(seg_phi_nHits->at(iSeg),bestQualTrigAM[indstat]);
	  if(IbestQualTrigHW[indstat]==-1) IhaveHW=false;
	  if(IbestQualTrigAM[indstat]==-1) IhaveAM=false;
	  if(IbestQualTrigTM[indstat]==-1) IhaveTM=false;

	  m_effs["hEffHWvsSegX"+chambTags.at(indstat)]->Fill(IhaveHW,mySegPos);
	  m_effs["hEffAMvsSegX"+chambTags.at(indstat)]->Fill(IhaveAM,mySegPos);
	  m_effs["hEffTMvsSegX"+chambTags.at(indstat)]->Fill(IhaveTM,mySegPos);
	}
	
	for (unsigned int i = 0; i<chambTags.size(); i++){
          for (unsigned int j = 0; j<labelTags.size(); j++){
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
	      m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset*25,mySegt0);
	      m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	      m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	      m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(myt0HW - offset*25 - mySegt0);
   	      m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(0)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
              m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(0)]->Fill(mySegPos,myt0HW - offset*25 - mySegt0);
              m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(0)]->Fill(mySegPsi,myt0HW - offset*25 - mySegt0);

	      if (j == 2 || j == 3 || j == 4) {
	      //if (j+1 == 6 || j+1 == 8 || j+1 == 9) {
	        m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(myt0HW - offset*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(1)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(1)]->Fill(mySegPos,myt0HW - offset*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(1)]->Fill(mySegPsi,myt0HW - offset*25 - mySegt0);
	      } else {	   
	        m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(myt0HW - offset*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)]->Fill(mySegPos,myt0HW - offset*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)]->Fill(mySegPsi,myt0HW - offset*25 - mySegt0);
	       
		short myHWSL = ph2TpgPhiHw_superLayer->at(bestTrigHW[i][j]);	

		 
		m_plots2["hPsi2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	        m_plots2["hTime2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset*25,mySegt0);
	        m_plots2["hPos2DSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	        m_plots["hPsiSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	        m_plots["hTimeSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myt0HW - offset*25 - mySegt0);
   	        m_plots["hPosSeg"+chambTags.at(i)+labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
                m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myPosHW,myt0HW - offset*25 - mySegt0);
                m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(myDirHW,myt0HW - offset*25 - mySegt0);
                //m_plots2["hTimeSegvsPos"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(mySegPos,myt0HW - eventoBX*25 - mySegt0);
                //m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + labelTags.at(2)+slTags.at(myHWSL/2)]->Fill(mySegPsi,myt0HW - eventoBX*25 - mySegt0);
	      }	   

 
	      m_plots2["hPsi2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]),mySegPsi);
	      m_plots2["hTime2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset,mySegt0);
	      m_plots2["hPos2DSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]),mySegPos);
	      m_plots["hPsiSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_dirLoc_phi->at(bestTrigHW[i][j]) - mySegPsi);
	      m_plots["hTimeSeg"+chambTags.at(i)+quTags.at(j)]->Fill(myt0HW - offset*25 - mySegt0);
   	      m_plots["hPosSeg"+chambTags.at(i)+quTags.at(j)]->Fill(ph2TpgPhiHw_posLoc_x->at(bestTrigHW[i][j]) - mySegPos);
              m_plots2["hTimeSegvsPos"+ chambTags.at(i) + quTags.at(j)]->Fill(mySegPos,myt0HW - offset*25 - mySegt0);
              m_plots2["hTimeSegvsPsi"+ chambTags.at(i) + quTags.at(j)]->Fill(mySegPsi,myt0HW - offset*25 - mySegt0);
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
