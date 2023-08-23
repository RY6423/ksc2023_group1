/*
  test.C
*/
#include "TCanvas.h"
#include "TF1.h"
#include "TH1F.h"
#include "TROOT.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

double count(string file_ini, int number=-1){
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);
  //書き込みファイル
  TFile* result = new TFile((file_ini+"_count.root").c_str(), "recreate"); 
  //読み込みファイル
  std::string file;
  if(number==-1) {
    file = "../data/"+file_ini+".root";
  } else {
    file = "../data/"+file_ini+"/"+file_ini+"_"+std::to_string(number)+".root";
  }
  TFile* tf = TFile::Open(file.c_str());
  TH1F* t = (TH1F*)tf->Get("ADC_HIGH_0");

  double a=74.1558,b=836.127;
  const int n=30;
  double area[n];
  double x[n];
  int n_max=0;
  for(int i=0;i<n;i++) {
    area[i] = t->Integral(t->FindBin(b+a*(i-0.5)), t->FindBin(b+a*(i+0.5)), "width");
    x[i]=i;
    if(area[i]!=0) n_max=i;
  }

  for(int i=0;i<n_max;i++) std::cout << area[i] << std::endl;
  double sum_0 = 0;
  double sum_1 = 0;
  for(int i=0;i<n;i++) sum_0 += area[i];
  for(int i=0;i<n;i++) sum_1 += area[i] * i;
  double ave=sum_1/sum_0;
  std::cout << "average:" << ave << std::endl;

  TGraph* tg1 = new TGraph(n_max, x, area) ;
  tg1-> SetMarkerStyle(20);
  tg1->Draw("ap");
  result->cd();
  tg1->Write();
  result->Write();
  result->Close();

  return ave;
  //std::cout<<"mean = " <<meanFit<<std::endl;
}
