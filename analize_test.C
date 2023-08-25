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
void poisson(double*,TH1F*);

void analize_test(int file_n_pre,double inter,bool split=false) {
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);

  const int ni=10000;
  Double_t x[ni];
  Double_t y[ni];
  Double_t y_e[ni];
  Double_t y_fit[ni];
  Double_t y_fit_e[ni];
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
    double par[4];
    poisson(par,th1);
    x[number-error_n] = double(number)*inter;
    y[number-error_n] = par[0];
    y_e[number-error_n]=par[1];
    y_fit[number-error_n] = par[2];
    y_fit_e[number-error_n]=par[3];
    number++;
  }

  //書き込み用ファイル
  mkdir("out",0777);
  mkdir(("out/"+file_ini).c_str(),0777);
  TFile* result = new TFile(("out/"+file_ini+"/"+file_ini+"_analize.root").c_str(), "recreate");

  TGraphErrors* tg1 = new TGraphErrors(number - error_n, x, y, nullptr, y_e);
  TGraphErrors* tg2 = new TGraphErrors(number - error_n, x, y_fit, nullptr, y_fit_e);

  result->cd();
  //tg1-> SetMarkerStyle(4);
  //tg1-> SetMarkerSize(0.5);
  tg1->SetTitle("");
  tg2->SetTitle("");
  tg1->GetXaxis()->SetTitle("position(mm)");
  tg2->GetXaxis()->SetTitle("position(mm)");
  tg1->GetYaxis()->SetTitle("number of photons");
  tg2->GetYaxis()->SetTitle("number of photons");
  tg1->Draw("ap");
  canvas -> Print(("out/"+file_ini+"/"+file_ini+"_analize.pdf").c_str());
  tg2->Draw("ap");
  canvas -> Print(("out/"+file_ini+"/"+file_ini+"_analize_poi.pdf").c_str());
  if(split) tg1->Draw("ap");
  tg1->Write();
  tg2->Write();
  return;
}

void poisson(double par[4],TH1F* t){
  const int n=40;
  double area[n];
  double x[n];
  for(int i=0;i<n;i++) {
    area[i] = t->Integral(t->FindBin(calib_b+calib_a*(i-0.5)), t->FindBin(calib_b+calib_a*(i+0.5)), "width");
    x[i]=i;
  }

  double sum_0 = 0;
  double sum_1 = 0;
  double disp=0;
  for(int i=0;i<n;i++) sum_0 += area[i];
  for(int i=0;i<n;i++) sum_1 += area[i] * i;
  par[0]=t->GetMean();
  for(int i=0;i<n;i++) disp+=(i-par[0])*(i-par[0])*area[i];
  disp/=sum_0;
  par[1]=t->GetMeanError();

  TGraph* tg1 = new TGraph(n, x, area);
  TF1* poi = new TF1("f","[0]*TMath::PoissonI(x, [1])",0,n);
  poi->SetParameters(sum_0,par[0]);
  tg1->Fit(poi);
  par[2]=poi->GetParameter(1);
  par[3]=poi->GetParError(1);

  std::cout << "ざっくり切りaverage:" << par[0] << "+-" << par[1] << std::endl;
  std::cout << "Fit_average:" << par[2] << "+-" << par[3] << std::endl;
  return;
}