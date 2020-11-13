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

#include "PlotTemplate.C"

void plotAll( std::string file ){

//  setTDRStyle();

TLatex latex;
latex.SetTextSize(0.03);

TString Lumi = "35.9";

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
  //std::vector<std::string> qualTags   = { "3h","4h"};
  //std::vector<std::string> qualTags   = { "Correlated"};
  //std::vector<std::string> qualTags   = { "All"};
  std::vector<std::string> qualTags   = { "All","Correlated","Legacy"};
  //std::vector<std::string> qualTags   = { "Correlated", "Uncorrelated","3h","4h","All", "Legacy","Q9","Q8","Q6"};


  std::string outputPath = "/eos/home-j/jleonhol/ntuplesResults/";
  TFile *inFile = TFile::Open( (outputPath + "results_resols_" +file + "_.root").c_str() );
  gSystem->Exec("mkdir summaryPlots/"  );
  gSystem->Exec("mkdir summaryPlots/" + TString(file) );
  for (unsigned int i = 0; i < qualTags.size(); i++){
    TString quality = qualTags.at(i);
    gSystem->Exec("mkdir summaryPlots/" + TString(file) + "/" + quality);
  }
 
  TFile outPlots( "summaryPlots/" + TString (file)+ "/outPlots.root","RECREATE");
  
  std::map<std::string,std::string> xlabels;
  xlabels["TimeRes"] = "Primitive - Segment Time (ns)";
  xlabels["PhiRes"] = "Primitive - Segment Phi (rad)"; 
  xlabels["PhiBRes"] = "Primitive - Segment Phi_{B} (rad)";
  xlabels["TanPsiRes"] = "Primitive - Segment Local direction (rad)";
  xlabels["xRes"] = "Primitive - Segment Position (cm)";
  
  
  
  std::map<std::string,float> ranges;
  ranges["TimeResAll"  ] = 20;
  ranges["PhiResAll"   ] = 0.00015;
  ranges["PhiBResAll"   ] = 0.01;
  ranges["TanPsiResAll"] = 0.01;
  ranges["xResAll"     ] = 0.05;
  ranges["TimeResLegacy"  ] = 20;
  ranges["PhiResLegacy"   ] = 0.00015;
  ranges["PhiBResLegacy"   ] = 0.01;
  ranges["TanPsiResLegacy"] = 0.01;
  ranges["xResLegacy"     ] = 0.075;
  ranges["TimeResCorrelated"  ] = 20;
  ranges["PhiResCorrelated"   ] = 0.00015;
  ranges["PhiBResCorrelated"   ] = 0.005;
  ranges["TanPsiResCorrelated"] = 0.005;
  ranges["xResCorrelated"     ] = 0.15;
  //ranges["xResCorrelated"     ] = 0.05;
  ranges["TimeRes3h"  ] = 20;
  ranges["PhiRes3h"   ] = 0.0002;
  ranges["PhiBRes3h"   ] = 0.1;
  ranges["TanPsiRes3h"] = 0.1;
  ranges["xRes3h"     ] = 0.35;
  ranges["TimeRes4h"  ] = 20;
  ranges["PhiRes4h"   ] = 0.0002;
  ranges["PhiBRes4h"   ] = 0.02;
  ranges["TanPsiRes4h"] = 0.05;
  ranges["xRes4h"     ] = 0.05;

  
  std::map<std::string,float> yaxis;
  yaxis["TimeRes3h"  ] = 500;
  yaxis["PhiRes3h"   ] = 200;
  yaxis["PhiBRes3h"   ] = 75;
  yaxis["TanPsiRes3h"] = 75;
  yaxis["xRes3h"     ] = 75;
  yaxis["TimeRes4h"  ] = 4000;
  yaxis["PhiRes4h"   ] = 4000;
  yaxis["PhiBRes4h"   ] = 500;
  yaxis["TanPsiRes4h"] = 650;
  yaxis["xRes4h"     ] = 650;
  yaxis["TimeResCorrelated"  ] = 1200;
  //yaxis["TimeResCorrelated"  ] = 3000;
  yaxis["PhiResCorrelated"   ] = 1000;
  //yaxis["PhiResCorrelated"   ] = 3000;
  yaxis["PhiBResCorrelated"   ] = 1500;
  //yaxis["PhiBResCorrelated"   ] = 3000;
  yaxis["TanPsiResCorrelated"] = 1500;
  //yaxis["TanPsiResCorrelated"] = 4000;
  yaxis["xResCorrelated"     ] = 150;
  //yaxis["xResCorrelated"     ] = 500;




 //TFile outPlots = TFile::Open("./outPlots.root","RECREATE");
 
 char name [128];
 std::map<std::string, TH1*> m_plots;

 for (auto & qual : qualTags ) {
 for (auto & mag : magnitudes ) {
   for (unsigned int i = 0; i < whTags.size(); i++) {
     auto whTag = whTags.at(i);
     m_plots[mag + "AM" + qual + whTag] = new TH1F(("h" + mag + "_AM_" +  qual  + "_" + whTag).c_str(),
							   ( mag + " Seg-TP distribution for " + whTag +  "; ; sigma").c_str(),
							   4,0.5,4.5);
     
     for (unsigned int j = 0; j < chambTags.size(); j++) {
       auto chambTag = chambTags.at(j);
       m_plots[mag + "AM" + qual + whTag]->GetXaxis()->SetBinLabel(j+1, chambTag.c_str());



       string namePlot = "h" + mag + "_" + "AM" + qual + "_" + whTag + "_" + chambTag + "_P2";
       cout << namePlot << endl; 
       inFile->cd();
       TH1F *hResSlope4h1 = (TH1F*)inFile->Get(namePlot.c_str());


     //  TCanvas *canvas10 = new TCanvas(namePlot.c_str(),namePlot.c_str(), 800, 800);
       //TCanvas* canvas10 = CreateCanvas(namePlot.c_str(), false, false);

       //hResSlope4h1->Draw();
 
       TH2F*  hResSlope4h;
       hResSlope4h=(TH2F*)hResSlope4h1->Clone();
    // hResSlope4h->Draw();
       std::string nameFile; 

       RooRealVar x("x","",-ranges[mag+qual],ranges[mag+qual]);
       RooRealVar mean("mean",",mean of Gaussian",0.,-ranges[mag+qual]/2,ranges[mag+qual]/2);
       RooRealVar sigma1("sigma1",",width of narrow Gaussian",ranges[mag+qual]/20,0,ranges[mag+qual]);
       RooRealVar sigma2("sigma2",",width of wide Gaussian",ranges[mag+qual]/2,0,3*ranges[mag+qual]);
       //RooRealVar sigma3("sigma3",",width of wide Gaussian",0.01,0,0.05);
       RooRealVar sigma3("sigma3",",width of wide Gaussian",100,100,200);
       RooRealVar fraction("fraction",",fraction of narrow Gaussian",2./3.,0.,1.);
       RooRealVar fraction2("fraction2",",fraction of narrow Gaussian",1./3.,0.,1.);

       RooGaussian gauss1("gauss1","Narrow Gaussian",x, mean, sigma1);
       RooGaussian gauss2("gauss2","Wide Gaussian",x, mean, sigma2);
       RooGaussian gauss3("gauss3","Wide Gaussian",x, mean, sigma3);

       RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss2,gauss3),RooArgList(fraction,fraction2));
       //RooAddPdf twogauss("twogauss","Two Gaussians pdf",RooArgList(gauss1,gauss3),fraction);

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

       TCanvas* canvas1 = CreateCanvas(namePlot.c_str(), false, false);
       RooPlot* xframe=x.frame();
       data.plotOn(xframe);
       twogauss.plotOn(xframe);
       //twogauss.plotOn(xframe,Components("gauss1"),LineColor(kRed));
       //twogauss.plotOn(xframe,Components("gauss2"),LineStyle(kDashed)); 
       //twogauss.plotOn(xframe,Components("gauss3"),LineStyle(kDotted)); 

       sprintf(name,"%s",namePlot.c_str());
       //xframe->SetTitle(name);
       xframe->SetTitle("");
       //xframe->SetXTitle("Primitive - Segment #Psi (rad) ");
       xframe->SetYTitle("Events");
       xframe->SetXTitle(xlabels[mag].c_str());
       xframe->GetXaxis()->SetTitleOffset(1.2);
       xframe->GetYaxis()->SetTitleOffset(1.6);
       gStyle->SetOptFit(1111);
       //TCanvas *canvas1 = new TCanvas();
       //xframe->GetYaxis()->SetRangeUser(1,yaxis[mag+qual]);
       //xframe->GetYaxis()->SetRangeUser(1,30000); // Better when logy
       xframe->Draw();
       //DrawPrelimLabel(canvas1);
       //DrawLumiLabel(canvas1, Lumi);

      TLatex tex;
      tex.SetTextSize(0.03);
      tex.DrawLatexNDC(0.76,0.87, (whTag + " " + chambTag).c_str());//typically for Phase-2
      tex.Draw("same");
  
  //double xPosition = 0.;
       double xPosition = ranges[mag+qual] / 10.;
       //double xPosition = hResSlope4h1->GetXaxis()->GetXmax() / 10.;

/*
       nameFile = "Fraction = "+std::to_string(fraction.getVal());
       sprintf(name,"%s",nameFile.c_str());
       //latex.DrawLatex(xPosition,5000,name);
       nameFile = "Fraction2 = "+std::to_string(fraction2.getVal());
       sprintf(name,"%s",nameFile.c_str());
       //latex.DrawLatex(xPosition,7500,name);
       if (mag == "PhiRes" || mag == "PhiBRes" || mag == "TanPsiRes") { 
         nameFile = "Mean = "+std::to_string(mean.getVal()*1000);
         sprintf(name,"%s mrad",nameFile.c_str());
         //latex.DrawLatex(xPosition,3000,name); 
         nameFile = "#sigma_{1} = "+std::to_string(sigma1.getVal()*1000. );
         sprintf(name,"%s mrad",nameFile.c_str());
         //latex.DrawLatex(xPosition,2000,name);
         nameFile = "#sigma_{2} = "+std::to_string(sigma2.getVal()*1000.);
         sprintf(name,"%s mrad",nameFile.c_str());
         latex.DrawLatex(xPosition,1200,name);
         nameFile = "#sigma_{3} = "+std::to_string(sigma3.getVal()*1000.);
         sprintf(name,"%s mrad",nameFile.c_str());
         //latex.DrawLatex(xPosition,800,name);
       } else if (mag == "TimeRes") {
         nameFile = "Mean = "+std::to_string(mean.getVal());
         sprintf(name,"%s ns",nameFile.c_str());
         latex.DrawLatex(xPosition,3000,name); 
         nameFile = "#sigma_{1} = "+std::to_string(sigma1.getVal() );
         sprintf(name,"%s ns",nameFile.c_str());
         latex.DrawLatex(xPosition,2000,name);
         nameFile = "#sigma_{2} = "+std::to_string(sigma2.getVal());
         sprintf(name,"%s ns",nameFile.c_str());
         latex.DrawLatex(xPosition,1200,name);
         nameFile = "#sigma_{3} = "+std::to_string(sigma3.getVal());
         sprintf(name,"%s ns",nameFile.c_str());
         latex.DrawLatex(xPosition,800,name);
       } else {
         nameFile = "Mean = "+std::to_string(mean.getVal());
         sprintf(name,"%s cm",nameFile.c_str());
         latex.DrawLatex(xPosition,3000,name); 
         nameFile = "#sigma_{1} = "+std::to_string(sigma1.getVal() );
         sprintf(name,"%s cm",nameFile.c_str());
         latex.DrawLatex(xPosition,2000,name);
         nameFile = "#sigma_{2} = "+std::to_string(sigma2.getVal());
         sprintf(name,"%s cm",nameFile.c_str());
         latex.DrawLatex(xPosition,1200,name);
         nameFile = "#sigma_{3} = "+std::to_string(sigma3.getVal());
         sprintf(name,"%s cm",nameFile.c_str());
         latex.DrawLatex(xPosition,800,name);

       }
*/
       sprintf(name,"summaryPlots/%s/%s/%s.png",file.c_str(), qual.c_str(), namePlot.c_str());
       //canvas1->SetLogy();
       outPlots.cd();
       gPad->SetName(namePlot.c_str());
       gPad->SaveAs(name);
       gPad->Write();
       sprintf(name,"summaryPlots/%s/%s/%s.pdf",file.c_str(), qual.c_str(), namePlot.c_str());
       //canvas1->SetLogy();
       outPlots.cd();
       gPad->SetName(namePlot.c_str());
       gPad->SaveAs(name);



    }
    outPlots.cd();
    m_plots[mag + "AM" + qual + whTag] -> Write();
    sprintf(name,"summaryPlots/%s/%s/h%s_AM_%s_%s.png",file.c_str(),qual.c_str(),mag.c_str(), qual.c_str(), whTag.c_str());
    TCanvas *canvas1 = new TCanvas();
    m_plots[mag + "AM" + qual + whTag] -> Draw();
    canvas1 -> SaveAs(name);
    sprintf(name,"summaryPlots/%s/%s/h%s_AM_%s_%s.pdf",file.c_str(),qual.c_str(),mag.c_str(), qual.c_str(), whTag.c_str());
    canvas1 -> SaveAs(name);
  }
} //magnitudes
} //qualTags

outPlots.Close();


}
