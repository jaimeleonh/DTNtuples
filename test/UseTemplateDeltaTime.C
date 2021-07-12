// Example usage of the plotting template (originally used for TRG-17-001)
// Author: O. Davignon (CERN)
#include "PlotTemplate.C"

void UseTemplateDeltaTime()
{
  // Example of how to use PlotTemplate.C

  // Modify the present script, then run using:
  // root -l
  // [0] .L ExampleUsage.C++
  // [1] ExampleUsage()

  gStyle->SetOptStat(000000);

  TString PlotName = "DeltaTime.pdf";
  bool LogY = false;
  bool DisplayGrid = false;
  TString Lumi = "35.9";

  TCanvas* myCanvas = CreateCanvas(PlotName, LogY, DisplayGrid);

  //here add the histos, functions, etc. you want to draw (drawing happens below)

  double minimumX = -25.;
  double maximumX = 25.;
  double minimumY = 0.0001;
  double maximumY = 1;

 TFile *file1 = TFile::Open("./outPlots.root");
 TH1F *myFunction = (TH1F*)file1->Get("Plot");
 TH1F *myFunction2 = (TH1F*)file1->Get("hTMSeg");

  // follows are two dummy examples of a function / a histo
 //sil  TF1* myFunction = new TF1("myFunction","x *100",minimumX,maximumX);
  myFunction->SetTitle("");

  // TH1F* myFunction = new TH1F("myFunction","myFuncion",100,minimumX,maximumX);
  // for(Int_t i = 1 ; i <= myFunction->GetNbinsX() ; ++i) myFunction->SetBinContent(i,100+float(i));
  // myFunction->SetTitle("");

  // when you read file(s) where histos / graphs are saved
  // TFile myFileContainingHistos("filename.root","READ");
  // myFunction = (TH1F*)myFileContainingHistos.Get("histoName");

  // follows are examples on how to format legends, axis, etc.

  //axis labels ranges & names
  if(LogY && minimumY==0.) cout<<"****** LogY == true and minimumY == 0, you probably want to avoid that ******"<<endl;
  myFunction->SetMinimum(minimumY);
  myFunction->SetMaximum(maximumY);
  myFunction->GetXaxis()->SetRangeUser(minimumX,maximumX);
  myFunction->GetYaxis()->SetTitle("");
  myFunction->GetXaxis()->SetTitle("Primitive-Segment Time (ns)");
  myFunction->GetXaxis()->SetTitleOffset(1.2);
  myFunction->GetYaxis()->SetTitleOffset(1.4);

  //line style  
  myFunction->SetLineColor(kBlue);
  myFunction->SetLineWidth(3);
  myFunction2->SetLineColor(kRed);
  myFunction2->SetLineWidth(3);

  //drawing and scaling
  
  int numEntries1 = 0;
  int max1 = 0;
  int numEntries2 = 0;
  int max2 = 0;

  for (int i = 0; i < myFunction->GetXaxis()->GetNbins(); i++) {
    numEntries1 = numEntries1 + myFunction->GetBinContent(i+1);
    if (myFunction->GetBinContent(i+1) > max1) max1 = myFunction->GetBinContent(i+1);
  }
  for (int i = 0; i < myFunction2->GetXaxis()->GetNbins(); i++) {
    numEntries2 = numEntries2 + myFunction2->GetBinContent(i+1);
    if (myFunction2->GetBinContent(i+1) > max2) max2 = myFunction2->GetBinContent(i+1);
  }
  
  //myFunction->Scale(1. / max1);
  myFunction->Scale(1. / numEntries1);
  //myFunction2->Scale(1. / max2);
  myFunction2->Scale(1. / numEntries2);
  
  myFunction->Draw("H");
//  myFunction2->Draw("Hsame");
  //myFunction->Draw("Psame");

  //legend
  TLegend* leg = new TLegend(0.6,0.48,0.88,0.6);
  //leg->SetBorderSize(0);
  //leg->SetFillStyle(0);
  leg->SetTextSize(0.025);
  //leg->SetHeader("here is a beautiful header");
  leg->AddEntry(myFunction,"Phase-2 prim. ");
  leg->AddEntry((TObject*)0,"Core #sigma=2.9ns","");
//  leg->AddEntry(myFunction2,"Twinmux prim.");
  leg->Draw("same");

  //line
  // double max_line = 115.;
  // TLine* a_line = new TLine(0.,max_line,1.,max_line);
  // a_line->SetLineColor(kRed);
  // a_line->SetLineWidth(3.);
  // a_line->SetLineStyle(2);
  // a_line->Draw("same");

  DrawPrelimLabel(myCanvas);
  DrawLumiLabel(myCanvas, Lumi);
  SaveCanvas(myCanvas, PlotName);

  return;
}
