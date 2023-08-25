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
const int n_max = 20;

double gaus_multi(const double *x, const double *p){
  double result=0;
  for(int i=0;i<n_max;i++) result += p[i*3]*TMath::Gaus(x[0],p[i*3+1],p[i*3+2]);
  result+=p[3*n_max];
  return result;
}

void calib(int file_i,int f_number){
  std::string file_s = std::to_string(file_i);
  std::string file_ini = "ksc_"+std::string(std::max(0, 3 - (int)file_s.size()), '0') + file_s;
  std::string number_s=std::to_string(f_number);
  std::string number_fill=std::string(std::max(0, 4 - (int)number_s.size()), '0') + number_s;
  std::string file="data/"+file_ini+"/"+file_ini+"_"+number_fill+".root";
  TFile* tf = TFile::Open(file.c_str());
  TH1F* th1 = (TH1F*)tf->Get("ADC_HIGH_0");

  TCanvas* canvas = new TCanvas("c","",0,0,600,400);

  TF1* func[n_max];
  TF1* func_all = new TF1("func_all",gaus_multi,0,2000,3*n_max+1);
  for(int i =0;i<n_max;i++) {
  }
  func_all->SetParameter(n_max*3,0);
  
  std::string file_out="out/calib/"+file_ini+"_"+number_fill+"_calib";
  th1->Fit(func_all);

  func_all->SetNpx(100000);
  //func->Dump();

  TH1* frame = canvas->DrawFrame(800,10,1000,1000);
  frame->SetTitle("");
  frame->GetXaxis()->SetTitle("ADC");
  frame->GetYaxis()->SetTitle("イベント数");
  TLegend *legend = new TLegend( 0.65, 0.68, 0.84, 0.78) ; //（）の中は位置の指定（左下の x , y 、右上の x , y ）
  legend->AddEntry(th1, "Data", "p");            // AddEntry( pointer , "interpretation" , "option" )
  legend->AddEntry(func_all, "fitting" , "pl") ; // AddEntry( pointer , "interpretation" , "option" )
  th1->Draw("same");
  legend->Draw();
  mkdir("out/calib",0777);
  canvas->Print((file_out+".pdf").c_str());

  double x[n_max],y[n_max],y_e[n_max];
  bool on = true;
  for(int i=0;i<n_max;i++) {
    x[i]=i;
    y[i]=func_all->GetParameter(i*3+1);
    if(y[i]<10) on=false;
    y_e[i]=func_all->GetParError(i*3+1);
    if(!on) {
      y[i]=0;
      y_e[i]=0;
    }
  }
  TGraphErrors* tg1 = new TGraphErrors(n_max,x,y,nullptr,y_e);
  TF1* line = new TF1("line","[0]+x*[1]",800,2000);
  tg1->Fit(line);
  tg1->SetTitle("");
  tg1->GetXaxis()->SetTitle("number of photons");
  tg1->GetYaxis()->SetTitle("ADC");
  legend->AddEntry(tg1, "Data", "p");            // AddEntry( pointer , "interpretation" , "option" )
  legend->AddEntry(line, "fitting" , "pl") ; // AddEntry( pointer , "interpretation" , "option" )
  legend->Draw();
  tg1->Draw("ap");
  canvas->Print((file_out+"_fit.pdf").c_str());

  TFile* tf_out = new TFile((file_out+".root").c_str(),"recreate");
  th1->Write();
  tg1->Write();
  tf_out->Write();
  tf_out->Close();


  //ofs << "#number" << " " << "mean" << " " << "m_error" << " " << "area" << " " << "a_error" << " " << "count_area" << std::endl;
  //for(int i=0;i<8;i++) {
  //  ofs << i << " " << func->GetParameter(i*3+1) << " " 
  //  << func->GetParError(i*3+1) << " "
  //  << func->GetParameter(i*3)*func->GetParameter(i*3+2) * std::sqrt(2*3.1415) << " " 
  //  << std::sqrt(func->GetParameter(i*3+1)) << " "
  //  << area[i]
  //  << std::endl;
  //}
}
