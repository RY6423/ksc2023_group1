/*
  test.C
*/
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TROOT.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

double count(TH1F*);

void analize(string file_ini, int number){
  //書き込み用ファイル
  TFile* result = new TFile((file_ini+"_analize.root").c_str(), "recreate");
  TTree* tree = new TTree("eventtree", "EventTree");

  const int ni=10000;
  Double_t x[ni];
  Double_t y[ni];
  
  int n;
  double photon_n;
  tree->Branch("number",&n);
  tree->Branch("photon",&photon_n);
  //データファイルオープン
  for(int i=0;i<number;i++) {
    std::string file;
    if(number==1) {
      file = "../data/"+file_ini+".root";
    } else {
      std::string s_i = std::to_string(i);
      s_i = std::string(std::max(0, 4 - (int)s_i.size()), '0') + s_i;
      file = "../data/" + file_ini + "/" + file_ini + "_" + s_i + ".root";
    }
    TFile* tf = new TFile(file.c_str());
    TH1F* th1 = (TH1F*)tf->Get("ADC_HIGH_0");
    if(!th1) continue;
    n=i;
    photon_n=count(th1);
    tree->Fill();
    
    x[i] = (Double_t) i;
    y[i] =            photon_n;

  }

  TGraph* tg1 = new TGraph(number, x, y) ;
  tg1-> SetMarkerStyle(20);
  tg1->Draw("ap");
  result->cd();
  tg1->Write();
  result->Write();
  result->Close();
}

double count(TH1F* t){
  double a=74.1558,b=836.127;
  const int n=40;
  double area[n];
  for(int i=0;i<n;i++) {
    area[i] = t->Integral(t->FindBin(b+a*(i-0.5)), t->FindBin(b+a*(i+0.5)), "width");
  }

  //for(int i=0;i<n;i++) std::cout << area[i] << std::endl;
  double sum_0 = 0;
  double sum_1 = 0;
  for(int i=0;i<n;i++) sum_0 += area[i];
  for(int i=0;i<n;i++) sum_1 += area[i] * i;
  double ave=sum_1/sum_0;
  std::cout << "average:" << ave << std::endl;
  return ave;
}
