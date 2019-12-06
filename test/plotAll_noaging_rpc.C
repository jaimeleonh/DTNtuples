#include <fstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "TROOT.h"
#include "TFile.h"
#include "TGraph.h"
#include "TPostScript.h"
#include "TLine.h"
#include "TText.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TProfile.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TCanvas.h"


#include <TLatex.h>


//#include "tdrstyle.C"


void plotAll_noaging_rpc(){

//  setTDRStyle();

TLatex latex;
latex.SetTextSize(0.03);

//TFile *inFile = TFile::Open("./results_nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000.root");
TFile *inFile = TFile::Open("./results_nu_pu250_noage_withrpc.root");
inFile->cd();

gStyle->SetPalette(1,0);
gStyle->SetOptFit(111111);
 gStyle->SetOptStat(111111);


  gSystem->Load("libRooFit");
  using namespace RooFit;
  
  std::vector<std::string> algoTags = { "AM","HB"};
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12"};

  gSystem->Exec("mkdir ratePlots/ratioPlots/");
 
 

 //TFile outPlots = TFile::Open("./outPlots.root","RECREATE");
 TFile outPlots("outPlots_ratios.root","RECREATE");

 
 char name [128];
 std::map<std::string, TH1*> m_plots;

 for (auto & algo : algoTags ) {
   for (auto & chambTag : chambTags ) {
     for (auto & wheelTag : whTags ) {
       m_plots["ratePrims" + algo + wheelTag + chambTag] = new TH1F(("ratePrims_" + algo + "_" + wheelTag + "_" + chambTag).c_str(),
 							(algo + " Primitive rate ratio in " + wheelTag + " " + chambTag  + "; ; Rate").c_str(),
							5, 0.5, 5.5);
       long counter[5] = {0,0,0,0,0};
       long totalCounter = 0;


       for (auto & secTag : secTags ) {

          m_plots["ratePrims" + algo + wheelTag + secTag + chambTag] = new TH1F(("ratePrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag).c_str(),
									(algo + " Primitive rate ratio in " + wheelTag + " " +  secTag + " " + chambTag  + "; ; Rate").c_str(),
									5, 0.5, 5.5);
       string namePlot = "ratePrims_" + algo + "_" + wheelTag + "_" + secTag + "_" + chambTag;
       cout << namePlot << endl; 
       //TFile *inFile = TFile::Open("./results_nu_pu250_noage_norpc.root");
       TH1F *hResSlope4h1 = (TH1F*)inFile->Get(namePlot.c_str());

       m_plots["ratePrims" + algo + wheelTag + chambTag]->GetXaxis()->SetBinLabel(1,hResSlope4h1->GetXaxis()->GetBinLabel(3));
       m_plots["ratePrims" + algo + wheelTag + chambTag]->GetXaxis()->SetBinLabel(2,hResSlope4h1->GetXaxis()->GetBinLabel(4));
       m_plots["ratePrims" + algo + wheelTag + chambTag]->GetXaxis()->SetBinLabel(3,hResSlope4h1->GetXaxis()->GetBinLabel(5));
       m_plots["ratePrims" + algo + wheelTag + chambTag]->GetXaxis()->SetBinLabel(4,hResSlope4h1->GetXaxis()->GetBinLabel(6));
       m_plots["ratePrims" + algo + wheelTag + chambTag]->GetXaxis()->SetBinLabel(5,hResSlope4h1->GetXaxis()->GetBinLabel(8));
       
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(1,hResSlope4h1->GetXaxis()->GetBinLabel(3));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(2,hResSlope4h1->GetXaxis()->GetBinLabel(4));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(3,hResSlope4h1->GetXaxis()->GetBinLabel(5));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(4,hResSlope4h1->GetXaxis()->GetBinLabel(6));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->GetXaxis()->SetBinLabel(5,hResSlope4h1->GetXaxis()->GetBinLabel(8));

       counter[0]+=hResSlope4h1->GetBinContent(3);
       counter[1]+=hResSlope4h1->GetBinContent(4);
       counter[2]+=hResSlope4h1->GetBinContent(5);
       counter[3]+=hResSlope4h1->GetBinContent(6);
       counter[4]+=hResSlope4h1->GetBinContent(8);

       totalCounter+=hResSlope4h1->GetBinContent(2);


       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->SetBinContent(1, (double) hResSlope4h1->GetBinContent(3) / (double) hResSlope4h1->GetBinContent(2));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->SetBinContent(2, (double) hResSlope4h1->GetBinContent(4) / (double) hResSlope4h1->GetBinContent(2));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->SetBinContent(3, (double) hResSlope4h1->GetBinContent(5) / (double) hResSlope4h1->GetBinContent(2));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->SetBinContent(4, (double) hResSlope4h1->GetBinContent(6) / (double) hResSlope4h1->GetBinContent(2));
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->SetBinContent(5, (double) hResSlope4h1->GetBinContent(8) / (double) hResSlope4h1->GetBinContent(2));

       TCanvas *canvas10 = new TCanvas(namePlot.c_str(),namePlot.c_str());
       outPlots.cd();
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->Draw();
       m_plots["ratePrims" + algo + wheelTag + secTag + chambTag]->Write();
       sprintf(name,"ratePlots/ratioPlots/%s.png", namePlot.c_str());
       //canvas1->SetLogy();
       //outPlots.cd();
       gPad->SetName(namePlot.c_str());
       //gPad->SaveAs(name);
       //canvas10->Write();
       //gPad->Write();
       sprintf(name,"ratePlots/ratioPlots/%s.pdf", namePlot.c_str());
       //gPad->SaveAs(name);
      }

      for (int i=0; i<5; i++){
         m_plots["ratePrims" + algo + wheelTag + chambTag]->SetBinContent(i+1, (double) counter[i] / (double)  totalCounter );
//         TCanvas *canvas10 = new TCanvas(namePlot.c_str(),namePlot.c_str());
      }
      outPlots.cd();
      m_plots["ratePrims" + algo + wheelTag + chambTag]->Draw();
      m_plots["ratePrims" + algo + wheelTag + chambTag]->Write();
     

    }
  }
} //qualTags

outPlots.Close();


}
