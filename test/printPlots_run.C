//#include "tdrstyle.C"
#include "TH1.h"
#include "TH2.h"
#include <TStyle.h>
#include <TCanvas.h>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"

void printPlots_run(std::string run) {

  const bool fileOK = false; 

  //setTDRStyle(); 
  gStyle->SetOptStat(1111111);
  std::vector<std::string> stuffTags = { "Time", "Pos", "Psi"};

  //std::string run = "331025";
  //std::string run = "330848";
  TString runNumber = run; 
  TString file = "results_run" + runNumber + ".root";
  gSystem->Exec("mkdir run" + runNumber);

  TFile data1(file);
  //TFile data1("results_run330792.root");
  //TFile data1("results_new_index.root");
  int a = 0;

  std::vector<std::string> chambTags = { "MB2","MB3", "MB4"};
  std::vector<std::string> slTags = { "SL1", "SL3"};
  //std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  //std::vector<std::string> quTags = {"Q1","Q2","Q3","Q4","Q5","Q6","Q7","Q8","Q9"};
  std::vector<std::string> quTags = {"3h","4h","Q6","Q8","Q9"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
  std::vector<std::string> labelTagsPlots = {"All", "Correlated", "Uncorrelated", "UncorrelatedSL1", "UncorrelatedSL3"};
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TEfficiency*> m_effs;

  char name [128];

  std::vector <std::string> categories {"perGroup","perQuality"}; 

  std::vector <std::string> generalEffPlots {"hEffHWvsSegX_", "hEffTMvsSegX_", "hEffAMvsSegX_","hEffHWvsSegXGoodBX_", "hEffTMvsSegXGoodBX_", "hEffAMvsSegXGoodBX_"}; 
  //std::vector <std::string> generalEffPlots {"hEffHWvsSegX", "hEffTMvsSegX", "hEffAMvsSegX", "hEffCorAM", "hEffCor"}; 
  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM"}; 
//  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM", "hBXDif"}; 
  std::vector <std::string> specific1DPlots {"hSLHW_", "hSLAM_", "hPrimsSegs_","hQualityHW_", "hQualityAM_"};
  std::vector <std::string> specific2DPlots {"hPrimTypeVsPos_","h2DHwQualSegNHits_","h2DEmuQualSegNHits_","h2DTMQualSegNHits_","hQualityVsBXHW_", "hQualityVsBXAM_"};
  //std::vector <std::string> moreSpecific1DPlots { };
  std::vector <std::string> moreSpecific1DPlots {"hBX_","hBXDif_","hBXEmul_","hBXDifEmul_", "hBXfromT0_", "hChi2FW_","hChi2Emul_", "hPsi_", "hTime_","hPos_", "hPsiSeg_", "hTimeSeg_","hPosSeg_"};
  std::vector <std::string> moreSpecific2DPlots {"hPsi2D_", "hTime2D_","hPos2D_","hPsi2DSeg_", "hTime2DSeg_","hPos2DSeg_", "hTimeSegvsPos_", "hTimeSegvsPsi_"};

  std::vector <std::string> axisAndUnits {"BX (BX units)", "BX (BX units)","BX (BX units)", "BX (BX units)", "BX (BX units)", "FW chi2 (U.A)", "Emul chi2 (U.A)", "Firmware - Emulator Psi (#circ)", "Firmware - Emulator Time (ns)", "Firmware - Emulator Position (cm)", "Firmware - Segment Psi (#circ)", "Firmware - Segment Time (ns)", "Firmware - Segment Position (cm)" };

  std::map<std::string, TH1*> m_plots_res;
  std::map<std::string, TH1*> m_plots_mean;

  for (auto & chambTag : chambTags) {
    for (int j = 0; j < moreSpecific1DPlots.size(); j++){
      auto specific1DPlot = moreSpecific1DPlots.at(j);
   
      m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "res_" + chambTag + "_" + categories.at(0)).c_str(), 
					    "Resolutions; ; Resolution (a.u.) ",
					    5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "mean_" + chambTag + "_" + categories.at(0)).c_str(), 
					    "Means; ; Mean (a.u.) ",
					    5,-0.5,4.5); 
      for (int i = 0; i < 5; i++){
        m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
        m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
      }
      
      m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "res_" +  chambTag + "_" + categories.at(1)).c_str(), 
					    "Resolutions; ; Resolution (a.u.) ",
					    5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "mean_" +  chambTag + "_" + categories.at(1)).c_str(), 
					    "Means; ; Mean (a.u.) ",
					    5,-0.5,4.5); 
      for (int i = 0; i < 5; i++){
        m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
        m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      }

      m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_res[specific1DPlot + "res_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());

      m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_mean[specific1DPlot + "mean_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());

    }
  }





/*
  for (auto & generalPlot : generalEffPlots) {
    std::string nameHisto = generalPlot;
    sprintf(name,"%s",nameHisto.c_str());
    m_effs[name] = (TEfficiency*) data1.Get(name);
    m_effs[name]->Draw();
    sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
    gPad->SaveAs(name);
    if (fileOK) cout << nameHisto << ".png" << endl;
  } */ 
  for (auto & generalPlot : general1DPlots) {
    std::string nameHisto = generalPlot;
    sprintf(name,"%s",nameHisto.c_str());
    m_plots[name] = (TH1F*) data1.Get(name);
    m_plots[name]->Draw();
    sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
    gPad->SaveAs(name);
    if (fileOK) cout << nameHisto << ".png" << endl;
  }
 
  for (int i = 0; i<chambTags.size(); i++) {
    auto chambTag = chambTags.at(i);

    for (auto & generalPlot : generalEffPlots) {
      std::string nameHisto = generalPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_effs[name] = (TEfficiency*) data1.Get(name);
      m_effs[name]->Draw();
      sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    } 
    for (auto & specificPlot : specific1DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots[name] = (TH1F*) data1.Get(name);
      m_plots[name]->Draw();
      sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    }
    for (auto & specificPlot : specific2DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots2[name] = (TH2F*) data1.Get(name);
      m_plots2[name]->Draw("colz");
      sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    }
    for (int j = 0; j<labelTags.size(); j++) {
      auto labelTag = labelTags.at(j);
    //for (const auto & labelTag : labelTags) {

      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + labelTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots[name] = (TH1F*) data1.Get(name);
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "res_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
       // if (specificPlot == "hChi2FW_" || specificPlot == "hChi2Emul_") gPad->SetLogy();
        gPad->SaveAs(name);
         if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + labelTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw("colz");
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      if (labelTag == "All" || labelTag == "Correlated") continue;
      for (int k = 0; k<slTags.size(); k++) {
        auto slTag = slTags.at(k);
    //  for (auto  slTag : slTags) {
        for (auto & specificPlot : moreSpecific1DPlots) {
          if (specificPlot == "hPsi_" || specificPlot == "hTime_" || specificPlot == "hPos_") continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          sprintf(name,"%s",nameHisto.c_str());
          m_plots[name] = (TH1F*) data1.Get(name);
          m_plots[name]->Draw();
          m_plots_res[specificPlot + "res_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetRMS(1));
          m_plots_mean[specificPlot + "mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetMean(1));
          sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      
        for (auto & specificPlot : moreSpecific2DPlots) {
          if (specificPlot == "hPsi2D_" || specificPlot == "hTime2D_" || specificPlot == "hPos2D_") continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          sprintf(name,"%s",nameHisto.c_str());
          m_plots2[name] = (TH2F*) data1.Get(name);
          m_plots2[name]->Draw("colz");
          sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      } // sl
    } // label
    //for (const auto & quTag : quTags) {
    for (int j = 0; j<quTags.size(); j++) {
      auto quTag = quTags.at(j);
      for (auto & specificPlot : moreSpecific1DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + quTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots[name] = (TH1F*) data1.Get(name);
        m_plots[name]->Draw();
        m_plots_res[specificPlot + "res_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "mean_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + quTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw("colz");
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
    } //qu
  } // chamber


  // PLOT RESOLS
 
  for (auto & chambTag : chambTags) {
    for (auto & specific1DPlot : moreSpecific1DPlots) {
      for (int i = 0; i<2; i++){
        std::string nameHisto = specific1DPlot + "res_" + chambTag + "_" + categories.at(i);
        m_plots_res[nameHisto]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      for (int i = 0; i<2; i++){
        std::string nameHisto = specific1DPlot + "mean_" + chambTag + "_" + categories.at(i);
        m_plots_mean[nameHisto]->Draw();
        sprintf(name,"run%s/%s.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
    }
  }



} // end macro
