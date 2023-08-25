/*
  analize.C
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
#include <sys/stat.h>

double calib_a=74.1558,calib_b=836.127;
void count(double*,TH1F*);

void analize_new(int file_n_pre,double inter) {
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);

  const int ni=10000;
  Double_t x[ni];
  Double_t y[ni];
  Double_t y_e[ni];
  int error_n=0;

  //データファイルオープン
  int number=0;
  while(true) {
    std::cout << number << " ";
    std::string file;
    std::string s_i = std::to_string(number);
    s_i = std::string(std::max(0, 4 - (int)s_i.size()), '0') + s_i;
    file = "data/" + file_ini + "/" + file_ini + "_" + s_i + ".root";
    TFile* tf = TFile::Open(file.c_str());
    if(!tf) break;
    TH1F* th1 = (TH1F*)tf->Get("ADC_HIGH_0");
    if(!th1){
      error_n++;
      number++;
    }
    if(!th1) continue;
    double par[2];
    count(par,th1);
    x[number-error_n] = double(number)*inter;
    y[number-error_n] = par[0];
    y_e[number-error_n]=par[1];
    number++;
  }

  //書き込み用ファイル
  mkdir(("out/"+file_ini).c_str(),0777);
  TFile* result = new TFile(("out/"+file_ini+"/"+file_ini+"_analize_new.root").c_str(), "recreate");

  TGraphErrors* tg1 = new TGraph(number - error_n, x, y);

  result->cd();
  //tg1-> SetMarkerStyle(4);
  //tg1-> SetMarkerSize(0.5);
  tg1->SetTitle("");
  tg1->GetXaxis()->SetTitle("position(mm)");
  tg1->GetYaxis()->SetTitle("number of photons");
  tg1->Draw("ap");
  canvas -> Print(("out/"+file_ini+"/"+file_ini+"_analize_new.pdf").c_str());
  tg1->Write();
  result->Write();
  result->Close();
  return;
}

void count(double par[2],TH1F* t){
  double area = t->Integral(t->FindBin(calib_b-calib_a*0.5), t->FindBin(calib_b+calib_a*0.5), "width");

  par[0]=area/t->GetEntries();
  par[1]=0;

  std::cout << "average:" << par[0] << "+-" << par[1] << std::endl;
  return;
}