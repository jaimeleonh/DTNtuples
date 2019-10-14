//#include "tdrstyle.C"
#include <TLatex.h>


void deltas () {

  TLatex latex;
  latex.SetTextSize(0.03);


  gStyle->SetOptStat(0);


  TFile data1("resols.root");
  TFile data2("resols_nopu.root");
  
  int a = 0; 

  if (a==0) {

TCanvas *canvas10 = new TCanvas();
  TH1F* E1 = (TH1F*) data1.Get("hPhiGenSeg_Wh-2_MB1"); 
  TH1F* E2 = (TH1F*) data1.Get("hPhiGenSeg_Wh-1_MB1"); 
  TH1F* E3 = (TH1F*) data1.Get("hPhiGenSeg_Wh0_MB1"); 
  TH1F* E4 = (TH1F*) data1.Get("hPhiGenSeg_Wh+1_MB1"); 
  TH1F* E5 = (TH1F*) data1.Get("hPhiGenSeg_Wh+2_MB1");
/*
  E1->Scale(1./ (double) E1->GetEntries());
  E2->Scale(1./ (double) E2->GetEntries());
  E3->Scale(1./ (double) E3->GetEntries());
  E4->Scale(1./ (double) E4->GetEntries());
  E5->Scale(1./ (double) E5->GetEntries());
*/

  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  E3->SetLineColor(kGreen);
  E4->SetLineColor(kOrange);
  E5->SetLineColor(kBlack);
  
  E1->SetTitle("DeltaPhi MB1 PU 200"); 
  E1->Draw(); 
  //E2->Draw("same"); 
  E3->Draw("same"); 
  //E4->Draw("same"); 
  //E5->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"Wh-2","l");
  leg->AddEntry(E2,"Wh-1","l");
  leg->AddEntry(E3,"Wh0","l");
  leg->AddEntry(E4,"Wh+1","l");
  leg->AddEntry(E5,"Wh+2","l");
  leg->Draw(); 

  gPad->SetLogy();
  gPad->SaveAs("deltaPhi.png"); 


  }

  if (a==0) {

TCanvas *canvas10 = new TCanvas();
  TH1F* E1 = (TH1F*) data1.Get("hPhiGenSeg_Wh-2_MB1"); 
  TH1F* E2 = (TH1F*) data2.Get("hPhiGenSeg_Wh-2_MB1"); 

  E1->SetTitle("DeltaPhi MB1 Wh-2"); 
 

 
  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  
  E1->SetTitle("DeltaPhi MB1 Wh-2"); 
  E1->Draw(); 
  E2->Draw("same"); 
  //E2->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"PU200","l");
  leg->AddEntry(E2,"PU0","l");
  leg->Draw(); 


  gPad->SaveAs("deltaPhi_both.png"); 


  }




  if (a==0) {
TCanvas *canvas10 = new TCanvas();
  TH1F* E1 = (TH1F*) data1.Get("hEtaGenSeg_Wh-2_MB1"); 
  TH1F* E2 = (TH1F*) data2.Get("hEtaGenSeg_Wh-2_MB1"); 

  E1->SetTitle("DeltaEta MB1 Wh-2"); 
  //E1->Scale(1./ (double) E1->GetEntries());
 

 
  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  
  E1->SetTitle("DeltaEta MB1 Wh-2"); 
  E1->Draw(); 
  E2->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"PU200","l");
  leg->AddEntry(E2,"PU0","l");
  leg->Draw(); 


 // gPad->SetLogy();
  gPad->SaveAs("deltaEta_both.png"); 


  }
  if (a==0) {
TCanvas *canvas10 = new TCanvas();

  TH1F* E1 = (TH1F*) data1.Get("hPhiGenSeg_Wh-2_MB1"); 

  E1->SetTitle("DeltaPhi MB1 Wh-2 PU200"); 
  //E1->Scale(1./ (double) E1->GetEntries());
 

  /*
  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  
  E1->SetTitle("DeltaPhi MB1 Wh-2"); 
  E1->Draw(); 
  E2->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"PU200","l");
  leg->AddEntry(E2,"PU0","l");
  leg->Draw(); 


  gPad->SetLogy();
  */

  //E1->Divide(E2);
  E1->Draw();

  gPad->SaveAs("deltaPhi_PU200.png"); 


  }
  if (a==0) {
  TCanvas *canvas10 = new TCanvas();

  TH1F* E1 = (TH1F*) data2.Get("hPhiGenSeg_Wh-2_MB1"); 

  E1->SetTitle("DeltaPhi MB1 Wh-2 NOPU"); 
  //E1->Scale(1./ (double) E1->GetEntries());
 

  /*
  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  
  E1->SetTitle("DeltaPhi MB1 Wh-2"); 
  E1->Draw(); 
  E2->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"PU200","l");
  leg->AddEntry(E2,"PU0","l");
  leg->Draw(); 


  gPad->SetLogy();
  */

  //E1->Divide(E2);
  E1->Draw();

  gPad->SaveAs("deltaPhi_PU0.png"); 
  }


  if (true) {

TCanvas *canvas10 = new TCanvas();
  TH1F* E1 = (TH1F*) data1.Get("hPhiGenSeg_Wh-2_MB1"); 
  TH1F* E2 = (TH1F*) data2.Get("hPhiGenSeg_Wh-2_MB1"); 

  E1->SetTitle("DeltaPhi Ratio MB1 Wh-2 "); 
  //E1->Scale(1./ (double) E1->GetEntries());
  //E2->Scale(1./ (double) E2->GetEntries());
 

  /*
  E1->SetLineColor(kBlue);
  E2->SetLineColor(kRed);
  
  E1->SetTitle("DeltaPhi MB1 Wh-2"); 
  E1->Draw(); 
  E2->Draw("same"); 

  TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
  leg->AddEntry(E1,"PU200","l");
  leg->AddEntry(E2,"PU0","l");
  leg->Draw(); 


  gPad->SetLogy();
  */

  E1->Divide(E2);
  E1->Draw();

  gPad->SaveAs("deltaPhi_divide.png"); 


  }
  

}
// end macro
