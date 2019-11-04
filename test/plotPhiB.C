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


#include "tdrstyle.C"


void plotPhiB(){

//  setTDRStyle();

TLatex latex;
latex.SetTextSize(0.03);


gStyle->SetPalette(1,0);
gStyle->SetOptFit(111111);
 gStyle->SetOptStat(111111);


  gSystem->Load("libRooFit");
  using namespace RooFit;

  //TFile *fileSL1 = TFile::Open("./segment_vs_jm_tanPhi_gauss/segment_vs_jm_tanPhi_gauss_qu_9.root ");

  // TH1F *hResSlope4h = (TH1F*)fileSL1->Get("segment_vs_jm_tanPhi_gauss_Wh-2_St1_Se1_Qu9");
  // TH1F *hResSlope3h = (TH1F*)fileSL1->Get("segment_vs_jm_tanPhi_gauss_Wh-2_St1_Se1_Qu9");

  // TFile *file = TFile::Open("segment_vs_jm_x_gauss/segment_vs_jm_x_gauss_qu_9.root ");

  // TH1F *hResPos4h = (TH1F*)file->Get("segment_vs_jm_x_gauss_Wh-2_St1_Se1_Qu9");
  // TH1F *hResPos3h = (TH1F*)file->Get("segment_vs_jm_x_gauss_Wh-2_St1_Se1_Qu9");

  TF1 *g1 = new TF1 ("m1", "gaus", -0.03, 0.03);
  g1->SetParameters(1000,0,0.004);
  g1->SetLineColor(kRed);
  


 
 


 
 char name [128];

 for (int i = 4; i<=4; i++) {
  std::string nameFile = "MB"+std::to_string(i);


string namePlot = "hPhiBRes_AMAll_Sec10_P2";
//string namePlot = "hPhiBRes_AMAll_Wh0_MB1_P2";

// sprintf(name,"hSegLocalAMDPhiC%s",nameFile.c_str());
// TFile *fileSL1 = TFile::Open("./eff_vlad.root ");
 //TFile *fileSL1 = TFile::Open("./resols_NOPU_good.root ");
 //TFile *fileSL1 = TFile::Open("./resols_NicolaChange_all.root ");
 TFile *fileSL1 = TFile::Open("./results_vlad_QualityFilter_index0.root");
 TH1F *hResSlope4h1 = (TH1F*)fileSL1->Get(namePlot.c_str());
 //TH1F *hResSlope4h1 = (TH1F*)fileSL1->Get("hTanPsiRes_AMCorrelated_Wh0_MB2_P2");
 //TH1F *hResSlope4h1 = (TH1F*)fileSL1->Get("hSegLocalAMDPhiCMB1");

 //TH1F *hResSlope4h1 = (TH1F*)fileSL1->Get(name);

 



 
// TFile *fileSL12 = TFile::Open("./segment_vs_jm_tanPhi_gauss/segment_vs_jm_tanPhi_gauss_qu_9.root ");

// TH1F *hResSlope4h2 = (TH1F*)fileSL12->Get("segment_vs_jm_tanPhi_gauss_Wh-2_St1_Se1_Qu9");


TCanvas *canvas10 = new TCanvas();

 hResSlope4h1->Draw();
// hResSlope4h2->Draw();

 TH2F*  hResSlope4h;
 hResSlope4h=(TH2F*)hResSlope4h1->Clone();
// hResSlope4h-> Add(hResSlope4h2,1);
 hResSlope4h->Draw();
 //hResSlope4h->GetYaxis()->SetRangeUser(0,1400);
 //hResSlope4h->GetYaxis()->SetRangeUser(-0.04,0.04);

//gStyle->SetOptFit(1111);

//RooRealVar x("x","",-0.15,0.15);
RooRealVar x("x","",-0.01,0.01);
//RooRealVar x("x","",-0.06,0.06);
RooRealVar mean("mean",",mean of Gaussian",0.,-0.05,0.05);
RooRealVar sigma1("sigma1",",width of narrow Gaussian",0.001,0,0.05);
RooRealVar sigma2("sigma2",",width of wide Gaussian",0.005,0,0.05);
//RooRealVar sigma3("sigma3",",width of wide Gaussian",100,100,200);
RooRealVar sigma3("sigma3",",width of wide Gaussian",0.01,0,0.05);
RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);
RooRealVar fraction2("fraction2",",fraction of narrow Gaussian",1./3.,0.,1.);

RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);
RooGaussian gauss3("gauss3","Wide Gaussian",x, mean, sigma3);

RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2,gauss3),RooArgList(fraction,fraction2));
//RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2),fraction);

RooDataHist data("data","data",x,hResSlope4h);

twogauss.fitTo(data,RooFit::Extended());

RooPlot* xframe=x.frame();
//xframe->GetXaxis()->SetRangeUser(-0.04,0.04);
data.plotOn(xframe);
twogauss.plotOn(xframe);
twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
twogauss.plotOn(xframe,Components("gauss3"),LineStyle(kDotted)); 

sprintf(name,"%s",namePlot.c_str());
xframe->SetTitle(name);
//xframe->SetTitle(" MB1 - Correlated Only");
//xframe->SetTitle("Segment vs AM Trigger #Delta #Psi MB1 Only correlated MP");
xframe->SetXTitle("Primitive - Segment #Psi (rad) ");
xframe->SetYTitle("Events");
gStyle->SetOptFit(1111);
TCanvas *canvas1 = new TCanvas();
xframe->GetYaxis()->SetRangeUser(1,30000);
xframe->Draw();




//latex.DrawLatex(.002,1000,"Fraction = 0.81");
//latex.DrawLatex(.001,5000,"#sigma_{1} = (4.15 #pm 0.01) #times 10^{-4} rad");  
//latex.DrawLatex(.002,500,"#sigma_{2} = 2.98#times 10^{-3}");
//latex.DrawLatex(.001,2000,"Mean = (-1.60 #pm 0.14) #times 10^{-6} rad");

/*latex.DrawLatex(.02,4500,"Fraction = 0.96");
latex.DrawLatex(.02,3000,"#sigma_{1} = 1.8#times 10^{-3}");  
latex.DrawLatex(.02,2000,"#sigma_{2} = 0.1");
latex.DrawLatex(.02,1200,"Mean = -1.2#times 10^{-5}");
*/

//latex.DrawLatex(.02,6000,"Entries = 26701");

nameFile = "Fraction = "+std::to_string(fraction.getVal());
sprintf(name,"%s",nameFile.c_str());
latex.DrawLatex(0.004,5000,name);
nameFile = "Fraction2 = "+std::to_string(fraction2.getVal());
sprintf(name,"%s",nameFile.c_str());
latex.DrawLatex(0.004,7500,name);
nameFile = "Mean = "+std::to_string(mean.getVal()*1000);
sprintf(name,"%s mrad",nameFile.c_str());
latex.DrawLatex(0.004,3000,name);  
nameFile = "#sigma_{1} = "+std::to_string(sigma1.getVal()*1000. );
sprintf(name,"%s mrad",nameFile.c_str());
latex.DrawLatex(0.004,2000,name);
nameFile = "#sigma_{2} = "+std::to_string(sigma2.getVal()*1000.);
sprintf(name,"%s mrad",nameFile.c_str());
latex.DrawLatex(0.004,1200,name);
nameFile = "#sigma_{3} = "+std::to_string(sigma3.getVal()*1000.);
sprintf(name,"%s mrad",nameFile.c_str());
latex.DrawLatex(0.004,800,name);

 //gPad->SaveAs("MB1_slope.png");

//sprintf(name,"./export/%s_slope_log.png",nameFile.c_str());
sprintf(name,"%s_3gauss_index0.png", namePlot.c_str());
//sprintf(name,"%s_2gaussconst_index0.png", namePlot.c_str());
 canvas1->SetLogy();
 gPad->SaveAs(name);





}


}
