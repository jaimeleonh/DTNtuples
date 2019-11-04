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

//#include "tdrstyle.C"

//#include "setTDRStyle"

void plotxSL1_prueba(){

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

  


 
 


 
 char name [128];

 for (int i = 1; i<=1; i++) {
  std::string nameFile = "MB"+std::to_string(i);

  std::string namePlot = "hxRes_AM4h_Sec11_P2";

 TFile *fileSL1 = TFile::Open("./resols_PU200_all.root ");
 TH1F *hResSlope4h1 = (TH1F*)fileSL1->Get(namePlot.c_str());

 



 
// TFile *fileSL12 = TFile::Open("./segment_vs_jm_tanPhi_gauss/segment_vs_jm_tanPhi_gauss_qu_9.root ");

// TH1F *hResSlope4h2 = (TH1F*)fileSL12->Get("segment_vs_jm_tanPhi_gauss_Wh-2_St1_Se1_Qu9");


TCanvas *canvas10 = new TCanvas();

 hResSlope4h1->Draw();
// hResSlope4h2->Draw();

 TH2F*  hResSlope4h;
 hResSlope4h=(TH2F*)hResSlope4h1->Clone();
// hResSlope4h-> Add(hResSlope4h2,1);
 hResSlope4h->Draw();
 hResSlope4h->GetYaxis()->SetRangeUser(0,1400);
 //hResSlope4h->GetYaxis()->SetRangeUser(0, 30000);

//gStyle->SetOptFit(1111);

RooRealVar x("x","",-0.5,0.5);
RooRealVar mean("mean",",mean of Gaussian",0.,-0.1,0.1);
RooRealVar sigma1("sigma1",",width of narrow Gaussian",0.008,0,0.2);
RooRealVar sigma2("sigma2",",width of wide Gaussian",0.030,0,0.2);
RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);

RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);

RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2),fraction);

RooDataHist data("data","data",x,hResSlope4h);

twogauss.fitTo(data,RooFit::Extended());

RooPlot* xframe=x.frame();
data.plotOn(xframe);
twogauss.plotOn(xframe);
twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 

sprintf(name,"%s",namePlot.c_str());
//sprintf(name,"Position resolution - Wh+2 MB1 Correlated Only",nameFile.c_str());
xframe->SetTitle(name);
//xframe->SetTitle(" MB1 - Correlated Only");
//xframe->SetTitle("Segment vs AM Trigger #Delta x MB1 Only correlated MP");
xframe->SetXTitle("Primitive - Segment Position (cm) ");
xframe->SetYTitle("Events");
gStyle->SetOptFit(1111);
TCanvas *canvas1 = new TCanvas();
xframe->GetYaxis()->SetRangeUser(1,30000);
xframe->Draw();


nameFile = "Fraction = "+std::to_string(fraction.getVal());
sprintf(name,"%s",nameFile.c_str());
latex.DrawLatex(.07,5000,name);
nameFile = "Mean = "+std::to_string(mean.getVal());
sprintf(name,"%s cm",nameFile.c_str());
latex.DrawLatex(.07,3000,name);  
nameFile = "#sigma_{1} = "+std::to_string(sigma1.getVal());
sprintf(name,"%s cm",nameFile.c_str());
latex.DrawLatex(.07,2000,name);
nameFile = "#sigma_{2} = "+std::to_string(sigma2.getVal());
sprintf(name,"%s cm",nameFile.c_str());
latex.DrawLatex(.07,1200,name);

/*latex.DrawLatex(.02,4500,"Fraction = 0.96");
latex.DrawLatex(.02,3000,"#sigma_{1} = 1.8#times 10^{-3}");  
latex.DrawLatex(.02,2000,"#sigma_{2} = 0.1");
latex.DrawLatex(.02,1200,"Mean = -1.2#times 10^{-5}");
*/

//latex.DrawLatex(.02,6000,"Entries = 26701");

nameFile = "MB"+std::to_string(i);
sprintf(name,"%s.png",namePlot.c_str());
 //gPad->SaveAs(name);
 //gPad->SaveAs("MB1_slope.png");

//sprintf(name,"./export/%s_x_log.png",nameFile.c_str());
 canvas1->SetLogy();
// gPad->SaveAs(name);





}


}
