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


void plotAll(){

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
  std::vector<std::string> qualTags   = { "All" };
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "Legacy","Q9","Q8","Q6"};

 
  std::map<std::string,float> ranges;
  ranges["TimeRes"  ] = 10;
  ranges["PhiRes"   ] = 0.0005;
  ranges["PhiBRes"   ] = 0.05;
  ranges["TanPsiRes"] = 0.02;
  ranges["xRes"     ] = 0.2;
 

 //TFile outPlots = TFile::Open("./outPlots.root","RECREATE");
 TFile outPlots("outPlots.root","RECREATE");

 
 char name [128];
 std::map<std::string, TH1*> m_plots;

 for (auto & qual : qualTags ) {
 for (auto & mag : magnitudes ) {
   for (unsigned int i = 0; i < whTags.size(); i++) {
     auto whTag = whTags.at(i);
     m_plots[mag + "AM" + qual + whTag] = new TH1F(("h" + mag + "_AM_" +  qual  + "_" + whTag).c_str(),
							   ( mag + " Seg-TP distribution for " + whTag +  "; ; sigma").c_str(),
							   4,0,4);
     
     for (unsigned int j = 0; j < chambTags.size(); j++) {
       auto chambTag = chambTags.at(j);



       string namePlot = "h" + mag + "_" + "AM" + qual + "_" + whTag + "_" + chambTag + "_P2";
       cout << namePlot << endl; 
       //string namePlot = "hPhiBRes_AMAll_Sec10_P2";
       TFile *inFile = TFile::Open("./results_vlad_QualityFilter_index0.root");
       inFile->cd();
       TH1F *hResSlope4h1 = (TH1F*)inFile->Get(namePlot.c_str());


       TCanvas *canvas10 = new TCanvas(namePlot.c_str(),namePlot.c_str());

       hResSlope4h1->Draw();
 
       TH2F*  hResSlope4h;
       hResSlope4h=(TH2F*)hResSlope4h1->Clone();
    // hResSlope4h->Draw();
       std::string nameFile; 

       RooRealVar x("x","",-ranges[mag],ranges[mag]);
       RooRealVar mean("mean",",mean of Gaussian",0.,-ranges[mag]/2,ranges[mag]/2);
       RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag]/20,0,ranges[mag]);
       RooRealVar sigma2("sigma2",",width of wide Gaussian",ranges[mag]/2,0,3*ranges[mag]);
       //RooRealVar sigma3("sigma3",",width of wide Gaussian",0.01,0,0.05);
       RooRealVar sigma3("sigma3",",width of wide Gaussian",100,100,200);
       RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);
       RooRealVar fraction2("fraction2",",fraction of narrow Gaussian",1./3.,0.,1.);

       RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
       RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);
       RooGaussian gauss3("gauss3","Wide Gaussian",x, mean, sigma3);

       RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2,gauss3),RooArgList(fraction,fraction2));
     //RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2),fraction);

       RooDataHist data("data","data",x,hResSlope4h);

       twogauss.fitTo(data,RooFit::Extended());

       double coreSigma;
       if (sigma1.getVal() > sigma2.getVal()) {
         if (sigma2.getVal() > sigma3.getVal()) coreSigma = sigma3.getVal();
	 else coreSigma = sigma2.getVal();
       } else if ( sigma1.getVal() > sigma3.getVal()) coreSigma = sigma3.getVal();	
       else coreSigma = sigma1.getVal();
 
       if (mag == "xRes" || mag == "TimeRes") m_plots[mag + "AM" + qual + whTag]->SetBinContent(j+1,coreSigma);
       else m_plots[mag + "AM" + qual + whTag]->SetBinContent(j+1,coreSigma*1000);

       RooPlot* xframe=x.frame();
       data.plotOn(xframe);
       twogauss.plotOn(xframe);
       twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
       twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
       twogauss.plotOn(xframe,Components("gauss3"),LineStyle(kDotted)); 

       sprintf(name,"%s",namePlot.c_str());
       xframe->SetTitle(name);
       //xframe->SetXTitle("Primitive - Segment #Psi (rad) ");
       xframe->SetYTitle("Events");
       gStyle->SetOptFit(1111);
       TCanvas *canvas1 = new TCanvas();
       xframe->GetYaxis()->SetRangeUser(1,30000);
       xframe->Draw();


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

       //sprintf(name,"%s.png", namePlot.c_str());
       canvas1->SetLogy();
       outPlots.cd();
       //canvas10->Write();
       gPad->SetName(namePlot.c_str());
       gPad->Write();
       //gPad->SaveAs(name);



    }
    outPlots.cd();
    m_plots[mag + "AM" + qual + whTag] -> Write();
  }
} //magnitudes
} //qualTags

outPlots.Close();


}
