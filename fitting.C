/*
  fitting.C
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


double table(const double *x, const double *p){
  const double sensor_size=p[0];
  if(-sensor_size/2<x[0]&&x[0]<sensor_size/2){
    return 1/sensor_size/2;
  } else {
    return 0.;
  }
}

void fitting(int file_n_pre) {
  gStyle->SetTitleSize(0.6,"xy");
  gStyle->SetLabelSize(0.6,"xy");
  //ファイルオープン
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;
  std::string file = "out/" + file_ini + "/" + file_ini + "_analize.root";
  TFile* tf = TFile::Open(file.c_str());
  TGraphErrors* th1 = (TGraphErrors*)tf->Get("Graph");

  //[0]..高さ、[1]..波数、[2]..幅、[3]..中心間隔、[4]..距離、[5]..バックグラウンド、[6]..中心座標、[7]..バックグラウンドの傾き、[8]..コンボリューション
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);
  TF1* func = new TF1("func","4*[0]*TMath::Power(TMath::Sin([1]*[2]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2)/([1]*[2]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2)*TMath::Cos([1]*[3]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2),2)+[5]+[7]*x",0,100);
  TF1* f_uniform = new TF1("f_uniform",table,-10,10,1);
  TF1Convolution* f_conv = new TF1Convolution("func", "f_uniform", -5, 35, true);
  TF1* f = new TF1("f", *f_conv, 0, 30, f_conv->GetNpar());
  f->SetParameters(2,10048,0.02,0.04,236,6,14,-0.01,1.9);
  f->FixParameter(1,10048);
  f->FixParameter(4,236);
  f->SetNpx(10000);
  th1->Fit("f","","",-5,35);
  TLegend *legend = new TLegend( 0.5, 0.6, 0.84, 0.78) ; //（）の中は位置の指定（左下の x , y 、右上の x , y ）
  legend->AddEntry(th1, "measurement result", "pe");            // AddEntry( pointer , "interpretation" , "option" )
  legend->AddEntry(f, "fitting" , "pl") ; // AddEntry( pointer , "interpretation" , "option" )
  th1->GetXaxis()->SetTitle("position(mm)");
  th1->GetXaxis()->SetTitleSize(0.05);
  th1->GetYaxis()->SetTitle("number of photons");
  th1->GetYaxis()->SetTitleSize(0.05);
  th1->Draw("ap");
  f->Draw("same");
  legend->Draw();
  canvas -> Print(("out/"+file_ini+"/"+file_ini+"_fitting.pdf").c_str());
}