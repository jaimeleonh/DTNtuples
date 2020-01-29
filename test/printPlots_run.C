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

  std::vector<std::string> chambTags = { "_MB2","_MB3", "_MB4"};
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

  std::vector <std::string> generalEffPlots {"hEffHWvsSegX", "hEffTMvsSegX", "hEffAMvsSegX","hEffHWvsSegXGoodBX", "hEffTMvsSegXGoodBX", "hEffAMvsSegXGoodBX"}; 
  //std::vector <std::string> generalEffPlots {"hEffHWvsSegX", "hEffTMvsSegX", "hEffAMvsSegX", "hEffCorAM", "hEffCor"}; 
  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM"}; 
//  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM", "hBXDif"}; 
  std::vector <std::string> specific1DPlots {"hSLHW", "hSLAM", "hPrimsSegs","hQualityHW", "hQualityAM", "hQualityTM", "hBXTM"};
  std::vector <std::string> specific2DPlots {"hPrimTypeVsPos","h2DHwQualSegNHits","h2DEmuQualSegNHits","h2DTMQualSegNHits","hQualityVsBXHW", "hQualityVsBXAM", "hHits"};
  //std::vector <std::string> moreSpecific1DPlots { };
  std::vector <std::string> moreSpecific1DPlots {"hBX","hBXDif","hBXEmul","hBXDifEmul", "hChi2FW","hChi2Emul","hPsiHW","hPsiEmul", "hPsi", "hTime","hPos", "hPsiSeg", "hTimeSeg","hPosSeg"};
  std::vector <std::string> moreSpecific2DPlots {"hPsi2D", "hTime2D","hPos2D","hPsi2DTM","hPos2DTM","hPhi2DTM","hPhiB2DTM","hPsi2DSeg", "hTime2DSeg","hPos2DSeg", "hTimeSegvsPos", "hTimeSegvsPsi"};

  std::vector <std::string> axisAndUnits {"BX (BX units)", "BX (BX units)","BX (BX units)", "BX (BX units)", "BX (BX units)", "FW chi2 (U.A)", "Emul chi2 (U.A)", "Firmware - Emulator Psi (#circ)", "Firmware - Emulator Time (ns)", "Firmware - Emulator Position (cm)", "Firmware Psi (#circ)","Emulator Psi (#circ)","Firmware - Segment Psi (#circ)", "Firmware - Segment Time (ns)", "Firmware - Segment Position (cm)" };

  std::map<std::string, TH1*> m_plots_res;
  std::map<std::string, TH1*> m_plots_mean;

  for (auto & chambTag : chambTags) {
    for (int j = 0; j < moreSpecific1DPlots.size(); j++){
      auto specific1DPlot = moreSpecific1DPlots.at(j);
   
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)).c_str(), 
					    "Resolutions; ; Resolution (a.u.) ",
					    5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)] = new TH1F((specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)).c_str(), 
					    "Means; ; Mean (a.u.) ",
					    5,-0.5,4.5); 
      for (int i = 0; i < 5; i++){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetXaxis()->SetBinLabel(i+1, labelTagsPlots[i].c_str());
      }
      
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_res_" +  chambTag + "_" + categories.at(1)).c_str(), 
					    "Resolutions; ; Resolution (a.u.) ",
					    5,-0.5,4.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_mean_" +  chambTag + "_" + categories.at(1)).c_str(), 
					    "Means; ; Mean (a.u.) ",
					    5,-0.5,4.5); 
      for (int i = 0; i < 5; i++){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      }

      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());

      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnits.at(j).c_str());

    }
  }


  for (auto & generalPlot : general1DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + generalPlot);
  }
  for (auto & generalPlot : generalEffPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + generalPlot);
  }
  for (auto & specificPlot : specific1DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
  }
  for (auto & specificPlot : specific2DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
  }
  for (auto & specificPlot : moreSpecific1DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
  }
  for (auto & specificPlot : moreSpecific2DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
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
    sprintf(name,"run%s/%s/%s.png",run.c_str(),generalPlot.c_str(),nameHisto.c_str());
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
      sprintf(name,"run%s/%s/%s.png",run.c_str(),generalPlot.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    } 
    for (auto & specificPlot : specific1DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots[name] = (TH1F*) data1.Get(name);
      m_plots[name]->Draw();
      sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    }
    for (auto & specificPlot : specific2DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots2[name] = (TH2F*) data1.Get(name);
      m_plots2[name]->Draw("colz");
      sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
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
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
       // if (specificPlot == "hChi2FW_" || specificPlot == "hChi2Emul") gPad->SetLogy();
        gPad->SaveAs(name);
         if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + labelTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw("colz");
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      if (labelTag == "All" || labelTag == "Correlated") continue;
      for (int k = 0; k<slTags.size(); k++) {
        auto slTag = slTags.at(k);
    //  for (auto  slTag : slTags) {
        for (auto & specificPlot : moreSpecific1DPlots) {
          if (specificPlot == "hPsi" || specificPlot == "hTime" || specificPlot == "hPos") continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          sprintf(name,"%s",nameHisto.c_str());
          m_plots[name] = (TH1F*) data1.Get(name);
          m_plots[name]->Draw();
          m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetRMS(1));
          m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(0)]->SetBinContent(3+1+k, m_plots[name]->GetMean(1));
          sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      
        for (auto & specificPlot : moreSpecific2DPlots) {
          if (specificPlot == "hPsi2D" || specificPlot == "hTime2D" || specificPlot == "hPos2D") continue;
          //if (specificPlot == "hPsi2DTM" ||specificPlot == "hPos2DTM" || specificPlot == "hPhi2DTM" ||specificPlot == "hPhiB2DTM" ) continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
          cout << nameHisto << endl; 
          sprintf(name,"%s",nameHisto.c_str());
          m_plots2[name] = (TH2F*) data1.Get(name);
          m_plots2[name]->Draw("colz");
          sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
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
        m_plots_res[specificPlot + "_res_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetRMS(1));
        m_plots_mean[specificPlot + "_mean_" + chambTag + "_" + categories.at(1)]->SetBinContent(j+1, m_plots[name]->GetMean(1));
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      for (auto & specificPlot : moreSpecific2DPlots) {
        std::string nameHisto = specificPlot + chambTag + "_" + quTag;
        sprintf(name,"%s",nameHisto.c_str());
        m_plots2[name] = (TH2F*) data1.Get(name);
        m_plots2[name]->Draw("colz");
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
    } //qu
  } // chamber


  // PLOT RESOLS
 
  for (auto & chambTag : chambTags) {
    for (auto & specific1DPlot : moreSpecific1DPlots) {
      for (int i = 0; i<2; i++){
        std::string nameHisto = specific1DPlot + "_res_" + chambTag + "_" + categories.at(i);
        m_plots_res[nameHisto]->Draw();
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specific1DPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
      for (int i = 0; i<2; i++){
        std::string nameHisto = specific1DPlot + "_mean_" + chambTag + "_" + categories.at(i);
        m_plots_mean[nameHisto]->Draw();
        sprintf(name,"run%s/%s/%s.png",run.c_str(),specific1DPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        if (fileOK) cout << nameHisto << ".png" << endl;
      }
    }
  }



} // end macro
