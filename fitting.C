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
  const double sensor_size=10;
  if(-sensor_size<x[0]&&x[0]<sensor_size){
    return 1.;
  } else {
    return 0.;
  }
}

void fitting(int file_n_pre) {
  //ファイルオープン
  std::string file_n = std::to_string(file_n_pre);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_n.size()), '0') + file_n;

  //[0]..高さ、[1]..波数、[2]..幅、[3]..中心間隔、[4]..距離、[5]..バックグラウンド、[6]..中心座標
  TF1* func = new TF1("func","4*[0]*TMath::Power(TMath::Sin([1]*[2]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2)/([1]*[2]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2)*TMath::Cos([1]*[3]*TMath::Sin(TMath::ATan2(x-[6],[4]))/2),2)+[5]",0,100);
  TF1* f_uniform = new TF1("f_uniform",table,0,100);
  TF1Convolution* f_conv = new TF1Convolution("func", "f_uniform", 0, 100, true);
  TF1* f = new TF1("f", *f_conv, 0, 100, f_conv->GetNpar());
  f->SetParameters(10,1428,0.02,0.04,200,1,50);
  f->Draw();
}