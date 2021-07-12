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




void plotNew( ){

//  setTDRStyle();

TLatex latex;
latex.SetTextSize(0.03);


gStyle->SetPalette(1,0);
gStyle->SetOptFit(111111);
 gStyle->SetOptStat(111111);


  gSystem->Load("libRooFit");
  using namespace RooFit;
  
  std::vector<std::string> chambTags = { "MB1", "MB2", "MB3", "MB4"};
  std::vector<std::string> whTags    = { "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"};
  std::vector<std::string> secTags   = { "Sec1", "Sec2", "Sec3", "Sec4", "Sec5", "Sec6", "Sec7", "Sec8","Sec9","Sec10","Sec11","Sec12","Sec13","Sec14"};
  //std::vector<std::string> magnitudes = { "PhiRes","PhiBRes", "TanPsiRes", "xRes"};
  std::vector<std::string> magnitudes = { "TimeRes", "PhiRes","PhiBRes", "TanPsiRes", "xRes"};
  std::vector<std::string> algos      = { "AM", "HB" };
  std::vector<std::string> qualTags   = { "3h","4h"};
  //std::vector<std::string> qualTags   = { "All","Correlated"};
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "Legacy","Q9","Q8","Q6"};

  TFile *inFile = TFile::Open(("./results_run342221.root" ));
  //TFile *inFile = TFile::Open(("./results_run335119.root" ));
  //TFile *inFile = TFile::Open(("./results_run335005.root" ));
 
  TFile outPlots( "./outPlots.root","RECREATE");
  
  for (auto & chambTag: chambTags) {
       std::string namePlotAll    = "hFWSeg_" + chambTag;
       std::string namePlotQ     = "hFWSegQ>2_" + chambTag;
       std::string namePlotTM     = "hTMSeg_" + chambTag;
 
       TH1F *hResAll = (TH1F*)inFile->Get(namePlotAll.c_str());
       TH1F *hResQ  = (TH1F*)inFile->Get(namePlotQ.c_str());
       TH1F *hResTM  = (TH1F*)inFile->Get(namePlotTM.c_str());
       
       hResTM->Write(); 



       TCanvas *canvas10 = new TCanvas("c1","c1");

      // hResSlope4h1->Draw();


       TH1F* hResSth = new TH1F (("Plot_" + chambTag).c_str(), "Plot", 101,-50.5, 50.5);
       int binDespl = 0;

       for (int i = 0; i < 101; i++) {
         if (i + binDespl > 100) continue; 
         hResSth->SetBinContent(i+binDespl+1, hResAll->GetBinContent(i+1));
       }
       hResSth->Write(); 
       
       TH1F* hResSth2 = new TH1F (("Plot2_"  + chambTag).c_str(), "Plot2", 101,-50.5, 50.5);
       binDespl = 0;

       for (int i = 0; i < 101; i++) {
         if (i + binDespl > 100) continue; 
         hResSth2->SetBinContent(i+binDespl+1, hResQ->GetBinContent(i+1));
       }
       hResSth2->Write(); 
       
       
       TH2F*  hResSlope;
       hResSlope = (TH2F*)hResSth->Clone();


       RooRealVar x("x","", -5. , 5.);
       //RooRealVar x("x","", -50. , 0.);
       RooRealVar mean("mean",",mean of Gaussian",0,-25/2., 25/2.);
       //RooRealVar mean("mean",",mean of Gaussian",-12.5,-50/2, 0.);
       RooRealVar sigma1("sigma1",",width of narrow Gaussian",1,0, 50/2);
//       RooRealVar sigma2("sigma2",",width of wide Gaussian",1,0, 50/2);
       //RooRealVar sigma3("sigma3",",width of wide Gaussian",0.01,0,0.05);
       RooRealVar sigma3("sigma3",",width of wide Gaussian",100,100,200);
       RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);
  //     RooRealVar fraction2("fraction2",",fraction of narrow Gaussian",1./3.,0.,1.);

       RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
    //   RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);
       RooGaussian gauss3("gauss3","Wide Gaussian",x, mean, sigma3);

       //RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2,gauss3),RooArgList(fraction,fraction2));
       RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss3),fraction);

       RooDataHist data("data","data",x,hResSlope);
       //RooDataHist dataTM("dataTM","dataTM",x,hResTMSth);

       twogauss.fitTo(data,RooFit::Extended());

       /*
       double coreSigma;
       if (sigma1.getVal() > sigma2.getVal()) {
         if (sigma2.getVal() > sigma3.getVal()) coreSigma = sigma3.getVal();
	     else coreSigma = sigma2.getVal();
       } else if ( sigma1.getVal() > sigma3.getVal()) coreSigma = sigma3.getVal();	
       else coreSigma = sigma1.getVal();
 */
       
       RooRealVar xnew("x","", -25. , 25.);
       RooPlot* xframe=xnew.frame();
       data.plotOn(xframe);
       //dataTM.plotOn(xframe);
       twogauss.plotOn(xframe);
       twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
   //    twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
       twogauss.plotOn(xframe,Components("gauss3"),LineStyle(kDotted)); 

       char name[128];
       sprintf(name,"%s",namePlotAll.c_str());
       xframe->SetTitle("");
       //xframe->SetXTitle("Primitive - Segment #Psi (rad) ");
       xframe->SetYTitle("Events");
       gStyle->SetOptFit(1111);
       TCanvas *canvas1 = new TCanvas();
       xframe->GetYaxis()->SetRangeUser(1,3000);
       xframe->Draw();
       gPad->SetName("plot");
       gPad->SaveAs("plot.png");
       gPad->Write();
  }
  outPlots.Close();


  }
