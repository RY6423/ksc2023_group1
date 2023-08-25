/*
  test.C
*/
#include "TCanvas.h"
#include "TF1.h"
#include "TH1I.h"
#include "TROOT.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
const int n_max = 30;

double gaus_multi(const double *x, const double *p){
  double result=0;
  for(int i=0;i<n_max) result += p[i*3]*TMath::Gaus(x[0],p[i*3+1],p[i*3+2]);
  return result;
}

void calib(TH1I* t){
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);

  TF1* func_all = new TF1("func_all",gaus_multi,0,2000,3*n_max)
  for(int i =0;i<n_max;i++) {
    t->SetParameter(i*3+1,)
  }

  //fit//
  func->SetNpx(10000);
  //func->Dump();
  //for(int i;i<8)
  std::ofstream ofs("calib.dat");
  if (!ofs) {
    std::cerr << "ファイルオープンに失敗" << std::endl;
    std::exit(1);
  }

  double mean[8];
  for(int i=0;i<8;i++) mean[i] = func->GetParameter(i*3+1);
  double area[8];
  for(int i=0;i<8;i++) {
    if(i==0) {
        area[i] = t->Integral(t->FindBin(800), t->FindBin((mean[i] + mean[i + 1]) / 2), "width");
    } else if(i==7) {
        area[i] = t->Integral(t->FindBin((mean[i - 1] + mean[i]) / 2), t->FindBin(1600), "width");
    } else {
        area[i] = t->Integral(t->FindBin((mean[i - 1] + mean[i]) / 2), t->FindBin((mean[i] + mean[i + 1]) / 2), "width");
    }
  }

  ofs << "#number" << " " << "mean" << " " << "m_error" << " " << "area" << " " << "a_error" << " " << "count_area" << std::endl;
  for(int i=0;i<8;i++) {
    ofs << i << " " << func->GetParameter(i*3+1) << " " 
    << func->GetParError(i*3+1) << " "
    << func->GetParameter(i*3)*func->GetParameter(i*3+2) * std::sqrt(2*3.1415) << " " 
    << std::sqrt(func->GetParameter(i*3+1)) << " "
    << area[i]
    << std::endl;
  }
  

  double meanFit = func->GetParameter(1);
  //std::cout<<"mean = " <<meanFit<<std::endl;
}
