//#include "tdrstyle.C"


void printPlots () {

  const bool fileOK = false; 

  //setTDRStyle(); 
  gStyle->SetOptStat(1111111);
  std::vector<std::string> stuffTags = { "Time", "Pos", "Psi"};

  std::string run = "330848";
  //std::string run = "330848";
  TString runNumber = run; 
  TString file = "results_run" + runNumber + ".root";
  gSystem->Exec("mkdir run" + runNumber);

  TFile data1(file);
  //TFile data1("results_run330792.root");
  //TFile data1("results_new_index.root");
  int a = 0;

  std::vector<std::string> chambTags = { "MB2", "MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  //std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  //std::vector<std::string> quTags = {"Q1","Q2","Q3","Q4","Q5","Q6","Q7","Q8","Q9"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q8","Q9"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TEfficiency*> m_effs;

  char name [128];

  std::vector <std::string> generalEffPlots {"hEffCorAM", "hEffCor"}; 
  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM"}; 
//  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM", "hBXDif"}; 
  std::vector <std::string> specific1DPlots {"hSLHW_", "hSLAM_", "hPrimsSegs_"};
  std::vector <std::string> moreSpecific1DPlots {"hBX_","hBXDif_", "hBXfromT0_", "hChi2_", "hPsi_", "hTime_","hPos_", "hPsiSeg_", "hTimeSeg_","hPosSeg_"};
  std::vector <std::string> moreSpecific2DPlots {"hPsi2D_", "hTime2D_","hPos2D_","hPsi2DTM_","hPos2DTM_","hPhi2DTM_","hPhiB2DTM_","hPsi2DSeg_", "hTime2DSeg_","hPos2DSeg_", "hTimeSegvsPos_", "hTimeSegvsPsi_"};


  for (auto & generalPlot : generalEffPlots) {
    std::string nameHisto = generalPlot;
    sprintf(name,"%s",nameHisto.c_str());
    m_effs[name] = (TEfficiency*) data1.Get(name);
    m_effs[name]->Draw();
    sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
    gPad->SaveAs(name);
    if (fileOK) cout << nameHisto << ".png" << endl;
  }
  for (auto & generalPlot : general1DPlots) {
    std::string nameHisto = generalPlot;
    sprintf(name,"%s",nameHisto.c_str());
    m_plots[name] = (TH1F*) data1.Get(name);
    m_plots[name]->Draw();
    sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
    gPad->SaveAs(name);
    if (fileOK) cout << nameHisto << ".png" << endl;
  }
  
  for (const auto & chambTag : chambTags) {

    for (auto & specificPlot : specific1DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots[name] = (TH1F*) data1.Get(name);
      m_plots[name]->Draw();
      sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    }
    for (const auto & labelTag : labelTags) {

      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + labelTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots[name] = (TH1F*) data1.Get(name);
        m_plots[name]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + labelTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      if (labelTag == "All" || labelTag == "Correlated") continue;
      for (auto  slTag : slTags) {
        for (auto & specificPlot : moreSpecific1DPlots) {
          if (specificPlot == "hPsi_" || specificPlot == "hTime_" || specificPlot == "hPos_") continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          sprintf(name,"%s",nameHisto.c_str());
          m_plots[name] = (TH1F*) data1.Get(name);
          m_plots[name]->Draw();
          sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      
        for (auto & specificPlot : moreSpecific2DPlots) {
          if (specificPlot == "hPsi2D_" || specificPlot == "hTime2D_" || specificPlot == "hPos2D_") continue;
          if (specificPlot == "hPsi2DTM_" ||specificPlot == "hPos2DTM_" || specificPlot == "hPhi2DTM_" ||specificPlot == "hPhiB2DTM_" ) continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          sprintf(name,"%s",nameHisto.c_str());
          m_plots2[name] = (TH2F*) data1.Get(name);
          m_plots2[name]->Draw();
          sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      } // sl
    } // label
    for (const auto & quTag : quTags) {
      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + quTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots[name] = (TH1F*) data1.Get(name);
        m_plots[name]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + quTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
    } //qu
  } // chamber

} // end macro
