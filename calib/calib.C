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

double gaus_multi(const double *x, const double *p){
  return p[0]*TMath::Gaus(x[0],p[1],p[2])
  + p[3]*TMath::Gaus(x[0],p[4],p[5])
  + p[6]*TMath::Gaus(x[0],p[7],p[8])
  + p[9]*TMath::Gaus(x[0],p[10],p[11])
  + p[12]*TMath::Gaus(x[0],p[13],p[14])
  + p[15]*TMath::Gaus(x[0],p[16],p[17])
  + p[18]*TMath::Gaus(x[0],p[19],p[20])
  + p[21]*TMath::Gaus(x[0],p[22],p[23]);
}

void calib(TH1I* t){
  TCanvas* canvas = new TCanvas("c","",0,0,600,400);

  //define fit function
  TF1 *func = new TF1("func",gaus_multi,0,2000,24);
  //set initial parameter 
  double _const[8] = {13,51,74,86,63,56,41,25};
  for(int i=0;i<8;i++) {
    func->SetParameter(i*3,_const[i]);
    func->SetParameter(i*3+1,836+74*i);
    func->SetParameter(i*3+2,20);
    func->SetParName(i * 3, ("const" + std::to_string(i)).c_str());
    func->SetParName(i*3+1,("mean" + std::to_string(i)).c_str());
    func->SetParName(i*3+2,("sigma" + std::to_string(i)).c_str());
  }

  //fit//
  func->SetNpx(10000);
  t->Fit(func,"","",800,1600);
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
