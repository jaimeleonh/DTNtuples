//#include "tdrstyle.C"


void resols_mb () {

 // setTDRStyle(); 
  gStyle->SetOptStat(0);


  TFile data1("resols_NOPU_new.root");
  //TFile data1("resol_vlad_t0.root");
  //TFile data1("results_new_index.root");
  //

  std::vector <std::string> categories = {"hTanPsiRes","hPhiRes", "hTimeRes", "hxRes", "BX_P2" };
  std::vector <std::string> chambTags = {"MB1","MB2","MB3","MB4"};
  std::vector <std::string> wheelTags = {"Wh.-2", "Wh.-1", "Wh.0", "Wh.+1","Wh.+2"}; 
  std::vector<std::string> qualTags   = {"Correlated", "3h","4h"};
  std::vector<std::string> algoTags   = {"AM","HB"};
  //std::vector<std::string> qualTags   = {"All","Correlated", "Uncorrelated","3h","4h"};

  char name [128];
  bool print = false; 

  for (auto & category : categories) {
    for (auto & chambTag : chambTags) {
      for (auto & wheelTag : wheelTags) {
        for (auto & algoTag : algoTags) {

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
 
      cout << wheelTag << " " << chambTag << " " << qualTags.at(0) << " Entries:" << E1->GetEntries() << endl;     
      cout << wheelTag << " " << chambTag << " " << qualTags.at(1) << " Entries:" << E2->GetEntries() << endl;     
      cout << wheelTag << " " << chambTag << " " << qualTags.at(2) << " Entries:" << E3->GetEntries() << endl;     
     // cout << wheelTag << " " << chambTag << " " << qualTags.at(3) << " Entries:" << E4->GetEntries() << endl;     
     // cout << wheelTag << " " << chambTag << " " << qualTags.at(4) << " Entries:" << E5->GetEntries() << endl;     
 
      E1->SetLineColor(kBlue);
      E2->SetLineColor(kRed);
      E3->SetLineColor(kGreen);
      //E4->SetLineColor(kOrange);
      //E5->SetLineColor(kBlack);
 
      E1->Draw();
      E2->Draw("same");
      E3->Draw("same");
      //E4->Draw("same");
      //E5->Draw("same");

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
      sprintf(name,"./plotsPU0/%s_%s_%s_%s.png",algoTag.c_str(), category.c_str(), wheelTag.c_str(), chambTag.c_str() ); 
      gPad->SetLogy();
      gPad->SaveAs(name);

        } //algoTags
      } //wheelTags
    } // chambTags
    print = false; 
  } //categories 




 











} // end macro
