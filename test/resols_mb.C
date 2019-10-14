//#include "tdrstyle.C"
#include <TLatex.h>


void resols_mb () {

  TLatex latex;
  latex.SetTextSize(0.03);


 // setTDRStyle(); 
  gStyle->SetOptStat(0);


  //TFile data1("resols_ptcut.root");
  //TFile data1("vlad_new_PU200_noaged_unique.root");
  TFile data1("resols_NicolaChange_all.root");
  TFile outFile("outPlots.root","RECREATE");
  //TFile data1("resol_vlad_t0.root");
  //

  std::vector <std::string> categories = {"hTanPsiRes","hPhiRes", "hTimeRes", "hxRes", "BX_P2" };
  std::vector <std::string> categoriesRes = {"TanPsiRes","PhiRes", "TimeRes", "xRes" };
  std::vector <std::string> stuffRes = {"Chamb","Wheel","Sec" };
  std::vector <std::string> chambTags = {"MB1","MB2","MB3","MB4"};
  std::vector <std::string> wheelTags = {"Wh-2", "Wh-1", "Wh0", "Wh+1","Wh+2"}; 
  std::vector<std::string> qualTags   = {"Correlated", "3h","4h"};
  std::vector<std::string> qualResTags   = {"Correlated", "Uncorrelated"};
  std::vector<std::string> qualNicolaTags   = {"Correlated","Legacy", "All"};
  std::vector<std::string> algoTags   = {"AM","HB"};
  //std::vector<std::string> qualTags   = {"All","Correlated", "Uncorrelated","3h","4h"};

  char name [128];
  bool print = false; 


  for (auto & category : categoriesRes) {
    for (auto & stuff : stuffRes) {
      for (auto & algoTag : algoTags) {
      data1.cd();
      sprintf(name,"%s_Res_per%s%s%s", category.c_str(),stuff.c_str(),algoTag.c_str(), qualResTags.at(0).c_str() ); 
      TH1F* E1 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_Res_per%s%s%s", category.c_str(),stuff.c_str(),algoTag.c_str(), qualResTags.at(1).c_str() ); 
      TH1F* E2 = (TH1F*) data1.Get(name); 
 
      E1->SetLineColor(kBlue);
      E2->SetLineColor(kRed);

      if (category == "TanPsiRes") E1->SetTitle("Local angle resolution");
      if (category == "PhiRes") E1->SetTitle("Global angle resolution");
      if (category == "TimeRes") E1->SetTitle("Local time resolution");
      if (category == "xRes") E1->SetTitle("Local position resolution");
      char name[128];
      sprintf(name,"%s_Res_per%s%s", category.c_str(),stuff.c_str(),algoTag.c_str()); 
      new TCanvas(name,name); 
      E1->Draw();
      E2->Draw("same");

      std::string nameFile; 

      double binx = E1->GetBinCenter (1) + E1->GetBinCenter (E1->GetNbinsX()) / 10; 
      double biny = E1->GetMaximum();

      TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
      sprintf(name,"%s", qualResTags.at(0).c_str()); 
      leg->AddEntry(E1,name,"l");
      sprintf(name,"%s", qualResTags.at(1).c_str()); 
      leg->AddEntry(E2,name,"l");
      leg->Draw();
      sprintf(name,"plots/%s_Res_per%s%s.png", category.c_str(),stuff.c_str(),algoTag.c_str()); 
      gPad->SaveAs(name);
      outFile.cd();
      gPad->Write();
      }
    } // chambTags
  } //categories 

  for (auto & category : categories) {
    for (auto & chambTag : chambTags) {
      for (auto & wheelTag : wheelTags) {
        for (auto & algoTag : algoTags) {

      data1.cd();
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualTags.at(0).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E1 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualTags.at(1).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E2 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualTags.at(2).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E3 = (TH1F*) data1.Get(name); 
      //sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualTags.at(3).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      //TH1F* E4 = (TH1F*) data1.Get(name); 
      //sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualTags.at(4).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      //TH1F* E5 = (TH1F*) data1.Get(name); 
 
     //cout << wheelTag << " " << chambTag << " " << qualTags.at(0) << " Entries:" << E1->GetEntries() << endl;     
     //cout << wheelTag << " " << chambTag << " " << qualTags.at(1) << " Entries:" << E2->GetEntries() << endl;     
     //cout << wheelTag << " " << chambTag << " " << qualTags.at(2) << " Entries:" << E3->GetEntries() << endl;     
     // cout << wheelTag << " " << chambTag << " " << qualTags.at(3) << " Entries:" << E4->GetEntries() << endl;     
     // cout << wheelTag << " " << chambTag << " " << qualTags.at(4) << " Entries:" << E5->GetEntries() << endl;     
 
      E1->SetLineColor(kBlue);
      E2->SetLineColor(kRed);
      E3->SetLineColor(kGreen);
      //E4->SetLineColor(kOrange);
      //E5->SetLineColor(kBlack);
 
      char name[128];
      new TCanvas(name,name); 
      sprintf(name,"%s_%s_%s_%s",algoTag.c_str(), category.c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      
      E1->Draw();
      E2->Draw("same");
      E3->Draw("same");
      //E4->Draw("same");
      //E5->Draw("same");

      std::string nameFile; 

      double binx = E1->GetBinCenter (1) + E1->GetBinCenter (E1->GetNbinsX()) / 10; 
      double biny = E1->GetMaximum();


      nameFile = qualTags.at(0) + ": "+std::to_string((int) E1->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,biny,name);
      nameFile = qualTags.at(1) + ": "+std::to_string((int) E2->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,0.7*biny,name);
      nameFile = qualTags.at(2) + ": "+std::to_string((int) E3->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,0.55*biny,name);


      TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
      sprintf(name,"%s", qualTags.at(0).c_str()); 
      leg->AddEntry(E1,name,"l");
      sprintf(name,"%s", qualTags.at(1).c_str()); 
      leg->AddEntry(E2,name,"l");
      sprintf(name,"%s", qualTags.at(2).c_str()); 
      leg->AddEntry(E3,name,"l");
      //sprintf(name,"%s", qualTags.at(3).c_str()); 
      //leg->AddEntry(E4,name,"l");
      //sprintf(name,"%s", qualTags.at(4).c_str()); 
      //leg->AddEntry(E5,name,"l");
      leg->Draw();
      sprintf(name,"./plots/%s_%s_%s_%s.png",algoTag.c_str(), category.c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      gPad->SetLogy();
      gPad->SaveAs(name);
      outFile.cd();
      gPad->Write();

        } //algoTags
      } //wheelTags
    } // chambTags
    print = false; 
  } //categories 


  for (auto & category : categories) {
    for (auto & chambTag : chambTags) {
      for (auto & wheelTag : wheelTags) {
        for (auto & algoTag : algoTags) {
      data1.cd();
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualNicolaTags.at(0).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E1 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualNicolaTags.at(1).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E2 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualNicolaTags.at(2).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E3 = (TH1F*) data1.Get(name);
      /*sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualNicolaTags.at(3).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E4 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_%s%s_%s_%s_P2", category.c_str(), algoTag.c_str(), qualNicolaTags.at(4).c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      TH1F* E5 = (TH1F*) data1.Get(name); 
 */
      //cout << wheelTag << " " << chambTag << " " << qualNicolaTags.at(0) << " Entries:" << E1->GetEntries() << endl;     
      //cout << wheelTag << " " << chambTag << " " << qualNicolaTags.at(1) << " Entries:" << E2->GetEntries() << endl;     
      //cout << wheelTag << " " << chambTag << " " << qualNicolaTags.at(2) << " Entries:" << E3->GetEntries() << endl;     
      int numEntries = E3->GetEntries();
      //E1->Scale(1./(double) numEntries); 
      //E2->Scale(1./(double) numEntries); 
      //E3->Scale(1./(double) numEntries); 
 
      E1->SetLineColor(kBlue);
      E2->SetLineColor(kRed);
      E3->SetLineColor(kGreen);
//      E4->SetLineColor(kOrange);
//      E5->SetLineColor(kBlack);
 
      char name[128];
      sprintf(name,"%s_%s_%s_%s_group",algoTag.c_str(), category.c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      new TCanvas(name,name); 
      E1->Draw("HIST");
      E2->Draw("HISTsame");
      E3->Draw("HISTsame");
//      E4->Draw("same");
//      E5->Draw("same");

      std::string nameFile; 

      double binx = E1->GetBinCenter (1) + E1->GetBinCenter (E1->GetNbinsX()) / 10; 
      double biny = E1->GetMaximum();

/*
      nameFile = qualTags.at(0) + ": "+std::to_string((int) E1->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,biny,name);
      nameFile = qualTags.at(1) + ": "+std::to_string((int) E2->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,0.7*biny,name);
      nameFile = qualTags.at(2) + ": "+std::to_string((int) E3->GetEntries());
      sprintf(name,"%s",nameFile.c_str());
      latex.DrawLatex(binx,0.55*biny,name);
*/

      TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
      sprintf(name,"%s", qualNicolaTags.at(0).c_str()); 
      leg->AddEntry(E1,name,"l");
      sprintf(name,"%s", qualNicolaTags.at(1).c_str()); 
      leg->AddEntry(E2,name,"l");
      sprintf(name,"%s", qualNicolaTags.at(2).c_str()); 
      leg->AddEntry(E3,name,"l");
/*      sprintf(name,"%s", qualNicolaTags.at(3).c_str()); 
      leg->AddEntry(E4,name,"l");
      sprintf(name,"%s", qualNicolaTags.at(4).c_str()); 
      leg->AddEntry(E5,name,"l");*/
      leg->Draw();
      sprintf(name,"./plots/%s_%s_%s_%s_group.png",algoTag.c_str(), category.c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      gPad->SetLogy();
      gPad->SaveAs(name);
      outFile.cd();
      gPad->Write();

        } //algoTags
      } //wheelTags
    } // chambTags
    print = false; 
  } //categories 


  for (auto & category : categoriesRes) {
    for (auto & stuff : stuffRes) {
      for (auto & algoTag : algoTags) {
      data1.cd();
      sprintf(name,"%s_Res_per%s%s%s", category.c_str(),stuff.c_str(),algoTag.c_str(), qualNicolaTags.at(0).c_str() ); 
      TH1F* E1 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_Res_per%s%s%s", category.c_str(),stuff.c_str(),algoTag.c_str(), qualNicolaTags.at(1).c_str() ); 
      TH1F* E2 = (TH1F*) data1.Get(name); 
      sprintf(name,"%s_Res_per%s%s%s", category.c_str(),stuff.c_str(),algoTag.c_str(), qualNicolaTags.at(2).c_str() ); 
      TH1F* E3 = (TH1F*) data1.Get(name); 
 
      E1->SetLineColor(kBlue);
      E1->SetMarkerStyle(20);
      E1->SetMarkerColor(kBlue);
      E2->SetLineColor(kRed);
      E2->SetMarkerStyle(22);
      E2->SetMarkerColor(kRed);
      E3->SetLineColor(kGreen);
      E3->SetMarkerStyle(23);
      E3->SetMarkerColor(kGreen);
 
      char name[128];
      sprintf(name,"plots/%s_Res_per%s%s_group", category.c_str(),stuff.c_str(),algoTag.c_str()); 
      new TCanvas(name,name); 
      
      if (category == "TanPsiRes") E1->SetTitle("Local angle resolution");
      if (category == "PhiRes") E1->SetTitle("Global angle resolution");
      if (category == "TimeRes") E1->SetTitle("Local time resolution");
      if (category == "xRes") E1->SetTitle("Local position resolution");
      E1->Draw("P");
      E2->Draw("Psame");
      E3->Draw("Psame");

      std::string nameFile; 

      double binx = E1->GetBinCenter (1) + E1->GetBinCenter (E1->GetNbinsX()) / 10; 
      double biny = E1->GetMaximum();

      TLegend *leg = new TLegend(0.6,0.6,0.80,0.8);
      sprintf(name,"%s", qualNicolaTags.at(0).c_str()); 
      leg->AddEntry(E1,name,"l");
      sprintf(name,"%s", qualNicolaTags.at(1).c_str()); 
      leg->AddEntry(E2,name,"l");
      sprintf(name,"%s", qualNicolaTags.at(2).c_str()); 
      leg->AddEntry(E3,name,"l");
      leg->Draw();
      sprintf(name,"plots/%s_Res_per%s%s_group.png", category.c_str(),stuff.c_str(),algoTag.c_str()); 
      gPad->SaveAs(name);
      outFile.cd();
      gPad->Write();
      }
    } // chambTags
  } //categories 
 











} // end macro
