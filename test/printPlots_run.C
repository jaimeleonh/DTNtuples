//#include "tdrstyle.C"
#include "TH1.h"
#include "TH2.h"
#include <TStyle.h>
#include <TCanvas.h>
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TEfficiency.h"

/*
double getMeanEfficiency ( std::string effPlot, TString file ) {
  TFile data1(file);
  cout << file << endl;
  char name[128];
  sprintf(name,"%s",effPlot.c_str());
  TEfficiency *plot = (TEfficiency*) data1.Get(name);
  cout << effPlot << endl;

  for (unsigned int i = 1; i<=plot->GetPaintedGraph()->GetXaxis()->GetNbins(); i++){
    cout << plot->GetPaintedGraph()->GetXaxis()->GetBinCenter(i); 
  }
}
*/

double getMeanEfficiency(TH1 *plot1, TH1 *plot2, double initialVal, double finalVal){
  double sumPassed = 0.;
  double sumTotal  = 0.;
  for (unsigned int i = 1; i<=plot1->GetXaxis()->GetNbins(); i++){
     if ( plot1->GetXaxis()->GetBinCenter(i) < initialVal || plot1->GetXaxis()->GetBinCenter(i) > finalVal  ) continue; 
     sumPassed = sumPassed + plot1->GetBinContent(i); 
     sumTotal  = sumTotal  + plot2->GetBinContent(i); 
  }
  
  return sumPassed/sumTotal;
}

void printPlots_run(std::string run) {

  const bool fileOK = false; 

  //setTDRStyle(); 
  //gStyle->SetOptStat(0);
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
  std::vector<std::string> quTagsSegs = {"3h","4h","Q6","Q8","Q9","bestQ"};
  std::vector<std::string> labelTags = {"All", "Correlated", "Uncorrelated"};
  std::vector<std::string> labelTagsPlots = {"All", "Correlated", "Uncorrelated", "UncorrelatedSL1", "UncorrelatedSL3"};
  
  std::map<std::string, TH1*> m_plots;
  std::map<std::string, TH2*> m_plots2;
  std::map<std::string, TEfficiency*> m_effs;

  char name [128];

  std::vector <std::string> categories {"perGroup","perQuality"}; 

  std::vector <std::string> generalEffPlots {"hEffHW", "hEffTM", "hEffAM"}; 
  //std::vector <std::string> generalEffPlots {"hEffHWvsSegX", "hEffTMvsSegX", "hEffAMvsSegX","hEffHWvsSegXGoodBX", "hEffTMvsSegXGoodBX", "hEffAMvsSegXGoodBX"}; 
  std::vector <std::string> effvsWhat = {"vsSegX","vsSegT0","vsph2SegX","vsph2SegT0"};  
  std::vector <std::string> effWhichBX = {"","GoodBX"};  
  
  
  
  
  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM", "Offset"}; 
//  std::vector <std::string> general1DPlots {"hQualityHW", "hQualityAM", "hBXDif"}; 
  std::vector <std::string> specific1DPlots {"hSLHW", "hSLAM", "hPrimsSegs","hQualityHW", "hQualityAM", "hQualityTM", "hBXTM", "hLatenciesHW"};
  std::vector <std::string> specific2DPlots {"hPrimTypeVsPos","h2DHwQualSegNHits","h2DEmuQualSegNHits","h2DTMQualSegNHits","hQualityVsBXHW", "hQualityVsBXAM", "hHits","hHitsVsLatenciesHW", "hQualityVsLatenciesHW", "hPositionVsHitsHW"};
  //std::vector <std::string> moreSpecific1DPlots { };
  std::vector <std::string> moreSpecific1DPlots {"hBX","hBXDif","hBXEmul","hBXDifEmul", "hChi2FW","hChi2Emul","hPsiHW","hPsiEmul", "hPsi", "hTime","hPos"};
  std::vector <std::string> moreSpecific1DPlotsSegs {"hPsiSeg", "hTimeSeg","hPosSeg","hPsiph2Seg", "hTimeph2Seg","hPosph2Seg"};
  std::vector <std::string> moreSpecific2DPlots {"hPsi2D", "hTime2D","hPos2D","hPsi2DTM","hPos2DTM","hPhi2DTM","hPhiB2DTM"};
  std::vector <std::string> moreSpecific2DPlotsSegs {"hPsi2DSeg", "hTime2DSeg","hPos2DSeg", "hTimeSegvsPos", "hTimeSegvsPsi","hTimeSegvsSegZ","hPsi2Dph2Seg", "hTime2Dph2Seg","hPos2Dph2Seg", "hTimeph2SegvsPos", "hTimeph2SegvsPsi", "hTimeph2Segvsph2SegZ"};

  std::vector <std::string> axisAndUnits {"BX (BX units)", "BX (BX units)","BX (BX units)", "BX (BX units)", "FW chi2 (U.A)", "Emul chi2 (U.A)", "HW Psi (#circ)", "Emulator Psi (#circ)","Firmware - Emulator Psi (#circ)", "Firmware - Emulator Time (ns)", "Firmware - Emulator Position (cm)"};
  std::vector <std::string> axisAndUnitsSegs {"Firmware - Segment Psi (#circ)", "Firmware - Segment Time (ns)", "Firmware - Segment Position (cm)", "Firmware - ph2Segment Psi (#circ)", "Firmware - ph2Segment Time (ns)", "Firmware - ph2Segment Position (cm)" };

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
  for (auto & chambTag : chambTags) {
    for (int j = 0; j < moreSpecific1DPlotsSegs.size(); j++){
      auto specific1DPlot = moreSpecific1DPlotsSegs.at(j);
   
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
					    6,-0.5,5.5); 
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)] = new TH1F((specific1DPlot + "_mean_" +  chambTag + "_" + categories.at(1)).c_str(), 
					    "Means; ; Mean (a.u.) ",
					    6,-0.5,5.5); 
      for (int i = 0; i < 6; i++){
        m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
        m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetXaxis()->SetBinLabel(i+1, quTags[i].c_str());
      }

      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      m_plots_res[specific1DPlot + "_res_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());

      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(0)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());
      m_plots_mean[specific1DPlot + "_mean_" + chambTag + "_" + categories.at(1)]->GetYaxis()->SetTitle(axisAndUnitsSegs.at(j).c_str());

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
  for (auto & specificPlot : moreSpecific1DPlotsSegs) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
  }
  for (auto & specificPlot : moreSpecific2DPlots) {
    gSystem->Exec("mkdir run" + runNumber + "/" + specificPlot);
  }
  for (auto & specificPlot : moreSpecific2DPlotsSegs) {
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
 
  //std::vector <std::string> effCats = {"","Q>2"};  
  std::vector <std::string> effCats = {"","Corr","Q>2"};  
  std::map <std::string, std::string> effLeg;
  effLeg[""] = "Every Quality"; 
  effLeg["Corr"] = "Correlated Only"; 
  effLeg["Q>2"] = "Quality > 2"; 
  effLeg["hEffHWvsSegX"] = "All BX"; 
  effLeg["hEffHWvsSegXGoodBX"] = "Good BX"; 
  effLeg["hEffHWvsSegT0"] = "All BX"; 
  effLeg["hEffHWvsSegT0GoodBX"] = "Good BX"; 
  effLeg["hEffHWvsph2SegX"] = "All BX"; 
  effLeg["hEffHWvsph2SegXGoodBX"] = "Good BX"; 
  effLeg["hEffHWvsph2SegT0"] = "All BX"; 
  effLeg["hEffHWvsph2SegT0GoodBX"] = "Good BX"; 
  effLeg["hEffHW"] = "HW Q>2"; 
  effLeg["hEffTM"] = "TM"; 
  
  
  std::map <std::string, std::string> effHWCatsTitles; 
  effHWCatsTitles["hEffHWvsSegX"] = "HW Eff in All BX vs Seg X; Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsSegXGoodBX"] = "HW Eff in Good BX vs Seg X; Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsSegXCombi"] = "HW Eff All Q vs Seg Position; Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsSegT0"] = "HW Eff in All BX vs Seg t0; Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsSegT0GoodBX"] = "HW Eff in Good BX vs Seg t0; Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsSegT0Combi"] = "HW Eff All Q vs Seg t0; Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegX"] = "HW Eff in All BX vs ph2Seg X; ph2Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegXGoodBX"] = "HW Eff in Good BX vs ph2Seg X; ph2Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegXCombi"] = "HW Eff All Q vs ph2Seg position; ph2Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegT0"] = "HW Eff in All BX vs ph2Seg t0; ph2Segment position (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegT0GoodBX"] = "HW Eff in Good BX vs ph2Seg t0; ph2Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffHWvsph2SegT0Combi"] = "HW Eff All Q vs ph2Seg t0; ph2Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsSegX"] = "Eff in All BX vs Seg X; Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsSegXGoodBX"] = "Eff in Good BX vs Seg X; Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsSegT0"] = "Eff in All BX vs Seg t0; Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsSegT0GoodBX"] = "Eff in Good BX vs Seg t0; Segment t0 (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsph2SegX"] = "Eff in All BX vs ph2Seg X; ph2Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsph2SegXGoodBX"] = "Eff in Good BX vs ph2Seg X; ph2Segment position (cm); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsph2SegT0"] = "Eff in All BX vs ph2Seg t0; ph2Segment position (ns); Efficiency (adim)"; 
  effHWCatsTitles["hEffvsph2SegT0GoodBX"] = "Eff in Good BX vs ph2Seg t0; ph2Segment t0 (ns); Efficiency (adim)"; 

  double mean;  
  char meanStr[40];
  double limitInfPos = -100.;
  double limitSupPos = 100.;
  double limitInfTime = 0.;
  double limitSupTime = 50.;


  for (int i = 0; i<chambTags.size(); i++) {
    auto chambTag = chambTags.at(i);

    for (auto & what : effvsWhat) { 
      if (chambTag == "_MB2") continue; 
      char namePassed[128]; 
      char nameTotal[128]; 
      std::string cat = effCats[0];
      
      std::string HWCat = "hEffHW" + what + effWhichBX[0]; // All BX 
      TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
      std::string nameHisto = HWCat + cat + chambTag;
      sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
      sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
      m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
      m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
      m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
      m_effs[name]->SetLineColor(2);
      if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
      else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
      sprintf(meanStr,"%.2f", mean);
      leg->AddEntry(m_effs[name], (effLeg[HWCat] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      m_effs[name]->SetTitle(( effHWCatsTitles[HWCat + what + "Combi"]).c_str());
      m_effs[name]->Draw();
      gPad->Update();
      auto graph =  m_effs[name]->GetPaintedGraph(); 
      graph->SetMinimum(0);
      graph->SetMaximum(1.2);
      gPad->Update();

      HWCat = "hEffHW" + what + effWhichBX[1]; // Good BX
      nameHisto = HWCat + cat + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
      sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
      m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
      m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
      m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
      m_effs[name]->SetTitle(( effHWCatsTitles[HWCat + what + "Combi"]).c_str());
      m_effs[name]->SetLineColor(3);
      if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
      else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
      mean = ( (int) round(mean * 100) ) / 100.;
      sprintf(meanStr,"%.2f", mean);
      leg->AddEntry(m_effs[name], (effLeg[HWCat] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      m_effs[name]->SetTitle(( effHWCatsTitles[HWCat + what + "Combi"]).c_str());
      m_effs[name]->Draw("same");

      nameHisto = "hEffHW" + what + chambTag;       
      leg->Draw();
      sprintf(name,"run%s/hEffHW/%s_AllvsGood.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
    } 


    for (auto & what : effvsWhat) { 
      if (chambTag == "_MB2") continue; 
      char namePassed[128]; 
      char nameTotal[128]; 
      TEfficiency* pEff = 0;
      
      for (auto & HWCat : effWhichBX){
      if (true) {      
      TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
      std::string cat = effCats[0];
      std::string nameHisto = "hEffHW" + what + HWCat + cat + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
      sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
      m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
      m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
      m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
      m_effs[name]->SetLineColor(2);
      if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
      else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
      sprintf(meanStr,"%.2f", mean);
      leg->AddEntry(m_effs[name], (effLeg[cat] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      m_effs[name]->SetTitle(effHWCatsTitles["hEffHW"+what+HWCat].c_str());
      m_effs[name]->Draw();
      gPad->Update();
      auto graph =  m_effs[name]->GetPaintedGraph(); 
      graph->SetMinimum(0);
      graph->SetMaximum(1.2);
      gPad->Update();

      cat = effCats[1];
      nameHisto = "hEffHW" + what + HWCat + cat + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
      sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
      m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
      m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
      m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
      m_effs[name]->SetLineColor(3);
      m_effs[name]->SetTitle(effHWCatsTitles[HWCat].c_str());
      m_effs[name]->SetTitle(effHWCatsTitles["hEffHW"+what+HWCat].c_str());
      if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime);
      else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
      sprintf(meanStr,"%.2f", mean);
      leg->AddEntry(m_effs[name], (effLeg[cat] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      m_effs[name]->Draw("same");

      cat = effCats[2];
      nameHisto = "hEffHW" + what + HWCat + cat + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
      sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
      m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
      m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
      m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
      m_effs[name]->SetTitle(effHWCatsTitles["hEffHW"+what+HWCat].c_str());
      m_effs[name]->SetLineColor(4);
      if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
      else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
      sprintf(meanStr,"%.2f", mean);
      leg->AddEntry(m_effs[name], (effLeg[cat] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      m_effs[name]->Draw("same");

      nameHisto = "hEffHW" + what + HWCat + chambTag;
      leg->Draw();
      sprintf(name,"run%s/hEffHW/%s_combined.png",run.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (fileOK) cout << nameHisto << ".png" << endl;
      }
      
      if (true) {
        TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
        std::string cat = "Q>2";
        std::string nameHisto = "hEffHW" + what + HWCat + cat + chambTag;
        sprintf(name,"%s",nameHisto.c_str());
        sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
        sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
        m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
        m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
        m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
        m_effs[name]->SetLineColor(2);
        
        if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
        else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
        sprintf(meanStr,"%.2f", mean);
        leg->AddEntry(m_effs[name], (effLeg["hEffHW"] + " (" + std::string(meanStr) + ")").c_str(),"l" );
      
        m_effs[name]->SetTitle(effHWCatsTitles["hEff" + what + HWCat].c_str());
        m_effs[name]->Draw();
        gPad->Update();
        auto graph =  m_effs[name]->GetPaintedGraph(); 
        graph->SetMinimum(0);
        graph->SetMaximum(1.2);
        gPad->Update();


        
        
        cat = "";
        nameHisto = "hEffTM" + what + HWCat + cat + chambTag;
        sprintf(name,"%s",nameHisto.c_str());
        sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
        sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
        m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
        m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
        m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
        m_effs[name]->SetTitle(effHWCatsTitles["hEff" + what + HWCat].c_str());
        m_effs[name]->SetLineColor(3);
        if      (what == "vsSegT0" || what == "vsph2SegT0") mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfTime, limitSupTime );
        else if (what == "vsSegX"  || what == "vsph2SegX" ) mean = getMeanEfficiency ( m_plots[namePassed], m_plots[nameTotal], limitInfPos, limitSupPos );
        sprintf(meanStr,"%.2f", mean);
        leg->AddEntry(m_effs[name], (effLeg["hEffTM"] + " (" + std::string(meanStr) + ")").c_str(),"l" );
        //leg->AddEntry(m_effs[name], (effLeg["hEffTM"] + " (" + mean + ")") .c_str(),"l" );
        m_effs[name]->Draw("same");
        
        nameHisto = "hEff" + what + HWCat + chambTag;
        leg->Draw();
        sprintf(name,"run%s/hEffHW/%s_HWTM.png",run.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);

      }

      }
    } 



    for (auto & generalPlot : generalEffPlots) {
      if (chambTag == "_MB2") continue; 
      char namePassed[128]; 
      char nameTotal[128]; 
      for (auto & what : effvsWhat){ 
        for (auto & whichBX : effWhichBX){ 
          std::string nameHisto = generalPlot + what + whichBX + chambTag;
          sprintf(name,"%s",nameHisto.c_str());
          sprintf(namePassed,"%s",(nameHisto+"passed").c_str());
          sprintf(nameTotal,"%s",(nameHisto+"total").c_str());
          m_plots[namePassed] = (TH1F*) data1.Get(namePassed);
          m_plots[nameTotal] = (TH1F*) data1.Get(nameTotal);
          m_effs[name] = new TEfficiency( *m_plots[namePassed], *m_plots[nameTotal]);
          m_effs[name]->Draw();
          gPad->Update();
          auto graph =  m_effs[name]->GetPaintedGraph(); 
          graph->SetMinimum(0);
          graph->SetMaximum(1.2);
          gPad->Update();
          sprintf(name,"run%s/%s/%s.png",run.c_str(),generalPlot.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
      }
    }



    for (auto & specificPlot : specific1DPlots) {
      std::string nameHisto = specificPlot + chambTag;
      sprintf(name,"%s",nameHisto.c_str());
      m_plots[name] = (TH1F*) data1.Get(name);
      m_plots[name]->Draw();
      sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
      gPad->SaveAs(name);
      if (specificPlot == "hLatenciesHW") {
        gPad->SetLogy();
        sprintf(name,"run%s/%s/%s_log.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
        gPad->SaveAs(name);
        gPad->SetLogy(0);
      }
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
      for (auto & specificPlot : moreSpecific1DPlotsSegs) {
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
      
      for (auto & specificPlot : moreSpecific2DPlotsSegs) {
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
          sprintf(name,"%s",nameHisto.c_str());
          m_plots2[name] = (TH2F*) data1.Get(name);
          m_plots2[name]->Draw("colz");
          sprintf(name,"run%s/%s/%s.png",run.c_str(),specificPlot.c_str(),nameHisto.c_str());
          gPad->SaveAs(name);
          if (fileOK) cout << nameHisto << ".png" << endl;
        }
        for (auto & specificPlot : moreSpecific1DPlotsSegs) {
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
      
        for (auto & specificPlot : moreSpecific2DPlotsSegs) {
          if (specificPlot == "hPsi2D" || specificPlot == "hTime2D" || specificPlot == "hPos2D") continue;
          //if (specificPlot == "hPsi2DTM" ||specificPlot == "hPos2DTM" || specificPlot == "hPhi2DTM" ||specificPlot == "hPhiB2DTM" ) continue;
          std::string nameHisto = specificPlot + chambTag + "_" + labelTag + "_" + slTag;
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
    }
    for (int j = 0; j<quTagsSegs.size(); j++) {
      auto quTag = quTagsSegs.at(j);
      for (auto & specificPlot : moreSpecific1DPlotsSegs) {
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
      
      for (auto & specificPlot : moreSpecific2DPlotsSegs) {
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
    for (auto & specific1DPlot : moreSpecific1DPlotsSegs) {
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
